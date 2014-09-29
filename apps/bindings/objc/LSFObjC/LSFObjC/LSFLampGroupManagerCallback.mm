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

#import "LSFLampGroupManagerCallback.h"

LSFLampGroupManagerCallback::LSFLampGroupManagerCallback(id<LSFLampGroupManagerCallbackDelegate> delegate) : _lgmDelegate(delegate)
{
    //Empty Constructor
}

LSFLampGroupManagerCallback::~LSFLampGroupManagerCallback()
{
    _lgmDelegate = nil;
}

void LSFLampGroupManagerCallback::GetAllLampGroupIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& lampGroupIDs)
{
    NSMutableArray *groupIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = lampGroupIDs.begin(); iter != lampGroupIDs.end(); ++iter)
    {
        [groupIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate getAllLampGroupIDsReplyWithCode: responseCode andGroupIDs: groupIDsArray];
    }
}

void LSFLampGroupManagerCallback::GetLampGroupNameReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID, const LSFString& language, const LSFString& lampGroupName)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate getLampGroupNameReplyWithCode: responseCode groupID: [NSString stringWithUTF8String: lampGroupID.c_str()] language: [NSString stringWithUTF8String: language.c_str()] andGroupName: [NSString stringWithUTF8String: lampGroupName.c_str()]];
    }
}

void LSFLampGroupManagerCallback::SetLampGroupNameReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID, const LSFString& language)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate setLampGroupNameReplyWithCode: responseCode groupID: [NSString stringWithUTF8String: lampGroupID.c_str()] andLanguage: [NSString stringWithUTF8String: language.c_str()]];
    }
}

void LSFLampGroupManagerCallback::LampGroupsNameChangedCB(const LSFStringList& lampGroupIDs)
{
    NSMutableArray *groupIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = lampGroupIDs.begin(); iter != lampGroupIDs.end(); ++iter)
    {
        [groupIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate lampGroupsNameChanged: groupIDsArray];
    }
}

void LSFLampGroupManagerCallback::CreateLampGroupReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate createLampGroupReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::LampGroupsCreatedCB(const LSFStringList& lampGroupIDs)
{
    NSMutableArray *groupIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = lampGroupIDs.begin(); iter != lampGroupIDs.end(); ++iter)
    {
        [groupIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate lampGroupsCreated: groupIDsArray];
    }
}

void LSFLampGroupManagerCallback::GetLampGroupReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID, const LampGroup& lampGroup)
{
    LSFStringList lamps = lampGroup.lamps;
    NSMutableArray *lampIDsArray = [[NSMutableArray alloc] init];

    for (std::list<LSFString>::const_iterator iter = lamps.begin(); iter != lamps.end(); ++iter)
    {
        [lampIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    LSFStringList groups = lampGroup.lampGroups;
    NSMutableArray *groupIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = groups.begin(); iter != groups.end(); ++iter)
    {
        [groupIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    LSFLampGroup *lsfLampGroup = [[LSFLampGroup alloc] init];
    [lsfLampGroup setLamps: lampIDsArray];
    [lsfLampGroup setLampGroups: groupIDsArray];
    
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate getLampGroupReplyWithCode: responseCode groupID: [NSString stringWithUTF8String: lampGroupID.c_str()] andLampGroup: lsfLampGroup];
    }
}

void LSFLampGroupManagerCallback::DeleteLampGroupReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate deleteLampGroupReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::LampGroupsDeletedCB(const LSFStringList& lampGroupIDs)
{
    NSMutableArray *groupIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = lampGroupIDs.begin(); iter != lampGroupIDs.end(); ++iter)
    {
        [groupIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate lampGroupsDeleted: groupIDsArray];
    }
}

void LSFLampGroupManagerCallback::TransitionLampGroupStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate transitionLampGroupToStateReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::PulseLampGroupWithStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate pulseLampGroupWithStateReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::PulseLampGroupWithPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate pulseLampGroupWithPresetReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::TransitionLampGroupStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate transitionLampGroupStateOnOffFieldReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::TransitionLampGroupStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate transitionLampGroupStateHueFieldReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::TransitionLampGroupStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate transitionLampGroupStateSaturationFieldReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::TransitionLampGroupStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate transitionLampGroupStateBrightnessFieldReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::TransitionLampGroupStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate transitionLampGroupStateColorTempFieldReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::ResetLampGroupStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate resetLampGroupStateReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::ResetLampGroupStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate resetLampGroupStateOnOffFieldReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::ResetLampGroupStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate resetLampGroupStateHueFieldReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::ResetLampGroupStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate resetLampGroupStateSaturationFieldReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::ResetLampGroupStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate resetLampGroupStateBrightnessFieldReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::ResetLampGroupStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate resetLampGroupStateColorTempFieldReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::UpdateLampGroupReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate updateLampGroupReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}

void LSFLampGroupManagerCallback::LampGroupsUpdatedCB(const LSFStringList& lampGroupIDs)
{
    NSMutableArray *groupIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = lampGroupIDs.begin(); iter != lampGroupIDs.end(); ++iter)
    {
        [groupIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate lampGroupsUpdated: groupIDsArray];
    }
}

void LSFLampGroupManagerCallback::TransitionLampGroupStateToPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID)
{
    if (_lgmDelegate != nil)
    {
        [_lgmDelegate transitionLampGroupStateToPresetReplyWithCode: responseCode andGroupID: [NSString stringWithUTF8String: lampGroupID.c_str()]];
    }
}