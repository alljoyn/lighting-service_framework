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

#import "LSFLampMaintenance.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"

@interface LSFLampMaintenance()

@property (nonatomic, strong) NSMutableArray *lampIDs;

-(void)pollLamps;

@end

@implementation LSFLampMaintenance

@synthesize lampIDs = _lampIDs;

+(LSFLampMaintenance *)getLampMaintenance
{
    static LSFLampMaintenance *lampMaintenance = nil;
    static dispatch_once_t onceToken;

    dispatch_once(&onceToken, ^{
        lampMaintenance = [[self alloc] init];
    });

    return lampMaintenance;
}

-(id)init
{
    self = [super init];

    if (self)
    {
        self.lampIDs = [[NSMutableArray alloc] init];
    }

    return self;
}

-(void)start
{
    [self pollLamps];
}

-(void)pollLamps
{
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        if (self.lampIDs.count > 0)
        {
            @synchronized(self.lampIDs)
            {
                NSString *lampID = [self.lampIDs objectAtIndex: 0];
                [self.lampIDs removeObjectAtIndex: 0];
                [self.lampIDs addObject: lampID];
                //NSLog(@"Ping lamp ID = %@", lampID);

//                LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
//                [ajManager.lsfLampManager pingLampWithID: lampID];

                double delay = (2.0 / (double)self.lampIDs.count);

                dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)((delay < 0.1 ? 0.1 : delay) * NSEC_PER_SEC)), ([LSFDispatchQueue getDispatchQueue]).queue, ^{
                    //NSLog(@"Should see a call to pollLamps in %f", delay);
                    [self pollLamps];
                });
            }
        }
        else
        {
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2 * NSEC_PER_SEC)), ([LSFDispatchQueue getDispatchQueue]).queue, ^{
                //NSLog(@"Should see a call to pollLamps in 2");
                [self pollLamps];
            });
        }
    });
}

-(void)addLampIDs: (NSArray *)lampIDs
{
    @synchronized(self.lampIDs)
    {
        for (NSString *lampID in lampIDs)
        {
            if (![self.lampIDs containsObject: lampID])
            {
                [self.lampIDs addObject: lampID];
            }
        }
    }
}

-(void)removeLampID: (NSArray *)lampID
{
    @synchronized(self.lampIDs)
    {
        [self.lampIDs removeObject: lampID];
    }
}

@end
