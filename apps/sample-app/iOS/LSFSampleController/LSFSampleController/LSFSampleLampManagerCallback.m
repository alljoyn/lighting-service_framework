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
#import "LSFAboutData.h"
#import "LSFGarbageCollector.h"

@interface LSFSampleLampManagerCallback()

@property (nonatomic, strong) dispatch_queue_t queue;

-(void)updateUI;
-(void)postUpdateLampID: (NSString *)lampID;
-(void)updateLampName: (NSString *)name forLampID: (NSString *)lampID;
-(void)postGetLampName: (NSString *)lampID;
-(void)updateLampDetailsForID: (NSString *)lampID withDetails: (LSFLampDetails *)lampDetails;
-(void)postGetLampDetails: (NSString *)lampID;
-(void)updateLampParametersForID: (NSString *)lampID withParameters: (LSFLampParameters *)lampParameters;
-(void)postGetLampParameters: (NSString *)lampID;
-(void)updateLampStateForID: (NSString *)lampID withState: (LSFLampState *)lampState;
-(void)postGetLampState: (NSString *)lampID;
-(void)updateLampStateOnOffForID: (NSString *)lampID withOnOff: (BOOL)onOff;
-(void)postGetLampStateOnOffField: (NSString *)lampID;
-(void)updateLampStateBrightnessForID: (NSString *)lampID withBrightness: (unsigned int)brightness;
-(void)postGetLampStateBrightnessField: (NSString *)lampID;
-(void)updateLampStateHueForID: (NSString *)lampID withHue: (unsigned int)hue;
-(void)postGetLampStateHueField: (NSString *)lampID;
-(void)updateLampStateSaturationForID: (NSString *)lampID withSaturation: (unsigned int)saturation;
-(void)postGetLampStateSaturationField: (NSString *)lampID;
-(void)updateLampStateColorTempForID: (NSString *)lampID withColorTemp: (unsigned int)colorTemp;
-(void)postGetLampStateColorTempField: (NSString *)lampID;
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
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManager - GetAllLampIDs() returned an error code: %i", rc);
    }
    else
    {
        dispatch_async(self.queue, ^{
            for (NSString *lampID in lampIDs)
            {
                [self postUpdateLampID: lampID];
            }
        });
    }
}

-(void)getLampNameReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID language: (NSString*)language andLampName: (NSString*)name
{
    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampName: name forLampID: lampID];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampName() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.2 * NSEC_PER_SEC), self.queue, ^{
            [self postGetLampName: lampID];
        });
    }
}

-(void)getLampManufacturerReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID language: (NSString*)language andManufacturer: (NSString*)manufacturer
{
    //TODO - unused at this point
}

-(void)setLampNameReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andLanguage: (NSString*)language
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManager - SetLampName() returned an error code: %i", rc);
    }

    dispatch_async(self.queue, ^{
        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        [lampManager getLampName: lampID];
    });
}

-(void)lampsNameChanged: (NSArray *)lampIDs
{
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
    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampDetailsForID: lampID withDetails: details];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampDetails() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.2 * NSEC_PER_SEC), self.queue, ^{
            [self postGetLampDetails: lampID];
        });
    }
}

-(void)getLampParametersReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andLampParameters: (LSFLampParameters *)params
{
    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampParametersForID: lampID withParameters: params];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampParameters() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.2 * NSEC_PER_SEC), self.queue, ^{
            [self postGetLampParameters: lampID];
        });
    }
}

-(void)getLampParametersEnergyUsageMilliwattsFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andEnergyUsage: (unsigned int)energyUsageMilliwatts
{
    //TODO - unused at this point
}

-(void)getLampParametersLumensFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString*)lampID andBrightnessLumens: (unsigned int)brightnessLumens
{
    //TODO - unused at this point
}

-(void)getLampStateReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andLampState: (LSFLampState *)state
{
    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampStateForID: lampID withState: state];
            [self postGetLampParameters: lampID];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampState() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.2 * NSEC_PER_SEC), self.queue, ^{
            [self postGetLampState: lampID];
        });
    }
}

