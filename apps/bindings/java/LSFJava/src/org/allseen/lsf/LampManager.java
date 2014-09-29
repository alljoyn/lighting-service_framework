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

public class LampManager extends BaseNativeClassWrapper {
    public LampManager(ControllerClient controller, LampManagerCallback callback) {
        createNativeObject(controller, callback);
    }

    public native ControllerClientStatus getAllLampIDs();
    public native ControllerClientStatus getLampManufacturer(String lampID, String language);
    public native ControllerClientStatus getLampName(String lampID, String language);
    public native ControllerClientStatus setLampName(String lampID, String lampName, String language);
    public native ControllerClientStatus getLampDetails(String lampID);
    public native ControllerClientStatus getLampParameters(String lampID);
    public native ControllerClientStatus getLampParametersEnergyUsageMilliwattsField(String lampID);
    public native ControllerClientStatus getLampParametersLumensField(String lampID);
    public native ControllerClientStatus getLampState(String lampID);
    public native ControllerClientStatus getLampStateOnOffField(String lampID);
    public native ControllerClientStatus getLampStateHueField(String lampID);
    public native ControllerClientStatus getLampStateSaturationField(String lampID);
    public native ControllerClientStatus getLampStateBrightnessField(String lampID);
    public native ControllerClientStatus getLampStateColorTempField(String lampID);
    public native ControllerClientStatus resetLampState(String lampID);
    public native ControllerClientStatus resetLampStateOnOffField(String lampID);
    public native ControllerClientStatus resetLampStateHueField(String lampID);
    public native ControllerClientStatus resetLampStateSaturationField(String lampID);
    public native ControllerClientStatus resetLampStateBrightnessField(String lampID);
    public native ControllerClientStatus resetLampStateColorTempField(String lampID);
    public native ControllerClientStatus transitionLampState(String lampID, LampState lampState, long transitionPeriod);
    public native ControllerClientStatus pulseLampWithState(String lampID, LampState toLampState, long period, long duration, long numPulses, LampState fromLampState);
    public native ControllerClientStatus pulseLampWithPreset(String lampID, String toPresetID, long period, long duration, long numPulses, String fromPresetID);
    public native ControllerClientStatus transitionLampStateOnOffField(String lampID, boolean onOff);
    public native ControllerClientStatus transitionLampStateHueField(String lampID, long hue, long transitionPeriod);
    public native ControllerClientStatus transitionLampStateSaturationField(String lampID, long saturation, long transitionPeriod);
    public native ControllerClientStatus transitionLampStateBrightnessField(String lampID, long brightness, long transitionPeriod);
    public native ControllerClientStatus transitionLampStateColorTempField(String lampID, long colorTemp, long transitionPeriod);
    public native ControllerClientStatus transitionLampStateToPreset(String lampID, String presetID, long transitionPeriod);
    public native ControllerClientStatus getLampFaults(String lampID);
    public native ControllerClientStatus getLampServiceVersion(String lampID);
    public native ControllerClientStatus clearLampFault(String lampID, long faultCode);
    public native ControllerClientStatus getLampSupportedLanguages(String lampID);
    public native ControllerClientStatus getLampDataSet(String lampID, String language);

    protected native void createNativeObject(ControllerClient controller, LampManagerCallback callback);

    @Override
    protected native void destroyNativeObject();
}
