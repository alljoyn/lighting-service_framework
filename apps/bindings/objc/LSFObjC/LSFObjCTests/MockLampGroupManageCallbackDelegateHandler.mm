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

#import "MockLampGroupManageCallbackDelegateHandler.h"

@interface MockLampGroupManageCallbackDelegateHandler()

@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation MockLampGroupManageCallbackDelegateHandler

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
-(void)getAllLampGroupIDsReplyWithCode: (LSFResponseCode)rc andGroupIDs: (NSArray*)groupIDs
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getAllLampGroupIDs"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupIDs];
}


-(void)getLampGroupNameReplyWithCode: (LSFResponseCode)rc groupID: (NSString*)groupID language: (NSString *)language andGroupName: (NSString*)name
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampGroupName"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
    [self.dataArray addObject: language];
    [self.dataArray addObject: name];
}

-(void)setLampGroupNameReplyWithCode: (LSFResponseCode)rc groupID: (NSString*)groupID andLanguage: (NSString *)language
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"setLampGroupName"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
    [self.dataArray addObject: language];
}

-(void)lampGroupsNameChanged: (NSArray *)groupIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"lampGroupsNameChanged"];
    [self.dataArray addObject: groupIDs];
}

-(void)createLampGroupReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"createLampGroup"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];

}

-(void)lampGroupsCreated: (NSArray *)groupIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"lampGroupsCreated"];
    [self.dataArray addObject: groupIDs];
}

-(void)getLampGroupReplyWithCode: (LSFResponseCode)rc groupID: (NSString *)groupID andLampGroup: (LSFLampGroup *)group
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"getLampGroup"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
    [self.dataArray addObject: [group lamps]];
    [self.dataArray addObject: [group lampGroups]];
}

-(void)deleteLampGroupReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"deleteLampGroup"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)lampGroupsDeleted: (NSArray *)groupIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"lampGroupsDeleted"];
    [self.dataArray addObject: groupIDs];
}

-(void)transitionLampGroupToStateReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampGroupToState"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)pulseLampGroupWithStateReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"pulseLampGroupWithState"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)pulseLampGroupWithPresetReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"pulseLampGroupWithPreset"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)transitionLampGroupStateOnOffFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampGroupStateOnOffField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)transitionLampGroupStateHueFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampGroupStateHueField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)transitionLampGroupStateSaturationFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampGroupStateSaturationField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)transitionLampGroupStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampGroupStateBrightnessField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)transitionLampGroupStateColorTempFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampGroupStateColorTempField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)resetLampGroupStateReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampGroupState"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)resetLampGroupStateOnOffFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampGroupStateOnOffField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)resetLampGroupStateHueFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampGroupStateHueField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)resetLampGroupStateSaturationFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampGroupStateSaturationField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)resetLampGroupStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampGroupStateBrightnessField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)resetLampGroupStateColorTempFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"resetLampGroupStateColorTempField"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)updateLampGroupReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"updateLampGroup"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

-(void)lampGroupsUpdated: (NSArray *)groupIDs
{
    [self.dataArray removeAllObjects];
    [self.dataArray addObject: @"lampGroupsUpdated"];
    [self.dataArray addObject: groupIDs];
}

-(void)transitionLampGroupStateToPresetReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    [self.dataArray removeAllObjects];
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: rc];
    [self.dataArray addObject: @"transitionLampGroupStateToPreset"];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: groupID];
}

@end
