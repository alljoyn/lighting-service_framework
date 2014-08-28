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

#import "LSFSampleControllerClientCallback.h"
#import "LSFAllJoynManager.h"
#import "LSFDispatchQueue.h"
#import "LSFControllerMaintenance.h"

@interface LSFSampleControllerClientCallback()

@property (nonatomic, strong) dispatch_queue_t queue;

@end

@implementation LSFSampleControllerClientCallback

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
 * Implementation of LSFControllerClientCallbackDelegate
 */
-(void)connectedToControllerServiceWithID: (NSString *)controllerServiceID andName: (NSString *)controllerServiceName
{
    NSLog(@"Connected to Controller Service with name: %@ and ID: %@", controllerServiceName, controllerServiceID);

    ([LSFAllJoynManager getAllJoynManager]).controllerName = controllerServiceName;

    ([LSFAllJoynManager getAllJoynManager]).isConnectedToController = YES;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.cscDelegate != nil)
        {
            [self.cscDelegate connectedToControllerService];
        }
    });
    
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
    
    dispatch_async(self.queue, ^{
        [ajManager.lsfLampGroupManager getAllLampGroupIDs];
        [ajManager.lsfPresetManager getAllPresetIDs];
        [ajManager.lsfSceneManager getAllSceneIDs];
        
        [ajManager.controllerMaintenance pollController];
    });
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 5 * NSEC_PER_SEC), ([LSFDispatchQueue getDispatchQueue]).queue, ^{
        [ajManager.controllerMaintenance garbageCollectLamps];
    });
}

-(void)connectToControllerServiceFailedForID: (NSString *)controllerServiceID andName: (NSString *)controllerServiceName
{
    NSLog(@"Connect to Controller Service with name: %@ and ID: %@ failed", controllerServiceName, controllerServiceID);
    ([LSFAllJoynManager getAllJoynManager]).isConnectedToController = NO;
}

-(void)disconnectedFromControllerServiceWithID: (NSString *)controllerServiceID andName: (NSString *)controllerServiceName
{
    NSLog(@"Disconnected from Controller Service with name: %@ and ID: %@", controllerServiceName, controllerServiceID);
    ([LSFAllJoynManager getAllJoynManager]).isConnectedToController = NO;
}

-(void)controllerClientError: (NSArray *)ec
{
    NSLog(@"Controller client experienced the following errors: ");
    
    for (NSNumber *error in ec)
    {
        NSLog(@"%i", [error intValue]);
    }
}

@end
