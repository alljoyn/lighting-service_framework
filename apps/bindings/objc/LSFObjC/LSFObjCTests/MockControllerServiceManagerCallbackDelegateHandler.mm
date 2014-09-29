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

#import "MockControllerServiceManagerCallbackDelegateHandler.h"

@interface MockControllerServiceManagerCallbackDelegateHandler()

@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation MockControllerServiceManagerCallbackDelegateHandler

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

//Delegate Methods
-(void)getControllerServiceVersionReply: (unsigned int)version
{
    [self.dataArray removeAllObjects];
    NSNumber *csVersion = [[NSNumber alloc] initWithUnsignedInteger: version];
    [self.dataArray addObject: @"getControllerServiceVersion"];
    [self.dataArray addObject: csVersion];
}

-(void)lightingResetControllerServiceReplyWithCode: (LSFResponseCode)rc
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"lightingResetControllerService"];
    [self.dataArray addObject: responseCode];
}

-(void)controllerServiceLightingReset
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"controllerServiceLightingReset"];
}

-(void)controllerServiceNameChangedForControllerID: (NSString *)controllerID andName: (NSString *)controllerName
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"controllerServiceNameChanged"];
    [self.dataArray addObject: controllerID];
    [self.dataArray addObject: controllerName];
}

@end
