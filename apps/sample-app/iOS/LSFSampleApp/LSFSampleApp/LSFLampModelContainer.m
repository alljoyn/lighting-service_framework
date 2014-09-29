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

#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"

@interface LSFLampModelContainer()

//-(void)populateDictionary;

@end

@implementation LSFLampModelContainer

@synthesize lampContainer = _lampContainer;

+(id)getLampModelContainer
{
    static LSFLampModelContainer *lampModelContainer = nil;
    static dispatch_once_t onceToken;
    
    dispatch_once(&onceToken, ^{
        lampModelContainer = [[self alloc] init];
    });
    
    return lampModelContainer;
}

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.lampContainer = [[NSMutableDictionary alloc] init];
        
        //TODO - remove this call, only used for testing
        //[self populateDictionary];
    }
    
    return self;
}

//TODO - delete function when testing is over
//-(void)populateDictionary
//{
//    LSFLampModel *model1 = [[LSFLampModel alloc] initWithLampID: @"LampID1"];
//    [model1 setName: @"Basic White Lamp"];
//    [model1 setState: [[LSFLampState alloc] initWithOnOff: YES brightness: 0 hue: 0 saturation: 0 colorTemp: 0]];
//    
//    //Set Lamp Details
//    LSFLampDetails *lampDetails = [[LSFLampDetails alloc] init];
//    lampDetails.dimmable = NO;
//    lampDetails.color = NO;
//    lampDetails.variableColorTemp = NO;
//    [model1 setLampDetails: lampDetails];
//    
//    [self.lampContainer setValue: model1 forKey: @"LampID1"];
//    
//    LSFLampModel *model2 = [[LSFLampModel alloc] init];
//    [model2 setTheID: @"LampID2"];
//    [model2 setName: @"Dimmable White Lamp"];
//    [model2 setState: [[LSFLampState alloc] initWithOnOff: NO brightness: 25 hue: 0 saturation: 0 colorTemp: 0]];
//    
//    //Set Lamp Details
//    lampDetails = [[LSFLampDetails alloc] init];
//    lampDetails.dimmable = YES;
//    lampDetails.color = NO;
//    lampDetails.variableColorTemp = NO;
//    [model2 setLampDetails: lampDetails];
//    
//    [self.lampContainer setValue: model2 forKey: @"LampID2"];
//    
//    LSFLampModel *model3 = [[LSFLampModel alloc] init];
//    [model3 setTheID: @"LampID3"];
//    [model3 setName: @"Tunable White Lamp"];
//    [model3 setState: [[LSFLampState alloc] initWithOnOff: NO brightness: 50 hue: 0 saturation: 0 colorTemp: 6000]];
//    
//    //Set Lamp Details
//    lampDetails = [[LSFLampDetails alloc] init];
//    lampDetails.dimmable = YES;
//    lampDetails.color = NO;
//    lampDetails.variableColorTemp = YES;
//    [model3 setLampDetails: lampDetails];
//    
//    [self.lampContainer setValue: model3 forKey: @"LampID3"];
//}

@end
