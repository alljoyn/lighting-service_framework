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
        postUpdateControllerDisplay();

        // Update lamp IDs
        if (SampleAppActivity.POLLING_DISTRIBUTED) {
            AllJoynManager.lampManager.getAllLampIDs();
        } else {
            new ControllerMaintenance(activity);
        }

        // Update all other object IDs
        postOnControllerConnected(controllerServiceDeviceID, controllerServiceName, 0);
        postGetAllLampGroupIDs();
        postGetAllPresetIDs();
        postGetAllBasicSceneIDs();
        postGetAllMasterSceneIDs();
    }

    @Override
    public void connectToControllerServiceFailedCB(String controllerServiceDeviceID, String controllerServiceName) {
        Log.w(SampleAppActivity.TAG, "Connection failed: " + controllerServiceName + " (" + controllerServiceDeviceID + ")");
        AllJoynManager.controllerConnected = false;
        postOnControllerDisconnected(controllerServiceDeviceID, controllerServiceName, 0);
    }

    @Override
    public void disconnectedFromControllerServiceCB(String controllerServiceDeviceID, String controllerServiceName) {
        Log.d(SampleAppActivity.TAG, "Disconnected: " + controllerServiceName + " (" + controllerServiceDeviceID + ")");
        AllJoynManager.controllerConnected = false;
        postOnControllerDisconnected(controllerServiceDeviceID, controllerServiceName, 0);
    }

    @Override
    public void controllerClientErrorCB(ErrorCode[] errorCodeList) {
        for (ErrorCode ec : errorCodeList) {
            if (!ec.equals(ErrorCode.NONE)) {
                activity.showErrorResponseCode(ec, "getAllSceneIDsReplyCB");
            }
        }
    }

    public void postOnControllerConnected(final String controllerID, final String controllerName, int delay) {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (activity.leaderControllerModel == null) {
                    activity.leaderControllerModel = new ControllerDataModel(controllerID, controllerName);
                } else {
                    activity.leaderControllerModel.id = controllerID;
                    activity.leaderControllerModel.setName(controllerName);
                }

                // update the timestamp
                activity.leaderControllerModel.timestamp = SystemClock.elapsedRealtime();

                postUpdateControllerDisplay();
            }
        }, delay);
    }

    public void postOnControllerDisconnected(final String controllerID, final String controllerName, int delay) {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                activity.leaderControllerModel = null;
                activity.clearModels();

                postUpdateControllerDisplay();
            }
        }, delay);
    }

    public void postOnControllerAnnouncedAboutData(final String controllerID, final String controllerName, int delay) {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (activity.leaderControllerModel != null) {
                    if (activity.leaderControllerModel.id.equals(controllerID)) {
                        activity.leaderControllerModel.setName(controllerName);
                        activity.leaderControllerModel.timestamp = SystemClock.elapsedRealtime();

                        postUpdateControllerDisplay();
                    }
                }
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

    public void postUpdateControllerDisplay() {
        // if connection status is ever changed, then prompt for updating the loading information
//        handler.post(new Runnable() {
        activity.postInForeground(new Runnable() {
            @Override
            public void run() {
                PageFrameParentFragment lampsPageFragment = (PageFrameParentFragment)fragmentManager.findFragmentByTag(LampsPageFragment.TAG);
                PageFrameParentFragment groupsPageFragment = (PageFrameParentFragment)fragmentManager.findFragmentByTag(GroupsPageFragment.TAG);
                PageFrameParentFragment scenesPageFragment = (PageFrameParentFragment)fragmentManager.findFragmentByTag(ScenesPageFragment.TAG);
                Fragment settingsFragment = null;

                if (lampsPageFragment != null) {
                    ScrollableTableFragment tableFragment = (ScrollableTableFragment) lampsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_TABLE);

                    if (!AllJoynManager.controllerConnected) {
                        lampsPageFragment.clearBackStack();
                    }

                    if (tableFragment != null) {
                        tableFragment.updateLoading();
                    }

                    if (settingsFragment == null) {
                        settingsFragment = lampsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_SETTINGS);
                    }
                }

                if (groupsPageFragment != null) {
                    ScrollableTableFragment tableFragment = (ScrollableTableFragment) groupsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_TABLE);

                    if (!AllJoynManager.controllerConnected) {
                        groupsPageFragment.clearBackStack();
                    }

                    if (tableFragment != null) {
                        tableFragment.updateLoading();
                    }

                    if (settingsFragment == null) {
                        settingsFragment = groupsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_SETTINGS);
                    }
                }

                if (scenesPageFragment != null) {
                    ScrollableTableFragment tableFragment = (ScrollableTableFragment) scenesPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_TABLE);

                    if (!AllJoynManager.controllerConnected) {
                        scenesPageFragment.clearBackStack();
                    }

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
