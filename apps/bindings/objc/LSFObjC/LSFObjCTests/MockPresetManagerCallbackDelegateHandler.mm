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

#import "MockPresetManagerCallbackDelegateHandler.h"

@interface MockPresetManagerCallbackDelegateHandler()

@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation MockPresetManagerCallbackDelegateHandler

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
-(void)getPresetReplyWithCode: (LSFResponseCode)rc presetID: (NSString *)presetID andPreset: (LSFLampState *)preset
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getPreset"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: presetID];
    NSNumber *onOff = [[NSNumber alloc] initWithBool: preset.onOff];
    NSNumber *brightness = [[NSNumber alloc] initWithUnsignedInt: preset.brightness];
    NSNumber *hue = [[NSNumber alloc] initWithUnsignedInt: preset.hue];
    NSNumber *saturation = [[NSNumber alloc] initWithUnsignedInt: preset.saturation];
    NSNumber *colorTemp = [[NSNumber alloc] initWithUnsignedInt: preset.colorTemp];
    [self.dataArray addObject: onOff];
    [self.dataArray addObject: brightness];
    [self.dataArray addObject: hue];
    [self.dataArray addObject: saturation];
    [self.dataArray addObject: colorTemp];
}

-(void)getAllPresetIDsReplyWithCode: (LSFResponseCode)rc andPresetIDs: (NSArray *)presetIDs
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getAllPresetIDs"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: presetIDs];
}

-(void)getPresetNameReplyWithCode: (LSFResponseCode)rc presetID: (NSString *)presetID language: (NSString *)language andPresetName: (NSString *)presetName
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getPresetName"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: presetID];
    [self.dataArray addObject: language];
    [self.dataArray addObject: presetName];
}

-(void)setPresetNameReplyWithCode: (LSFResponseCode)rc presetID: (NSString *)presetID andLanguage: (NSString *)language
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"setPresetName"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: presetID];
    [self.dataArray addObject: language];
}

-(void)presetsNameChanged: (NSArray *)presetIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"presetsNameChanged"];
    [self.dataArray addObject: presetIDs];
}

-(void)createPresetReplyWithCode: (LSFResponseCode)rc andPresetID: (NSString *)presetID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"createPreset"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: presetID];
}

-(void)presetsCreated: (NSArray *)presetIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"presetsCreated"];
    [self.dataArray addObject: presetIDs];
}

-(void)updatePresetReplyWithCode: (LSFResponseCode)rc andPresetID: (NSString *)presetID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"updatePreset"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: presetID];
}

-(void)presetsUpdated: (NSArray *)presetIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"presetsUpdated"];
    [self.dataArray addObject: presetIDs];
}

-(void)deletePresetReplyWithCode: (LSFResponseCode)rc andPresetID: (NSString *)presetID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"deletePreset"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: presetID];
}

-(void)presetsDeleted: (NSArray *)presetIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"presetsDeleted"];
    [self.dataArray addObject: presetIDs];
}

-(void)getDefaultLampStateReplyWithCode: (LSFResponseCode)rc andLampState: (LSFLampState *)defaultLampState
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getDefaultLampState"];
    [self.dataArray addObject: responseCode];
    NSNumber *onOff = [[NSNumber alloc] initWithBool: defaultLampState.onOff];
    NSNumber *brightness = [[NSNumber alloc] initWithUnsignedInt: defaultLampState.brightness];
    NSNumber *hue = [[NSNumber alloc] initWithUnsignedInt: defaultLampState.hue];
    NSNumber *saturation = [[NSNumber alloc] initWithUnsignedInt: defaultLampState.saturation];
    NSNumber *colorTemp = [[NSNumber alloc] initWithUnsignedInt: defaultLampState.colorTemp];
    [self.dataArray addObject: onOff];
    [self.dataArray addObject: brightness];
    [self.dataArray addObject: hue];
    [self.dataArray addObject: saturation];
    [self.dataArray addObject: colorTemp];
}

-(void)setDefaultLampStateReplyWithCode: (LSFResponseCode)rc
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"setDefaultLampState"];
    [self.dataArray addObject: responseCode];
}

-(void)defaultLampStateChanged
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"defaultLampStateChanged"];
}

@end
