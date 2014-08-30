/*
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

package org.allseen.lsf.sampleapp;

import java.lang.reflect.Method;
import java.util.Calendar;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;

import org.allseen.lsf.LampGroup;
import org.allseen.lsf.ResponseCode;

import android.app.ActionBar;
import android.app.AlertDialog;
import android.app.FragmentTransaction;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.PopupMenu;
import android.widget.TextView;
import android.widget.Toast;

public class SampleAppActivity extends FragmentActivity implements ActionBar.TabListener, PopupMenu.OnMenuItemClickListener {
    public static final String TAG = "LSFSampleApp";
    public static final String TAG_TRACE = "LSFSampleApp########";
    public static final String LANGUAGE = "en";

    public static final boolean ERROR_CODE_ENABLED = false; // enables all error dialogs
    public static final boolean ERROR_CODE_VERBOSE = true; // when set to false enables only dependency errors

    public static final int POLLING_DELAY = 10000;
    public static final int LAMP_EXPIRATION = 15000;

    public static final long STATE_TRANSITION_DURATION = 100;
    public static final long FIELD_TRANSITION_DURATION = 0;
    public static final long FIELD_CHANGE_HOLDOFF = 25;

    private static long lastFieldChangeMillis = 0;

    public enum Type {
        LAMP, GROUP, SCENE, ELEMENT
    }

    private SampleAppViewPager viewPager;
    public Handler handler;

    public volatile boolean isForeground;
    public volatile Queue<Runnable> runInForeground;

    public ControllerDataModel leaderControllerModel;

    public Map<String, LampDataModel> lampModels = new HashMap<String, LampDataModel>();
    public Map<String, GroupDataModel> groupModels = new HashMap<String, GroupDataModel>();
    public Map<String, PresetDataModel> presetModels = new HashMap<String, PresetDataModel>();
    public Map<String, BasicSceneDataModel> basicSceneModels = new HashMap<String, BasicSceneDataModel>();
    public Map<String, MasterSceneDataModel> masterSceneModels = new HashMap<String, MasterSceneDataModel>();

    public GroupDataModel pendingGroupModel;
    public BasicSceneDataModel pendingBasicSceneModel;
    public MasterSceneDataModel pendingMasterSceneModel;

    public LampGroup pendingBasicSceneElementMembers;
    public CapabilityData pendingBasicSceneElementCapability;

    public NoEffectDataModel pendingNoEffectModel;
    public TransitionEffectDataModel pendingTransitionEffectModel;
    public PulseEffectDataModel pendingPulseEffectModel;

    public SampleControllerClientCallback controllerClientCB;
    public SampleLampManagerCallback lampManagerCB;
    public SampleGroupManagerCallback groupManagerCB;
    public SamplePresetManagerCallback presetManagerCB;
    public SampleBasicSceneManagerCallback sceneManagerCB;
    public SampleMasterSceneManagerCallback masterSceneManagerCB;

    public GarbageCollector garbageCollector;

    public PageFrameParentFragment pageFrameParent;

    private AlertDialog wifiDisconnectAlertDialog;
    private AlertDialog errorCodeAlertDialog;
    private String errorCodeAlertDialogMessage;

    private MenuItem addActionMenuItem;
    private MenuItem nextActionMenuItem;
    private MenuItem doneActionMenuItem;
    private String popupItemID;
    private String popupSubItemID;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sample_app);

        viewPager = (SampleAppViewPager) findViewById(R.id.sampleAppViewPager);
        viewPager.setActivity(this);

        handler = new Handler(Looper.getMainLooper());
        runInForeground = new LinkedList<Runnable>();

        FragmentManager fragmentManager = getSupportFragmentManager();
        AboutManager aboutManager = new AboutManager(this, handler);
        controllerClientCB = new SampleControllerClientCallback(this, fragmentManager, handler);
        lampManagerCB = new SampleLampManagerCallback(this, fragmentManager, handler);
        groupManagerCB = new SampleGroupManagerCallback(this, fragmentManager, handler);
        presetManagerCB = new SamplePresetManagerCallback(this, fragmentManager, handler);
        sceneManagerCB = new SampleBasicSceneManagerCallback(this, fragmentManager, handler);
        masterSceneManagerCB = new SampleMasterSceneManagerCallback(this, fragmentManager, handler);

        garbageCollector = new GarbageCollector(this, SampleAppActivity.POLLING_DELAY, SampleAppActivity.LAMP_EXPIRATION);

        // Setup localized strings in data models
        ControllerDataModel.defaultName = this.getString(R.string.default_controller_name);

        LampDataModel.dataNotFound = this.getString(R.string.data_not_found);
        LampDataModel.defaultName = this.getString(R.string.default_lamp_name);

        GroupDataModel.defaultName = this.getString(R.string.default_group_name);
        PresetDataModel.defaultName = this.getString(R.string.default_preset_name);

        NoEffectDataModel.defaultName = this.getString(R.string.effect_name_none);
        TransitionEffectDataModel.defaultName = this.getString(R.string.effect_name_transition);
        PulseEffectDataModel.defaultName = this.getString(R.string.effect_name_pulse);

        BasicSceneDataModel.defaultName = this.getString(R.string.default_basic_scene_name);
        MasterSceneDataModel.defaultName = this.getString(R.string.default_master_scene_name);

        // Start up the AllJoynManager
        Log.d(SampleAppActivity.TAG, "===========================================");
        Log.d(SampleAppActivity.TAG, "AllJoyn Manager init()");

        AllJoynManager.init(
            getSupportFragmentManager(),
            controllerClientCB,
            lampManagerCB,
            groupManagerCB,
            presetManagerCB,
            sceneManagerCB,
            masterSceneManagerCB,
            aboutManager,
            this);

        garbageCollector.start();

        // Handle wifi disconnect errors
        IntentFilter filter = new IntentFilter();
        filter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);

        registerReceiver(new BroadcastReceiver() {
            @Override
            public void onReceive(final Context context, Intent intent) {
                NetworkInfo wifiNetworkInfo = ((ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE)).getNetworkInfo(ConnectivityManager.TYPE_WIFI);

                // determine if wifi AP mode is on
                boolean isWifiApEnabled = false;
                WifiManager wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
                // need reflection because wifi ap is not in the public API
                try {
                    Method isWifiApEnabledMethod = wifiManager.getClass().getDeclaredMethod("isWifiApEnabled");
                    isWifiApEnabled = (Boolean) isWifiApEnabledMethod.invoke(wifiManager);
                } catch (Exception e) {
                    e.printStackTrace();
                }

                Log.d(SampleAppActivity.TAG, "Connectivity state " + wifiNetworkInfo.getState().name() + " - connected:" + wifiNetworkInfo.isConnected() + " hotspot:" + isWifiApEnabled);

                wifiConnectionStateUpdate(wifiNetworkInfo.isConnected() || isWifiApEnabled);
            }
        }, filter);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);

        addActionMenuItem = menu.findItem(R.id.action_add);
        nextActionMenuItem = menu.findItem(R.id.action_next);
        doneActionMenuItem = menu.findItem(R.id.action_done);

        if (pageFrameParent == null) {
            updateActionBar(viewPager.getCurrentItem() != 0, false, false);
        }

        return true;
    }

    @Override
    public void onResume() {
        super.onResume();

        isForeground = true;
    }

    @Override
    public void onResumeFragments() {
        super.onResumeFragments();

        // run everything that was queued up whilst in the background
        Log.d(SampleAppActivity.TAG, "Clearing foreground runnable queue");
        while (!runInForeground.isEmpty()) {
            handler.post(runInForeground.remove());
        }
    }

    @Override
    public void onPause() {
        super.onPause();

        isForeground = false;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle item selection
        switch (item.getItemId()) {
            case android.R.id.home:
                if (pageFrameParent != null) {
                    onBackPressed();
                }
                return true;
            case R.id.action_add:
                if (pageFrameParent != null) {
                    pageFrameParent.onActionAdd();
                } else if (viewPager.getCurrentItem() == 1) {
                    doAddGroup((GroupsPageFragment)(getSupportFragmentManager().findFragmentByTag(GroupsPageFragment.TAG)));
                } else {
                    showSceneAddPopup(findViewById(R.id.action_add));
                }
                return true;
            case R.id.action_next:
                if (pageFrameParent != null) pageFrameParent.onActionNext();
                return true;
            case R.id.action_done:
                if (pageFrameParent != null) pageFrameParent.onActionDone();
                return true;
            case R.id.action_settings:
                showSettingsFragment();
                return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onTabSelected(ActionBar.Tab tab,
            FragmentTransaction fragmentTransaction) {
        // When the given tab is selected, switch to the corresponding page in
        // the ViewPager.
        viewPager.setCurrentItem(tab.getPosition());

        updateActionBar(tab.getPosition() != 0, false, false);
    }

    @Override
    public void onTabUnselected(ActionBar.Tab tab,
            FragmentTransaction fragmentTransaction) {
    }

    @Override
    public void onTabReselected(ActionBar.Tab tab,
            FragmentTransaction fragmentTransaction) {
    }

    @Override
    public void onBackPressed() {
        int backStackCount = pageFrameParent != null ? pageFrameParent.onBackPressed() : 0;

        if (backStackCount == 1) {
            onClearBackStack();
        } else if (backStackCount == 0) {
            super.onBackPressed();
        }
    }

    public void postInForeground(final Runnable r) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                if (isForeground) {
                    Log.d(SampleAppActivity.TAG, "Foreground runnable running now");
                    handler.post(r);
                } else {
                    Log.d(SampleAppActivity.TAG, "Foreground runnable running later");
                    runInForeground.add(r);
                }
            }
        });
    }

    public void onClearBackStack() {
        pageFrameParent = null;
        resetActionBar();
    }

    public void onItemButtonMore(PageFrameParentFragment parent, Type type, View button, String itemID, String subItemID) {
        switch (type) {
            case LAMP:
                showInfoFragment(parent, itemID);
                return;
            case GROUP:
                showGroupMorePopup(button, itemID);
                return;
            case SCENE:
                showSceneMorePopup(button, itemID);
                return;
            case ELEMENT:
                showSceneElementMorePopup(button, itemID, subItemID);
                return;
        }
    }

    private void showInfoFragment(PageFrameParentFragment parent, String itemID) {
        pageFrameParent = parent;

        parent.showInfoChildFragment(itemID);
    }

    private void applyBasicScene(String basicSceneID) {
        BasicSceneDataModel basicSceneModel = basicSceneModels.get(basicSceneID);

        if (basicSceneModel != null) {
            String message = String.format(this.getString(R.string.toast_basic_scene_apply), basicSceneModel.name);

            AllJoynManager.sceneManager.applyScene(basicSceneID);

            Toast.makeText(this, message, Toast.LENGTH_LONG).show();
        }
    }

    private void applyMasterScene(String masterSceneID) {
        MasterSceneDataModel masterSceneModel = masterSceneModels.get(masterSceneID);

        if (masterSceneModel != null) {
            String message = String.format(this.getString(R.string.toast_master_scene_apply), masterSceneModel.name);

            AllJoynManager.masterSceneManager.applyMasterScene(masterSceneID);

            Toast.makeText(this, message, Toast.LENGTH_LONG).show();
        }
    }

    public void wifiConnectionStateUpdate(boolean connected) {
        final SampleAppActivity activity = this;
        if (connected) {
            handler.post(new Runnable() {
                @Override
                public void run() {
                    Log.d(SampleAppActivity.TAG, "wifi connected");

                    postInForeground(new Runnable() {
                        @Override
                        public void run() {
                            if (wifiDisconnectAlertDialog != null) {
                                Log.d(SampleAppActivity.TAG, "starting AllJoyn");
                                AllJoynManager.init(
                                        getSupportFragmentManager(),
                                        controllerClientCB,
                                        lampManagerCB,
                                        groupManagerCB,
                                        presetManagerCB,
                                        sceneManagerCB,
                                        masterSceneManagerCB,
                                        new AboutManager(activity, handler),
                                        activity);

                                wifiDisconnectAlertDialog.dismiss();
                                wifiDisconnectAlertDialog = null;
                            }
                        }
                    });
                }
            });
        } else {
            handler.post(new Runnable() {
                @Override
                public void run() {
                    Log.d(SampleAppActivity.TAG, "wifi disconnected");

                    postInForeground(new Runnable() {
                        @Override
                        public void run() {
                            if (wifiDisconnectAlertDialog == null) {
                                Log.d(SampleAppActivity.TAG, "closing AllJoyn");
                                AllJoynManager.destroy(getSupportFragmentManager());

                                View view = activity.getLayoutInflater().inflate(R.layout.view_loading, null);
                                ((TextView) view.findViewById(R.id.loadingText1)).setText(activity.getText(R.string.no_wifi_message));
                                ((TextView) view.findViewById(R.id.loadingText2)).setText(activity.getText(R.string.searching_wifi));

                                AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(activity);
                                alertDialogBuilder.setView(view);
                                alertDialogBuilder.setTitle(R.string.no_wifi);
                                alertDialogBuilder.setCancelable(false);
                                wifiDisconnectAlertDialog = alertDialogBuilder.create();
                                wifiDisconnectAlertDialog.show();
                            }
                        }
                    });
                }
            });
        }
    }

    public void showErrorResponseCode(Enum code, String source) {
        final SampleAppActivity activity = this;
        // creates a message about the error
        StringBuilder sb = new StringBuilder();

        final boolean dependencyError = (code instanceof ResponseCode) && (code == ResponseCode.ERR_DEPENDENCY);
        if (dependencyError) {
            // dependency error
            sb.append(this.getString(R.string.error_dependency));

        } else {
            // default error message
            sb.append(this.getString(R.string.error_code));
            sb.append(" ");
            sb.append(code.name());
            sb.append(source != null ? " - " + source : "");

        }
        final String message = sb.toString();

        Log.w(SampleAppActivity.TAG, message);

        if (ERROR_CODE_ENABLED) {
            handler.post(new Runnable() {
                @Override
                public void run() {
                    AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(activity);
                    alertDialogBuilder.setTitle(R.string.error_title);
                    alertDialogBuilder.setPositiveButton(R.string.dialog_ok, new OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int id) {
                            errorCodeAlertDialog = null;
                            errorCodeAlertDialogMessage = null;
                            dialog.cancel();
                        }
                    });

                    errorCodeAlertDialog = alertDialogBuilder.create();
                    if (ERROR_CODE_VERBOSE || (!ERROR_CODE_VERBOSE && dependencyError)) {
                        if (errorCodeAlertDialogMessage == null) {
                            errorCodeAlertDialogMessage = message;
                            errorCodeAlertDialog.setMessage(errorCodeAlertDialogMessage);
                            errorCodeAlertDialog.show();
                        } else if (!errorCodeAlertDialogMessage.contains(message)) {
                            errorCodeAlertDialogMessage += System.getProperty("line.separator") + message;
                            errorCodeAlertDialog.setMessage(errorCodeAlertDialogMessage);
                            errorCodeAlertDialog.show();
                        }
                    }
                }
            });
        }
    }

    public void showItemNameDialog(int titleID, ItemNameAdapter adapter) {
        if (adapter != null) {
            View view = getLayoutInflater().inflate(R.layout.view_dialog_item_name, null, false);
            EditText nameText = (EditText)view.findViewById(R.id.itemNameText);
            AlertDialog alertDialog = new AlertDialog.Builder(this)
                .setTitle(titleID)
                .setView(view)
                .setPositiveButton(R.string.dialog_ok, adapter)
                .setNegativeButton(R.string.dialog_cancel, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }})
                .create();

            nameText.addTextChangedListener(new ItemNameDialogTextWatcher(alertDialog, nameText));
            nameText.setText(adapter.getCurrentName());

            alertDialog.show();
        }
    }

    private void showConfirmDeleteBasicSceneDialog(final String basicSceneID) {
        if (basicSceneID != null) {
            BasicSceneDataModel basicSceneModel = basicSceneModels.get(basicSceneID);

            if (basicSceneModel != null) {
                showConfirmDeleteDialog(R.string.menu_basic_scene_delete, R.string.label_basic_scene, basicSceneModel.name, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        Log.d(SampleAppActivity.TAG, "Delete basic scene ID: " + basicSceneID);
                        AllJoynManager.sceneManager.deleteScene(basicSceneID);
                    }});
            }
        }
    }

    private void doDeleteSceneElement(String basicSceneID, String elementID ) {
        BasicSceneDataModel basicSceneModel = basicSceneModels.get(basicSceneID);

        if (basicSceneModel != null && basicSceneModel.removeElement(elementID)) {
            AllJoynManager.sceneManager.updateScene(basicSceneModel.id, basicSceneModel.toScene());
        }
    }

    private void showConfirmDeleteMasterSceneDialog(final String masterSceneID) {
        if (masterSceneID != null) {
            MasterSceneDataModel masterSceneModel = masterSceneModels.get(masterSceneID);

            if (masterSceneModel != null) {
                showConfirmDeleteDialog(R.string.menu_master_scene_delete, R.string.label_master_scene, masterSceneModel.name, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        Log.d(SampleAppActivity.TAG, "Delete master scene ID: " + masterSceneID);
                        AllJoynManager.masterSceneManager.deleteMasterScene(masterSceneID);
                    }});
            }
        }
    }

    private void showConfirmDeleteGroupDialog(final String groupID) {
        if (groupID != null) {
            GroupDataModel groupModel = groupModels.get(groupID);

            if (groupModel != null) {
                showConfirmDeleteDialog(R.string.menu_group_delete, R.string.label_group, groupModel.name, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        Log.d(SampleAppActivity.TAG, "Delete group ID: " + groupID);
                        AllJoynManager.groupManager.deleteLampGroup(groupID);
                    }});
            }
        }
    }

    private void showConfirmDeletePresetDialog(final String presetID) {
        if (presetID != null) {
            PresetDataModel presetModel = presetModels.get(presetID);

            if (presetModel != null) {
                showConfirmDeleteDialog(R.string.menu_preset_delete, R.string.label_preset, presetModel.name, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        Log.d(SampleAppActivity.TAG, "Delete preset ID: " + presetID);
                        AllJoynManager.presetManager.deletePreset(presetID);
                    }});
            }
        }
    }

    private void showConfirmDeleteDialog(int titleID, int labelID, String itemName, DialogInterface.OnClickListener onOKListener) {
        View view = getLayoutInflater().inflate(R.layout.view_dialog_confirm_delete, null, false);

        String format = getResources().getString(R.string.dialog_text_delete);
        String label = getResources().getString(labelID);
        String text = String.format(format, label, itemName);

        ((TextView)view.findViewById(R.id.confirmDeleteText)).setText(text);

        new AlertDialog.Builder(this)
            .setTitle(titleID)
            .setView(view)
            .setPositiveButton(R.string.dialog_ok, onOKListener)
            .setNegativeButton(R.string.dialog_cancel, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int id) {
                    dialog.cancel();
                }})
            .create()
            .show();
    }

    private void showSceneInfo(boolean master) {
        ScenesPageFragment scenesPageFragment = (ScenesPageFragment)getSupportFragmentManager().findFragmentByTag(ScenesPageFragment.TAG);
        scenesPageFragment.setMasterMode(master);
        showInfoFragment(scenesPageFragment, popupItemID);
    }

    public void showLampDetailsFragment(LampsPageFragment parent, String key) {
        pageFrameParent = parent;
        parent.showDetailsChildFragment(key);
    }

    public void doAddGroup(GroupsPageFragment parent) {
        if (parent != null) {
            pageFrameParent = parent;
            parent.showEnterNameChildFragment();
        }
    }

    public void showGroupMorePopup(View anchor, String groupID) {
        popupItemID = groupID;

        PopupMenu popup = new PopupMenu(this, anchor);
        popup.inflate(R.menu.group_more);
        popup.getMenu().findItem(R.id.group_delete).setEnabled(groupID != SampleGroupManager.ALL_LAMPS_GROUP_ID);
        popup.setOnMenuItemClickListener(this);
        popup.show();
    }

    public boolean isSwipeable() {
        return (pageFrameParent == null);
    }

    public void showSceneMorePopup(View anchor, String sceneID) {
        boolean basicScene = basicSceneModels.containsKey(sceneID);

        popupItemID = sceneID;

        PopupMenu popup = new PopupMenu(this, anchor);
        popup.inflate(basicScene ? R.menu.basic_scene_more : R.menu.master_scene_more);
        popup.setOnMenuItemClickListener(this);
        popup.show();
    }

    public void showSceneAddPopup(View anchor) {
        popupItemID = null;

        PopupMenu popup = new PopupMenu(this, anchor);
        popup.inflate(R.menu.scene_add);
        popup.setOnMenuItemClickListener(this);
        popup.show();
    }

    public void showSceneElementMorePopup(View anchor, String itemID, String subItemID) {
        popupItemID = itemID;
        popupSubItemID = subItemID;

        PopupMenu popup = new PopupMenu(this, anchor);
        popup.inflate(R.menu.basic_scene_element_more);
        popup.setOnMenuItemClickListener(this);
        popup.show();
    }

    public void showPresetMorePopup(View anchor, String itemID) {
        popupItemID = itemID;
        popupSubItemID = null;

        PopupMenu popup = new PopupMenu(this, anchor);
        popup.inflate(R.menu.preset_more);
        popup.setOnMenuItemClickListener(this);
        popup.show();
    }

    protected void showSettingsFragment() {
        if (pageFrameParent == null) {
            int pageIndex = viewPager.getCurrentItem();
            String pageTag;

            if (pageIndex == 0) {
                pageTag = LampsPageFragment.TAG;
            } else if (pageIndex == 1) {
                pageTag = GroupsPageFragment.TAG;
            } else {
                pageTag = ScenesPageFragment.TAG;
            }

            pageFrameParent = (PageFrameParentFragment)getSupportFragmentManager().findFragmentByTag(pageTag);
        }

        pageFrameParent.showSettingsChildFragment("");
    }

    @Override
    public boolean onMenuItemClick(MenuItem item) {
        Log.d(SampleAppActivity.TAG, "onMenuItemClick(): " + item.getItemId());
        boolean result = true;

        switch (item.getItemId()) {
            case R.id.group_info:
                showInfoFragment((GroupsPageFragment)(getSupportFragmentManager().findFragmentByTag(GroupsPageFragment.TAG)), popupItemID);
                break;
            case R.id.group_delete:
                showConfirmDeleteGroupDialog(popupItemID);
                break;
            case R.id.basic_scene_info:
                showSceneInfo(false);
                break;
            case R.id.basic_scene_apply:
                applyBasicScene(popupItemID);
                break;
            case R.id.basic_scene_delete:
                showConfirmDeleteBasicSceneDialog(popupItemID);
                break;
            case R.id.basic_scene_element_delete:
                doDeleteSceneElement(popupItemID, popupSubItemID);
                break;
            case R.id.master_scene_info:
                showSceneInfo(true);
                break;
            case R.id.master_scene_apply:
                applyMasterScene(popupItemID);
                break;
            case R.id.master_scene_delete:
                showConfirmDeleteMasterSceneDialog(popupItemID);
                break;
            case R.id.preset_delete:
                showConfirmDeletePresetDialog(popupItemID);
                break;
            case R.id.scene_add_basic:
                doAddScene((ScenesPageFragment)(getSupportFragmentManager().findFragmentByTag(ScenesPageFragment.TAG)), false);
                break;
            case R.id.scene_add_master:
                doAddScene((ScenesPageFragment)(getSupportFragmentManager().findFragmentByTag(ScenesPageFragment.TAG)), true);
                break;
            default:
                result = false;
                break;
        }

        return result;
    }

    public void doAddScene(ScenesPageFragment parent, boolean isMaster) {
        if (parent != null) {
            pendingNoEffectModel = null;
            pendingTransitionEffectModel = null;
            pendingPulseEffectModel = null;

            pageFrameParent = parent;
            parent.setMasterMode(isMaster);
            parent.showEnterNameChildFragment();
        }
    }

    public void resetActionBar() {
        updateActionBar(null, true, viewPager.getCurrentItem() != 0, false, false);
    }

    public void updateActionBar(int titleID, boolean tabs, boolean add, boolean next, boolean done) {
        updateActionBar(getResources().getString(titleID), tabs, add, next, done);
    }

    protected void updateActionBar(String title, boolean tabs, boolean add, boolean next, boolean done) {
        Log.d(SampleAppActivity.TAG, "Updating action bar to " + title);
        ActionBar actionBar = getActionBar();

        actionBar.setTitle(title != null ? title : getTitle());
        actionBar.setNavigationMode(tabs ? ActionBar.NAVIGATION_MODE_TABS : ActionBar.NAVIGATION_MODE_STANDARD);
        actionBar.setDisplayHomeAsUpEnabled(!tabs);

        updateActionBar(add, next, done);
    }

    protected void updateActionBar(boolean add, boolean next, boolean done) {
        if (addActionMenuItem != null) {
            addActionMenuItem.setVisible(add);
        }

        if (nextActionMenuItem != null) {
            nextActionMenuItem.setVisible(next);
        }

        if (doneActionMenuItem != null) {
            doneActionMenuItem.setVisible(done);
        }
    }

    public void setActionBarNextEnabled(boolean isEnabled) {
        if (nextActionMenuItem != null) {
            nextActionMenuItem.setEnabled(isEnabled);
        }
    }

    public void setActionBarDoneEnabled(boolean isEnabled) {
        if (doneActionMenuItem != null) {
            doneActionMenuItem.setEnabled(isEnabled);
        }
    }

    public void togglePower(SampleAppActivity.Type type, String itemID) {
        // determines the action to take, based on the type
        switch (type) {
        case LAMP:
            LampDataModel lampModel = lampModels.get(itemID);
            if (lampModel != null) {
                // raise brightness to 25% if needed
                if (!lampModel.state.getOnOff() && lampModel.state.getBrightness() == 0) {
                    setBrightness(type, itemID, 25);
                }

                Log.d(SampleAppActivity.TAG, "Toggle power for " + itemID);

                AllJoynManager.lampManager.transitionLampStateOnOffField(lampModel.id, !lampModel.state.getOnOff());
            }
            break;

        case GROUP:
            GroupDataModel groupModel = groupModels.get(itemID);
            if (groupModel != null) {
                // raise brightness to 25% if needed
                if (!groupModel.state.getOnOff() && groupModel.state.getBrightness() == 0) {
                    setBrightness(type, itemID, 25);
                }

                Log.d(SampleAppActivity.TAG, "Toggle power for " + itemID);

                // Group fields cannot be read back directly, so set it here
                groupModel.state.setOnOff(!groupModel.state.getOnOff());

                AllJoynManager.groupManager.transitionLampGroupStateOnOffField(groupModel.id, groupModel.state.getOnOff());
            }
            break;

        case SCENE:
        case ELEMENT:

            break;

        }
    }

    private boolean allowFieldChange() {
        boolean allow = false;
        long currentTimeMillis = Calendar.getInstance().getTimeInMillis();

        if (currentTimeMillis - lastFieldChangeMillis > FIELD_CHANGE_HOLDOFF) {
            lastFieldChangeMillis = currentTimeMillis;
            allow = true;
        }

        return allow;
    }

    public void setBrightness(SampleAppActivity.Type type, String itemID, int viewBrightness) {
        long modelBrightness = DimmableItemScaleConverter.convertBrightnessViewToModel(viewBrightness);

        Log.d(SampleAppActivity.TAG, "Set brightness for " + itemID + " to " + viewBrightness + "(" + modelBrightness + ")");

        // determines the action to take, based on the type
        if (allowFieldChange()) {
            switch (type) {
                case LAMP:
                    LampDataModel lampModel = lampModels.get(itemID);
                    if (lampModel != null) {
                        AllJoynManager.lampManager.transitionLampStateBrightnessField(itemID, modelBrightness, FIELD_TRANSITION_DURATION);

                        if (viewBrightness == 0) {
                            AllJoynManager.lampManager.transitionLampStateOnOffField(lampModel.id, false);
                        } else {
                            if (DimmableItemScaleConverter.convertBrightnessModelToView(lampModel.state.getBrightness()) == 0 && lampModel.state.getOnOff() == false) {
                                AllJoynManager.lampManager.transitionLampStateOnOffField(lampModel.id, true);
                            }
                        }
                    }
                    break;

                case GROUP:
                    GroupDataModel groupModel = groupModels.get(itemID);
                    if (groupModel != null) {
                        AllJoynManager.groupManager.transitionLampGroupStateBrightnessField(itemID, modelBrightness, FIELD_TRANSITION_DURATION);

                        if (viewBrightness == 0) {
                            AllJoynManager.groupManager.transitionLampGroupStateOnOffField(groupModel.id, false);
                        } else {
                            if (DimmableItemScaleConverter.convertBrightnessModelToView(groupModel.state.getBrightness()) == 0 && groupModel.state.getOnOff() == false) {
                                AllJoynManager.groupManager.transitionLampGroupStateOnOffField(groupModel.id, true);
                            }
                        }

                        // Group fields cannot be read back directly, so set it here
                        groupModel.state.setBrightness(modelBrightness);
                    }
                    break;

                case SCENE:
                case ELEMENT:
                    break;

            }
        }
    }

    public void setHue(SampleAppActivity.Type type, String itemID, int viewHue) {
        long modelHue = DimmableItemScaleConverter.convertHueViewToModel(viewHue);

        Log.d(SampleAppActivity.TAG, "Set hue for " + itemID + " to " + viewHue + "(" + modelHue + ")");

        // determines the action to take, based on the type
        if (allowFieldChange()) {
            switch (type) {
            case LAMP:
                LampDataModel lampModel = lampModels.get(itemID);
                if (lampModel != null) {
                    AllJoynManager.lampManager.transitionLampStateHueField(itemID, modelHue, FIELD_TRANSITION_DURATION);
                }
                break;

            case GROUP:
                GroupDataModel groupModel = groupModels.get(itemID);
                if (groupModel != null) {
                    // Group fields cannot be read back directly, so set it here
                    groupModel.state.setHue(modelHue);

                    AllJoynManager.groupManager.transitionLampGroupStateHueField(itemID, modelHue, FIELD_TRANSITION_DURATION);
                }
                break;

            case SCENE:
            case ELEMENT:
                break;

            }
        }
    }

    public void setSaturation(SampleAppActivity.Type type, String itemID, int viewSaturation) {
        long modelSaturation = DimmableItemScaleConverter.convertSaturationViewToModel(viewSaturation);

        Log.d(SampleAppActivity.TAG, "Set saturation for " + itemID + " to " + viewSaturation + "(" + modelSaturation + ")");

        // determines the action to take, based on the type
        if (allowFieldChange()) {
            switch (type) {
            case LAMP:
                LampDataModel lampModel = lampModels.get(itemID);
                if (lampModel != null) {
                    AllJoynManager.lampManager.transitionLampStateSaturationField(itemID, modelSaturation, FIELD_TRANSITION_DURATION);
                }
                break;

            case GROUP:
                GroupDataModel groupModel = groupModels.get(itemID);
                if (groupModel != null) {
                    // Group fields cannot be read back directly, so set it here
                    groupModel.state.setSaturation(modelSaturation);

                    AllJoynManager.groupManager.transitionLampGroupStateSaturationField(itemID, modelSaturation, FIELD_TRANSITION_DURATION);
                }
                break;

            case SCENE:
            case ELEMENT:
                break;

            }
        }
    }

    public void setColorTemp(SampleAppActivity.Type type, String itemID, int viewColorTemp) {
        long modelColorTemp = DimmableItemScaleConverter.convertColorTempViewToModel(viewColorTemp);

        Log.d(SampleAppActivity.TAG, "Set color temp for " + itemID + " to " + viewColorTemp + "(" + modelColorTemp + ")");

        // determines the action to take, based on the type
        if (allowFieldChange()) {
            switch(type) {
            case LAMP:
                LampDataModel lampModel = lampModels.get(itemID);
                if (lampModel != null) {
                    AllJoynManager.lampManager.transitionLampStateColorTempField(itemID, modelColorTemp, FIELD_TRANSITION_DURATION);
                }
                break;

            case GROUP:
                GroupDataModel groupModel = groupModels.get(itemID);
                if (groupModel != null) {
                    // Group fields cannot be read back directly, so set it here
                    groupModel.state.setColorTemp(modelColorTemp);

                    AllJoynManager.groupManager.transitionLampGroupStateColorTempField(itemID, modelColorTemp, FIELD_TRANSITION_DURATION);
                }
                break;

            case SCENE:
            case ELEMENT:
                break;
            }
        }
    }
}
