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

#import "LSFSampleLampManagerCallback.h"
#import "LSFAllJoynManager.h"
#import "LSFLampModelContainer.h"
#import "LSFGroupModelContainer.h"
#import "LSFGroupModel.h"
#import "LSFLampModel.h"
#import "LSFConstants.h"
#import "LSFDispatchQueue.h"

@interface LSFSampleLampManagerCallback()

@property (nonatomic, strong) dispatch_queue_t queue;

-(void)updateUI;
-(void)updateLampIDs: (NSArray *)lampIDs;
-(void)updateLampName: (NSString *)name forLampID: (NSString *)lampID;
-(void)updateLampDetailsForID: (NSString *)lampID withDetails: (LSFLampDetails *)lampDetails;
-(void)updateLampParametersForID: (NSString *)lampID withParameters: (LSFLampParameters *)lampParameters;
-(void)updateEnergyUsageForID: (NSString *)lampID withEnergyUsage: (unsigned int)energyUsage;
-(void)updateLumensForID: (NSString *)lampID withLumens: (unsigned int)lumens;
-(void)updateLampStateForID: (NSString *)lampID withState: (LSFLampState *)lampState;
-(void)updateLampStateOnOffForID: (NSString *)lampID withOnOff: (BOOL)onOff;
-(void)updateLampStateBrightnessForID: (NSString *)lampID withBrightness: (unsigned int)brightness;
-(void)updateLampStateHueForID: (NSString *)lampID withHue: (unsigned int)hue;
-(void)updateLampStateSaturationForID: (NSString *)lampID withSaturation: (unsigned int)saturation;
-(void)updateLampStateColorTempForID: (NSString *)lampID withColorTemp: (unsigned int)colorTemp;
-(void)calculateLampGroupState: (LSFGroupModel *)groupModel;
-(void)updateLampGroupState;

@end

@implementation LSFSampleLampManagerCallback

@synthesize reloadUIDelegate = _reloadUIDelegate;
@synthesize cscDelegate = _cscDelegate;
@synthesize queue = _queue;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.queue = ([LSFDispatchQueue getDispatchQueue]).queue;
    }
    
    return self;
}

/*
 * Implementation of LSFLampManagerCallbackDelegate
 */
-(void)getAllLampIDsReplyWithCode: (LSFResponseCode)rc andLampIDs: (NSArray *)lampIDs
{
    NSLog(@"Get all Lamp IDs callback executing");
    
    if ([lampIDs count] > 0)
    {
        ([LSFAllJoynManager getAllJoynManager]).foundLamps = YES;
        
        dispatch_async(dispatch_get_main_queue(), ^{
            if (self.cscDelegate != nil)
            {
                [self.cscDelegate lampsFound];
            }
        });
    }
    
    dispatch_async(self.queue, ^{
        [self updateLampIDs: lampIDs];
    });
}

-(void)getLampNameReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID language: (NSString*)language andLampName: (NSString*)name
{
    NSLog(@"Get lamp name callback executing. Name = %@. LSFResponse Code = %u", name, rc);
    dispatch_async(self.queue, ^{
        [self updateLampName: name forLampID: lampID];
    });
}

-(void)getLampManufacturerReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID language: (NSString*)language andManufacturer: (NSString*)manufacturer
{
    //TODO - unused at this point
}

-(void)setLampNameReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andLanguage: (NSString*)language
{
    NSLog(@"Set lamp name callback executing. LampID = %@. Language = %@. Response Code = %i", lampID, language, rc);
    dispatch_async(self.queue, ^{
        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        [lampManager getLampName: lampID];
    });
}

-(void)lampsNameChanged: (NSArray *)lampIDs
{
    NSLog(@"Lamps name changed callback executing");
    dispatch_async(self.queue, ^{
        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        for (NSString *lampID in lampIDs)
        {
            [lampManager getLampName: lampID];
        }
    });
}

