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

#import "LSFPresetManager.h"
#import "LSFPresetManagerCallback.h"

@interface LSFPresetManager()

@property (nonatomic, readonly) lsf::PresetManager *presetManager;
@property (nonatomic, assign) LSFPresetManagerCallback *presetManagerCallback;

@end

@implementation LSFPresetManager

@synthesize presetManager =_presetManager;
@synthesize presetManagerCallback = _presetManagerCallback;

-(id)initWithControllerClient: (LSFControllerClient *)controllerClient andPresetManagerCallbackDelegate: (id<LSFPresetManagerCallbackDelegate>)pmDelegate
{
    self = [super init];
    
    if (self)
    {
        self.presetManagerCallback = new LSFPresetManagerCallback(pmDelegate);
        self.handle = new lsf::PresetManager(*(static_cast<lsf::ControllerClient*>(controllerClient.handle)), *(self.presetManagerCallback));
    }
    
    return self;
}

-(ControllerClientStatus)getAllPresetIDs
{
    return self.presetManager->GetAllPresetIDs();
}

-(ControllerClientStatus)getPresetWithID: (NSString *)presetID
{
    std::string pid([presetID UTF8String]);
    return self.presetManager->GetPreset(pid);
}

-(ControllerClientStatus)getPresetNameWithID: (NSString *)presetID
{
    std::string pid([presetID UTF8String]);
    return self.presetManager->GetPresetName(pid);
}

-(ControllerClientStatus)getPresetNameWithID: (NSString *)presetID andLanguage: (NSString *)language
{
    std::string pid([presetID UTF8String]);
    std::string lang([language UTF8String]);
    return self.presetManager->GetPresetName(pid, lang);
}

-(ControllerClientStatus)setPresetNameWithID: (NSString *)presetID andPresetName: (NSString *)name
{
    std::string pid([presetID UTF8String]);
    std::string presetName([name UTF8String]);
    return self.presetManager->SetPresetName(pid, presetName);
}

-(ControllerClientStatus)setPresetNameWithID: (NSString *)presetID presetName: (NSString *)name andLanguage: (NSString *)language
{
    std::string pid([presetID UTF8String]);
    std::string presetName([name UTF8String]);
    std::string lang([language UTF8String]);
    return self.presetManager->SetPresetName(pid, presetName, lang);
}

-(ControllerClientStatus)createPresetWithState: (LSFLampState *)preset andPresetName: (NSString *)name
{
    std::string presetName([name UTF8String]);
    return self.presetManager->CreatePreset(*static_cast<lsf::LampState*>(preset.handle), presetName);
}

-(ControllerClientStatus)createPresetWithState: (LSFLampState *)preset presetName: (NSString *)name andLanguage: (NSString *)language
{
    std::string presetName([name UTF8String]);
    std::string lang([language UTF8String]);
    return self.presetManager->CreatePreset(*static_cast<lsf::LampState*>(preset.handle), presetName, lang);
}

-(ControllerClientStatus)updatePresetWithID: (NSString *)presetID andState: (LSFLampState *)preset
{
    std::string pid([presetID UTF8String]);
    return self.presetManager->UpdatePreset(pid, *static_cast<lsf::LampState*>(preset.handle));
}

-(ControllerClientStatus)deletePresetWithID: (NSString *)presetID
{
    std::string pid([presetID UTF8String]);
    return self.presetManager->DeletePreset(pid);
}

-(ControllerClientStatus)getDefaultLampState
{
    return self.presetManager->GetDefaultLampState();
}

-(ControllerClientStatus)setDefaultLampState: (LSFLampState *)defaultLampState
{
    return self.presetManager->SetDefaultLampState(*static_cast<lsf::LampState*>(defaultLampState.handle));
}

-(ControllerClientStatus)getPresetDataSetWithID: (NSString *)presetID
{
    std::string pid([presetID UTF8String]);
    return self.presetManager->GetPresetDataSet(pid);
}

-(ControllerClientStatus)getPresetDataSetWithID: (NSString *)presetID andLanguage: (NSString *)language
{
    std::string pid([presetID UTF8String]);
    std::string lang([language UTF8String]);
    return self.presetManager->GetPresetDataSet(pid, lang);
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
-(lsf::PresetManager *)presetManager
{
    return static_cast<lsf::PresetManager*>(self.handle);
}

@end
