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

#import "MockLampManagerCallbackDelegateHandler.h"

@interface MockLampManagerCallbackDelegateHandler()

@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation MockLampManagerCallbackDelegateHandler

@synthesize dataArray = _dataArray;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.dataArray = [[NSMutableArray alloc] init];
    }
    
    return self;
}

-(NSArray *)getCallbackData
{
    return self.dataArray;
}

//Delegate methods
-(void)getAllLampIDsReplyWithCode: (LSFResponseCode)rc andLampIDs: (NSArray *)lampIDs
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getAllLampIDs"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampIDs];

}

-(void)getLampNameReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID language: (NSString*)language andLampName: (NSString*)name
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampName"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: language];
    [self.dataArray addObject: name];
}

-(void)getLampManufacturerReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID language: (NSString*)language andManufacturer: (NSString*)manufacturer
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampManufacturer"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: language];
    [self.dataArray addObject: manufacturer];

}

-(void)setLampNameReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andLanguage: (NSString*)language
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"setLampName"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: language];
    [self.dataArray addObject: lampID];
}

-(void)lampsNameChanged: (NSArray *)lampIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"lampsNameChanged"];
    [self.dataArray addObject: lampIDs];
}

-(void)lampsFound: (NSArray *)lampIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"lampsFound"];
    [self.dataArray addObject: lampIDs];
}

-(void)lampsLost: (NSArray *)lampIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"lampsLost"];
    [self.dataArray addObject: lampIDs];
}

-(void)pingLampReplyWithCode: (LSFResponseCode)rc andLampID: (NSString *)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"pingLamp"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)getLampDetailsReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andLampDetails: (LSFLampDetails *)details
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampDetails"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    NSNumber *make = [[NSNumber alloc] initWithInt: MAKE_LIFX];
    NSNumber *model = [[NSNumber alloc] initWithInt: MODEL_LED];
    NSNumber *type = [[NSNumber alloc] initWithInt: TYPE_LAMP];
    NSNumber *lampType = [[NSNumber alloc] initWithInt: LAMPTYPE_A15];
    NSNumber *lampBaseType = [[NSNumber alloc] initWithInt: BASETYPE_E5];
    NSNumber *lampBeamAngle = [[NSNumber alloc] initWithUnsignedInt: 100];
    NSNumber *dimmable = [[NSNumber alloc] initWithBool: YES];
    NSNumber *color = [[NSNumber alloc] initWithBool: YES];
    NSNumber *variableColorTemp = [[NSNumber alloc] initWithBool: NO];
    NSNumber *hasEffects = [[NSNumber alloc] initWithBool: NO];
    NSNumber *maxVoltage = [[NSNumber alloc] initWithUnsignedInt: 120];
    NSNumber *minVoltage = [[NSNumber alloc] initWithUnsignedInt: 120];
    NSNumber *wattage = [[NSNumber alloc] initWithUnsignedInt: 100];
    NSNumber *incandescentEquivalent = [[NSNumber alloc] initWithUnsignedInt: 9];
    NSNumber *maxLumens = [[NSNumber alloc] initWithUnsignedInt: 100];
    NSNumber *minTemp = [[NSNumber alloc] initWithUnsignedInt: 2400];
    NSNumber *maxTemp = [[NSNumber alloc] initWithUnsignedInt: 5000];
    NSNumber *cri = [[NSNumber alloc] initWithUnsignedInt: 93];
    [self.dataArray addObject: make];
    [self.dataArray addObject: model];
    [self.dataArray addObject: type];
    [self.dataArray addObject: lampType];
    [self.dataArray addObject: lampBaseType];
    [self.dataArray addObject: lampBeamAngle];
    [self.dataArray addObject: dimmable];
    [self.dataArray addObject: color];
    [self.dataArray addObject: variableColorTemp];
    [self.dataArray addObject: hasEffects];
    [self.dataArray addObject: maxVoltage];
    [self.dataArray addObject: minVoltage];
    [self.dataArray addObject: wattage];
    [self.dataArray addObject: incandescentEquivalent];
    [self.dataArray addObject: maxLumens];
    [self.dataArray addObject: minTemp];
    [self.dataArray addObject: maxTemp];
    [self.dataArray addObject: cri];
}

-(void)getLampParametersReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andLampParameters: (LSFLampParameters *)params
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampParameters"];
    [self.dataArray addObject: responseCode];
    NSNumber *eum = [[NSNumber alloc] initWithUnsignedInt: params.energyUsageMilliwatts];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: eum];
    NSNumber *lume = [[NSNumber alloc] initWithUnsignedInt: params.lumens];
    [self.dataArray addObject: lume];
}