-(void)getLampDetailsReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andLampDetails: (LSFLampDetails *)details
{
    NSLog(@"Get lamp details callback executing. LSFResponse Code = %u", rc);
    dispatch_async(self.queue, ^{
        [self updateLampDetailsForID: lampID withDetails: details];
    });
}

-(void)getLampParametersReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andLampParameters: (LSFLampParameters *)params
{
    NSLog(@"Get lamp parameters callback executing. LSFResponse Code = %u", rc);
    dispatch_async(self.queue, ^{
        [self updateLampParametersForID: lampID withParameters: params];
    });
}

-(void)getLampParametersEnergyUsageMilliwattsFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andEnergyUsage: (unsigned int)energyUsageMilliwatts
{
    NSLog(@"Get lamp parameters energy usage callback executing. LSFResponse Code = %u", rc);
    dispatch_async(self.queue, ^{
        [self updateEnergyUsageForID: lampID withEnergyUsage: energyUsageMilliwatts];
    });
}

-(void)getLampParametersLumensFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andBrightnessLumens: (unsigned int)brightnessLumens
{
    NSLog(@"Get lamp parameters lumens callback executing. LSFResponse Code = %u", rc);
    dispatch_async(self.queue, ^{
        [self updateLumensForID: lampID withLumens: brightnessLumens];
    });
}

-(void)getLampStateReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andLampState: (LSFLampState *)state
{
    NSLog(@"Get lamp state callback executing. LSFResponse Code = %u", rc);
    dispatch_async(self.queue, ^{
        [self updateLampStateForID: lampID withState: state];
    });
}

-(void)getLampStateOnOffFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andOnOff: (BOOL)onOff
{
    NSLog(@"Get Lamp State OnOff field executing. OnOff = %@", onOff ? @"ON" : @"OFF");
    dispatch_async(self.queue, ^{
        [self updateLampStateOnOffForID: lampID withOnOff: onOff];
    });
}

-(void)getLampStateHueFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andHue: (unsigned int)hue
{
    NSLog(@"Get Lamp State Hue field executing. Hue = %i", hue);
    dispatch_async(self.queue, ^{
        [self updateLampStateHueForID: lampID withHue: hue];
    });
}

-(void)getLampStateSaturationFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andSaturation: (unsigned int)saturation
{
    NSLog(@"Get Lamp State Saturation field executing. Saturation = %i", saturation);
    dispatch_async(self.queue, ^{
        [self updateLampStateSaturationForID: lampID withSaturation: saturation];
    });
}

-(void)getLampStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andBrightness: (unsigned int)brightness
{
    NSLog(@"Get Lamp State Brightness field executing. Brightness = %i", brightness);
    dispatch_async(self.queue, ^{
        [self updateLampStateBrightnessForID: lampID withBrightness: brightness];
    });
}

-(void)getLampStateColorTempFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andColorTemp: (unsigned int)colorTemp
{
    NSLog(@"Get Lamp State ColorTemp field executing. Color temp = %i", colorTemp);
    dispatch_async(self.queue, ^{
        [self updateLampStateColorTempForID: lampID withColorTemp: colorTemp];
    });
}

-(void)resetLampStateReplyWithCode: (LSFResponseCode)rc andLampID: (NSString *)lampID
{
    //TODO - unused at this point
}

-(void)lampsStateChanged: (NSArray *)lampIDs
{
    NSLog(@"Lamp State Changed callback executing");
    dispatch_async(self.queue, ^{
        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        
        for (NSString *lampID in lampIDs)
        {
            [lampManager getLampStateForID: lampID];
        }
    });
}

-(void)transitionLampStateReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    NSLog(@"Transition Lamp State callback executing. LSFResponse Code = %u", rc);
    dispatch_async(self.queue, ^{
        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        [lampManager getLampStateForID: lampID];
    });
}

-(void)pulseLampWithStateReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    //TODO - unused at this point
}

-(void)pulseLampWithPresetReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    //TODO - unused at this point
}

