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

#import "LSFPresetModelContainer.h"
#import "LSFPresetModel.h"

@interface LSFPresetModelContainer()

//-(void)populateDictionary;

@end

@implementation LSFPresetModelContainer

@synthesize presetContainer = _presetContainer;

+(id)getPresetModelContainer
{
    static LSFPresetModelContainer *presetModelContainer = nil;
    static dispatch_once_t onceToken;
    
    dispatch_once(&onceToken, ^{
        presetModelContainer = [[self alloc] init];
    });
    
    return presetModelContainer;
}

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.presetContainer = [[NSMutableDictionary alloc] init];
        
        //TODO - remove this call, only used for testing
        //[self populateDictionary];
    }
    
    return self;
}

/*
 * Private Functions
 */
//TODO - delete function when testing is over
//-(void)populateDictionary
//{
//    LSFPresetModel *model1 = [[LSFPresetModel alloc] initWithPresetID: @"presetID1"];
//    model1.presetName = @"Max";
//    model1.preset = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
//    [self.presetContainer setValue: model1 forKey: @"presetID1"];
//    
//    LSFPresetModel *model2 = [[LSFPresetModel alloc] initWithPresetID: @"presetID2"];
//    model2.presetName = @"Half";
//    model2.preset = [[LSFLampState alloc] initWithOnOff: YES brightness: 50 hue: 180 saturation: 50 colorTemp: 5000];
//    [self.presetContainer setValue: model2 forKey: @"presetID2"];
//}
@end
