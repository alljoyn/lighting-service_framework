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

#import "LSFGarbageCollector.h"
#import "LSFLampModel.h"
#import "LSFLampModelContainer.h"
#import "LSFDispatchQueue.h"
#import "LSFConstants.h"
#import "LSFLampMaintenance.h"

@interface LSFGarbageCollector()

@property (nonatomic) unsigned int delay;
@property (nonatomic) unsigned int timeout;

-(BOOL)isExpired: (LSFLampModel *)lampModel;
-(void)run;

@end

@implementation LSFGarbageCollector

@synthesize reloadLightsDelegate = _reloadLightsDelegate;
@synthesize timeout = _timeout;
@synthesize delay = _delay;

+(LSFGarbageCollector *)getGarbageCollector
{
    static LSFGarbageCollector *garbageCollector = nil;
    static dispatch_once_t onceToken;

    dispatch_once(&onceToken, ^{
        garbageCollector = [[self alloc] init];
    });

    return garbageCollector;
}

-(id)init
{
    self = [super init];

    if (self)
    {
        LSFConstants *constants = [LSFConstants getConstants];

        self.delay = constants.pollingDelaySeconds;
        self.timeout = constants.lampExperationMilliseconds;
    }

    return self;
}

-(void)start
{
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        [self run];
    });
}

-(BOOL)isLampExpired: (NSString *)lampID
{
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = container.lampContainer;

    LSFLampModel *lampModel = [lamps valueForKey: lampID];
    return [self isExpired: lampModel];
}

-(BOOL)isExpired: (LSFLampModel *)lampModel
{
//    long long currentTimestamp = (long long)([[NSDate date] timeIntervalSince1970] * 1000);
//    return (lampModel == nil) || ((lampModel.timestamp + self.timeout) < currentTimestamp);

    return NO;
}

-(void)run
{
    //NSLog(@"LSFGarbageCollector - run() executing");

    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    NSArray *values = [container.lampContainer allValues];

    for (LSFLampModel *model in values)
    {
        if ([self isExpired: model])
        {
            NSLog(@"Pruned lamp %@", model.theID);
            [container.lampContainer removeObjectForKey: model.theID];

            dispatch_async(dispatch_get_main_queue(), ^{
                LSFLampMaintenance *lm = [LSFLampMaintenance getLampMaintenance];
                [lm removeLampID: model.theID];

                if (self.reloadLightsDelegate != nil)
                {
                    [self.reloadLightsDelegate deleteLampWithID: model.theID andName: model.name];
                }
            });
        }
    }

    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, self.delay * NSEC_PER_SEC), ([LSFDispatchQueue getDispatchQueue]).queue, ^{
        [self run];
    });
}

@end