-(void)transitionLampStateOnOffFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    NSLog(@"Transition Lamp State OnOff field callback executing");
//    dispatch_async(self.queue, ^{
//        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
//        [lampManager getLampStateOnOffFieldForID: lampID];
//    });
}

-(void)transitionLampStateHueFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    NSLog(@"Transition Lamp State Hue field callback executing");
//    dispatch_async(self.queue, ^{
//        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
//        [lampManager getLampStateHueFieldForID: lampID];
//    });
}

-(void)transitionLampStateSaturationFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    NSLog(@"Transition Lamp State Saturation field callback executing");
//    dispatch_async(self.queue, ^{
//        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
//        [lampManager getLampStateSaturationFieldForID: lampID];
//    });
}

-(void)transitionLampStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    NSLog(@"Transition Lamp State Brightness field callback executing");
//    dispatch_async(self.queue, ^{
//        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
//        [lampManager getLampStateBrightnessFieldForID: lampID];
//    });
}

-(void)transitionLampStateColorTempFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    NSLog(@"Transition Lamp State ColorTemp field callback executing");
//    dispatch_async(self.queue, ^{
//        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
//        [lampManager getLampStateColorTempFieldForID: lampID];
//    });
}

-(void)getLampFaultsReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andFaultCodes: (NSArray *)codes
{
    //TODO - unused at this point
}

-(void)getLampServiceVersionReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andLampServiceVersion: (unsigned int)lampServiceVersion
{
    //TODO - unused at this point
}

-(void)clearLampFaultReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andFaultCode: (LampFaultCode)faultCode
{
    //TODO - unused at this point
}

-(void)resetLampStateOnOffFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    //TODO - unused at this point
}

-(void)resetLampStateHueFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    //TODO - unused at this point
}

-(void)resetLampStateSaturationFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    //TODO - unused at this point
}

-(void)resetLampStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    //TODO - unused at this point
}

-(void)resetLampStateColorTempFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    //TODO - unused at this point
}

-(void)transitionLampStateToPresetReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    //TODO - unused at this point
}

-(void)getLampSupportedLanguagesReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andSupportedLanguages: (NSArray*)supportedLanguages
{
    //TODO - unused at this point
}

/*
 * Private function implementations
 */
-(void)updateLampIDs: (NSArray *)lampIDs
{
    LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
    
    for (NSString *lampID in lampIDs)
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFLampModel *lampModel = [lamps valueForKey: lampID];
        
        if (lampModel == nil)
        {
            lampModel = [[LSFLampModel alloc] initWithLampID: lampID];
            [lamps setValue: lampModel forKey: lampID];
            
            //Calls are only made when new lamps are found
            [lampManager getLampName: lampID];
            [lampManager getLampStateForID: lampID];
            [lampManager getLampDetailsForID: lampID];
        }
        
        if (!lampModel.lampInitialized)
        {
            lampModel.lampInitialized = YES;
            
            [lampManager getLampName: lampID];
            [lampManager getLampStateForID: lampID];
            [lampManager getLampDetailsForID: lampID];
        }
        
        //We have to call get parameters all the time since we do not get a callback when these values update
        [lampManager getLampParametersForID: lampID];
        
        //Update timestamp for pruning purposes
        long long timestamp = (long long)([[NSDate date] timeIntervalSince1970] * 1000);
        NSLog(@"Current timestamp is %lld", timestamp);
        lampModel.timestamp = timestamp;
        
        [self updateUI];
    }
}

-(void)updateLampName: (NSString *)name forLampID: (NSString *)lampID
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    
    if (lampModel != nil)
    {
        lampModel.name = [NSString stringWithString: name];
        [self updateUI];
    }
    else
    {
        NSLog(@"updateLampName - Did not find LampModel for lamp ID");
    }
}

