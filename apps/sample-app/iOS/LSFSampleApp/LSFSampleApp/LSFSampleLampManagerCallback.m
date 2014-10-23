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
#import "LSFTabManager.h"
#import "LSFEnums.h"

@interface LSFSampleLampManagerCallback()

@property (nonatomic, strong) dispatch_queue_t queue;

-(void)postUpdateLampID: (NSString *)lampID;
-(void)postRemoveLampID: (NSString *)lampID;
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
-(void)updateLampGroupState;
-(void)updateLampWithID: (NSString *)lampID withCallbackOperation: (LampCallbackOperation)callbackOp;
-(void)deleteLampWithID: (NSString *)lampID withLampName: (NSString *)lampName andCallbackOperation: (LampCallbackOperation)callbackOp;

@end

@implementation LSFSampleLampManagerCallback

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
    NSLog(@"LSFSampleLampManagerCallback - getAllLampIDs(). LampIDs Count = %i", lampIDs.count);

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
    //NSLog(@"LSFSampleLampGroupManager - GetLampName()");

    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampName: name forLampID: lampID];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampName() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, ([LSFConstants getConstants]).RETRY_INTERVAL * NSEC_PER_SEC), self.queue, ^{
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

-(void)lampsNameChangedWithID: (NSString *)lampID andName: (NSString *)name
{
    dispatch_async(self.queue, ^{
        [self updateLampName: name forLampID: lampID];
    });
}

-(void)lampsFound: (NSArray *)lampIDs
{
    NSLog(@"LSFSampleLampGroupManager - LampsFound()");

    dispatch_async(self.queue, ^{
        for (NSString *lampID in lampIDs)
        {
            [self postUpdateLampID: lampID];
        }
    });
}

-(void)lampsLost: (NSArray *)lampIDs
{
    NSLog(@"LSFSampleLampManagerCallback - lampsLost()");

    dispatch_async(self.queue, ^{
        for (NSString *lampID in lampIDs)
        {
            [self postRemoveLampID: lampID];
        }
    });
}

-(void)getLampDetailsReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andLampDetails: (LSFLampDetails *)details
{
    //NSLog(@"LSFSampleLampGroupManager - GetLampDetails()");

    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampDetailsForID: lampID withDetails: details];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampDetails() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, ([LSFConstants getConstants]).RETRY_INTERVAL * NSEC_PER_SEC), self.queue, ^{
            [self postGetLampDetails: lampID];
        });
    }
}

-(void)getLampParametersReplyWithCode: (LSFResponseCode)rc lampID: (NSString *)lampID andLampParameters: (LSFLampParameters *)params
{
    //NSLog(@"LSFSampleLampGroupManager - GetLampParameters()");

    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampParametersForID: lampID withParameters: params];
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampParameters() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, ([LSFConstants getConstants]).RETRY_INTERVAL * NSEC_PER_SEC), self.queue, ^{
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
    //NSLog(@"LSFSampleLampGroupManager - GetLampState()");

    if (rc == LSF_OK)
    {
        dispatch_async(self.queue, ^{
            [self updateLampStateForID: lampID withState: state];
            [self postGetLampParameters: lampID]; //TODO - only call when LampInfo page is being displayed
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampState() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, ([LSFConstants getConstants]).RETRY_INTERVAL * NSEC_PER_SEC), self.queue, ^{
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
            [self postGetLampParameters: lampID]; //TODO - only call when LampInfo page is being displayed
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampStateOnOffField() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, ([LSFConstants getConstants]).RETRY_INTERVAL * NSEC_PER_SEC), self.queue, ^{
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

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, ([LSFConstants getConstants]).RETRY_INTERVAL * NSEC_PER_SEC), self.queue, ^{
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

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, ([LSFConstants getConstants]).RETRY_INTERVAL * NSEC_PER_SEC), self.queue, ^{
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
            [self postGetLampParameters: lampID]; //TODO - only call when LampInfo page is being displayed
        });
    }
    else
    {
        NSLog(@"LSFSampleLampGroupManager - GetLampStateBrightnessField() returned an error code: %i", rc);

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, ([LSFConstants getConstants]).RETRY_INTERVAL * NSEC_PER_SEC), self.queue, ^{
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

        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, ([LSFConstants getConstants]).RETRY_INTERVAL * NSEC_PER_SEC), self.queue, ^{
            [self postGetLampStateColorTempField: lampID];
        });
    }
}

