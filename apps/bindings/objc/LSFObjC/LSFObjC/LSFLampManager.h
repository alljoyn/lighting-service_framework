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

#import "LSFObject.h"
#import "LSFLampState.h"
#import "LSFControllerClient.h"
#import "LSFLampManagerCallbackDelegate.h"
#import "LampManager.h"

using namespace lsf;

@interface LSFLampManager : LSFObject

-(id)initWithControllerClient: (LSFControllerClient *)controllerClient andLampManagerCallbackDelegate: (id<LSFLampManagerCallbackDelegate>)lmDelegate;
-(ControllerClientStatus)getAllLampIDs;
-(ControllerClientStatus)getLampManufacturerForID: (NSString *)lampID;
-(ControllerClientStatus)getLampManufacturerForID: (NSString *)lampID andLanguage: (NSString *)language;
-(ControllerClientStatus)getLampName: (NSString *)lampID;
-(ControllerClientStatus)getLampName: (NSString *)lampID andLanguage: (NSString *)language;
-(ControllerClientStatus)setLampNameWithID: (NSString *)lampID andName: (NSString *)name;
-(ControllerClientStatus)setLampNameWithID: (NSString *)lampID name: (NSString *)name andLanguage: (NSString *)language;
-(ControllerClientStatus)getLampDetailsForID: (NSString *)lampID;
//-(ControllerClientStatus)pingLampWithID: (NSString *)lampID;
-(ControllerClientStatus)getLampParametersForID: (NSString *)lampID;
-(ControllerClientStatus)getLampParameterEnergyUsageMilliwattsFieldForID: (NSString *)lampID;
-(ControllerClientStatus)getLampParameterLumensFieldForID: (NSString *)lampID;
-(ControllerClientStatus)getLampStateForID: (NSString *)lampID;
-(ControllerClientStatus)getLampStateOnOffFieldForID: (NSString *)lampID;
-(ControllerClientStatus)getLampStateHueFieldForID: (NSString *)lampID;
-(ControllerClientStatus)getLampStateSaturationFieldForID: (NSString *)lampID;
-(ControllerClientStatus)getLampStateBrightnessFieldForID: (NSString *)lampID;
-(ControllerClientStatus)getLampStateColorTempFieldForID: (NSString *)lampID;
-(ControllerClientStatus)resetLampStateForID: (NSString *)lampID;
-(ControllerClientStatus)resetLampStateOnOffFieldForID: (NSString *)lampID;
-(ControllerClientStatus)resetLampStateHueFieldForID: (NSString *)lampID;
-(ControllerClientStatus)resetLampStateSaturationFieldForID: (NSString *)lampID;
-(ControllerClientStatus)resetLampStateBrightnessFieldForID: (NSString *)lampID;
-(ControllerClientStatus)resetLampStateColorTempFieldForID: (NSString *)lampID;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID toLampState: (LSFLampState *)lampState;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID toLampState: (LSFLampState *)lampState withTransitionPeriod: (unsigned int)transitionPeriod;
-(ControllerClientStatus)pulseLampID: (NSString *)lampID toLampState: (LSFLampState *)toLampState withPeriod: (unsigned int)period duration: (unsigned int)duration andNumPulses: (unsigned int)numPulses;
-(ControllerClientStatus)pulseLampID: (NSString *)lampID toLampState: (LSFLampState *)toLampState withPeriod: (unsigned int)period duration: (unsigned int)duration numPulses: (unsigned int)numPulses andFromLampState: (LSFLampState *)fromLampState;
-(ControllerClientStatus)pulseLampID: (NSString *)lampID toPreset: (NSString *)toPresetID withPeriod: (unsigned int)period duration: (unsigned int)duration andNumPulses: (unsigned int)numPulses;
-(ControllerClientStatus)pulseLampID: (NSString *)lampID toPreset: (NSString *)toPresetID withPeriod: (unsigned int)period duration: (unsigned int)duration andNumPulses: (unsigned int)numPulses andFromPresetID: (NSString *)fromPresetID;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID onOffField: (BOOL)onOff;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID hueField: (unsigned int)hue;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID hueField: (unsigned int)hue withTransitionPeriod: (unsigned int)transitionPeriod;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID saturationField: (unsigned int)saturation;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID saturationField: (unsigned int)saturation withTransitionPeriod: (unsigned int)transitionPeriod;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID brightnessField: (unsigned int)brightness;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID brightnessField: (unsigned int)brightness withTransitionPeriod: (unsigned int)transitionPeriod;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID colorTempField: (unsigned int)colorTemp;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID colorTempField: (unsigned int)colorTemp withTransitionPeriod: (unsigned int)transitionPeriod;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID toPresetID: (NSString *)presetID;
-(ControllerClientStatus)transitionLampID: (NSString *)lampID toPresetID: (NSString *)presetID withTransitionPeriod: (unsigned int)transitionPeriod;
-(ControllerClientStatus)getLampFaultsForID: (NSString *)lampID;
-(ControllerClientStatus)getLampServiceVersionForID: (NSString *)lampID;
-(ControllerClientStatus)clearLampFaultForID: (NSString *) lampID andFaultCode: (LampFaultCode)faultCode;
-(ControllerClientStatus)getLampSupportedLanguagesForID: (NSString *) lampID;
-(ControllerClientStatus)getLampDataSetForID: (NSString *)lampID;
-(ControllerClientStatus)getLampDataSetForID: (NSString *)lampID andLanguage: (NSString *)language;

@end