-(void)updateLampDetailsForID: (NSString *)lampID withDetails: (LSFLampDetails *)lampDetails
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    
    if (lampModel != nil)
    {
        lampModel.lampDetails = lampDetails;
        lampModel.capability = [[LSFCapabilityData alloc] initWithDimmable: lampDetails.dimmable color: lampDetails.color andTemp: lampDetails.variableColorTemp];
        [self updateUI];
    }
    else
    {
        NSLog(@"updateLampDetails - Did not find LampModel for lamp ID");
    }
}

-(void)updateLampParametersForID: (NSString *)lampID withParameters: (LSFLampParameters *)lampParameters
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    
    if (lampModel != nil)
    {
        lampModel.lampParameters = lampParameters;
        [self updateUI];
    }
    else
    {
        NSLog(@"updateLampParameters - Did not find LampModel for lamp ID");
    }
}

-(void)updateEnergyUsageForID: (NSString *)lampID withEnergyUsage: (unsigned int)energyUsage
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    
    if (lampModel != nil)
    {
        lampModel.lampParameters.energyUsageMilliwatts = energyUsage;
        [self updateUI];
    }
    else
    {
        NSLog(@"updateEnergyUsage - Did not find LampModel for lamp ID");
    }
}

-(void)updateLumensForID: (NSString *)lampID withLumens: (unsigned int)lumens
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    
    if (lampModel != nil)
    {
        lampModel.lampParameters.lumens = lumens;
        [self updateUI];
    }
    else
    {
        NSLog(@"updateLumens - Did not find LampModel for lamp ID");
    }
}

-(void)updateLampStateForID: (NSString *)lampID withState: (LSFLampState *)lampState
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    LSFConstants *constants = [LSFConstants getConstants];
    
    if (lampModel != nil)
    {
        lampModel.state.onOff = lampState.onOff;
        lampModel.state.brightness = [constants unscaleLampStateValue: lampState.brightness withMax: 100];
        lampModel.state.hue = [constants unscaleLampStateValue: lampState.hue withMax: 360];
        lampModel.state.saturation = [constants unscaleLampStateValue: lampState.saturation withMax: 100];
        lampModel.state.colorTemp = [constants unscaleColorTemp: lampState.colorTemp];
        
        dispatch_async(self.queue, ^{
            [self updateLampGroupState];
        });
        
        [self updateUI];
    }
    else
    {
        NSLog(@"updateLampState - Did not find LampModel for lamp ID");
    }
}

-(void)updateLampStateOnOffForID: (NSString *)lampID withOnOff: (BOOL)onOff
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    
    if (lampModel != nil)
    {
        [lampModel.state setOnOff: onOff];
        
        dispatch_async(self.queue, ^{
            [self updateLampGroupState];
        });
        
        [self updateUI];
    }
    else
    {
        NSLog(@"updateLampStateOnOff - Did not find LampModel for lamp ID");
    }
}

-(void)updateLampStateBrightnessForID: (NSString *)lampID withBrightness: (unsigned int)brightness
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    LSFConstants *constants = [LSFConstants getConstants];
    
    if (lampModel != nil)
    {
        [lampModel.state setBrightness: [constants unscaleLampStateValue: brightness withMax: 100]];
        
        dispatch_async(self.queue, ^{
            [self updateLampGroupState];
        });
        
        [self updateUI];
    }
    else
    {
        NSLog(@"updateLampStateBrightness - Did not find LampModel for lamp ID");
    }
}

-(void)updateLampStateHueForID: (NSString *)lampID withHue: (unsigned int)hue
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    LSFConstants *constants = [LSFConstants getConstants];
    
    if (lampModel != nil)
    {
        [lampModel.state setHue: [constants unscaleLampStateValue: hue withMax: 360]];
        
        dispatch_async(self.queue, ^{
            [self updateLampGroupState];
        });
        
        [self updateUI];
    }
    else
    {
        NSLog(@"updateLampStateHue - Did not find LampModel for lamp ID");
    }
}

