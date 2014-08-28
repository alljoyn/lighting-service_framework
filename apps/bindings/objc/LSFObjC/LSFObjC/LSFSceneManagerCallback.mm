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

#import "LSFSceneManagerCallback.h"
#import "LSFUtils.h"

LSFSceneManagerCallback::LSFSceneManagerCallback(id<LSFSceneManagerCallbackDelegate> delegate) : _smDelegate(delegate)
{
    //Empty Constructor
}

LSFSceneManagerCallback::~LSFSceneManagerCallback()
{
    _smDelegate = nil;
}

void LSFSceneManagerCallback::GetAllSceneIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& sceneIDs)
{
    NSMutableArray *sceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = sceneIDs.begin(); iter != sceneIDs.end(); ++iter)
    {
        [sceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_smDelegate != nil)
    {
        [_smDelegate getAllSceneIDsReplyWithCode: responseCode andSceneIDs: sceneIDsArray];
    }
}

void LSFSceneManagerCallback::GetSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const LSFString& language, const LSFString& sceneName)
{
    if (_smDelegate != nil)
    {
        [_smDelegate getSceneNameReplyWithCode: responseCode sceneID: [NSString stringWithUTF8String: sceneID.c_str()] language: [NSString stringWithUTF8String: language.c_str()] andName: [NSString stringWithUTF8String: sceneName.c_str()]];
    }
}

void LSFSceneManagerCallback::SetSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const LSFString& language)
{
    if (_smDelegate != nil)
    {
        [_smDelegate setSceneNameReplyWithCode: responseCode sceneID: [NSString stringWithUTF8String: sceneID.c_str()] andLanguage: [NSString stringWithUTF8String: language.c_str()]];
    }
}

void LSFSceneManagerCallback::ScenesNameChangedCB(const LSFStringList& sceneIDs)
{
    NSMutableArray *sceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = sceneIDs.begin(); iter != sceneIDs.end(); ++iter)
    {
        [sceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_smDelegate != nil)
    {
        [_smDelegate scenesNameChanged: sceneIDsArray];
    }
}

void LSFSceneManagerCallback::CreateSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID)
{
    if (_smDelegate != nil)
    {
        [_smDelegate createSceneReplyWithCode: responseCode andSceneID: [NSString stringWithUTF8String: sceneID.c_str()]];
    }
}

void LSFSceneManagerCallback::ScenesCreatedCB(const LSFStringList& sceneIDs)
{
    NSMutableArray *sceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = sceneIDs.begin(); iter != sceneIDs.end(); ++iter)
    {
        [sceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_smDelegate != nil)
    {
        [_smDelegate scenesCreated: sceneIDsArray];
    }
}

void LSFSceneManagerCallback::UpdateSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID)
{
    if (_smDelegate != nil)
    {
        [_smDelegate updateSceneReplyWithCode: responseCode andSceneID: [NSString stringWithUTF8String: sceneID.c_str()]];
    }
}

void LSFSceneManagerCallback::ScenesUpdatedCB(const LSFStringList& sceneIDs)
{
    NSMutableArray *sceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = sceneIDs.begin(); iter != sceneIDs.end(); ++iter)
    {
        [sceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_smDelegate != nil)
    {
        [_smDelegate scenesUpdated: sceneIDsArray];
    }
}

void LSFSceneManagerCallback::DeleteSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID)
{
    if (_smDelegate != nil)
    {
        [_smDelegate deleteSceneReplyWithCode: responseCode andSceneID: [NSString stringWithUTF8String: sceneID.c_str()]];
    }
}

void LSFSceneManagerCallback::ScenesDeletedCB(const LSFStringList& sceneIDs)
{
    NSMutableArray *sceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = sceneIDs.begin(); iter != sceneIDs.end(); ++iter)
    {
        [sceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_smDelegate != nil)
    {
        [_smDelegate scenesDeleted: sceneIDsArray];
    }
}

void LSFSceneManagerCallback::GetSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const Scene& data)
{
    NSArray *stateTransitionEffects = [LSFUtils convertStateTransitionEffectsToNSArray: data.transitionToStateComponent];
    NSArray *presetTransitionEffects = [LSFUtils convertPresetTransitionEffectsToNSArray: data.transitionToPresetComponent];
    NSArray *statePulseEffects = [LSFUtils convertStatePulseEffectsToNSArray: data.pulseWithStateComponent];
    NSArray *presetPulseEffects = [LSFUtils convertPresetPulseEffectsToNSArray: data.pulseWithPresetComponent];
    
    LSFScene *scene = [[LSFScene alloc] initWithStateTransitionEffects: stateTransitionEffects presetTransitionEffects: presetTransitionEffects statePulseEffects: statePulseEffects andPresetPulseEffects: presetPulseEffects];
    
    if (_smDelegate != nil)
    {
        [_smDelegate getSceneReplyWithCode: responseCode sceneID: [NSString stringWithUTF8String: sceneID.c_str()] andScene: scene];
    }
}

void LSFSceneManagerCallback::ApplySceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID)
{
    if (_smDelegate != nil)
    {
        [_smDelegate applySceneReplyWithCode: responseCode andSceneID: [NSString stringWithUTF8String: sceneID.c_str()]];
    }
}

void LSFSceneManagerCallback::ScenesAppliedCB(const LSFStringList& sceneIDs)
{
    NSMutableArray *sceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = sceneIDs.begin(); iter != sceneIDs.end(); ++iter)
    {
        [sceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_smDelegate != nil)
    {
        [_smDelegate scenesApplied: sceneIDsArray];
    }
}
