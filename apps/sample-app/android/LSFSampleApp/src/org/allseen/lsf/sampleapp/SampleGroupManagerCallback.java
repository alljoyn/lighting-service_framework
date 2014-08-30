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

import org.allseen.lsf.LampGroup;
import org.allseen.lsf.LampGroupManagerCallback;
import org.allseen.lsf.ResponseCode;

import android.os.Handler;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.util.Log;

public class SampleGroupManagerCallback extends LampGroupManagerCallback {
    protected SampleAppActivity activity;
    protected FragmentManager fragmentManager;
    protected Handler handler;

    protected String flattenTriggerGroupID;

    public SampleGroupManagerCallback(SampleAppActivity activity, FragmentManager fragmentManager, Handler handler) {
        super();

        this.activity = activity;
        this.fragmentManager = fragmentManager;
        this.handler = handler;
    }

    public void refreshAllLampGroupIDs() {
        int count = activity.groupModels.size();

        if (count > 0) {
            getAllLampGroupIDsReplyCB(ResponseCode.OK, activity.groupModels.keySet().toArray(new String[count]));
        }
    }

    @Override
    public void getAllLampGroupIDsReplyCB(ResponseCode rc, String[] groupIDs) {
        if (!rc.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(rc, "getAllLampGroupIDsReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "---------------------------");
        Log.d(SampleAppActivity.TAG, "getAllLampGroupIDsReplyCB()");
        postProcessLampGroupIDs(groupIDs);
        postProcessLampGroupID(SampleGroupManager.ALL_LAMPS_GROUP_ID);

        for (final String groupID : groupIDs) {
            postProcessLampGroupID(groupID);
        }
    }

    @Override
    public void getLampGroupNameReplyCB(ResponseCode rc, String groupID, String language, String groupName) {
        if (!rc.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(rc, "getLampGroupNameReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "getLampGroupNameReplyCB(): " + groupName);
        postUpdateLampGroupName(groupID, groupName);
    }

    @Override
    public void setLampGroupNameReplyCB(ResponseCode responseCode, String lampGroupID, String language) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "setLampGroupNameReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "setLampGroupNameReplyCB(): " + lampGroupID);
        AllJoynManager.groupManager.getLampGroupName(lampGroupID, SampleAppActivity.LANGUAGE);
    }

    @Override
    public void lampGroupsNameChangedCB(final String[] lampGroupIDs) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                boolean containsNewIDs = false;

                for (final String groupID : lampGroupIDs) {
                    if (activity.groupModels.containsKey(groupID)) {
                        Log.d(SampleAppActivity.TAG, "lampGroupsNameChangedCB(): " + groupID);
                        AllJoynManager.groupManager.getLampGroupName(groupID, SampleAppActivity.LANGUAGE);
                    } else {
                        containsNewIDs = true;
                    }
                }

                if (containsNewIDs) {
                    AllJoynManager.groupManager.getAllLampGroupIDs();
                }
            }
        });
    }

    @Override
    public void createLampGroupReplyCB(ResponseCode responseCode, String lampGroupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "createLampGroupReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "createLampGroupReplyCB(): " + lampGroupID);
        postProcessLampGroupID(lampGroupID);
    }

    @Override
    public void lampGroupsCreatedCB(String[] groupIDs) {
        AllJoynManager.groupManager.getAllLampGroupIDs();
    }

    @Override
    public void getLampGroupReplyCB(ResponseCode responseCode, String lampGroupID, LampGroup lampGroup) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "getLampGroupReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "getLampGroupReplyCB(): " + lampGroupID + ": " +  lampGroup);
        postUpdateLampGroup(lampGroupID, lampGroup);
    }

    @Override
    public void deleteLampGroupReplyCB(ResponseCode responseCode, String lampGroupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "deleteLampGroupReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "deleteLampGroupReplyCB(): " + lampGroupID);
    }

    @Override
    public void lampGroupsDeletedCB(String[] groupIDs) {
        Log.d(SampleAppActivity.TAG, "lampGroupsDeletedCB(): " + groupIDs.length);
        postDeleteGroups(groupIDs);
    }

    @Override
    public void transitionLampGroupStateOnOffFieldReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampGroupStateOnOffFieldReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "transitionLampGroupStateOnOffFieldReplyCB(): " + groupID);
        postUpdateDimmableItemTask(groupID);
        postUpdateLampGroupMemberLamps(groupID);
    }

    @Override
    public void transitionLampGroupStateHueFieldReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampGroupStateHueFieldReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "transitionLampGroupStateHueFieldReplyCB(): " + groupID);
        postUpdateDimmableItemTask(groupID);
        postUpdateLampGroupMemberLamps(groupID);
    }

    @Override
    public void transitionLampGroupStateSaturationFieldReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampGroupStateSaturationFieldReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "transitionLampGroupStateSaturationFieldReplyCB(): " + groupID);
        postUpdateDimmableItemTask(groupID);
        postUpdateLampGroupMemberLamps(groupID);
    }

    @Override
    public void transitionLampGroupStateBrightnessFieldReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampGroupStateBrightnessFieldReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "transitionLampGroupStateBrightnessFieldReplyCB(): " + groupID);
        postUpdateDimmableItemTask(groupID);
        postUpdateLampGroupMemberLamps(groupID);
    }

    @Override
    public void transitionLampGroupStateColorTempFieldReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampGroupStateColorTempFieldReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "transitionLampGroupStateColorTempFieldReplyCB(): " + groupID);
        postUpdateDimmableItemTask(groupID);
        postUpdateLampGroupMemberLamps(groupID);
    }

    @Override
    public void lampGroupsUpdatedCB(String[] groupIDs) {
        Log.d(SampleAppActivity.TAG, "lampGroupsUpdatedCB(): " + groupIDs.length);
        for (String groupID : groupIDs) {
            Log.d(SampleAppActivity.TAG, "lampGroupsUpdatedCB()" + groupID);
            AllJoynManager.groupManager.getLampGroup(groupID);
        }
    }

    protected void postProcessLampGroupIDs(String[] groupIDs) {
        final String lastGroupID = groupIDs.length > 0 ? groupIDs[groupIDs.length - 1] : SampleGroupManager.ALL_LAMPS_GROUP_ID;

        handler.post(new Runnable() {
            @Override
            public void run() {
                flattenTriggerGroupID = lastGroupID;
            }
        });
    }

    protected void postProcessLampGroupID(final String groupID) {
        Log.d(SampleAppActivity.TAG, "postProcessLampGroupID(): " + groupID);
        handler.post(new Runnable() {
            @Override
            public void run() {
                if (!activity.groupModels.containsKey(groupID)) {
                    Log.d(SampleAppActivity.TAG, "new group: " + groupID);
                    postUpdateLampGroupID(groupID);
                    AllJoynManager.groupManager.getLampGroupName(groupID, SampleAppActivity.LANGUAGE);
                    AllJoynManager.groupManager.getLampGroup(groupID);
                } else if (groupID.equals(flattenTriggerGroupID)) {
                    Log.d(SampleAppActivity.TAG, "got trigger group ID: " + flattenTriggerGroupID);
                    postFlattenLampGroups();
                }
            }
        });
    }

    protected void postUpdateLampGroupID(final String groupID) {
        Log.d(SampleAppActivity.TAG, "postUpdateLampGroupID(): " + groupID);
        handler.post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = activity.groupModels.get(groupID);

                if (groupModel == null) {
                    groupModel = new GroupDataModel(groupID);
                    activity.groupModels.put(groupID, groupModel);
                }
            }
        });

        postUpdateDimmableItemTask(groupID);
    }

    protected void postUpdateLampGroupName(final String groupID, final String groupName) {
        Log.d(SampleAppActivity.TAG, "postUpdateLampGroupName() " + groupID + ", " +  groupName);
        handler.post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = activity.groupModels.get(groupID);

                if (groupModel != null) {
                    groupModel.name = groupName;
                }
            }
        });

        postUpdateDimmableItemTask(groupID);
    }

    protected void postUpdateLampGroup(final String groupID, final LampGroup lampGroup) {
        Log.d(SampleAppActivity.TAG, "Updating lamp group state " + groupID + ": " +  lampGroup);
        handler.post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = activity.groupModels.get(groupID);

                if (groupModel != null) {
                    groupModel.members = lampGroup;
                }

                if (groupID.equals(flattenTriggerGroupID)) {
                    Log.d(SampleAppActivity.TAG, "got trigger group ID: " + flattenTriggerGroupID);
                    postFlattenLampGroups();
                }
            }
        });

        postUpdateDimmableItemTask(groupID);
    }

    protected void postFlattenLampGroups() {
        Log.d(SampleAppActivity.TAG, "postFlattenLampGroup()");
        handler.post(new Runnable() {
            @Override
            public void run() {
                new GroupsFlattener().flattenGroups(activity.groupModels);

                for (GroupDataModel groupModel : activity.groupModels.values()) {
                    postUpdateLampGroupState(groupModel);
                }
            }
        });
    }

    protected void postUpdateLampGroupState(final GroupDataModel groupModel) {
        Log.d(SampleAppActivity.TAG, "postUpdateLampGroupState()");
        handler.post(new Runnable() {
            @Override
            public void run() {
                boolean powerOn = false;
                long hue = 0;
                long saturation = 0;
                long brightness = 0;
                long colorTemp = 0;

                long countDim = 0;
                long countColor = 0;
                long countTemp = 0;
                CapabilityData capability = new CapabilityData();

                for (String lampID : groupModel.getLamps()) {
                    LampDataModel lampModel = activity.lampModels.get(lampID);

                    if (lampModel != null) {
                        capability.includeData(lampModel.getCapability());

                        powerOn = powerOn || lampModel.state.getOnOff();

                        if (lampModel.getDetails().isDimmable()) {
                            brightness = brightness + lampModel.state.getBrightness();
                            countDim++;
                        }

                        if (lampModel.getDetails().hasColor()) {
                            hue = hue + lampModel.state.getHue();
                            saturation = saturation + lampModel.state.getSaturation();
                            countColor++;
                        }

                        if (lampModel.getDetails().hasVariableColorTemp()) {
                            colorTemp = colorTemp + lampModel.state.getColorTemp();
                            countTemp++;
                        }

                        Log.d(SampleAppActivity.TAG, lampID + ": power on " + powerOn + " (" + countDim + ") brightness " + brightness + " (" + countColor + ") hue " + hue + " saturation " + saturation + " (" + countTemp + ") colorTemp " + colorTemp);
                    } else {
                        Log.d(SampleAppActivity.TAG, "missing lamp: " + lampID);
                    }
                }

                double divisorDim = countDim > 0 ? countDim : 1.0;
                double divisorColor = countColor > 0 ? countColor : 1.0;
                double divisorTemp = countTemp > 0 ? countTemp : 1.0;

                groupModel.state.setOnOff(powerOn);
                groupModel.state.setHue(Math.round(hue / divisorColor));
                groupModel.state.setSaturation(Math.round(saturation / divisorColor));
                groupModel.state.setBrightness(Math.round(brightness / divisorDim));
                groupModel.state.setColorTemp(Math.round(colorTemp / divisorTemp));
                groupModel.setCapability(capability);
                Log.d(SampleAppActivity.TAG, "updating group " + groupModel.name + " - " + groupModel.getCapability().toString());
            }
        });

        postUpdateDimmableItemTask(groupModel.id);
    }

    protected void postUpdateLampGroupMemberLamps(final String groupID) {
        handler.post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = activity.groupModels.get(groupID);

                if (groupModel != null) {
                    for (String lampID : groupModel.getLamps()) {
                        AllJoynManager.lampManager.getLampState(lampID);
                    }
                }
            }
        });
    }

    protected void postDeleteGroups(final String[] groupIDs) {
        Log.d(SampleAppActivity.TAG, "Removing deleted groups");
        handler.post(new Runnable() {
            @Override
            public void run() {
                Fragment pageFragment = fragmentManager.findFragmentByTag(GroupsPageFragment.TAG);
                FragmentManager childManager = pageFragment != null ? pageFragment.getChildFragmentManager() : null;
                GroupsTableFragment tableFragment = childManager != null ? (GroupsTableFragment)childManager.findFragmentByTag(PageFrameParentFragment.CHILD_TAG_TABLE) : null;

                for (String groupID : groupIDs) {
                    activity.groupModels.remove(groupID);

                    if (tableFragment != null) {
                        tableFragment.removeElement(groupID);
                    }
                }
            }
        });
    }

    protected void postUpdateDimmableItemTask(final String groupID) {
        Log.d(SampleAppActivity.TAG, "postUpdateDimmableItemTask(): " + groupID);
        handler.post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = activity.groupModels.get(groupID);

                if (groupModel != null) {
                    Fragment pageFragment = fragmentManager.findFragmentByTag(GroupsPageFragment.TAG);

                    if (pageFragment != null) {
                        GroupsTableFragment tableFragment = (GroupsTableFragment)pageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_TABLE);

                        if (tableFragment != null) {
                            tableFragment.addElement(groupID);
                        }

                        GroupInfoFragment infoFragment = (GroupInfoFragment)pageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_INFO);

                        if (infoFragment != null) {
                            infoFragment.updateInfoFields(groupModel);
                        }
                    }
                }
            }
        });
    }
}