-(void)updateLampStateSaturationForID: (NSString *)lampID withSaturation: (unsigned int)saturation
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    LSFConstants *constants = [LSFConstants getConstants];
    
    if (lampModel != nil)
    {
        [lampModel.state setSaturation: [constants unscaleLampStateValue: saturation withMax: 100]];
        
        dispatch_async(self.queue, ^{
            [self updateLampGroupState];
        });
        
        [self updateUI];
    }
    else
    {
        NSLog(@"updateLampStateSaturation - Did not find LampModel for lamp ID");
    }
}

-(void)updateLampStateColorTempForID: (NSString *)lampID withColorTemp: (unsigned int)colorTemp
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    LSFConstants *constants = [LSFConstants getConstants];
    
    if (lampModel != nil)
    {
        [lampModel.state setColorTemp: [constants unscaleColorTemp: colorTemp]];
        
        dispatch_async(self.queue, ^{
            [self updateLampGroupState];
        });
        
        [self updateUI];
    }
    else
    {
        NSLog(@"updateLampStateColorTemp - Did not find LampModel for lamp ID");
    }
}

-(void)updateLampGroupState
{
    NSMutableDictionary *groups = ((LSFGroupModelContainer *)[LSFGroupModelContainer getGroupModelContainer]).groupContainer;
    
    for (LSFGroupModel *groupModel in [groups allValues])
    {
        [self calculateLampGroupState: groupModel];
        //[self updateUI];
    }
}

-(void)calculateLampGroupState: (LSFGroupModel *)groupModel
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    
    BOOL powerOn = NO;
    uint64_t brightness = 0;
    uint64_t hue = 0;
    uint64_t saturation = 0;
    uint64_t colorTemp = 0;
    
    uint64_t countDim = 0;
    uint64_t countColor = 0;
    uint64_t countTemp = 0;
    LSFCapabilityData *capability = [[LSFCapabilityData alloc] init];
    
    for (NSString *lampID in groupModel.lamps)
    {
        LSFLampModel *lampModel = [lamps valueForKey: lampID];
        
        if (lampModel != nil)
        {
            [capability includeData: lampModel.capability];
            
            powerOn = powerOn || lampModel.state.onOff;
            
            if (lampModel.lampDetails.dimmable)
            {
                brightness = brightness + lampModel.state.brightness;
                countDim++;
            }
            
            if (lampModel.lampDetails.color)
            {
                hue = hue + lampModel.state.hue;
                saturation = saturation + lampModel.state.saturation;
                countColor++;
            }
            
            if (lampModel.lampDetails.variableColorTemp)
            {
                colorTemp = colorTemp + lampModel.state.colorTemp;
                countTemp++;
            }
        }
        else
        {
            NSLog(@"postUpdateLampGroupState - missing lamp");
        }
    }
    
    double divisorDim = countDim > 0 ? countDim : 1.0;
    double divisorColor = countColor > 0 ? countColor : 1.0;
    double divisorTemp = countTemp > 0 ? countTemp : 1.0;
    
    groupModel.state.onOff = powerOn;
    groupModel.state.brightness = (uint32_t)round(brightness / divisorDim);
    groupModel.state.hue = (uint32_t)round(hue / divisorColor);
    groupModel.state.saturation = (uint32_t)round(saturation / divisorColor);
    groupModel.state.colorTemp = (uint32_t)round(colorTemp / divisorTemp);
    groupModel.capability = capability;
    
    NSLog(@"Group Model State");
    NSLog(@"OnOff = %@", groupModel.state.onOff ? @"On" : @"Off");
    NSLog(@"Brightness = %u", groupModel.state.brightness);
    NSLog(@"Hue = %u", groupModel.state.hue);
    NSLog(@"Saturation = %u", groupModel.state.saturation);
    NSLog(@"Color Temp = %u", groupModel.state.colorTemp);
    
    [self updateUI];
}

-(void)updateUI
{
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.reloadUIDelegate != nil)
        {
            [self.reloadUIDelegate reloadUI];
        }
    });
}

@end
