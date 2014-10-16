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
package org.allseen.lsf.helper.callback;

import java.util.Iterator;

import org.allseen.lsf.LampGroup;
import org.allseen.lsf.LampGroupManagerCallback;
import org.allseen.lsf.ResponseCode;
import org.allseen.lsf.helper.facade.Group;
import org.allseen.lsf.helper.manager.AllJoynManager;
import org.allseen.lsf.helper.manager.GroupCollectionManager;
import org.allseen.lsf.helper.manager.LightingSystemManager;
import org.allseen.lsf.helper.manager.SampleGroupManager;
import org.allseen.lsf.helper.model.GroupDataModel;
import org.allseen.lsf.helper.model.LampCapabilities;
import org.allseen.lsf.helper.model.LampDataModel;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class HelperGroupManagerCallback extends LampGroupManagerCallback {
    public LightingSystemManager director;

    protected String flattenTriggerGroupID;

    public HelperGroupManagerCallback(LightingSystemManager director) {
        super();

        this.director = director;
    }

    public void refreshAllLampGroupIDs() {
        int count = director.getGroupCollectionManager().size();

        if (count > 0) {
            getAllLampGroupIDsReplyCB(ResponseCode.OK, director.getGroupCollectionManager().getIDArray());
        }
    }

    @Override
    public void getAllLampGroupIDsReplyCB(ResponseCode responseCode, String[] groupIDs) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getGroupCollectionManager().sendErrorEvent("getAllLampGroupIDsReplyCB", responseCode);
        }

        postProcessLampGroupIDs(groupIDs);
        postProcessLampGroupID(SampleGroupManager.ALL_LAMPS_GROUP_ID);

        for (final String groupID : groupIDs) {
            postProcessLampGroupID(groupID);
        }
    }

    @Override
    public void getLampGroupNameReplyCB(ResponseCode responseCode, String groupID, String language, String groupName) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getGroupCollectionManager().sendErrorEvent("getLampGroupNameReplyCB", responseCode, groupID);
        }

        postUpdateLampGroupName(groupID, groupName);
    }

    @Override
    public void setLampGroupNameReplyCB(ResponseCode responseCode, String groupID, String language) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getGroupCollectionManager().sendErrorEvent("setLampGroupNameReplyCB", responseCode, groupID);
        }

        AllJoynManager.groupManager.getLampGroupName(groupID, LightingSystemManager.LANGUAGE);
    }

    @Override
    public void lampGroupsNameChangedCB(final String[] lampGroupIDs) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                boolean containsNewIDs = false;

                for (final String groupID : lampGroupIDs) {
                    if (director.getGroupCollectionManager().hasID(groupID)) {
                        AllJoynManager.groupManager.getLampGroupName(groupID, LightingSystemManager.LANGUAGE);
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
    public void createLampGroupReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getGroupCollectionManager().sendErrorEvent("createLampGroupReplyCB", responseCode, groupID);
        }

        postProcessLampGroupID(groupID);
    }

    @Override
    public void lampGroupsCreatedCB(String[] groupIDs) {
        AllJoynManager.groupManager.getAllLampGroupIDs();
    }

    @Override
    public void getLampGroupReplyCB(ResponseCode responseCode, String groupID, LampGroup lampGroup) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getGroupCollectionManager().sendErrorEvent("getLampGroupReplyCB", responseCode, groupID);
        }

        postUpdateLampGroup(groupID, lampGroup);
    }

    @Override
    public void deleteLampGroupReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getGroupCollectionManager().sendErrorEvent("deleteLampGroupReplyCB", responseCode, groupID);
        }
    }

    @Override
    public void lampGroupsDeletedCB(String[] groupIDs) {
        postDeleteGroups(groupIDs);
    }

    @Override
    public void transitionLampGroupStateOnOffFieldReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getGroupCollectionManager().sendErrorEvent("transitionLampGroupStateOnOffFieldReplyCB", responseCode, groupID);
        }

        postSendGroupChanged(groupID);
        postUpdateLampGroupMemberLamps(groupID);
    }

    @Override
    public void transitionLampGroupStateHueFieldReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getGroupCollectionManager().sendErrorEvent("transitionLampGroupStateHueFieldReplyCB", responseCode, groupID);
        }

        postSendGroupChanged(groupID);
        postUpdateLampGroupMemberLamps(groupID);
    }

    @Override
    public void transitionLampGroupStateSaturationFieldReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getGroupCollectionManager().sendErrorEvent("transitionLampGroupStateSaturationFieldReplyCB", responseCode, groupID);
        }

        postSendGroupChanged(groupID);
        postUpdateLampGroupMemberLamps(groupID);
    }

    @Override
    public void transitionLampGroupStateBrightnessFieldReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getGroupCollectionManager().sendErrorEvent("transitionLampGroupStateBrightnessFieldReplyCB", responseCode, groupID);
        }

        postSendGroupChanged(groupID);
        postUpdateLampGroupMemberLamps(groupID);
    }

    @Override
    public void transitionLampGroupStateColorTempFieldReplyCB(ResponseCode responseCode, String groupID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getGroupCollectionManager().sendErrorEvent("transitionLampGroupStateColorTempFieldReplyCB", responseCode, groupID);
        }

        postSendGroupChanged(groupID);
        postUpdateLampGroupMemberLamps(groupID);
    }

    @Override
    public void lampGroupsUpdatedCB(String[] groupIDs) {
        for (String groupID : groupIDs) {
            AllJoynManager.groupManager.getLampGroup(groupID);
        }
    }

    protected void postProcessLampGroupIDs(String[] groupIDs) {
        final String lastGroupID = groupIDs.length > 0 ? groupIDs[groupIDs.length - 1] : SampleGroupManager.ALL_LAMPS_GROUP_ID;

        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                flattenTriggerGroupID = lastGroupID;
            }
        });
    }

    protected void postProcessLampGroupID(final String groupID) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                if (!director.getGroupCollectionManager().hasID(groupID)) {
                    postUpdateLampGroupID(groupID);
                    AllJoynManager.groupManager.getLampGroupName(groupID, LightingSystemManager.LANGUAGE);
                    AllJoynManager.groupManager.getLampGroup(groupID);
                } else if (groupID.equals(flattenTriggerGroupID)) {
                    postFlattenLampGroups();
                }
            }
        });
    }

    protected void postUpdateLampGroupID(final String groupID) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                if (!director.getGroupCollectionManager().hasID(groupID)) {
                    director.getGroupCollectionManager().addGroup(groupID);
                }
            }
        });

        postSendGroupChanged(groupID);
    }

    protected void postUpdateLampGroupName(final String groupID, final String groupName) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = director.getGroupCollectionManager().getModel(groupID);

                if (groupModel != null) {
                    groupModel.setName(groupName);
                }
            }
        });

        postSendGroupChanged(groupID);
    }

    protected void postUpdateLampGroup(final String groupID, final LampGroup lampGroup) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = director.getGroupCollectionManager().getModel(groupID);

                if (groupModel != null) {
                    groupModel.members = lampGroup;
                }

                if (groupID.equals(flattenTriggerGroupID)) {
                    postFlattenLampGroups();
                }
            }
        });

        postSendGroupChanged(groupID);
    }

    protected void postFlattenLampGroups() {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                GroupCollectionManager groupCollectionManager = director.getGroupCollectionManager();

                groupCollectionManager.flattenGroups();

                for (Iterator<Group> i = groupCollectionManager.getGroupIterator(); i.hasNext();) {
                    postUpdateLampGroupState(i.next().getGroupDataModel());
                }
            }
        });
    }

    protected void postUpdateLampGroupState(final GroupDataModel groupModel) {
        director.getHandler().post(new Runnable() {
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
                LampCapabilities capability = new LampCapabilities();

                for (String lampID : groupModel.getLamps()) {
                    LampDataModel lampModel = director.getLampCollectionManager().getModel(lampID);

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
                    } else {
                        //TODO-FIX Log.d(SampleAppActivity.TAG, "missing lamp: " + lampID);
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
            }
        });

        postSendGroupChanged(groupModel.id);
    }

    protected void postUpdateLampGroupMemberLamps(final String groupID) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                GroupDataModel groupModel = director.getGroupCollectionManager().getModel(groupID);

                if (groupModel != null) {
                    for (String lampID : groupModel.getLamps()) {
                        AllJoynManager.lampManager.getLampState(lampID);
                    }
                }
            }
        });
    }

    protected void postDeleteGroups(final String[] groupIDs) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                for (String groupID : groupIDs) {
                    director.getGroupCollectionManager().removeGroup(groupID);
                }
            }
        });
    }

    protected void postSendGroupChanged(final String groupID) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                director.getGroupCollectionManager().sendChangedEvent(groupID);
            }
        });
    }
}
