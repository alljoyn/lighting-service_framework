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

#import "LSFStatePulseEffect.h"
#import "LSFTypes.h"
#import "LSFUtils.h"

@interface LSFStatePulseEffect()

@property (nonatomic, readonly) lsf::PulseLampsLampGroupsWithState *statePulseEffect;

@end

@implementation LSFStatePulseEffect

@synthesize statePulseEffect = _statePulseEffect;

-(id)initWithLampIDs: (NSArray *)lampIDs lampGroupIDs: (NSArray *)lampGroupIDs toState: (LSFLampState *)toLampState period: (unsigned int)period duration: (unsigned int)duration andNumPulses: (unsigned int)numPulses
{
    self = [super init];
    
    if (self)
    {
        lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
        lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
        self.handle = new lsf::PulseLampsLampGroupsWithState(lampIDList, lampGroupIDList, *(static_cast<lsf::LampState*>(toLampState.handle)), period, duration, numPulses);
    }
    
    return self;
}

-(id)initWithLampIDs: (NSArray *)lampIDs lampGroupIDs: (NSArray *)lampGroupIDs fromState: (LSFLampState *)fromLampState toState: (LSFLampState *)toLampState period: (unsigned int)period duration: (unsigned int)duration andNumPulses: (unsigned int)numPulses
{
    self = [super init];
    
    if (self)
    {
        lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
        lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
        self.handle = new lsf::PulseLampsLampGroupsWithState(lampIDList, lampGroupIDList, *(static_cast<lsf::LampState*>(fromLampState.handle)), *(static_cast<lsf::LampState*>(toLampState.handle)), period, duration, numPulses);
    }
    
    return self;
}

-(void)setLamps: (NSArray *)lampIDs
{
    lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
    self.statePulseEffect->lamps = lampIDList;
}

-(NSArray *)lamps
{
    lsf::LSFStringList lampIDs = self.statePulseEffect->lamps;
    return [LSFUtils convertStringListToNSArray: lampIDs];
}

-(void)setLampGroups: (NSArray *)lampGroupIDs
{
    lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
    self.statePulseEffect->lampGroups = lampGroupIDList;
}

-(NSArray *)lampGroups
{
    lsf::LSFStringList lampGroupIDs = self.statePulseEffect->lampGroups;
    return [LSFUtils convertStringListToNSArray: lampGroupIDs];
}

-(void)setPeriod: (unsigned int)period
{
    self.statePulseEffect->pulsePeriod = period;
}

-(unsigned int)period
{
    return self.statePulseEffect->pulsePeriod;
}

-(void)setDuration: (unsigned int)duration
{
    self.statePulseEffect->pulseDuration = duration;
}

-(unsigned int)duration
{
    return self.statePulseEffect->pulseDuration;
}

-(void)setNumPulses: (unsigned int)numPulses
{
    self.statePulseEffect->numPulses = numPulses;
}

-(unsigned int)numPulses
{
    return self.statePulseEffect->numPulses;
}

-(void)setFromLampState: (LSFLampState *)fromLampState
{
    self.statePulseEffect->fromState = *(static_cast<lsf::LampState*>(fromLampState.handle));
}

-(LSFLampState *)fromLampState
{
    return [[LSFLampState alloc] initWithOnOff: self.statePulseEffect->fromState.onOff brightness: self.statePulseEffect->fromState.brightness hue: self.statePulseEffect->fromState.hue saturation: self.statePulseEffect->fromState.saturation colorTemp: self.statePulseEffect->fromState.colorTemp];
}

-(void)setToLampState: (LSFLampState *)toLampState
{
    self.statePulseEffect->toState = *(static_cast<lsf::LampState*>(toLampState.handle));
}

-(LSFLampState *)toLampState
{
    return [[LSFLampState alloc] initWithOnOff: self.statePulseEffect->toState.onOff brightness: self.statePulseEffect->toState.brightness hue: self.statePulseEffect->toState.hue saturation: self.statePulseEffect->toState.saturation colorTemp: self.statePulseEffect->toState.colorTemp];
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
- (lsf::PulseLampsLampGroupsWithState *)statePulseEffect
{
    return static_cast<lsf::PulseLampsLampGroupsWithState*>(self.handle);
}

@end
