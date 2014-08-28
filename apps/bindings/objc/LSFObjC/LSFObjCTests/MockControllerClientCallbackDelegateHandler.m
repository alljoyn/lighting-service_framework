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

#import "MockControllerClientCallbackDelegateHandler.h"

@interface MockControllerClientCallbackDelegateHandler()

@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation MockControllerClientCallbackDelegateHandler

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
-(void)connectedToControllerServiceWithID: (NSString *)controllerServiceID andName: (NSString *)controllerServiceName
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"connectedToControllerService"];
    [self.dataArray addObject: controllerServiceID];
    [self.dataArray addObject: controllerServiceName];
}

-(void)connectToControllerServiceFailedForID: (NSString *)controllerServiceID andName: (NSString *)controllerServiceName
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"connectToControllerServiceFailed"];
    [self.dataArray addObject: controllerServiceID];
    [self.dataArray addObject: controllerServiceName];
}

-(void)disconnectedFromControllerServiceWithID: (NSString *)controllerServiceID andName: (NSString *)controllerServiceName
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"disconnectedFromControllerService"];
    [self.dataArray addObject: controllerServiceID];
    [self.dataArray addObject: controllerServiceName];
}

-(void)controllerClientError: (NSArray *)ec
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"controllerClientError"];
    [self.dataArray addObject: ec];
}

@end
