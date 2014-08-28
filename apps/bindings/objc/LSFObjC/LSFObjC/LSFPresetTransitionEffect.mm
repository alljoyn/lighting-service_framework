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

#import "LSFPresetTransitionEffect.h"
#import "LSFTypes.h"
#import "LSFUtils.h"

@interface LSFPresetTransitionEffect()

@property (nonatomic, readonly) lsf::TransitionLampsLampGroupsToPreset *presetTransitionEffect;

@end

@implementation LSFPresetTransitionEffect

-(id)initWithLampIDs: (NSArray *)lampIDs lampGroupIDs: (NSArray *)lampGroupIDs andPresetID: (NSString *)presetID
{
    self = [super init];
    
    if (self)
    {
        lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
        lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
        std::string pid([presetID UTF8String]);
        self.handle = new lsf::TransitionLampsLampGroupsToPreset(lampIDList, lampGroupIDList, pid);
    }
    
    return self;
}

-(id)initWithLampIDs: (NSArray *)lampIDs lampGroupIDs: (NSArray *)lampGroupIDs presetID: (NSString *)presetID andTransitionPeriod: (unsigned int)transitionPeriod
{
    self = [super init];
    
    if (self)
    {
        lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
        lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
        std::string pid([presetID UTF8String]);
        self.handle = new lsf::TransitionLampsLampGroupsToPreset(lampIDList, lampGroupIDList, pid, transitionPeriod);
    }
    
    return self;
}

-(void)setLamps: (NSArray *)lampIDs
{
    lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
    self.presetTransitionEffect->lamps = lampIDList;
}

-(NSArray *)lamps
{
    lsf::LSFStringList lampIDs = self.presetTransitionEffect->lamps;
    return [LSFUtils convertStringListToNSArray: lampIDs];
}

-(void)setLampGroups: (NSArray *)lampGroupIDs
{
    lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
    self.presetTransitionEffect->lampGroups = lampGroupIDList;
}

-(NSArray *)lampGroups
{
    lsf::LSFStringList lampGroupIDs = self.presetTransitionEffect->lampGroups;
    return [LSFUtils convertStringListToNSArray: lampGroupIDs];
}

-(void)setTransitionPeriod: (unsigned int)transitionPeriod
{
    self.presetTransitionEffect->transitionPeriod = transitionPeriod;
}

-(unsigned int)transitionPeriod
{
    return self.presetTransitionEffect->transitionPeriod;
}

-(void)setPresetID: (NSString *)presetID
{
    std::string pid([presetID UTF8String]);
    self.presetTransitionEffect->presetID = pid;
}

-(NSString *)presetID
{
    return [NSString stringWithUTF8String: (self.presetTransitionEffect->presetID).c_str()];
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
- (lsf::TransitionLampsLampGroupsToPreset *)presetTransitionEffect
{
    return static_cast<lsf::TransitionLampsLampGroupsToPreset*>(self.handle);
}

@end
