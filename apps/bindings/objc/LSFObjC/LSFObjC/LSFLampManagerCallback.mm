/******************************************************************************
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
 ******************************************************************************/

#import "LSFLampManagerCallback.h"

LSFLampManagerCallback::LSFLampManagerCallback(id<LSFLampManagerCallbackDelegate> delegate) : _lmDelegate(delegate)
{
    //Empty Constructor
}

LSFLampManagerCallback::~LSFLampManagerCallback()
{
    _lmDelegate = nil;
}

void LSFLampManagerCallback::GetAllLampIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& lampIDs)
{
    NSMutableArray *lampIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = lampIDs.begin(); iter != lampIDs.end(); ++iter)
    {
        [lampIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    [_lmDelegate getAllLampIDsReplyWithCode: responseCode andLampIDs: lampIDsArray];
}

void LSFLampManagerCallback::GetLampNameReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LSFString& language, const LSFString& lampName)
{
    [_lmDelegate getLampNameReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] language: [NSString stringWithUTF8String: language.c_str()] andLampName: [NSString stringWithUTF8String: lampName.c_str()]];
}

void LSFLampManagerCallback::GetLampManufacturerReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LSFString& language, const LSFString& manufacturer)
{
    [_lmDelegate getLampManufacturerReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] language: [NSString stringWithUTF8String: language.c_str()] andManufacturer: [NSString stringWithUTF8String: manufacturer.c_str()]];
}

void LSFLampManagerCallback::SetLampNameReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LSFString& language)
{
    [_lmDelegate setLampNameReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andLanguage: [NSString stringWithUTF8String: language.c_str()]];
}

void LSFLampManagerCallback::LampNameChangedCB(const LSFString& lampID, const LSFString& lampName)
{
    [_lmDelegate lampsNameChangedWithID: [NSString stringWithUTF8String: lampID.c_str()] andName: [NSString stringWithUTF8String: lampName.c_str()]];
}

void LSFLampManagerCallback::LampsFoundCB(const LSFStringList& lampIDs)
{
    NSMutableArray *lampIDsArray = [[NSMutableArray alloc] init];

    for (std::list<LSFString>::const_iterator iter = lampIDs.begin(); iter != lampIDs.end(); ++iter)
    {
        [lampIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }

    [_lmDelegate lampsFound: lampIDsArray];
}

void LSFLampManagerCallback::LampsLostCB(const LSFStringList& lampIDs)
{
    NSMutableArray *lampIDsArray = [[NSMutableArray alloc] init];

    for (std::list<LSFString>::const_iterator iter = lampIDs.begin(); iter != lampIDs.end(); ++iter)
    {
        [lampIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }

    [_lmDelegate lampsLost: lampIDsArray];
}

void LSFLampManagerCallback::GetLampDetailsReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LampDetails& lampDetails)
{
    LSFLampDetails *details = [[LSFLampDetails alloc] init];
    details.lampMake = lampDetails.make;
    details.lampModel = lampDetails.model;
    details.deviceType = lampDetails.type;
    details.lampType = lampDetails.lampType;
    details.baseType = lampDetails.lampBaseType;
    details.lampBeamAngle = lampDetails.lampBeamAngle;
    details.dimmable = lampDetails.dimmable;
    details.color = lampDetails.color;
    details.variableColorTemp = lampDetails.variableColorTemp;
    details.hasEffects = lampDetails.hasEffects;
    details.maxVoltage = lampDetails.maxVoltage;
    details.minVoltage = lampDetails.minVoltage;
    details.wattage = lampDetails.wattage;
    details.incandescentEquivalent = lampDetails.incandescentEquivalent;
    details.maxLumens = lampDetails.maxLumens;
    details.minTemperature = lampDetails.minTemperature;
    details.maxTemperature = lampDetails.maxTemperature;
    details.colorRenderingIndex = lampDetails.colorRenderingIndex;
    details.lampID = [NSString stringWithUTF8String: lampID.c_str()];
    
    [_lmDelegate getLampDetailsReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andLampDetails: details];
}

void LSFLampManagerCallback::GetLampParametersReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LampParameters& lampParameters)
{
    LSFLampParameters *parameters = [[LSFLampParameters alloc] init];
    parameters.lumens = lampParameters.lumens;
    parameters.energyUsageMilliwatts = lampParameters.energyUsageMilliwatts;
    
    [_lmDelegate getLampParametersReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andLampParameters: parameters];
}

void LSFLampManagerCallback::GetLampParametersEnergyUsageMilliwattsFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& energyUsageMilliwatts)
{
    [_lmDelegate getLampParametersEnergyUsageMilliwattsFieldReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andEnergyUsage: energyUsageMilliwatts];
}

