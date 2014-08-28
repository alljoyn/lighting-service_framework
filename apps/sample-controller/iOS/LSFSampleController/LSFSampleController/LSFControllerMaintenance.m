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

#import "LSFControllerMaintenance.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"

@implementation LSFControllerMaintenance

@synthesize reloadUIDelegate = _reloadUIDelegate;
@synthesize cscDelegate = _cscDelegate;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        //Empty constructor
    }
    
    return self;
}

-(void)pollController
{
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampManager getAllLampIDs];
        [ajManager.lsfLampGroupManager getAllLampGroupIDs]; //TODO - change how groups are updated
    });
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 5 * NSEC_PER_SEC), ([LSFDispatchQueue getDispatchQueue]).queue, ^{
        [self pollController];
    });
}

-(void)garbageCollectLamps
{
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
        NSArray *values = [container.lampContainer allValues];
        
        for (LSFLampModel *model in values)
        {
            long long currentTimestamp = (long long)([[NSDate date] timeIntervalSince1970] * 1000);
            if ((model.timestamp + 10000) < currentTimestamp)
            {
                NSLog(@"Lamps count is %i", [container.lampContainer count]);
                NSLog(@"Pruned lamp %@", model.theID);
                [container.lampContainer removeObjectForKey: model.theID];
                NSLog(@"Lamps count is now %i", [container.lampContainer count]);
                
                dispatch_async(dispatch_get_main_queue(), ^{
                    if (([container.lampContainer count] == 0) && self.reloadUIDelegate != nil)
                    {
                        [self.reloadUIDelegate reloadUI];
                    }
                });
            }
            else
            {
                NSLog(@"10 second timeout interval not exceeded");
            }
        }
    });
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 10 * NSEC_PER_SEC), ([LSFDispatchQueue getDispatchQueue]).queue, ^{
        [self garbageCollectLamps];
    });
}


@end
