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

#import "LSFMasterSceneManager.h"
#import "LSFMasterSceneManagerCallback.h"

@interface LSFMasterSceneManager()

@property (nonatomic, readonly) lsf::MasterSceneManager *masterSceneManager;
@property (nonatomic, assign) LSFMasterSceneManagerCallback *masterSceneManagerCallback;

@end

@implementation LSFMasterSceneManager

@synthesize masterSceneManager = _masterSceneManager;
@synthesize masterSceneManagerCallback = _masterSceneManagerCallback;

-(id)initWithControllerClient: (LSFControllerClient *)controllerClient andMasterSceneManagerCallbackDelegate: (id<LSFMasterSceneManagerCallbackDelegate>)msmDelegate
{
    self = [super init];
    
    if (self)
    {
        self.masterSceneManagerCallback = new LSFMasterSceneManagerCallback(msmDelegate);
        self.handle = new lsf::MasterSceneManager(*(static_cast<lsf::ControllerClient*>(controllerClient.handle)), *(self.masterSceneManagerCallback));
    }
    
    return self;
}

-(ControllerClientStatus)getAllMasterSceneIDs
{
    return self.masterSceneManager->GetAllMasterSceneIDs();
}

-(ControllerClientStatus)getMasterSceneNameWithID: (NSString *)masterSceneID
{
    std::string msid([masterSceneID UTF8String]);
    return self.masterSceneManager->GetMasterSceneName(msid);
}

-(ControllerClientStatus)getMasterSceneNameWithID: (NSString *)masterSceneID andLanguage: (NSString *)language
{
    std::string msid([masterSceneID UTF8String]);
    std::string lang([language UTF8String]);
    return self.masterSceneManager->GetMasterSceneName(msid, lang);
}

-(ControllerClientStatus)setMasterSceneNameWithID: (NSString *)masterSceneID andMasterSceneName: (NSString *)masterSceneName
{
    std::string msid([masterSceneID UTF8String]);
    std::string name([masterSceneName UTF8String]);
    return self.masterSceneManager->SetMasterSceneName(msid, name);
}

-(ControllerClientStatus)setMasterSceneNameWithID: (NSString *)masterSceneID masterSceneName: (NSString *)masterSceneName andLanguage: (NSString *)language
{
    std::string msid([masterSceneID UTF8String]);
    std::string name([masterSceneName UTF8String]);
    std::string lang([language UTF8String]);
    return self.masterSceneManager->SetMasterSceneName(msid, name, lang);
}

-(ControllerClientStatus)createMasterScene: (LSFMasterScene *)masterScene withName: (NSString *)masterSceneName
{
    std::string name([masterSceneName UTF8String]);
    return self.masterSceneManager->CreateMasterScene(*(static_cast<lsf::MasterScene*>(masterScene.handle)), name);
}

-(ControllerClientStatus)createMasterScene: (LSFMasterScene *)masterScene withName: (NSString *)masterSceneName andLanguage: (NSString *)language
{
    std::string name([masterSceneName UTF8String]);
    std::string lang([language UTF8String]);
    return self.masterSceneManager->CreateMasterScene(*(static_cast<lsf::MasterScene*>(masterScene.handle)), name, lang);
}

-(ControllerClientStatus)updateMasterSceneWithID: (NSString *)masterSceneID andMasterScene: (LSFMasterScene *)masterScene
{
    std::string msid([masterSceneID UTF8String]);
    return self.masterSceneManager->UpdateMasterScene(msid, *(static_cast<lsf::MasterScene*>(masterScene.handle)));
}

-(ControllerClientStatus)getMasterSceneWithID: (NSString *)masterSceneID
{
    std::string msid([masterSceneID UTF8String]);
    return self.masterSceneManager->GetMasterScene(msid);
}

-(ControllerClientStatus)deleteMasterSceneWithID: (NSString *)masterSceneID
{
    std::string msid([masterSceneID UTF8String]);
    return self.masterSceneManager->DeleteMasterScene(msid);
}

-(ControllerClientStatus)applyMasterSceneWithID: (NSString *)masterSceneID
{
    std::string msid([masterSceneID UTF8String]);
    return self.masterSceneManager->ApplyMasterScene(msid);
}

-(ControllerClientStatus)getMasterSceneDataWithID: (NSString *)masterSceneID
{
    std::string msid([masterSceneID UTF8String]);
    return self.masterSceneManager->GetMasterSceneDataSet(msid);
}

-(ControllerClientStatus)getMasterSceneDataWithID: (NSString *)masterSceneID andLanguage: (NSString *)language
{
    std::string msid([masterSceneID UTF8String]);
    std::string lang([language UTF8String]);
    return self.masterSceneManager->GetMasterSceneDataSet(msid, lang);
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
-(lsf::MasterSceneManager *)masterSceneManager
{
    return static_cast<lsf::MasterSceneManager*>(self.handle);
}

@end