-(void)getLampStateOnOffFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andOnOff: (BOOL)onOff
{
    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampStateOnOffForID: lampID withOnOff: onOff];
            [self postGetLampParameters: lampID];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampStateOnOffField() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.2 * NSEC_PER_SEC), self.queue, ^{
            [self postGetLampStateOnOffField: lampID];
        });
    }
}

-(void)getLampStateHueFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andHue: (unsigned int)hue
{
    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampStateHueForID: lampID withHue: hue];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampStateHueField() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.2 * NSEC_PER_SEC), self.queue, ^{
            [self postGetLampStateHueField: lampID];
        });
    }
}

-(void)getLampStateSaturationFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andSaturation: (unsigned int)saturation
{
    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampStateSaturationForID: lampID withSaturation: saturation];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampStateSaturationField() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.2 * NSEC_PER_SEC), self.queue, ^{
            [self postGetLampStateSaturationField: lampID];
        });
    }
}

-(void)getLampStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andBrightness: (unsigned int)brightness
{
    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampStateBrightnessForID: lampID withBrightness: brightness];
            [self postGetLampParameters: lampID];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampStateBrightnessField() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.2 * NSEC_PER_SEC), self.queue, ^{
            [self postGetLampStateBrightnessField: lampID];
        });
    }
}

-(void)getLampStateColorTempFieldReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andColorTemp: (unsigned int)colorTemp
{
    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampStateColorTempForID: lampID withColorTemp: colorTemp];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampStateColorTempField() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 0.2 * NSEC_PER_SEC), self.queue, ^{
            [self postGetLampStateColorTempField: lampID];
        });
    }
}

-(void)resetLampStateReplyWithCode: (LSFResponseCode)rc andLampID: (NSString *)lampID
{
    //TODO - unused at this point
}

-(void)lampsStateChanged: (NSArray *)lampIDs
{
    NSLog(@"LSFSampleLampGroupManager - LampsStateChanged()");

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
    //TODO - unused at this point
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
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManager - TransitionLampStateOnOffField() returned an error code: %i", rc);
    }

    dispatch_async(self.queue, ^{
        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        [lampManager getLampStateOnOffFieldForID: lampID];
    });
}

-(void)transitionLampStateHueFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManager - TransitionLampStateHueField() returned an error code: %i", rc);
    }

    dispatch_async(self.queue, ^{
        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        [lampManager getLampStateHueFieldForID: lampID];
    });
}

-(void)transitionLampStateSaturationFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManager - TransitionLampStateSaturationField() returned an error code: %i", rc);
    }

    dispatch_async(self.queue, ^{
        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        [lampManager getLampStateSaturationFieldForID: lampID];
    });
}

-(void)transitionLampStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManager - TransitionLampStateBrightnessField() returned an error code: %i", rc);
    }

    dispatch_async(self.queue, ^{
        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        [lampManager getLampStateBrightnessFieldForID: lampID];
    });
}

-(void)transitionLampStateColorTempFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManager - TransitionLampStateColorTempField() returned an error code: %i", rc);
    }

    dispatch_async(self.queue, ^{
        LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        [lampManager getLampStateColorTempFieldForID: lampID];
    });
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
-(void)postUpdateLampID: (NSString *)lampID
{
    [self postUpdateLampID: lampID withAboutData: nil];
}

-(void)postUpdateLampID: (NSString *)lampID withAboutData: (LSFAboutData *)aboutData
{
    LSFLampManager *lampManager = ((LSFAllJoynManager *)[LSFAllJoynManager getAllJoynManager]).lsfLampManager;
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];

    if (lampModel == nil)
    {
        NSLog(@"Creating Lamp Model for ID: %@", lampID);
        lampModel = [[LSFLampModel alloc] initWithLampID: lampID];
        [lamps setValue: lampModel forKey: lampID];
    }

    if ([lampModel.name isEqualToString: @"[Loading lamp info...]"])
    {
        NSLog(@"Getting data set for lamp: %@", lampModel.theID);

        //Calls are only made when lamp name is still equal to default name
        [lampManager getLampName: lampID];
        [lampManager getLampStateForID: lampID];
        [lampManager getLampDetailsForID: lampID];
        [lampManager getLampParametersForID: lampID];
    }

    if (aboutData != nil)
    {
        NSLog(@"Updating lamp about data");
        lampModel.aboutData = aboutData;
    }

    //Update timestamp for pruning purposes
    long long timestamp = (long long)([[NSDate date] timeIntervalSince1970] * 1000);
    lampModel.timestamp = timestamp;

    [self updateUI];
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

