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

#import "LSFStateTransitionEffect.h"
#import "LSFTypes.h"
#import "LSFUtils.h"

@interface LSFStateTransitionEffect()

@property (nonatomic, readonly) lsf::TransitionLampsLampGroupsToState *stateTransitionEffect;

@end

@implementation LSFStateTransitionEffect

@synthesize stateTransitionEffect = _stateTransitionEffect;

-(id)initWithLampIDs: (NSArray *)lampIDs lampGroupIDs: (NSArray *)lampGroupIDs andLampState: (LSFLampState *)lampState
{
    self = [super init];
    
    if (self)
    {
        lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
        lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
        self.handle = new lsf::TransitionLampsLampGroupsToState(lampIDList, lampGroupIDList, *(static_cast<lsf::LampState*>(lampState.handle)));
    }
    
    return self;
}

-(id)initWithLampIDs: (NSArray *)lampIDs lampGroupIDs: (NSArray *)lampGroupIDs lampState: (LSFLampState *)lampState andTransitionPeriod: (unsigned int)transitionPeriod
{
    self = [super init];
    
    if (self)
    {
        lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
        lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
        self.handle = new lsf::TransitionLampsLampGroupsToState(lampIDList, lampGroupIDList, *(static_cast<lsf::LampState*>(lampState.handle)), transitionPeriod);
    }
    
    return self;
}

-(void)setLamps: (NSArray *)lampIDs
{
    lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
    self.stateTransitionEffect->lamps = lampIDList;
}

-(NSArray *)lamps
{
    lsf::LSFStringList lampIDs = self.stateTransitionEffect->lamps;
    return [LSFUtils convertStringListToNSArray: lampIDs];
}

-(void)setLampGroups: (NSArray *)lampGroupIDs
{
    lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
    self.stateTransitionEffect->lampGroups = lampGroupIDList;
}

-(NSArray *)lampGroups
{
    lsf::LSFStringList lampGroupIDs = self.stateTransitionEffect->lampGroups;
    return [LSFUtils convertStringListToNSArray: lampGroupIDs];
}

-(void)setTransitionPeriod: (unsigned int)transitionPeriod
{
    self.stateTransitionEffect->transitionPeriod = transitionPeriod;
}

-(unsigned int)transitionPeriod
{
    return self.stateTransitionEffect->transitionPeriod;
}

-(void)setLampState: (LSFLampState *)lampState
{
    self.stateTransitionEffect->state = *(static_cast<lsf::LampState*>(lampState.handle));
}

-(LSFLampState *)lampState
{
    return [[LSFLampState alloc] initWithOnOff: self.stateTransitionEffect->state.onOff brightness: self.stateTransitionEffect->state.brightness hue: self.stateTransitionEffect->state.hue saturation: self.stateTransitionEffect->state.saturation colorTemp: self.stateTransitionEffect->state.colorTemp];
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
- (lsf::TransitionLampsLampGroupsToState *)stateTransitionEffect
{
    return static_cast<lsf::TransitionLampsLampGroupsToState*>(self.handle);
}

@end
