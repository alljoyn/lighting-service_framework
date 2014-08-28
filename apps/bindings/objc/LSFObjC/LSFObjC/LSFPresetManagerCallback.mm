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

#import "LSFPresetManagerCallback.h"

LSFPresetManagerCallback::LSFPresetManagerCallback(id<LSFPresetManagerCallbackDelegate> delegate) : _pmDelegate(delegate)
{
    //Empty Constructor
}

LSFPresetManagerCallback::~LSFPresetManagerCallback()
{
    _pmDelegate = nil;
}

void LSFPresetManagerCallback::GetPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID, const LampState& preset)
{
    LSFLampState *presetState = [[LSFLampState alloc] init];
    presetState.onOff = preset.onOff;
    presetState.brightness = preset.brightness;
    presetState.hue = preset.hue;
    presetState.saturation = preset.saturation;
    presetState.colorTemp = preset.colorTemp;
    
    if (_pmDelegate != nil)
    {
        [_pmDelegate getPresetReplyWithCode: responseCode presetID: [NSString stringWithUTF8String: presetID.c_str()] andPreset: presetState];
    }
}

void LSFPresetManagerCallback::GetAllPresetIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& presetIDs)
{
    NSMutableArray *presetIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = presetIDs.begin(); iter != presetIDs.end(); ++iter)
    {
        [presetIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_pmDelegate != nil)
    {
        [_pmDelegate getAllPresetIDsReplyWithCode: responseCode andPresetIDs: presetIDsArray];
    }
}

void LSFPresetManagerCallback::GetPresetNameReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID, const LSFString& language, const LSFString& presetName)
{
    if (_pmDelegate != nil)
    {
        [_pmDelegate getPresetNameReplyWithCode: responseCode presetID: [NSString stringWithUTF8String: presetID.c_str()] language: [NSString stringWithUTF8String: language.c_str()] andPresetName: [NSString stringWithUTF8String: presetName.c_str()]];
    }
}

void LSFPresetManagerCallback::SetPresetNameReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID, const LSFString& language)
{
    if (_pmDelegate != nil)
    {
        [_pmDelegate setPresetNameReplyWithCode: responseCode presetID: [NSString stringWithUTF8String: presetID.c_str()] andLanguage: [NSString stringWithUTF8String: language.c_str()]];
    }
}

void LSFPresetManagerCallback::PresetsNameChangedCB(const LSFStringList& presetIDs)
{
    NSMutableArray *presetIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = presetIDs.begin(); iter != presetIDs.end(); ++iter)
    {
        [presetIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_pmDelegate != nil)
    {
        [_pmDelegate presetsNameChanged: presetIDsArray];
    }
}

void LSFPresetManagerCallback::CreatePresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID)
{
    if (_pmDelegate != nil)
    {
        [_pmDelegate createPresetReplyWithCode: responseCode andPresetID: [NSString stringWithUTF8String: presetID.c_str()]];
    }
}

void LSFPresetManagerCallback::PresetsCreatedCB(const LSFStringList& presetIDs)
{
    NSMutableArray *presetIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = presetIDs.begin(); iter != presetIDs.end(); ++iter)
    {
        [presetIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_pmDelegate != nil)
    {
        [_pmDelegate presetsCreated: presetIDsArray];
    }
}

void LSFPresetManagerCallback::UpdatePresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID)
{
    if (_pmDelegate != nil)
    {
        [_pmDelegate updatePresetReplyWithCode: responseCode andPresetID: [NSString stringWithUTF8String: presetID.c_str()]];
    }
}

void LSFPresetManagerCallback::PresetsUpdatedCB(const LSFStringList& presetIDs)
{
    NSMutableArray *presetIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = presetIDs.begin(); iter != presetIDs.end(); ++iter)
    {
        [presetIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_pmDelegate != nil)
    {
        [_pmDelegate presetsUpdated: presetIDsArray];
    }
}

void LSFPresetManagerCallback::DeletePresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID)
{
    if (_pmDelegate != nil)
    {
        [_pmDelegate deletePresetReplyWithCode: responseCode andPresetID: [NSString stringWithUTF8String: presetID.c_str()]];
    }
}

void LSFPresetManagerCallback::PresetsDeletedCB(const LSFStringList& presetIDs)
{
    NSMutableArray *presetIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = presetIDs.begin(); iter != presetIDs.end(); ++iter)
    {
        [presetIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    if (_pmDelegate != nil)
    {
        [_pmDelegate presetsDeleted: presetIDsArray];
    }
}

void LSFPresetManagerCallback::GetDefaultLampStateReplyCB(const LSFResponseCode& responseCode, const LampState& defaultLampState)
{
    LSFLampState *defaultState = [[LSFLampState alloc] init];
    defaultState.onOff = defaultLampState.onOff;
    defaultState.brightness = defaultLampState.brightness;
    defaultState.hue = defaultLampState.hue;
    defaultState.saturation = defaultLampState.saturation;
    defaultState.colorTemp = defaultLampState.colorTemp;
    
    if (_pmDelegate != nil)
    {
        [_pmDelegate getDefaultLampStateReplyWithCode: responseCode andLampState: defaultState];
    }
}

void LSFPresetManagerCallback::SetDefaultLampStateReplyCB(const LSFResponseCode& responseCode)
{
    if (_pmDelegate != nil)
    {
        [_pmDelegate setDefaultLampStateReplyWithCode: responseCode];
    }
}

void LSFPresetManagerCallback::DefaultLampStateChangedCB(void)
{
    if (_pmDelegate != nil)
    {
        [_pmDelegate defaultLampStateChanged];
    }
}