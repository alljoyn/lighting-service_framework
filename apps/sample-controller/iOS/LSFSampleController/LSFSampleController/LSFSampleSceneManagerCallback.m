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

#import "LSFSampleSceneManagerCallback.h"
#import "LSFSceneModelContainer.h"
#import "LSFSceneDataModel.h"
#import "LSFAllJoynManager.h"
#import "LSFDispatchQueue.h"

@interface LSFSampleSceneManagerCallback()

@property (nonatomic, strong) dispatch_queue_t queue;

-(void)postProcessSceneID: (NSString *)sceneID;
-(void)postUpdateSceneID: (NSString *)sceneID;
-(void)postUpdateScene: (NSString *)sceneID withName: (NSString *)sceneName;
-(void)postDeleteScenes: (NSArray *)sceneIDs;
-(void)postUpdateScene:(NSString *)sceneID withScene:(LSFScene *)scene;
-(void)updateUI;

@end

@implementation LSFSampleSceneManagerCallback

@synthesize reloadUIDelegate = _reloadUIDelegate;
@synthesize cscDelegate = _cscDelegate;
@synthesize queue = _queue;

-(id)init
{
    self = [super init];

    if (self)
    {
        self.queue = ([LSFDispatchQueue getDispatchQueue]).queue;
    }

    return self;
}

/*
 * Implementation of LSFSceneManagerCallbackDelegate
 */
-(void)getAllSceneIDsReplyWithCode: (LSFResponseCode)rc andSceneIDs: (NSArray *)sceneIDs
{
    NSLog(@"LSFSampleSceneManagerCallback - getAllSceneIDsReplyCB() executing. Response Code = %i", rc);

    if (rc != LSF_OK)
    {
        //TODO - do something
    }
    dispatch_async(self.queue, ^{
        for (NSString *sceneID in sceneIDs)
        {
            [self postProcessSceneID: sceneID];
        }
    });
}

-(void)getSceneNameReplyWithCode: (LSFResponseCode)rc sceneID: (NSString *)sceneID language: (NSString *)language andName: (NSString *)sceneName
{
    NSLog(@"LSFSampleSceneManagerCallback - getSceneNameReplyCB() executing. Response Code = %i", rc);

    if (rc != LSF_OK)
    {
        //TODO - do something
    }

    dispatch_async(self.queue, ^{
        [self postUpdateScene: sceneID withName: sceneName];
    });
}

-(void)setSceneNameReplyWithCode: (LSFResponseCode)rc sceneID: (NSString *)sceneID andLanguage: (NSString *)language
{
    NSLog(@"LSFSampleSceneManagerCallback - setSceneNameReplyCB() executing");

    if (rc != LSF_OK)
    {
        //TODO - do something
    }

    dispatch_async(self.queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfSceneManager getSceneNameWithID: sceneID];
    });
}

-(void)scenesNameChanged: (NSArray *)sceneIDs
{
    NSLog(@"LSFSampleSceneManagerCallback - scenesNameChangedReplyCB() executing");

    dispatch_async(self.queue, ^{
        BOOL containsNewSceneIDs = NO;
        LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

        for (NSString *sceneID in sceneIDs)
        {
            LSFSceneDataModel *model = [container.sceneContainer objectForKey: sceneID];

            if (model != nil)
            {
                [ajManager.lsfSceneManager getSceneNameWithID: sceneID];
            }
            else
            {
                containsNewSceneIDs = YES;
            }
        }

        if (containsNewSceneIDs)
        {
            [ajManager.lsfSceneManager getAllSceneIDs];
        }
    });
}

-(void)createSceneReplyWithCode: (LSFResponseCode)rc andSceneID: (NSString *)sceneID
{
    NSLog(@"LSFSampleSceneManagerCallback - createSceneReplyCB() executing");

    if (rc != LSF_OK)
    {
        //TODO - do something
    }

    dispatch_async(self.queue, ^{
        [self postProcessSceneID: sceneID];
    });
}

-(void)scenesCreated: (NSArray *)sceneIDs
{
    NSLog(@"LSFSampleSceneManagerCallback - scenesCreatedReplyCB() executing");

    dispatch_async(self.queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfSceneManager getAllSceneIDs];
    });
}

-(void)updateSceneReplyWithCode: (LSFResponseCode)rc andSceneID: (NSString *)sceneID
{
    NSLog(@"LSFSampleSceneManagerCallback - updateSceneReplyCB() executing");

    if (rc != LSF_OK)
    {
        //TODO - do something
    }
}

