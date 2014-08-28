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

#import "LSFScene.h"
#import "LSFTypes.h"
#import "LSFUtils.h"

@interface LSFScene()

@property (nonatomic, readonly) lsf::Scene *scene;

@end

@implementation LSFScene

@synthesize scene = _scene;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.handle = new lsf::Scene();
    }
    
    return self;
}

-(id)initWithStateTransitionEffects: (NSArray *)stateTransitionEffects presetTransitionEffects: (NSArray *)presetTransitionEffects statePulseEffects: (NSArray *)statePulseEffects andPresetPulseEffects: (NSArray *)presetPulseEffects
{
    self = [super init];
    
    if (self)
    {
        lsf::TransitionLampsLampGroupsToStateList tllgtsl = [LSFUtils convertNSArrayToStateTransitionEffects: stateTransitionEffects];
        lsf::TransitionLampsLampGroupsToPresetList tllgtpl = [LSFUtils convertNSArrayToPresetTransitionEffects: presetTransitionEffects];
        lsf::PulseLampsLampGroupsWithStateList pllgwsl = [LSFUtils convertNSArrayToStatePulseEffects: statePulseEffects];
        lsf::PulseLampsLampGroupsWithPresetList pllgwpl = [LSFUtils convertNSArrayToPresetPulseEffects: presetPulseEffects];
        
        self.handle = new lsf::Scene(tllgtsl, tllgtpl, pllgwsl, pllgwpl);
    }
    
    return self;
}

-(void)setTransitionToStateComponent: (NSArray *)transitionToStateComponent
{
    self.scene->transitionToStateComponent = [LSFUtils convertNSArrayToStateTransitionEffects: transitionToStateComponent];
}

-(NSArray *)transitionToStateComponent
{
    return [LSFUtils convertStateTransitionEffectsToNSArray: self.scene->transitionToStateComponent];
}

-(void)setTransitionToPresetComponent: (NSArray *)transitionToPresetComponent
{
    self.scene->transitionToPresetComponent = [LSFUtils convertNSArrayToPresetTransitionEffects: transitionToPresetComponent];
}

-(NSArray *)transitionToPresetComponent
{
    return [LSFUtils convertPresetTransitionEffectsToNSArray: self.scene->transitionToPresetComponent];
}

-(void)setPulseWithStateComponent: (NSArray *)pulseWithStateComponent
{
    self.scene->pulseWithStateComponent = [LSFUtils convertNSArrayToStatePulseEffects: pulseWithStateComponent];
}

-(NSArray *)pulseWithStateComponent
{
    return [LSFUtils convertStatePulseEffectsToNSArray: self.scene->pulseWithStateComponent];
}

-(void)setPulseWithPresetComponent: (NSArray *)pulseWithPresetComponent
{
    self.scene->pulseWithPresetComponent = [LSFUtils convertNSArrayToPresetPulseEffects: pulseWithPresetComponent];
}

-(NSArray *)pulseWithPresetComponent
{
    return [LSFUtils convertPresetPulseEffectsToNSArray: self.scene->pulseWithPresetComponent];
}

-(LSFResponseCode)isDependentOnPreset: (NSString *)presetID
{
    std::string pid([presetID UTF8String]);
    return self.scene->IsDependentOnPreset(pid);
}

-(LSFResponseCode)isDependentOnLampGroup: (NSString *)lampGroupID
{
    std::string lgid([lampGroupID UTF8String]);
    return self.scene->IsDependentOnLampGroup(lgid);
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
- (lsf::Scene *)scene
{
    return static_cast<lsf::Scene*>(self.handle);
}

@end