-(void)postGetLampName: (NSString *)lampID
{
    LSFGarbageCollector *garbageCollector = [LSFGarbageCollector getGarbageCollector];

    if (![garbageCollector isLampExpired: lampID])
    {
        NSLog(@"LSFSampleLampManagerCallback - postGetLampName() lampID is not expired, retrying getLampName()");

        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampManager getLampName: lampID];
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

-(void)postGetLampDetails: (NSString *)lampID
{
    LSFGarbageCollector *garbageCollector = [LSFGarbageCollector getGarbageCollector];

    if (![garbageCollector isLampExpired: lampID])
    {
        NSLog(@"LSFSampleLampManagerCallback - postGetLampDetails() lampID is not expired, retrying getLampDetails()");

        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampManager getLampDetailsForID: lampID];
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

-(void)postGetLampParameters: (NSString *)lampID
{
    LSFGarbageCollector *garbageCollector = [LSFGarbageCollector getGarbageCollector];

    if (![garbageCollector isLampExpired: lampID])
    {
        NSLog(@"LSFSampleLampManagerCallback - postGetLampParameters() lampID is not expired, retrying getLampParameters()");

        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampManager getLampParametersForID: lampID];
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

-(void)postGetLampState: (NSString *)lampID
{
    LSFGarbageCollector *garbageCollector = [LSFGarbageCollector getGarbageCollector];

    if (![garbageCollector isLampExpired: lampID])
    {
        NSLog(@"LSFSampleLampManagerCallback - postGetLampState() lampID is not expired, retrying getLampState()");

        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampManager getLampStateForID: lampID];
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

-(void)postGetLampStateOnOffField: (NSString *)lampID
{
    LSFGarbageCollector *garbageCollector = [LSFGarbageCollector getGarbageCollector];

    if (![garbageCollector isLampExpired: lampID])
    {
        NSLog(@"LSFSampleLampManagerCallback - postGetLampStateOnOffField() lampID is not expired, retrying getLampStateOnOffField()");

        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampManager getLampStateOnOffFieldForID: lampID];
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

-(void)postGetLampStateBrightnessField: (NSString *)lampID
{
    LSFGarbageCollector *garbageCollector = [LSFGarbageCollector getGarbageCollector];

    if (![garbageCollector isLampExpired: lampID])
    {
        NSLog(@"LSFSampleLampManagerCallback - postGetLampStateBrightnessField() lampID is not expired, retrying getLampStateBrightnessField()");

        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampManager getLampStateBrightnessFieldForID: lampID];
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

-(void)postGetLampStateHueField: (NSString *)lampID
{
    LSFGarbageCollector *garbageCollector = [LSFGarbageCollector getGarbageCollector];

    if (![garbageCollector isLampExpired: lampID])
    {
        NSLog(@"LSFSampleLampManagerCallback - postGetLampStateHueField() lampID is not expired, retrying getLampStateHueField()");

        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampManager getLampStateHueFieldForID: lampID];
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

-(void)postGetLampStateSaturationField: (NSString *)lampID
{
    LSFGarbageCollector *garbageCollector = [LSFGarbageCollector getGarbageCollector];

    if (![garbageCollector isLampExpired: lampID])
    {
        NSLog(@"LSFSampleLampManagerCallback - postGetLampStateSaturationField() lampID is not expired, retrying getLampStateSaturationField()");

        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampManager getLampStateSaturationFieldForID: lampID];
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

-(void)postGetLampStateColorTempField: (NSString *)lampID
{
    LSFGarbageCollector *garbageCollector = [LSFGarbageCollector getGarbageCollector];

    if (![garbageCollector isLampExpired: lampID])
    {
        NSLog(@"LSFSampleLampManagerCallback - postGetLampStateColorTempField() lampID is not expired, retrying getLampStateColorTempField()");

        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampManager getLampStateColorTempFieldForID: lampID];
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
