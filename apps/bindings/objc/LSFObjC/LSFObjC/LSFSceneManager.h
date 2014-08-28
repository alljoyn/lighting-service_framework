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

#import <Foundation/Foundation.h>
#import "LSFObject.h"
#import "LSFControllerClient.h"
#import "LSFSceneManagerCallbackDelegate.h"
#import "LSFScene.h"
#import "SceneManager.h"

@interface LSFSceneManager : LSFObject

-(id)initWithControllerClient: (LSFControllerClient *)controllerClient andSceneManagerCallbackDelegate: (id<LSFSceneManagerCallbackDelegate>)smDelegate;
-(ControllerClientStatus)getAllSceneIDs;
-(ControllerClientStatus)getSceneNameWithID: (NSString *)sceneID;
-(ControllerClientStatus)getSceneNameWithID: (NSString *)sceneID andLanguage: (NSString *)language;
-(ControllerClientStatus)setSceneNameWithID: (NSString *)sceneID andSceneName: (NSString *)sceneName;
-(ControllerClientStatus)setSceneNameWithID: (NSString *)sceneID sceneName: (NSString *)sceneName andLanguage: (NSString *)language;
-(ControllerClientStatus)createScene: (LSFScene *)scene andSceneName: (NSString *)sceneName;
-(ControllerClientStatus)createScene: (LSFScene *)scene sceneName: (NSString *)sceneName andLanguage: (NSString *)language;
-(ControllerClientStatus)updateSceneWithID: (NSString *)sceneID withScene: (LSFScene *)scene;
-(ControllerClientStatus)deleteSceneWithID: (NSString *)sceneID;
-(ControllerClientStatus)getSceneWithID: (NSString *)sceneID;
-(ControllerClientStatus)applySceneWithID: (NSString *)sceneID;
-(ControllerClientStatus)getSceneDataWithID: (NSString *)sceneID;
-(ControllerClientStatus)getSceneDataWithID: (NSString *)sceneID andLanguage: (NSString *)language;

@end