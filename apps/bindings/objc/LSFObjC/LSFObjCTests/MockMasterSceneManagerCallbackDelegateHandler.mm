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

#import "MockMasterSceneManagerCallbackDelegateHandler.h"

@interface MockMasterSceneManagerCallbackDelegateHandler()

@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation MockMasterSceneManagerCallbackDelegateHandler

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
-(void)getAllMasterSceneIDsReplyWithCode: (LSFResponseCode)rc andMasterSceneIDs: (NSArray *)masterSceneIDs
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getAllMasterSceneIDs"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: masterSceneIDs];
}

-(void)getMasterSceneNameReplyWithCode: (LSFResponseCode)rc masterSceneID: (NSString *)masterSceneID language: (NSString *)language andName: (NSString *)masterSceneName
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getMasterSceneName"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: masterSceneID];
    [self.dataArray addObject: language];
    [self.dataArray addObject: masterSceneName];
}

-(void)setMasterSceneNameReplyWithCode: (LSFResponseCode)rc masterSceneID: (NSString *)masterSceneID andLanguage: (NSString *)language
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"setMasterSceneName"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: masterSceneID];
    [self.dataArray addObject: language];
}

-(void)masterScenesNameChanged: (NSArray *)masterSceneIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"masterScenesNameChanged"];
    [self.dataArray addObject: masterSceneIDs];
}

-(void)createMasterSceneReplyWithCode: (LSFResponseCode)rc andMasterSceneID: (NSString *)masterSceneID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"createMasterScene"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: masterSceneID];
}

-(void)masterScenesCreated: (NSArray *)masterSceneIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"masterScenesCreated"];
    [self.dataArray addObject: masterSceneIDs];
}

-(void)getMasterSceneReplyWithCode: (LSFResponseCode)rc masterSceneID: (NSString *)masterSceneID andMasterScene: (LSFMasterScene *)masterScene
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getMasterScene"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: masterSceneID];
    [self.dataArray addObject: masterScene.sceneIDs];
}

-(void)deleteMasterSceneReplyWithCode: (LSFResponseCode)rc andMasterSceneID: (NSString *)masterSceneID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"deleteMasterScene"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: masterSceneID];
}

-(void)masterScenesDeleted: (NSArray *)masterSceneIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"masterScenesDeleted"];
    [self.dataArray addObject: masterSceneIDs];
}

-(void)updateMasterSceneReplyWithCode: (LSFResponseCode)rc andMasterSceneID: (NSString *)masterSceneID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"updateMasterScene"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: masterSceneID];
}

-(void)masterScenesUpdated: (NSArray *)masterSceneIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"masterScenesUpdated"];
    [self.dataArray addObject: masterSceneIDs];
}

-(void)applyMasterSceneReplyWithCode: (LSFResponseCode)rc andMasterSceneID: (NSString *)masterSceneID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"applyMasterScene"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: masterSceneID];
}

-(void)masterScenesApplied: (NSArray *)masterSceneIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"masterScenesApplied"];
    [self.dataArray addObject: masterSceneIDs];
}

@end