-(void)scenesUpdated: (NSArray *)sceneIDs
{
    NSLog(@"LSFSampleSceneManagerCallback - scenesUpdatedReplyCB() executing");

    dispatch_async(self.queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

        for (NSString *sceneID in sceneIDs)
        {
            [ajManager.lsfSceneManager getSceneWithID: sceneID];
        }
    });
}

-(void)deleteSceneReplyWithCode: (LSFResponseCode)rc andSceneID: (NSString *)sceneID
{
    NSLog(@"LSFSampleSceneManagerCallback - deleteSceneReplyCB() executing");

    if (rc != LSF_OK)
    {
        //TODO - do something
    }
}

-(void)scenesDeleted: (NSArray *)sceneIDs
{
    NSLog(@"LSFSampleSceneManagerCallback - scenesDeletedReplyCB() executing");

    dispatch_async(self.queue, ^{
        [self postDeleteScenes: sceneIDs];
    });
}

-(void)getSceneReplyWithCode: (LSFResponseCode)rc sceneID: (NSString *)sceneID andScene: (LSFScene *)scene
{
    NSLog(@"LSFSampleSceneManagerCallback - getSceneReplyCB() executing");

    if (rc != LSF_OK)
    {
        //TODO - do something
    }

    dispatch_async(self.queue, ^{
        [self postUpdateScene: sceneID withScene: scene];
    });
}

-(void)applySceneReplyWithCode: (LSFResponseCode)rc andSceneID: (NSString *)sceneID
{
    NSLog(@"LSFSampleSceneManagerCallback - applySceneReplyCB() executing");

    if (rc != LSF_OK)
    {
        //TODO - do something
    }
}

-(void)scenesApplied: (NSArray *)sceneIDs
{
    NSLog(@"LSFSampleSceneManagerCallback - scenesAppliedReplyCB() executing");
}

/*
 * Private Functions
 */
-(void)postProcessSceneID: (NSString *)sceneID
{
    NSLog(@"postProcessSceneID(): %@", sceneID);

    LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
    LSFSceneDataModel *sceneModel = [container.sceneContainer valueForKey: sceneID];

    if (sceneModel == nil)
    {
        NSLog(@"Scene ID not found creating scene model");
        [self postUpdateSceneID: sceneID];

        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfSceneManager getSceneNameWithID: sceneID];
        [ajManager.lsfSceneManager getSceneWithID: sceneID];
    }
}

-(void)postUpdateSceneID: (NSString *)sceneID
{
    NSLog(@"postUpdateSceneID(): %@", sceneID);

    LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
    LSFSceneDataModel *sceneModel = [container.sceneContainer valueForKey: sceneID];

    if (sceneModel == nil)
    {
        NSLog(@"SceneModel not found. Creating scene model for %@", sceneID);

        sceneModel = [[LSFSceneDataModel alloc] initWithSceneID: sceneID];
        [container.sceneContainer setValue: sceneModel forKey: sceneID];
    }

    [self updateUI];
}

-(void)postUpdateScene: (NSString *)sceneID withName: (NSString *)sceneName
{
    NSLog(@"postUpdateSceneName(): %@ and name: %@", sceneID, sceneName);

    LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
    LSFSceneDataModel *sceneModel = [container.sceneContainer valueForKey: sceneID];

    if (sceneModel != nil)
    {
        sceneModel.name = sceneName;
    }

    [self updateUI];
}

-(void)postDeleteScenes: (NSArray *)sceneIDs
{
    NSLog(@"postDeleteScenes()");

    LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
    NSMutableDictionary *scenes = container.sceneContainer;

    for (NSString *sceneID in sceneIDs)
    {
        [scenes removeObjectForKey: sceneID];
    }

    [self updateUI];
}

-(void)postUpdateScene:(NSString *)sceneID withScene:(LSFScene *)scene
{
    NSLog(@"postUpdateScene(): %@", sceneID);

    LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
    LSFSceneDataModel *sceneModel = [container.sceneContainer valueForKey: sceneID];

    if (sceneModel != nil)
    {
        [sceneModel fromScene: scene];
    }

    [self updateUI];
}

-(void)updateUI
{
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.reloadUIDelegate != nil)
        {
            [self.reloadUIDelegate reloadUI];
        }
    });
}

@end
