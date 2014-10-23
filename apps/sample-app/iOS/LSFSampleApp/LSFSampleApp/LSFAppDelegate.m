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

#import "LSFAppDelegate.h"
#import "LSFLampModelContainer.h"
#import "LSFGroupModelContainer.h"
#import "LSFPresetModelContainer.h"
#import "LSFMasterSceneModelContainer.h"
#import "LSFSceneModelContainer.h"
#import "LSFDispatchQueue.h"
#import "LSFConstants.h"
#import "LSFWifiMonitor.h"
#import "LSFTabManager.h"
#import "LSFAllJoynManager.h"

@implementation LSFAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [LSFAllJoynManager getAllJoynManager];
    [LSFLampModelContainer getLampModelContainer];
    [LSFGroupModelContainer getGroupModelContainer];
    [LSFPresetModelContainer getPresetModelContainer];
    [LSFMasterSceneModelContainer getMasterSceneModelContainer];
    [LSFSceneModelContainer getSceneModelContainer];
    [LSFDispatchQueue getDispatchQueue];
    [LSFConstants getConstants];
    [LSFWifiMonitor getWifiMonitor];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Called when the apps state will change to inactive. Use this method to pause the app.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // If your app supports background execution, use this method to release shared resources and save any data.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Undo the changes that were made when the app entered the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart tasks that were stopped when the app entered the background and refresh the UI.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Implement when app is about to terminate and save data, if needed.
}

@end
