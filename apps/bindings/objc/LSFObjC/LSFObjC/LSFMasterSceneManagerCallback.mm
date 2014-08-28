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

#import "LSFMasterSceneManagerCallback.h"
#import "LSFUtils.h"

LSFMasterSceneManagerCallback::LSFMasterSceneManagerCallback(id<LSFMasterSceneManagerCallbackDelegate> delegate) : _msmDelegate(delegate)
{
    //Empty Constructor
}

LSFMasterSceneManagerCallback::~LSFMasterSceneManagerCallback()
{
    _msmDelegate = nil;
}

void LSFMasterSceneManagerCallback::GetAllMasterSceneIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& masterSceneList)
{
    NSMutableArray *masterSceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = masterSceneList.begin(); iter != masterSceneList.end(); ++iter)
    {
        [masterSceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_msmDelegate != nil)
    {
        [_msmDelegate getAllMasterSceneIDsReplyWithCode: responseCode andMasterSceneIDs: masterSceneIDsArray];
    }
}

void LSFMasterSceneManagerCallback::GetMasterSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID, const LSFString& language, const LSFString& masterSceneName)
{
    if (_msmDelegate != nil)
    {
        [_msmDelegate getMasterSceneNameReplyWithCode: responseCode masterSceneID: [NSString stringWithUTF8String: masterSceneID.c_str()] language: [NSString stringWithUTF8String: language.c_str()] andName: [NSString stringWithUTF8String: masterSceneName.c_str()]];
    }
}

void LSFMasterSceneManagerCallback::SetMasterSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID, const LSFString& language)
{
    if (_msmDelegate != nil)
    {
        [_msmDelegate setMasterSceneNameReplyWithCode: responseCode masterSceneID: [NSString stringWithUTF8String: masterSceneID.c_str()] andLanguage: [NSString stringWithUTF8String: language.c_str()]];
    }
}

void LSFMasterSceneManagerCallback::MasterScenesNameChangedCB(const LSFStringList& masterSceneIDs)
{
    NSMutableArray *masterSceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = masterSceneIDs.begin(); iter != masterSceneIDs.end(); ++iter)
    {
        [masterSceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_msmDelegate != nil)
    {
        [_msmDelegate masterScenesNameChanged: masterSceneIDsArray];
    }
}

void LSFMasterSceneManagerCallback::CreateMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID)
{
    if (_msmDelegate != nil)
    {
        [_msmDelegate createMasterSceneReplyWithCode: responseCode andMasterSceneID: [NSString stringWithUTF8String: masterSceneID.c_str()]];
    }
}

void LSFMasterSceneManagerCallback::MasterScenesCreatedCB(const LSFStringList& masterSceneIDs)
{
    NSMutableArray *masterSceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = masterSceneIDs.begin(); iter != masterSceneIDs.end(); ++iter)
    {
        [masterSceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_msmDelegate != nil)
    {
        [_msmDelegate masterScenesCreated: masterSceneIDsArray];
    }
}

void LSFMasterSceneManagerCallback::GetMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID, const MasterScene& masterScene)
{
    LSFMasterScene *ms = [[LSFMasterScene alloc] initWithSceneIDs: [LSFUtils convertStringListToNSArray: masterScene.scenes]];
    
    if (_msmDelegate != nil)
    {
        [_msmDelegate getMasterSceneReplyWithCode: responseCode masterSceneID: [NSString stringWithUTF8String: masterSceneID.c_str()] andMasterScene: ms];
    }
}

void LSFMasterSceneManagerCallback::DeleteMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID)
{
    if (_msmDelegate != nil)
    {
        [_msmDelegate deleteMasterSceneReplyWithCode: responseCode andMasterSceneID: [NSString stringWithUTF8String: masterSceneID.c_str()]];
    }
}

void LSFMasterSceneManagerCallback::MasterScenesDeletedCB(const LSFStringList& masterSceneIDs)
{
    NSMutableArray *masterSceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = masterSceneIDs.begin(); iter != masterSceneIDs.end(); ++iter)
    {
        [masterSceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_msmDelegate != nil)
    {
        [_msmDelegate masterScenesDeleted: masterSceneIDsArray];
    }
}

void LSFMasterSceneManagerCallback::UpdateMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID)
{
    if (_msmDelegate != nil)
    {
        [_msmDelegate updateMasterSceneReplyWithCode: responseCode andMasterSceneID: [NSString stringWithUTF8String: masterSceneID.c_str()]];
    }
}

void LSFMasterSceneManagerCallback::MasterScenesUpdatedCB(const LSFStringList& masterSceneIDs)
{
    NSMutableArray *masterSceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = masterSceneIDs.begin(); iter != masterSceneIDs.end(); ++iter)
    {
        [masterSceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_msmDelegate != nil)
    {
        [_msmDelegate masterScenesUpdated: masterSceneIDsArray];
    }
}

void LSFMasterSceneManagerCallback::ApplyMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID)
{
    if (_msmDelegate != nil)
    {
        [_msmDelegate applyMasterSceneReplyWithCode: responseCode andMasterSceneID: [NSString stringWithUTF8String: masterSceneID.c_str()]];
    }
}

void LSFMasterSceneManagerCallback::MasterScenesAppliedCB(const LSFStringList& masterSceneIDs)
{
    NSMutableArray *masterSceneIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = masterSceneIDs.begin(); iter != masterSceneIDs.end(); ++iter)
    {
        [masterSceneIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_msmDelegate != nil)
    {
        [_msmDelegate masterScenesApplied: masterSceneIDsArray];
    }
}