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

#import "LSFTabManager.h"
#import "LSFAppDelegate.h"
#import "LSFLampModelContainer.h"
#import "LSFGroupModelContainer.h"
#import "LSFSceneModelContainer.h"
#import "LSFMasterSceneModelContainer.h"

@interface LSFTabManager()

@property (nonatomic, strong) UITabBarController *tabBarController;

@end

@implementation LSFTabManager

@synthesize tabBarController = _tabBarController;

+(id)getTabManager
{
    static LSFTabManager *tabManager = nil;
    static dispatch_once_t onceToken;

    dispatch_once(&onceToken, ^{
        tabManager = [[self alloc] init];
    });

    return tabManager;
}

-(id)init
{
    self = [super init];

    if (self)
    {
        LSFAppDelegate *appDelegate = (LSFAppDelegate *)[[UIApplication sharedApplication] delegate];
        self.tabBarController = (UITabBarController *)appDelegate.window.rootViewController;

        [self updateLampsTab];
        [self updateGroupsTab];
        [self updateScenesTab];
    }

    return self;
}

-(void)updateLampsTab
{
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = container.lampContainer;

    dispatch_async(dispatch_get_main_queue(), ^{
        [[self.tabBarController.tabBar.items objectAtIndex: 0] setTitle: [NSString stringWithFormat: @"Lamps (%i)", lamps.count]];
    });
}

-(void)updateGroupsTab
{
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = container.groupContainer;

    dispatch_async(dispatch_get_main_queue(), ^{
        [[self.tabBarController.tabBar.items objectAtIndex: 1] setTitle: [NSString stringWithFormat: @"Groups (%i)", groups.count]];
    });
}

-(void)updateScenesTab
{
    LSFSceneModelContainer *sceneContainer = [LSFSceneModelContainer getSceneModelContainer];
    LSFMasterSceneModelContainer *masterScenesContainer = [LSFMasterSceneModelContainer getMasterSceneModelContainer];
    NSMutableDictionary *scenes = sceneContainer.sceneContainer;
    NSMutableDictionary *masterScenes = masterScenesContainer.masterScenesContainer;

    dispatch_async(dispatch_get_main_queue(), ^{
        [[self.tabBarController.tabBar.items objectAtIndex: 2] setTitle: [NSString stringWithFormat: @"Scenes (%i)", (scenes.count + masterScenes.count)]];
    });
}

@end