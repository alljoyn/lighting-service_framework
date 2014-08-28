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

#import "LSFSceneManager.h"
#import "LSFSceneManagerCallback.h"

@interface LSFSceneManager()

@property (nonatomic, readonly) lsf::SceneManager *sceneManager;
@property (nonatomic, assign) LSFSceneManagerCallback *sceneManagerCallback;

@end

@implementation LSFSceneManager

@synthesize sceneManager = _sceneManager;
@synthesize sceneManagerCallback = _sceneManagerCallback;

-(id)initWithControllerClient: (LSFControllerClient *)controllerClient andSceneManagerCallbackDelegate: (id<LSFSceneManagerCallbackDelegate>)smDelegate
{
    self = [super init];
    
    if (self)
    {
        self.sceneManagerCallback = new LSFSceneManagerCallback(smDelegate);
        self.handle = new lsf::SceneManager(*(static_cast<lsf::ControllerClient*>(controllerClient.handle)), *(self.sceneManagerCallback));
    }
    
    return self;
}

-(ControllerClientStatus)getAllSceneIDs
{
    return self.sceneManager->GetAllSceneIDs();
}

-(ControllerClientStatus)getSceneNameWithID: (NSString *)sceneID
{
    std::string sid([sceneID UTF8String]);
    return self.sceneManager->GetSceneName(sid);
}

-(ControllerClientStatus)getSceneNameWithID: (NSString *)sceneID andLanguage: (NSString *)language
{
    std::string sid([sceneID UTF8String]);
    std::string lang([language UTF8String]);
    return self.sceneManager->GetSceneName(sid, lang);
}

-(ControllerClientStatus)setSceneNameWithID: (NSString *)sceneID andSceneName: (NSString *)sceneName
{
    std::string sid([sceneID UTF8String]);
    std::string name([sceneName UTF8String]);
    return self.sceneManager->SetSceneName(sid, name);
}

-(ControllerClientStatus)setSceneNameWithID: (NSString *)sceneID sceneName: (NSString *)sceneName andLanguage: (NSString *)language
{
    std::string sid([sceneID UTF8String]);
    std::string name([sceneName UTF8String]);
    std::string lang([language UTF8String]);
    return self.sceneManager->SetSceneName(sid, name, lang);
}

-(ControllerClientStatus)createScene: (LSFScene *)scene andSceneName: (NSString *)sceneName
{
    std::string name([sceneName UTF8String]);
    return self.sceneManager->CreateScene(*(static_cast<lsf::Scene*>(scene.handle)), name);
}

-(ControllerClientStatus)createScene: (LSFScene *)scene sceneName: (NSString *)sceneName andLanguage: (NSString *)language
{
    std::string name([sceneName UTF8String]);
    std::string lang([language UTF8String]);
    return self.sceneManager->CreateScene(*(static_cast<lsf::Scene*>(scene.handle)), name, lang);
}

-(ControllerClientStatus)updateSceneWithID: (NSString *)sceneID withScene: (LSFScene *)scene
{
    std::string sid([sceneID UTF8String]);
    return self.sceneManager->UpdateScene(sid, *(static_cast<lsf::Scene*>(scene.handle)));
}

-(ControllerClientStatus)deleteSceneWithID: (NSString *)sceneID
{
    std::string sid([sceneID UTF8String]);
    return self.sceneManager->DeleteScene(sid);
}

-(ControllerClientStatus)getSceneWithID: (NSString *)sceneID
{
    std::string sid([sceneID UTF8String]);
    return self.sceneManager->GetScene(sid);
}

-(ControllerClientStatus)applySceneWithID: (NSString *)sceneID
{
    std::string sid([sceneID UTF8String]);
    return self.sceneManager->ApplyScene(sid);
}

-(ControllerClientStatus)getSceneDataWithID: (NSString *)sceneID
{
    std::string sid([sceneID UTF8String]);
    return self.sceneManager->GetSceneDataSet(sid);
}

-(ControllerClientStatus)getSceneDataWithID: (NSString *)sceneID andLanguage: (NSString *)language
{
    std::string sid([sceneID UTF8String]);
    std::string lang([language UTF8String]);
    return self.sceneManager->GetSceneDataSet(sid, lang);
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
-(lsf::SceneManager *)sceneManager
{
    return static_cast<lsf::SceneManager*>(self.handle);
}

@end
