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
package org.alljoyn.lsf.sampleapp;

import org.alljoyn.lsf.LampState;
import org.alljoyn.lsf.PresetManagerCallback;
import org.alljoyn.lsf.ResponseCode;

import android.os.Handler;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.util.Log;

public class SamplePresetManagerCallback extends PresetManagerCallback {
    protected SampleAppActivity activity;
    protected FragmentManager fragmentManager;
    protected Handler handler;

    public SamplePresetManagerCallback(SampleAppActivity activity, FragmentManager fragmentManager, Handler handler) {
        super();

        this.activity = activity;
        this.fragmentManager = fragmentManager;
        this.handler = handler;
    }

    @Override
    public void getPresetReplyCB(ResponseCode responseCode, String presetID, LampState preset) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "getPresetReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "getPresetReplyCB(): " + presetID + ": " +  preset);
        postUpdatePreset(presetID, preset);
    }

    @Override
    public void getAllPresetIDsReplyCB(ResponseCode responseCode, String[] presetIDs) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "getAllPresetIDsReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "getAllPresetIDsReplyCB()");

        for (final String presetID : presetIDs) {
            postProcessPresetID(presetID);
        }
    }

    @Override
    public void getPresetNameReplyCB(ResponseCode responseCode, String presetID, String language, String presetName) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "getPresetNameReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "getPresetNameReplyCB(): " + presetName);
        postUpdatePresetName(presetID, presetName);
    }

    @Override
    public void setPresetNameReplyCB(ResponseCode responseCode, String presetID, String language) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "setPresetNameReplyCB");
        }

        // Currently nothing to do
    }

    @Override
    public void presetsNameChangedCB(final String[] presetIDs) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                boolean containsNewIDs = false;

                for (final String presetID : presetIDs) {
                    if (activity.presetModels.containsKey(presetID)) {
                        Log.d(SampleAppActivity.TAG, "presetsNameChangedCB(): " + presetID);
                        AllJoynManager.presetManager.getPresetName(presetID, SampleAppActivity.LANGUAGE);
                    } else {
                        containsNewIDs = true;
                    }
                }

                if (containsNewIDs) {
                    AllJoynManager.presetManager.getAllPresetIDs();
                }
            }
        });
    }

    @Override
    public void createPresetReplyCB(ResponseCode responseCode, String presetID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "createPresetReplyCB");
        }

        // Currently nothing to do
    }

    @Override
    public void presetsCreatedCB(String[] presetIDs) {
        AllJoynManager.presetManager.getAllPresetIDs();
    }

    @Override
    public void updatePresetReplyCB(ResponseCode responseCode, String presetID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "updatePresetReplyCB");
        }

        // Currently nothing to do
    }

    @Override
    public void presetsUpdatedCB(String[] presetIDs) {
        Log.d(SampleAppActivity.TAG, "presetsUpdatedCB(): " + presetIDs.length);
        for (String presetID : presetIDs) {
            Log.d(SampleAppActivity.TAG, "presetsUpdatedCB()" + presetID);
            AllJoynManager.presetManager.getPreset(presetID);
        }
    }

    @Override
    public void deletePresetReplyCB(ResponseCode responseCode, String presetID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "deletePresetReplyCB");
        }

        // Currently nothing to do
    }

    @Override
    public void presetsDeletedCB(String[] presetIDs) {
        Log.d(SampleAppActivity.TAG, "presetsDeletedCB(): " + presetIDs.length);
        postDeletePresets(presetIDs);
    }

    @Override
    public void getDefaultLampStateReplyCB(ResponseCode responseCode, LampState defaultLampState) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "getDefaultLampStateReplyCB");
        }

        // Currently nothing to do
    }

    @Override
    public void setDefaultLampStateReplyCB(ResponseCode responseCode) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "setDefaultLampStateReplyCB");
        }

        // Currently nothing to do
    }

    @Override
    public void defaultLampStateChangedCB() {
        // Currently nothing to do
    }

    protected void postProcessPresetID(final String presetID) {
        Log.d(SampleAppActivity.TAG, "postProcessPresetID(): " + presetID);
        handler.post(new Runnable() {
            @Override
            public void run() {
                if (!activity.presetModels.containsKey(presetID)) {
                    Log.d(SampleAppActivity.TAG, "new preset: " + presetID);
                    postUpdatePresetID(presetID);
                    AllJoynManager.presetManager.getPresetName(presetID, SampleAppActivity.LANGUAGE);
                    AllJoynManager.presetManager.getPreset(presetID);
                }
            }
        });
    }

    protected void postUpdatePresetID(final String presetID) {
        Log.d(SampleAppActivity.TAG, "postUpdateLampGroupID(): " + presetID);
        handler.post(new Runnable() {
            @Override
            public void run() {
                PresetDataModel presetModel = activity.presetModels.get(presetID);

                if (presetModel == null) {
                    presetModel = new PresetDataModel(presetID);
                    activity.presetModels.put(presetID, presetModel);
                }
            }
        });

        postUpdateInfoFragments();
    }

    protected void postUpdatePresetName(final String presetID, final String presetName) {
        Log.d(SampleAppActivity.TAG, "postUpdatePresetName() " + presetID + ", " +  presetName);
        handler.post(new Runnable() {
            @Override
            public void run() {
                PresetDataModel presetModel = activity.presetModels.get(presetID);

                if (presetModel != null) {
                    presetModel.name = presetName;
                }
            }
        });

        postUpdateInfoFragments();
    }

    protected void postUpdatePreset(final String presetID, final LampState preset) {
        Log.d(SampleAppActivity.TAG, "Updating preset " + presetID + ": " +  preset);
        handler.post(new Runnable() {
            @Override
            public void run() {
                PresetDataModel presetModel = activity.presetModels.get(presetID);

                if (presetModel != null) {
                    presetModel.state = preset;
                }
            }
        });

        postUpdateInfoFragments();
    }

    protected void postDeletePresets(final String[] presetIDs) {
        Log.d(SampleAppActivity.TAG, "Removing deleted presets");
        handler.post(new Runnable() {
            @Override
            public void run() {
                for (String presetID : presetIDs) {
                    activity.presetModels.remove(presetID);
                }
            }
        });

        postUpdateInfoFragments();
    }

    protected void postUpdateInfoFragments() {
        Log.d(SampleAppActivity.TAG, "postUpdateInfoFragments()");
        handler.post(new Runnable() {
            @Override
            public void run() {
                Fragment lampsPageFragment = fragmentManager.findFragmentByTag(LampsPageFragment.TAG);

                if (lampsPageFragment != null) {
                    LampInfoFragment lampsInfoFragment = (LampInfoFragment)lampsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_INFO);

                    if (lampsInfoFragment != null) {
                        lampsInfoFragment.updatePresetFields();
                    }
                }

                Fragment groupsPageFragment = fragmentManager.findFragmentByTag(GroupsPageFragment.TAG);

                if (groupsPageFragment != null) {
                    GroupInfoFragment groupsInfoFragment = (GroupInfoFragment)groupsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_INFO);

                    if (groupsInfoFragment != null) {
                        groupsInfoFragment.updatePresetFields();
                    }
                }
            }
        });
    }
}
