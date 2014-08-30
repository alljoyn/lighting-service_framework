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

import java.util.Map;

import org.alljoyn.about.AboutKeys;
import org.alljoyn.bus.Variant;
import org.allseen.lsf.ControllerClientCallback;
import org.allseen.lsf.ErrorCode;

import android.os.Handler;
import android.os.SystemClock;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.util.Log;

public class SampleControllerClientCallback extends ControllerClientCallback {
    protected SampleAppActivity activity;
    protected FragmentManager fragmentManager;
    protected Handler handler;

    public SampleControllerClientCallback(SampleAppActivity activity, FragmentManager fragmentManager, Handler handler) {
        super();

        this.activity = activity;
        this.fragmentManager = fragmentManager;
        this.handler = handler;
    }

    @Override
    public void connectedToControllerServiceCB(String controllerServiceDeviceID, String controllerServiceName) {
        Log.d(SampleAppActivity.TAG, "Connection succeeded: " + controllerServiceName + " (" + controllerServiceDeviceID + ")");
        AllJoynManager.controllerConnected = true;
        updateUI();

        // Update lamp IDs
        new ControllerMaintenance(activity);

        // Update all other object IDs
        postUpdateControllerID(controllerServiceDeviceID, controllerServiceName, 0);
        postGetAllLampGroupIDs();
        postGetAllPresetIDs();
        postGetAllBasicSceneIDs();
        postGetAllMasterSceneIDs();
    }

    @Override
    public void connectToControllerServiceFailedCB(String controllerServiceDeviceID, String controllerServiceName) {
        Log.w(SampleAppActivity.TAG, "Connection failed: " + controllerServiceName + " (" + controllerServiceDeviceID + ")");
        AllJoynManager.controllerConnected = false;
        updateUI();
    }

    @Override
    public void disconnectedFromControllerServiceCB(String controllerServiceDeviceID, String controllerServiceName) {
        Log.d(SampleAppActivity.TAG, "Disconnected: " + controllerServiceName + " (" + controllerServiceDeviceID + ")");
        AllJoynManager.controllerConnected = false;
        updateUI();
    }

    @Override
    public void controllerClientErrorCB(ErrorCode[] errorCodeList) {
        for (ErrorCode ec : errorCodeList) {
            if (!ec.equals(ErrorCode.NONE)) {
                activity.showErrorResponseCode(ec, "getAllSceneIDsReplyCB");
            }
        }
    }

    public void postUpdateControllerID(final String controllerID, final Map<String, Variant> announceData, int delay) {
        String controllerName = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_DEVICE_NAME, announceData, null);

        postUpdateControllerID(controllerID, controllerName, delay);
    }

    public void postUpdateControllerID(final String controllerID, final String controllerName, int delay) {
        Log.d(SampleAppActivity.TAG, "Updating ID " + controllerID);
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                Log.d(SampleAppActivity.TAG, "update controller model " + controllerID);

                if (activity.leaderControllerModel == null) {
                    activity.leaderControllerModel = new ControllerDataModel(controllerID, controllerName);
                    updateUI();
                } else if (activity.leaderControllerModel.id.equals(controllerID)){
                    activity.leaderControllerModel.name = controllerName;
                    updateUI();
                }

                // update the timestamp
                activity.leaderControllerModel.timestamp = SystemClock.elapsedRealtime();
            }
        }, delay);
    }

    protected void postGetAllLampGroupIDs() {
        handler.postDelayed((new Runnable() {
            @Override
            public void run() {
                AllJoynManager.groupManager.getAllLampGroupIDs();
            }
        }), 100);
    }

    protected void postGetAllPresetIDs() {
        handler.postDelayed((new Runnable() {
            @Override
            public void run() {
                AllJoynManager.presetManager.getAllPresetIDs();
            }
        }), 200);
    }

    protected void postGetAllBasicSceneIDs() {
        handler.postDelayed((new Runnable() {
            @Override
            public void run() {
                AllJoynManager.sceneManager.getAllSceneIDs();
            }
        }), 300);
    }

    protected void postGetAllMasterSceneIDs() {
        handler.postDelayed((new Runnable() {
            @Override
            public void run() {
                AllJoynManager.masterSceneManager.getAllMasterSceneIDs();
            }
        }), 400);
    }

    protected void updateUI() {
        // if connection status is ever changed, then prompt for updating the loading information
        handler.post(new Runnable() {
            @Override
            public void run() {
                Fragment lampsPageFragment = fragmentManager.findFragmentByTag(LampsPageFragment.TAG);
                Fragment groupsPageFragment = fragmentManager.findFragmentByTag(GroupsPageFragment.TAG);
                Fragment scenesPageFragment = fragmentManager.findFragmentByTag(ScenesPageFragment.TAG);
                Fragment settingsFragment = null;

                if (lampsPageFragment != null) {
                    ScrollableTableFragment tableFragment = (ScrollableTableFragment) lampsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_TABLE);
                    if (tableFragment != null) {
                        tableFragment.updateLoading();
                    }

                    if (settingsFragment == null) {
                        settingsFragment = lampsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_SETTINGS);
                    }
                }

                if (groupsPageFragment != null) {
                    ScrollableTableFragment tableFragment = (ScrollableTableFragment) groupsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_TABLE);
                    if (tableFragment != null) {
                        tableFragment.updateLoading();
                    }

                    if (settingsFragment == null) {
                        settingsFragment = groupsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_SETTINGS);
                    }
                }

                if (scenesPageFragment != null) {
                    ScrollableTableFragment tableFragment = (ScrollableTableFragment) scenesPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_TABLE);
                    if (tableFragment != null) {
                        tableFragment.updateLoading();
                    }

                    if (settingsFragment == null) {
                        settingsFragment = scenesPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_SETTINGS);
                    }
                }

                if (settingsFragment != null) {
                    ((SettingsFragment)settingsFragment).onUpdateView();
                }
            }
        });
    }
}
