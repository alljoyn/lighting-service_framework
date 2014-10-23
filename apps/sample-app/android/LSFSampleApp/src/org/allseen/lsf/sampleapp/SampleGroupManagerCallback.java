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

import java.util.HashSet;
import java.util.Set;

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

    protected final ColorAverager averageHue = new ColorAverager();
    protected final ColorAverager averageSaturation = new ColorAverager();
    protected final ColorAverager averageBrightness = new ColorAverager();
    protected final ColorAverager averageColorTemp = new ColorAverager();

    protected Set<String> groupIDsWithPendingMembers = new HashSet<String>();
    protected Set<String> groupIDsWithPendingFlatten = new HashSet<String>();

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
        Log.d(SampleAppActivity.TAG, "getAllLampGroupIDsReplyCB()");
        if (!rc.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(rc, "getAllLampGroupIDsReplyCB");
        }

        postProcessLampGroupID(AllLampsDataModel.ALL_LAMPS_GROUP_ID, true, true);

        for (final String groupID : groupIDs) {
            postProcessLampGroupID(groupID, true, true);
        }
    }

    @Override
    public void getLampGroupNameReplyCB(ResponseCode rc, String groupID, String language, String groupName) {
        Log.d(SampleAppActivity.TAG, "getLampGroupNameReplyCB(): " + groupID + ": " + groupName);
        if (!rc.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(rc, "getLampGroupNameReplyCB");
            AllJoynManager.groupManager.getLampGroupName(groupID, SampleAppActivity.LANGUAGE);
        } else {
            postUpdateLampGroupName(groupID, groupName);
        }
    }

    @Override
    public void setLampGroupNameReplyCB(ResponseCode responseCode, String lampGroupID, String language) {
        Log.d(SampleAppActivity.TAG, "setLampGroupNameReplyCB(): " + lampGroupID);
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "setLampGroupNameReplyCB");
        }

        AllJoynManager.groupManager.getLampGroupName(lampGroupID, SampleAppActivity.LANGUAGE);
    }

    @Override
    public void lampGroupsNameChangedCB(final String[] lampGroupIDs) {
        Log.d(SampleAppActivity.TAG, "lampGroupsNameChangedCB(): " + lampGroupIDs.length);
        for (String groupID : lampGroupIDs) {
            postProcessLampGroupID(groupID, true, false);
        }
    }

    @Override
    public void createLampGroupReplyCB(ResponseCode responseCode, String lampGroupID) {
        Log.d(SampleAppActivity.TAG, "createLampGroupReplyCB(): " + lampGroupID);
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "createLampGroupReplyCB");
        }
    }

    @Override
    public void lampGroupsCreatedCB(String[] groupIDs) {
        Log.d(SampleAppActivity.TAG, "lampGroupsCreatedCB(): " + groupIDs.length);
        for (final String groupID : groupIDs) {
            postProcessLampGroupID(groupID, true, true);
        }
    }

    @Override
    public void getLampGroupReplyCB(ResponseCode responseCode, String groupID, LampGroup lampGroup) {
        Log.d(SampleAppActivity.TAG, "getLampGroupReplyCB(): " + groupID);
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "getLampGroupReplyCB");
            AllJoynManager.groupManager.getLampGroup(groupID);
        } else {
            postUpdateLampGroup(groupID, lampGroup);
        }
    }

    @Override
    public void deleteLampGroupReplyCB(ResponseCode responseCode, String lampGroupID) {
        Log.d(SampleAppActivity.TAG, "deleteLampGroupReplyCB(): " + lampGroupID);
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "deleteLampGroupReplyCB");
        }
    }

    @Override
    public void lampGroupsDeletedCB(String[] groupIDs) {
        Log.d(SampleAppActivity.TAG, "lampGroupsDeletedCB(): " + groupIDs.length);
        postDeleteGroups(groupIDs);
    }

    @Override
    public void transitionLampGroupStateOnOffFieldReplyCB(ResponseCode responseCode, String groupID) {
        Log.d(SampleAppActivity.TAG, "transitionLampGroupStateOnOffFieldReplyCB(): " + groupID);
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampGroupStateOnOffFieldReplyCB");
        }
    }

    @Override
    public void transitionLampGroupStateHueFieldReplyCB(ResponseCode responseCode, String groupID) {
        Log.d(SampleAppActivity.TAG, "transitionLampGroupStateHueFieldReplyCB(): " + groupID);
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampGroupStateHueFieldReplyCB");
        }
    }

    @Override
    public void transitionLampGroupStateSaturationFieldReplyCB(ResponseCode responseCode, String groupID) {
        Log.d(SampleAppActivity.TAG, "transitionLampGroupStateSaturationFieldReplyCB(): " + groupID);
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampGroupStateSaturationFieldReplyCB");
        }
    }

    @Override
    public void transitionLampGroupStateBrightnessFieldReplyCB(ResponseCode responseCode, String groupID) {
        Log.d(SampleAppActivity.TAG, "transitionLampGroupStateBrightnessFieldReplyCB(): " + groupID);
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampGroupStateBrightnessFieldReplyCB");
        }
    }

    @Override
    public void transitionLampGroupStateColorTempFieldReplyCB(ResponseCode responseCode, String groupID) {
        Log.d(SampleAppActivity.TAG, "transitionLampGroupStateColorTempFieldReplyCB(): " + groupID);
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampGroupStateColorTempFieldReplyCB");
        }
    }

    @Override
    public void lampGroupsUpdatedCB(String[] groupIDs) {
        Log.d(SampleAppActivity.TAG, "lampGroupsUpdatedCB(): " + groupIDs.length);
        for (String groupID : groupIDs) {
            postProcessLampGroupID(groupID, false, true);
        }
    }

    protected void postProcessLampGroupID(final String groupID, final boolean needName, final boolean needState) {
        Log.d(SampleAppActivity.TAG, "postProcessLampGroupID(): " + groupID);
        handler.post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = activity.groupModels.get(groupID);
                boolean getName = needName;
                boolean getState = needState;

                if (groupModel == null) {
                    Log.d(SampleAppActivity.TAG, "new group: " + groupID);

                    groupModel = groupID != AllLampsDataModel.ALL_LAMPS_GROUP_ID ? new GroupDataModel(groupID) : new AllLampsDataModel();

                    getName = true;
                    getState = true;

                    activity.groupModels.put(groupID, groupModel);
                }

                if (getName) {
                    AllJoynManager.groupManager.getLampGroupName(groupID, SampleAppActivity.LANGUAGE);
                }

                if (getState) {
                    groupIDsWithPendingMembers.add(groupID);
                    AllJoynManager.groupManager.getLampGroup(groupID);
                }
            }
        });
    }

    protected void postUpdateLampGroupName(final String groupID, final String groupName) {
        Log.d(SampleAppActivity.TAG, "postUpdateLampGroupName() " + groupID + ", " +  groupName);
        handler.post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = activity.groupModels.get(groupID);

                if (groupModel != null) {
                    groupModel.setName(groupName);
                }
            }
        });

        postUpdateGroupUI(groupID);
    }

    protected void postUpdateLampGroup(final String groupID, final LampGroup lampGroup) {
        Log.d(SampleAppActivity.TAG, "postUpdateLampGroup(): " + groupID);
        handler.post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = activity.groupModels.get(groupID);

                groupIDsWithPendingMembers.remove(groupID);

                if (groupModel != null) {
                    groupModel.members = lampGroup;

                    groupIDsWithPendingFlatten.add(groupID);

                    if (groupIDsWithPendingMembers.size() == 0) {
                        for (String groupID : groupIDsWithPendingFlatten) {
                            postFlattenLampGroup(groupID);
                        }

                        groupIDsWithPendingFlatten.clear();

                        postUpdateLampGroupState(groupModel);
                    }
                } else {
                    Log.e(SampleAppActivity.TAG, "postUpdateLampGroup(): group not found: " + groupID);
                }
            }
        });
    }

    protected void postFlattenLampGroup(final String groupID) {
        Log.d(SampleAppActivity.TAG, "postFlattenLampGroup()");
        handler.post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = activity.groupModels.get(groupID);

                if (groupModel != null) {
                    new GroupsFlattener().flattenGroup(activity.groupModels, groupModel);
                }
            }
        });
    }

    public void postUpdateDependentLampGroups(final String lampID) {
        Log.d(SampleAppActivity.TAG, "postUpdateDependentLampGroups(): " + lampID);
        handler.post(new Runnable() {
            @Override
            public void run() {
                for (GroupDataModel groupModel : activity.groupModels.values()) {
                    Set<String> lampIDs = groupModel.getLamps();

                    if (lampIDs != null && lampIDs.contains(lampID)) {
                        postUpdateLampGroupState(groupModel);
                    }
                }
            }
        });
    }

    protected void postUpdateLampGroupState(final GroupDataModel groupModel) {
        Log.d(SampleAppActivity.TAG, "postUpdateLampGroupState(): " + groupModel.id);
        handler.post(new Runnable() {
            @Override
            public void run() {
                CapabilityData capability = new CapabilityData();
                int countOn = 0;
                int countOff = 0;

                averageHue.reset();
                averageSaturation.reset();
                averageBrightness.reset();
                averageColorTemp.reset();

                for (String lampID : groupModel.getLamps()) {
                    LampDataModel lampModel = activity.lampModels.get(lampID);

                    if (lampModel != null) {
                        capability.includeData(lampModel.getCapability());

                        if ( lampModel.state.getOnOff()) {
                            countOn++;
                        } else {
                            countOff++;
                        }

                        if (lampModel.getDetails().hasColor()) {
                            averageHue.add(lampModel.state.getHue());
                            averageSaturation.add(lampModel.state.getSaturation());
                        }

                        if (lampModel.getDetails().isDimmable()) {
                            averageBrightness.add(lampModel.state.getBrightness());
                        }

                        if (lampModel.getDetails().hasVariableColorTemp()) {
                            averageColorTemp.add(lampModel.state.getColorTemp());
                        }
                    } else {
                        Log.d(SampleAppActivity.TAG, "missing lamp: " + lampID);
                    }
                }

                groupModel.setCapability(capability);

                groupModel.state.setOnOff(countOn > 0);
                groupModel.state.setHue(averageHue.getAverage());
                groupModel.state.setSaturation(averageSaturation.getAverage());
                groupModel.state.setBrightness(averageBrightness.getAverage());
                groupModel.state.setColorTemp(averageColorTemp.getAverage());

                groupModel.uniformity.power = countOn == 0 || countOff == 0;
                groupModel.uniformity.hue = averageHue.isUniform();
                groupModel.uniformity.saturation = averageSaturation.isUniform();
                groupModel.uniformity.brightness = averageBrightness.isUniform();
                groupModel.uniformity.colorTemp = averageColorTemp.isUniform();

                Log.d(SampleAppActivity.TAG, "updating group " + groupModel.getName() + " - " + groupModel.getCapability().toString());
            }
        });

        postUpdateGroupUI(groupModel.id);
    }

    protected void postDeleteGroups(final String[] groupIDs) {
        Log.d(SampleAppActivity.TAG, "postDeleteGroups(): " + groupIDs.length);
        handler.post(new Runnable() {
            @Override
            public void run() {
                Fragment pageFragment = fragmentManager.findFragmentByTag(GroupsPageFragment.TAG);
                FragmentManager childManager = pageFragment != null ? pageFragment.getChildFragmentManager() : null;
                GroupsTableFragment tableFragment = childManager != null ? (GroupsTableFragment)childManager.findFragmentByTag(PageFrameParentFragment.CHILD_TAG_TABLE) : null;
                GroupInfoFragment infoFragment = childManager != null ? (GroupInfoFragment)childManager.findFragmentByTag(PageFrameParentFragment.CHILD_TAG_INFO) : null;

                for (String groupID : groupIDs) {
                    String name = activity.groupModels.get(groupID).getName();
                    activity.groupModels.remove(groupID);

                    if (tableFragment != null) {
                        tableFragment.removeElement(groupID);
                    }

                    if ((infoFragment != null) && (infoFragment.key.equals(groupID))) {
                        activity.createLostConnectionErrorDialog(name);
                    }
                }
            }
        });
    }

    protected void postUpdateGroupUI(final String groupID) {
        Log.d(SampleAppActivity.TAG, "postUpdateGroupUI(): " + groupID);
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
