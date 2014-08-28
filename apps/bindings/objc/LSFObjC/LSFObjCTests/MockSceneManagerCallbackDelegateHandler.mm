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

#import "MockSceneManagerCallbackDelegateHandler.h"

@interface MockSceneManagerCallbackDelegateHandler()

@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation MockSceneManagerCallbackDelegateHandler

@synthesize dataArray = _dataArray;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.dataArray = [[NSMutableArray alloc] init];
    }
    
    return self;
}

-(NSArray *)getCallbackData
{
    return self.dataArray;
}


//Delegate methods
-(void)getAllSceneIDsReplyWithCode: (LSFResponseCode)rc andSceneIDs: (NSArray *)sceneIDs
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getAllSceneIDs"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: sceneIDs];
}

-(void)getSceneNameReplyWithCode: (LSFResponseCode)rc sceneID: (NSString *)sceneID language: (NSString *)language andName: (NSString *)sceneName
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getSceneName"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: sceneID];
    [self.dataArray addObject: language];
    [self.dataArray addObject: sceneName];
}

-(void)setSceneNameReplyWithCode: (LSFResponseCode)rc sceneID: (NSString *)sceneID andLanguage: (NSString *)language
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"setSceneName"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: sceneID];
    [self.dataArray addObject: language];
}

-(void)scenesNameChanged: (NSArray *)sceneIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"scenesNameChanged"];
    [self.dataArray addObject: sceneIDs];
}

-(void)createSceneReplyWithCode: (LSFResponseCode)rc andSceneID: (NSString *)sceneID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"createScene"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: sceneID];
}

-(void)scenesCreated: (NSArray *)sceneIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"scenesCreated"];
    [self.dataArray addObject: sceneIDs];
}

-(void)updateSceneReplyWithCode: (LSFResponseCode)rc andSceneID: (NSString *)sceneID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"updateScene"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: sceneID];
}

-(void)scenesUpdated: (NSArray *)sceneIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"scenesUpdated"];
    [self.dataArray addObject: sceneIDs];
}

-(void)deleteSceneReplyWithCode: (LSFResponseCode)rc andSceneID: (NSString *)sceneID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"deleteScene"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: sceneID];
}

-(void)scenesDeleted: (NSArray *)sceneIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"scenesDeleted"];
    [self.dataArray addObject: sceneIDs];
}

-(void)getSceneReplyWithCode: (LSFResponseCode)rc sceneID: (NSString *)sceneID andScene: (LSFScene *)scene
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getScene"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: sceneID];
    
    NSArray *transitionToPresetComponent = scene.transitionToPresetComponent;
    LSFPresetTransitionEffect *pte = [transitionToPresetComponent objectAtIndex: 0];
    NSNumber *tp = [[NSNumber alloc] initWithInt: pte.transitionPeriod];
    
    [self.dataArray addObject: pte.lamps];
    [self.dataArray addObject: pte.lampGroups];
    [self.dataArray addObject: pte.presetID];
    [self.dataArray addObject: tp];
}

-(void)applySceneReplyWithCode: (LSFResponseCode)rc andSceneID: (NSString *)sceneID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"applyScene"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: sceneID];
}

-(void)scenesApplied: (NSArray *)sceneIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"scenesApplied"];
    [self.dataArray addObject: sceneIDs];
}

@end