-(void)resetLampStateReplyWithCode: (LSFResponseCode)rc andLampID: (NSString *)lampID
{
    //TODO - unused at this point
}

-(void)lampsStateChangedWithID: (NSString *)lampID andLampState: (LSFLampState *)state
{
    NSLog(@"LSFSampleLampManagerCallback - lampsStateChanged() for lamp ID %@", lampID);

    dispatch_async(self.queue, ^{
        [self updateLampStateForID: lampID withState: state];
        [self postGetLampParameters: lampID]; //TODO - only call when LampInfo page is being displayed
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
}

-(void)transitionLampStateHueFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManager - TransitionLampStateHueField() returned an error code: %i", rc);
    }
}

-(void)transitionLampStateSaturationFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManager - TransitionLampStateSaturationField() returned an error code: %i", rc);
    }
}

-(void)transitionLampStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManager - TransitionLampStateBrightnessField() returned an error code: %i", rc);
    }
}

-(void)transitionLampStateColorTempFieldReplyWithCode: (LSFResponseCode)rc andLampID: (NSString*)lampID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManager - TransitionLampStateColorTempField() returned an error code: %i", rc);
    }
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
        //NSLog(@"Creating Lamp Model for ID: %@", lampID);
        lampModel = [[LSFLampModel alloc] initWithLampID: lampID];
        [lamps setValue: lampModel forKey: lampID];

        dispatch_async(self.queue, ^{
            LSFTabManager *tabManager = [LSFTabManager getTabManager];
            [tabManager updateLampsTab];
        });

        [self updateLampWithID: lampID withCallbackOperation: LampFound];
    }

    if ([lampModel.name isEqualToString: @"[Loading lamp info...]"])
    {
        //NSLog(@"Getting data set for lamp: %@", lampModel.theID);

        //Calls are only made when lamp name is still equal to default name
        [lampManager getLampDataSetForID: lampID];
    }

    if (aboutData != nil)
    {
        //NSLog(@"Updating lamp about data");
        lampModel.aboutData = aboutData;
    }
}

-(void)postRemoveLampID: (NSString *)lampID
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    NSString *lampName = [NSString stringWithString: lampModel.name];

    if (lampModel != nil)
    {
        [lamps removeObjectForKey: lampID];
        [self deleteLampWithID: lampID withLampName: lampName andCallbackOperation: LampDeleted];

        dispatch_async(self.queue, ^{
            LSFTabManager *tabManager = [LSFTabManager getTabManager];
            [tabManager updateLampsTab];
        });
    }
    else
    {
        NSLog(@"postRemoveLampID - lampModel is nil, cannot remove from model");
    }
}

-(void)updateLampName: (NSString *)name forLampID: (NSString *)lampID
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    
    if (lampModel != nil)
    {
        lampModel.name = [NSString stringWithString: name];
        [self updateLampWithID: lampID withCallbackOperation: LampNameUpdated];
    }
    else
    {
        NSLog(@"updateLampName - Did not find LampModel for lamp ID");
    }
}

-(void)postGetLampName: (NSString *)lampID
{
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    if (ajManager.isConnectedToController)
    {
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
        [self updateLampWithID: lampID withCallbackOperation: LampDetailsUpdated];
    }
    else
    {
        NSLog(@"updateLampDetails - Did not find LampModel for lamp ID");
    }
}

-(void)postGetLampDetails: (NSString *)lampID
{
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    if (ajManager.isConnectedToController)
    {
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
        [self updateLampWithID: lampID withCallbackOperation: LampParametersUpdated];
    }
    else
    {
        NSLog(@"updateLampParameters - Did not find LampModel for lamp ID");
    }
}