void LSFLampManagerCallback::GetLampParametersLumensFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& brightnessLumens)
{
    [_lmDelegate getLampParametersLumensFieldReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andBrightnessLumens: brightnessLumens];
}

void LSFLampManagerCallback::GetLampStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LampState& lampState)
{
    LSFLampState *state = [[LSFLampState alloc] init];
    state.onOff = lampState.onOff;
    state.brightness = lampState.brightness;
    state.hue = lampState.hue;
    state.saturation = lampState.saturation;
    state.colorTemp = lampState.colorTemp;
    
    [_lmDelegate getLampStateReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andLampState: state];
}

void LSFLampManagerCallback::GetLampStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const bool& onOff)
{
    [_lmDelegate getLampStateOnOffFieldReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andOnOff: onOff];
}

void LSFLampManagerCallback::GetLampStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& hue)
{
    [_lmDelegate getLampStateHueFieldReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andHue:hue];
}

void LSFLampManagerCallback::GetLampStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& saturation)
{
    [_lmDelegate getLampStateSaturationFieldReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andSaturation:saturation];
}

void LSFLampManagerCallback::GetLampStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& brightness)
{
    [_lmDelegate getLampStateBrightnessFieldReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andBrightness:brightness];
}

void LSFLampManagerCallback::GetLampStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& colorTemp)
{
    [_lmDelegate getLampStateColorTempFieldReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andColorTemp:colorTemp];
}

void LSFLampManagerCallback::ResetLampStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate resetLampStateReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::LampStateChangedCB(const LSFString& lampID, const LampState& lampState)
{
    LSFLampState *state = [[LSFLampState alloc] init];
    state.onOff = lampState.onOff;
    state.brightness = lampState.brightness;
    state.hue = lampState.hue;
    state.saturation = lampState.saturation;
    state.colorTemp = lampState.colorTemp;

    [_lmDelegate lampsStateChangedWithID: [NSString stringWithUTF8String: lampID.c_str()] andLampState: state];
}

void LSFLampManagerCallback::TransitionLampStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate transitionLampStateReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::PulseLampWithStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate pulseLampWithStateReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::PulseLampWithPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate pulseLampWithPresetReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::TransitionLampStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate transitionLampStateOnOffFieldReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::TransitionLampStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate transitionLampStateHueFieldReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::TransitionLampStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate transitionLampStateSaturationFieldReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::TransitionLampStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate transitionLampStateBrightnessFieldReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::TransitionLampStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate transitionLampStateColorTempFieldReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::GetLampFaultsReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LampFaultCodeList& faultCodes)
{
    NSMutableArray *faultCodeArray = [[NSMutableArray alloc] init];
    
    for (std::list<LampFaultCode>::const_iterator iter = faultCodes.begin(); iter != faultCodes.end(); ++iter)
    {
        [faultCodeArray addObject: [NSNumber numberWithInt: (*iter)]];
    }
    
    [_lmDelegate getLampFaultsReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andFaultCodes: faultCodeArray];
}

void LSFLampManagerCallback::GetLampServiceVersionReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& lampServiceVersion)
{
    [_lmDelegate getLampServiceVersionReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andLampServiceVersion:lampServiceVersion];
}

void LSFLampManagerCallback::ClearLampFaultReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LampFaultCode& faultCode)
{
    [_lmDelegate clearLampFaultReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andFaultCode: faultCode];
}

void LSFLampManagerCallback::ResetLampStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate resetLampStateOnOffFieldReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::ResetLampStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate resetLampStateHueFieldReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::ResetLampStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate resetLampStateSaturationFieldReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::ResetLampStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate resetLampStateBrightnessFieldReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::ResetLampStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate resetLampStateColorTempFieldReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::TransitionLampStateToPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    [_lmDelegate transitionLampStateToPresetReplyWithCode: responseCode andLampID: [NSString stringWithUTF8String: lampID.c_str()]];
}

void LSFLampManagerCallback::GetLampSupportedLanguagesReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LSFStringList& supportedLanguages)
{
    NSMutableArray *supportedLanguagesArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = supportedLanguages.begin(); iter != supportedLanguages.end(); ++iter)
    {
        [supportedLanguagesArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    [_lmDelegate getLampSupportedLanguagesReplyWithCode: responseCode lampID: [NSString stringWithUTF8String: lampID.c_str()] andSupportedLanguages: supportedLanguagesArray];
}