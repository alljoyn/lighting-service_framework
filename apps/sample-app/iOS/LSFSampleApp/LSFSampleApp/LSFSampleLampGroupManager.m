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

#import "LSFSampleLampGroupManager.h"
#import "LSFAllLampsLampGroup.h"
#import "LSFConstants.h"
#import "LSFLampModelContainer.h"
#import "LSFAllJoynManager.h"

@interface LSFSampleLampGroupManager()

@property (nonatomic, weak) id<LSFLampGroupManagerCallbackDelegate> delegate;
@property (nonatomic, strong) LSFAllLampsLampGroup *allLampsLampGroup;

@end

@implementation LSFSampleLampGroupManager

@synthesize delegate = _delegate;
@synthesize allLampsLampGroup = _allLampsLampGroup;

-(id)initWithControllerClient: (LSFControllerClient *)controllerClient andLampManagerCallbackDelegate: (id<LSFLampGroupManagerCallbackDelegate>)lgmDelegate
{
    //NSLog(@"LSFSampleLampGroupManager - constructor executing");
    
    self = [super initWithControllerClient: controllerClient andLampManagerCallbackDelegate: lgmDelegate];
    
    if (self)
    {
        self.delegate = lgmDelegate;
        self.allLampsLampGroup = [[LSFAllLampsLampGroup alloc] init];
    }
    
    return self;
}

-(ControllerClientStatus)getLampGroupNameForID: (NSString *)groupID
{
    //NSLog(@"LSFSampleLampGroupManager - getLampGroupName() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        [self.delegate getLampGroupNameReplyWithCode: LSF_OK groupID: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID language: @"en" andGroupName: @"All Lamps"];
    }
    else
    {
        status = [super getLampGroupNameForID: groupID];
    }
    
    return status;
}

-(ControllerClientStatus)getLampGroupNameForID: (NSString *)groupID andLanguage: (NSString *)language
{
    //NSLog(@"LSFSampleLampGroupManager - getLampGroupName() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        [self.delegate getLampGroupNameReplyWithCode: LSF_OK groupID: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID language: language andGroupName: @"All Lamps"];
    }
    else
    {
        status = [super getLampGroupNameForID: groupID andLanguage: @"en"];
    }
    
    return status;
}

