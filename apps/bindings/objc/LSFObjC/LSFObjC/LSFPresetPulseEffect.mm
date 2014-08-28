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

#import "LSFPresetPulseEffect.h"
#import "LSFTypes.h"
#import "LSFUtils.h"

@interface LSFPresetPulseEffect()

@property (nonatomic, readonly) lsf::PulseLampsLampGroupsWithPreset *presetPulseEffect;

@end

@implementation LSFPresetPulseEffect

@synthesize presetPulseEffect = _presetPulseEffect;

-(id)initWithLampIDs: (NSArray *)lampIDs lampGroupIDs: (NSArray *)lampGroupIDs toPresetID: (NSString *)toPresetID period: (unsigned int)period duration: (unsigned int)duration andNumPulses: (unsigned int)numPulses
{
    self = [super init];
    
    if (self)
    {
        lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
        lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
        std::string tpid([toPresetID UTF8String]);
        self.handle = new lsf::PulseLampsLampGroupsWithPreset(lampIDList, lampGroupIDList, tpid, period, duration, numPulses);
    }
    
    return self;
}

-(id)initWithLampIDs: (NSArray *)lampIDs lampGroupIDs: (NSArray *)lampGroupIDs fromPresetID: (NSString *)fromPresetID toPresetID: (NSString *)toPresetID period: (unsigned int)period duration: (unsigned int)duration andNumPulses: (unsigned int)numPulses
{
    self = [super init];
    
    if (self)
    {
        lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
        lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
        std::string fpid([fromPresetID UTF8String]);
        std::string tpid([toPresetID UTF8String]);
        self.handle = new lsf::PulseLampsLampGroupsWithPreset(lampIDList, lampGroupIDList, fpid, tpid, period, duration, numPulses);
    }
    
    return self;
}

-(void)setLamps: (NSArray *)lampIDs
{
    lsf::LSFStringList lampIDList = [LSFUtils convertNSArrayToStringList: lampIDs];
    self.presetPulseEffect->lamps = lampIDList;
}

-(NSArray *)lamps
{
    lsf::LSFStringList lampIDs = self.presetPulseEffect->lamps;
    return [LSFUtils convertStringListToNSArray: lampIDs];
}

-(void)setLampGroups: (NSArray *)lampGroupIDs
{
    lsf::LSFStringList lampGroupIDList = [LSFUtils convertNSArrayToStringList: lampGroupIDs];
    self.presetPulseEffect->lampGroups = lampGroupIDList;
}

-(NSArray *)lampGroups
{
    lsf::LSFStringList lampGroupIDs = self.presetPulseEffect->lampGroups;
    return [LSFUtils convertStringListToNSArray: lampGroupIDs];
}

-(void)setPeriod: (unsigned int)period
{
    self.presetPulseEffect->pulsePeriod = period;
}

-(unsigned int)period
{
    return self.presetPulseEffect->pulsePeriod;
}

-(void)setDuration: (unsigned int)duration
{
    self.presetPulseEffect->pulseDuration = duration;
}

-(unsigned int)duration
{
    return self.presetPulseEffect->pulseDuration;
}

-(void)setNumPulses: (unsigned int)numPulses
{
    self.presetPulseEffect->numPulses = numPulses;
}

-(unsigned int)numPulses
{
    return self.presetPulseEffect->numPulses;
}

-(void)setFromPresetID: (NSString *)fromPresetID
{
    std::string fpid([fromPresetID UTF8String]);
    self.presetPulseEffect->fromPreset = fpid;
}

-(NSString *)fromPresetID
{
    return [NSString stringWithUTF8String: (self.presetPulseEffect->fromPreset).c_str()];
}

-(void)setToPresetID: (NSString *)toPresetID
{
    std::string tpid([toPresetID UTF8String]);
    self.presetPulseEffect->toPreset = tpid;
}

-(NSString *)toPresetID
{
    return [NSString stringWithUTF8String: (self.presetPulseEffect->toPreset).c_str()];
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
- (lsf::PulseLampsLampGroupsWithPreset *)presetPulseEffect
{
    return static_cast<lsf::PulseLampsLampGroupsWithPreset*>(self.handle);
}

@end
