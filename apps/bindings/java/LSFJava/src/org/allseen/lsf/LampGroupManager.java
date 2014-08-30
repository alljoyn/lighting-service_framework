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
package org.allseen.lsf;

public class LampGroupManager extends BaseNativeClassWrapper {
    public LampGroupManager(ControllerClient controller, LampGroupManagerCallback callback) {
           createNativeObject(controller, callback);
    }

    public native ControllerClientStatus getAllLampGroupIDs();
    public native ControllerClientStatus getLampGroupName(String groupID, String language);
    public native ControllerClientStatus setLampGroupName(String groupID, String groupName, String language);
    public native ControllerClientStatus createLampGroup(LampGroup group, String groupName, String language);
    public native ControllerClientStatus updateLampGroup(String groupID, LampGroup group);
    public native ControllerClientStatus getLampGroup(String groupID);
    public native ControllerClientStatus deleteLampGroup(String groupID);
    public native ControllerClientStatus transitionLampGroupState(String groupID, LampState state, long duration);
    public native ControllerClientStatus pulseLampGroupWithState(String groupID, LampState toState, long period, long duration, long count, LampState fromState);
    public native ControllerClientStatus pulseLampGroupWithPreset(String groupID, String toPresetID, long period, long duration, long count, String fromPresetID);
    public native ControllerClientStatus transitionLampGroupStateOnOffField(String groupID, boolean onOff);
    public native ControllerClientStatus transitionLampGroupStateHueField(String groupID, long hue, long duration);
    public native ControllerClientStatus transitionLampGroupStateSaturationField(String groupID, long saturation, long duration);
    public native ControllerClientStatus transitionLampGroupStateBrightnessField(String groupID, long brightness, long duration);
    public native ControllerClientStatus transitionLampGroupStateColorTempField(String groupID, long colorTemp, long duration);
    public native ControllerClientStatus transitionLampGroupStateToPreset(String groupID, String presetID, long duration);
    public native ControllerClientStatus resetLampGroupState(String groupID);
    public native ControllerClientStatus resetLampGroupStateOnOffField(String groupID);
    public native ControllerClientStatus resetLampGroupStateHueField(String groupID);
    public native ControllerClientStatus resetLampGroupStateSaturationField(String groupID);
    public native ControllerClientStatus resetLampGroupStateBrightnessField(String groupID);
    public native ControllerClientStatus resetLampGroupStateColorTempField(String groupID);

    protected native void createNativeObject(ControllerClient controller, LampGroupManagerCallback callback);

    @Override
    protected native void destroyNativeObject();
}