-(ControllerClientStatus)getLampGroupWithID: (NSString *)groupID
{
    //NSLog(@"LSFSampleLampGroupManager - getLampGroup() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        [self.delegate getLampGroupReplyWithCode: LSF_OK groupID: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID andLampGroup: self.allLampsLampGroup];
    }
    else
    {
        status = [super getLampGroupWithID: groupID];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID toState: (LSFLampState *)state
{
    //NSLog(@"LSFSampleLampGroupManager - transitionLampGroupToState() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID toLampState: state];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID toState: state];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID toState: (LSFLampState *)state withTransitionPeriod: (unsigned int)transitionPeriod
{
    //NSLog(@"LSFSampleLampGroupManager - transitionLampGroupToState() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID toLampState: state withTransitionPeriod: transitionPeriod];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super getLampGroupWithID: groupID];
    }
    
    return status;
}

-(ControllerClientStatus)pulseLampGroupID: (NSString *)groupID toLampState: (LSFLampState *)toState withPeriod: (unsigned int)period duration: (unsigned int)duration andNumPulses: (unsigned int)numPulses
{
    //TODO
    
    //NSLog(@"LSFSampleLampGroupManager - pulseLampGroupWithToState() executing");
    return CONTROLLER_CLIENT_OK;
}

-(ControllerClientStatus)pulseLampGroupID: (NSString *)groupID toLampState: (LSFLampState *)toState withPeriod: (unsigned int)period duration: (unsigned int)duration numPulses: (unsigned int)numPulses fromLampState: (LSFLampState *)fromState
{
    //TODO
    
    //NSLog(@"LSFSampleLampGroupManager - pulseLampGroupWithToFromState() executing");
    return CONTROLLER_CLIENT_OK;
}

-(ControllerClientStatus)pulseLampGroupID: (NSString *)groupID toPreset: (NSString *)toPresetID withPeriod: (unsigned int)period duration: (unsigned int)duration andNumPulses: (unsigned int)numPulses
{
    //TODO
    
    //NSLog(@"LSFSampleLampGroupManager - pulseLampGroupWithToPreset() executing");
    return CONTROLLER_CLIENT_OK;
}

-(ControllerClientStatus)pulseLampGroupID: (NSString *)groupID toPreset: (NSString *)toPresetID withPeriod: (unsigned int)period duration: (unsigned int)duration andNumPulses: (unsigned int)numPulses fromPreset: (NSString *)fromPresetID
{
    //TODO
    
    //NSLog(@"LSFSampleLampGroupManager - pulseLampGroupWithToFromPreset() executing");
    return CONTROLLER_CLIENT_OK;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID onOffField: (BOOL)onOff
{
    //NSLog(@"LSFSampleLampGroupManager - transitionOnOff() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID onOffField: onOff];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID onOffField: onOff];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID hueField: (unsigned int)hue
{
    //NSLog(@"LSFSampleLampGroupManager - transitionHue() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID hueField: hue];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID hueField: hue];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID hueField: (unsigned int)hue withTransitionPeriod: (unsigned int)transitionPeriod
{
    //NSLog(@"LSFSampleLampGroupManager - transitionHueWithPeriod() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID hueField: hue withTransitionPeriod: transitionPeriod];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID hueField: hue withTransitionPeriod: transitionPeriod];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID saturationField: (unsigned int)saturation
{
    //NSLog(@"LSFSampleLampGroupManager - transitionSaturation() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID saturationField: saturation];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID saturationField: saturation];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID saturationField: (unsigned int)saturation withTransitionPeriod: (unsigned int)transitionPeriod
{
    //NSLog(@"LSFSampleLampGroupManager - transitionSaturationWithPeriod() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID saturationField: saturation withTransitionPeriod: transitionPeriod];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID saturationField: saturation withTransitionPeriod: transitionPeriod];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID brightnessField: (unsigned int)brightness
{
    //NSLog(@"LSFSampleLampGroupManager - transitionBrightness() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID brightnessField: brightness];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID brightnessField: brightness];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID brightnessField: (unsigned int)brightness withTransitionPeriod: (unsigned int)transitionPeriod
{
    //NSLog(@"LSFSampleLampGroupManager - transitionBrightnessWithPeriod() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID brightnessField: brightness withTransitionPeriod: transitionPeriod];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID brightnessField: brightness withTransitionPeriod: transitionPeriod];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID colorTempField: (unsigned int)colorTemp
{
    //NSLog(@"LSFSampleLampGroupManager - transitionColorTemp() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID colorTempField: colorTemp];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID colorTempField: colorTemp];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID colorTempField: (unsigned int)colorTemp withTransitionPeriod: (unsigned int)transitionPeriod
{
    //NSLog(@"LSFSampleLampGroupManager - transitionColorTempWithPeriod() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID colorTempField: colorTemp withTransitionPeriod: transitionPeriod];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID colorTempField: colorTemp withTransitionPeriod: transitionPeriod];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID toPreset: (NSString *)presetID
{
    //NSLog(@"LSFSampleLampGroupManager - transitionToPreset() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID toPresetID: presetID];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID toPreset: presetID];
    }
    
    return status;
}

-(ControllerClientStatus)transitionLampGroupID: (NSString *)groupID toPreset: (NSString *)presetID withTransitionPeriod: (unsigned int)transitionPeriod
{
    //NSLog(@"LSFSampleLampGroupManager - transitionToPresetWithTransition() executing");
    
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;
    
    if ([groupID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
    {
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *lampID in [lamps allKeys])
        {
            status = [ajManager.lsfLampManager transitionLampID: lampID toPresetID: presetID withTransitionPeriod: transitionPeriod];
            
            if (status != CONTROLLER_CLIENT_OK)
            {
                break;
            }
        }
    }
    else
    {
        status = [super transitionLampGroupID: groupID toPreset: presetID withTransitionPeriod: transitionPeriod];
    }
    
    return status;
}

-(ControllerClientStatus)resetLampGroupStateForID: (NSString *)groupID
{
    //TODO
    
    //NSLog(@"LSFSampleLampGroupManager - resetLampGroupState() executing");
    return CONTROLLER_CLIENT_OK;
}

-(ControllerClientStatus)resetLampGroupStateOnOffFieldForID: (NSString *)groupID
{
    //TODO
    
    //NSLog(@"LSFSampleLampGroupManager - resetOnOff() executing");
    return CONTROLLER_CLIENT_OK;
}

-(ControllerClientStatus)resetLampGroupStateHueFieldForID: (NSString *)groupID
{
    //TODO
    
    //NSLog(@"LSFSampleLampGroupManager - resetHue() executing");
    return CONTROLLER_CLIENT_OK;
}

-(ControllerClientStatus)resetLampGroupStateSaturationFieldForID: (NSString *)groupID
{
    //TODO
    
    //NSLog(@"LSFSampleLampGroupManager - resetSaturation() executing");
    return CONTROLLER_CLIENT_OK;
}

-(ControllerClientStatus)resetLampGroupStateBrightnessFieldForID: (NSString *)groupID
{
    //TODO
    
    //NSLog(@"LSFSampleLampGroupManager - resetBrightness() executing");
    return CONTROLLER_CLIENT_OK;
}

-(ControllerClientStatus)resetLampGroupStateColorTempFieldForID: (NSString *)groupID
{
    //TODO
    
    //NSLog(@"LSFSampleLampGroupManager - resetColorTemp() executing");
    return CONTROLLER_CLIENT_OK;
}

@end
