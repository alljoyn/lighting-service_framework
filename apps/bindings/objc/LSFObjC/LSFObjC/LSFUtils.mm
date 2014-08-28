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

#import "LSFUtils.h"
#import "LSFLampState.h"
#import "LSFPresetPulseEffect.h"
#import "LSFPresetTransitionEffect.h"
#import "LSFStatePulseEffect.h"
#import "LSFStateTransitionEffect.h"

@implementation LSFUtils

+(lsf::LSFStringList)convertNSArrayToStringList: (NSArray *)array
{
    lsf::LSFStringList stringList;
    
    for (NSString *string in array)
    {
        stringList.push_back([string UTF8String]);
    }
    
    return stringList;
}

+(NSArray *)convertStringListToNSArray: (lsf::LSFStringList)list
{
    NSMutableArray *array = [[NSMutableArray alloc] init];
    
    for (std::list<lsf::LSFString>::const_iterator iter = list.begin(); iter != list.end(); ++iter)
    {
        [array addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    return array;
}

+(lsf::TransitionLampsLampGroupsToStateList)convertNSArrayToStateTransitionEffects: (NSArray *)stateTransitionEffects
{
    lsf::TransitionLampsLampGroupsToStateList stateTransitionEffectsList;
    
    for (LSFStateTransitionEffect *ste in stateTransitionEffects)
    {
        stateTransitionEffectsList.push_back(*(static_cast<lsf::TransitionLampsLampGroupsToState*>(ste.handle)));
    }
    
    return stateTransitionEffectsList;
}

+(lsf::TransitionLampsLampGroupsToPresetList)convertNSArrayToPresetTransitionEffects: (NSArray *)presetTransitionEffects
{
    lsf::TransitionLampsLampGroupsToPresetList presetTransitionEffectsList;
    
    for (LSFPresetTransitionEffect *pte in presetTransitionEffects)
    {
        presetTransitionEffectsList.push_back(*(static_cast<lsf::TransitionLampsLampGroupsToPreset*>(pte.handle)));
    }
    
    return presetTransitionEffectsList;
}

+(lsf::PulseLampsLampGroupsWithStateList)convertNSArrayToStatePulseEffects: (NSArray *)statePulseEffects
{
    lsf::PulseLampsLampGroupsWithStateList statePulseEffectsList;
    
    for (LSFStatePulseEffect *spe in statePulseEffects)
    {
        statePulseEffectsList.push_back(*(static_cast<lsf::PulseLampsLampGroupsWithState*>(spe.handle)));
    }
    
    return statePulseEffectsList;
}

+(lsf::PulseLampsLampGroupsWithPresetList)convertNSArrayToPresetPulseEffects: (NSArray *)presetPulseEffects
{
    lsf::PulseLampsLampGroupsWithPresetList presetPulseEffectsList;
    
    for (LSFPresetPulseEffect *ppe in presetPulseEffects)
    {
        presetPulseEffectsList.push_back(*(static_cast<lsf::PulseLampsLampGroupsWithPreset*>(ppe.handle)));
    }
    
    return presetPulseEffectsList;
}

+(NSArray *)convertStateTransitionEffectsToNSArray: (lsf::TransitionLampsLampGroupsToStateList)stateTransitionEffects
{
    NSMutableArray *array = [[NSMutableArray alloc] init];
    
    for (lsf::TransitionLampsLampGroupsToStateList::const_iterator iter = stateTransitionEffects.begin(); iter != stateTransitionEffects.end(); ++iter)
    {
        NSArray *lampIDs = [self convertStringListToNSArray: (*iter).lamps];
        NSArray *lampGroupIDs = [self convertStringListToNSArray: (*iter).lampGroups];
        
        LSFLampState *state = [[LSFLampState alloc] initWithOnOff: (*iter).state.onOff brightness: (*iter).state.brightness hue: (*iter).state.hue saturation: (*iter).state.saturation colorTemp: (*iter).state.colorTemp];
        
        LSFStateTransitionEffect *ste = [[LSFStateTransitionEffect alloc] initWithLampIDs: lampIDs lampGroupIDs: lampGroupIDs lampState: state andTransitionPeriod: (*iter).transitionPeriod];
        [array addObject: ste];
    }
    
    return array;
}

+(NSArray *)convertPresetTransitionEffectsToNSArray: (lsf::TransitionLampsLampGroupsToPresetList)presetTransitionEffects
{
    NSMutableArray *array = [[NSMutableArray alloc] init];
    
    for (lsf::TransitionLampsLampGroupsToPresetList::const_iterator iter = presetTransitionEffects.begin(); iter != presetTransitionEffects.end(); ++iter)
    {
        NSArray *lampIDs = [self convertStringListToNSArray: (*iter).lamps];
        NSArray *lampGroupIDs = [self convertStringListToNSArray: (*iter).lampGroups];
        
        LSFPresetTransitionEffect *pte = [[LSFPresetTransitionEffect alloc] initWithLampIDs: lampIDs lampGroupIDs: lampGroupIDs presetID: [NSString stringWithUTF8String: ((*iter).presetID).c_str()] andTransitionPeriod: (*iter).transitionPeriod];
        
        [array addObject: pte];
    }
    
    return array;
}

+(NSArray *)convertStatePulseEffectsToNSArray: (lsf::PulseLampsLampGroupsWithStateList)statePulseEffects
{
    NSMutableArray *array = [[NSMutableArray alloc] init];
    
    for (lsf::PulseLampsLampGroupsWithStateList::const_iterator iter = statePulseEffects.begin(); iter != statePulseEffects.end(); ++iter)
    {
        NSArray *lampIDs = [self convertStringListToNSArray: (*iter).lamps];
        NSArray *lampGroupIDs = [self convertStringListToNSArray: (*iter).lampGroups];
        
        LSFLampState *fromState = [[LSFLampState alloc] initWithOnOff: (*iter).fromState.onOff brightness: (*iter).fromState.brightness hue: (*iter).fromState.hue saturation: (*iter).fromState.saturation colorTemp: (*iter).fromState.colorTemp];
        LSFLampState *toState = [[LSFLampState alloc] initWithOnOff: (*iter).toState.onOff brightness: (*iter).toState.brightness hue: (*iter).toState.hue saturation: (*iter).toState.saturation colorTemp: (*iter).toState.colorTemp];
        
        LSFStatePulseEffect *spe = [[LSFStatePulseEffect alloc] initWithLampIDs: lampIDs lampGroupIDs: lampGroupIDs fromState: fromState toState: toState period: (*iter).pulsePeriod duration: (*iter).pulseDuration andNumPulses: (*iter).numPulses];
        
        [array addObject: spe];
    }
    
    return array;
}

+(NSArray *)convertPresetPulseEffectsToNSArray: (lsf::PulseLampsLampGroupsWithPresetList)presetPulseEffects
{
    NSMutableArray *array = [[NSMutableArray alloc] init];
    
    for (lsf::PulseLampsLampGroupsWithPresetList::const_iterator iter = presetPulseEffects.begin(); iter != presetPulseEffects.end(); ++iter)
    {
        NSArray *lampIDs = [self convertStringListToNSArray: (*iter).lamps];
        NSArray *lampGroupIDs = [self convertStringListToNSArray: (*iter).lampGroups];
        
        LSFPresetPulseEffect *ppe = [[LSFPresetPulseEffect alloc] initWithLampIDs: lampIDs lampGroupIDs: lampGroupIDs fromPresetID: [NSString stringWithUTF8String: ((*iter).fromPreset).c_str()] toPresetID: [NSString stringWithUTF8String: ((*iter).toPreset).c_str()] period: (*iter).pulsePeriod duration: (*iter).pulseDuration andNumPulses: (*iter).numPulses];
        
        [array addObject: ppe];
    }
    
    return array;
}

@end