-(void)postGetLampParameters: (NSString *)lampID
{
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    if (ajManager.isConnectedToController)
    {
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

        [self updateLampGroupState];
        [self updateLampWithID: lampID withCallbackOperation: LampStateUpdated];
    }
    else
    {
        NSLog(@"updateLampState - Did not find LampModel for lamp ID");
    }
}

-(void)postGetLampState: (NSString *)lampID
{
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    if (ajManager.isConnectedToController)
    {
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
        [self updateLampWithID: lampID withCallbackOperation: LampStateUpdated];
    }
    else
    {
        NSLog(@"updateLampStateOnOff - Did not find LampModel for lamp ID");
    }
}

-(void)postGetLampStateOnOffField: (NSString *)lampID
{
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    if (ajManager.isConnectedToController)
    {
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
        [self updateLampWithID: lampID withCallbackOperation: LampStateUpdated];
    }
    else
    {
        NSLog(@"updateLampStateBrightness - Did not find LampModel for lamp ID");
    }
}

-(void)postGetLampStateBrightnessField: (NSString *)lampID
{
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    if (ajManager.isConnectedToController)
    {
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
        [self updateLampWithID: lampID withCallbackOperation: LampStateUpdated];
    }
    else
    {
        NSLog(@"updateLampStateHue - Did not find LampModel for lamp ID");
    }
}

-(void)postGetLampStateHueField: (NSString *)lampID
{
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    if (ajManager.isConnectedToController)
    {
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
        [self updateLampWithID: lampID withCallbackOperation: LampStateUpdated];
    }
    else
    {
        NSLog(@"updateLampStateSaturation - Did not find LampModel for lamp ID");
    }
}

-(void)postGetLampStateSaturationField: (NSString *)lampID
{
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    if (ajManager.isConnectedToController)
    {
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
        [self updateLampWithID: lampID withCallbackOperation: LampStateUpdated];
    }
    else
    {
        NSLog(@"updateLampStateColorTemp - Did not find LampModel for lamp ID");
    }
}

-(void)postGetLampStateColorTempField: (NSString *)lampID
{
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    if (ajManager.isConnectedToController)
    {
        [ajManager.lsfLampManager getLampStateColorTempFieldForID: lampID];
    }
}

-(void)updateLampGroupState
{
    dispatch_async(self.queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.slgmc refreshAllLampGroupIDs];
    });
}

-(void)updateLampWithID: (NSString *)lampID withCallbackOperation: (LampCallbackOperation)callbackOp
{
    dispatch_async(dispatch_get_main_queue(), ^{
        NSNumber *lampOp = [[NSNumber alloc] initWithInt: callbackOp];
        NSDictionary *userInfoDict = [[NSDictionary alloc] initWithObjects: [[NSArray alloc] initWithObjects: lampOp, lampID, nil] forKeys: [[NSArray alloc] initWithObjects: @"operation", @"lampID", nil]];
        [[NSNotificationCenter defaultCenter] postNotificationName: @"LampNotification" object: self userInfo: userInfoDict];
    });
}

-(void)deleteLampWithID: (NSString *)lampID withLampName: (NSString *)lampName andCallbackOperation: (LampCallbackOperation)callbackOp
{
    [self updateLampGroupState];

    dispatch_async(dispatch_get_main_queue(), ^{
        NSNumber *lampOp = [[NSNumber alloc] initWithInt: callbackOp];
        NSDictionary *userInfoDict = [[NSDictionary alloc] initWithObjects: [[NSArray alloc] initWithObjects: lampOp, lampID, lampName, nil] forKeys: [[NSArray alloc] initWithObjects: @"operation", @"lampID", @"lampName", nil]];
        [[NSNotificationCenter defaultCenter] postNotificationName: @"LampNotification" object: self userInfo: userInfoDict];
    });
}

@end