-(void)getLampParametersEnergyUsageMilliwattsFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andEnergyUsage: (unsigned int)energyUsageMilliwatts
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampParametersEnergyUsageMilliwatts"];
    [self.dataArray addObject: responseCode];
    NSNumber *eum = [[NSNumber alloc] initWithUnsignedInt: energyUsageMilliwatts];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: eum];
}

-(void)getLampParametersLumensFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andBrightnessLumens: (unsigned int)brightnessLumens;
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampParametersLumens"];
    [self.dataArray addObject: responseCode];
    NSNumber *lume = [[NSNumber alloc] initWithUnsignedInt: brightnessLumens];
    [self.dataArray addObject: lume];
    [self.dataArray addObject: lampID];
}

-(void)getLampStateReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andLampState: (LSFLampState *)state
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampState"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    NSNumber *onOff = [[NSNumber alloc] initWithBool: state.onOff];
    NSNumber *brightness = [[NSNumber alloc] initWithUnsignedInt: state.brightness];
    NSNumber *hue = [[NSNumber alloc] initWithUnsignedInt: state.hue];
    NSNumber *saturation = [[NSNumber alloc] initWithUnsignedInt: state.saturation];
    NSNumber *colorTemp = [[NSNumber alloc] initWithUnsignedInt: state.colorTemp];
    [self.dataArray addObject: onOff];
    [self.dataArray addObject: brightness];
    [self.dataArray addObject: hue];
    [self.dataArray addObject: saturation];
    [self.dataArray addObject: colorTemp];
}

-(void)getLampStateOnOffFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andOnOff: (BOOL)onOff
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampStateOnOffField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    NSNumber *onOffField = [[NSNumber alloc] initWithBool: onOff];
    [self.dataArray addObject: onOffField];
}

-(void)getLampStateHueFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andHue: (unsigned int)hue
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampStateHueField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    NSNumber *hueField = [[NSNumber alloc] initWithUnsignedInt: hue];
    [self.dataArray addObject: hueField];
}

-(void)getLampStateSaturationFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andSaturation: (unsigned int)saturation
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampStateSaturationField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    NSNumber *saturationField = [[NSNumber alloc] initWithUnsignedInt: saturation];
    [self.dataArray addObject: saturationField];
}

-(void)getLampStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andBrightness: (unsigned int)brightness
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampStateBrightnessField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    NSNumber *brightnessField = [[NSNumber alloc] initWithUnsignedInt: brightness];
    [self.dataArray addObject: brightnessField];
}

-(void)getLampStateColorTempFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andColorTemp: (unsigned int)colorTemp
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampStateColorTempField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    NSNumber *colorTempField = [[NSNumber alloc] initWithUnsignedInt: colorTemp];
    [self.dataArray addObject: colorTempField];
}

-(void)resetLampStateReplyWithCode: (LSFResponseCode)rc andLampID: (NSString *)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampState"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)lampsStateChanged: (NSArray *)lampIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"lampsStateChanged"];
    [self.dataArray addObject: lampIDs];
}

-(void)transitionLampStateReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampState"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)pulseLampWithStateReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"pulseWithState"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)pulseLampWithPresetReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"pulseWithPreset"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)transitionLampStateOnOffFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampStateOnOffField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)transitionLampStateHueFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampStateHueField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)transitionLampStateSaturationFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampStateSaturationField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)transitionLampStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampStateBrightnessField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)transitionLampStateColorTempFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampStateColorTempField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)getLampFaultsReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andFaultCodes: (NSArray *)codes;
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampFaults"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: codes];
}

-(void)getLampServiceVersionReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andLampServiceVersion: (unsigned int)lampServiceVersion
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampServiceVersion"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    NSNumber *version = [[NSNumber alloc] initWithUnsignedInt: lampServiceVersion];
    [self.dataArray addObject: version];
}

-(void)clearLampFaultReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andFaultCode: (LampFaultCode)faultCode
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"clearLampFault"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    NSNumber *code = [[NSNumber alloc] initWithUnsignedInt: faultCode];
    [self.dataArray addObject: code];
}

-(void)resetLampStateOnOffFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampStateOnOffField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)resetLampStateHueFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampStateHueField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)resetLampStateSaturationFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampStateSaturationField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)resetLampStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampStateBrightnessField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)resetLampStateColorTempFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampStateColorTempField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)transitionLampStateToPresetReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampStateToPreset"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
}

-(void)getLampSupportedLanguagesReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andSupportedLanguages: (NSArray*)supportedLanguages
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampSupportedLanguages"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: supportedLanguages];
}

@end
