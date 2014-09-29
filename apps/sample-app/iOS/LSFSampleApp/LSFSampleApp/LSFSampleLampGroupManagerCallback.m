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

#import "LSFSampleLampGroupManagerCallback.h"
#import "LSFDispatchQueue.h"
#import "LSFConstants.h"
#import "LSFGroupModelContainer.h"
#import "LSFGroupModel.h"
#import "LSFGroupsFlattener.h"
#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"
#import "LSFAllJoynManager.h"

@interface LSFSampleLampGroupManagerCallback()

@property (nonatomic, strong) dispatch_queue_t queue;
@property (nonatomic, strong) NSString *flattenTriggerGroupID;

-(void)postProcessLampGroupIDs: (NSArray *)groupIDs;
-(void)postProcessLampGroupID: (NSString *)groupID;
-(void)postUpdateLampGroupID: (NSString *)groupID;
-(void)postUpdateLampGroupName: (NSString *)groupID andName: (NSString *)name;
-(void)postUpdateLampGroup: (NSString *)groupID andLampGroup: (LSFLampGroup *)lampGroup;
-(void)postFlattenLampGroups;
-(void)postUpdateLampGroupState: (LSFGroupModel *)groupModel;
-(void)postUpdateLampGroupMembersLamps: (NSString *)groupID;
-(void)postDeleteGroups: (NSArray *)groupIDs;
-(void)updateGroupWithID: (NSString *)groupID;

@end

@implementation LSFSampleLampGroupManagerCallback

@synthesize reloadGroupsDelegate = _reloadGroupsDelegate;
@synthesize queue = _queue;
@synthesize flattenTriggerGroupID = _flattenTriggerGroupID;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.queue = ([LSFDispatchQueue getDispatchQueue]).queue;
    }
    
    return self;
}

-(void)refreshAllLampGroupIDs
{
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = container.groupContainer;

    if (groups.count > 0)
    {
        [self getAllLampGroupIDsReplyWithCode: LSF_OK andGroupIDs: [groups allKeys]];
    }
}

/*
 * Implementation of LSFLampGroupManagerCallbackDelegate
 */
-(void)getAllLampGroupIDsReplyWithCode: (LSFResponseCode)rc andGroupIDs: (NSArray*)groupIDs
{
    //NSLog(@"getAllLampGroupIDs() executing");

    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManagerCallback - getAllLampGroupIDsReply() returned error code %i", rc);
    }

    dispatch_async(self.queue, ^{
        [self postProcessLampGroupIDs: groupIDs];
        [self postProcessLampGroupID: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID];

        for (NSString *groupID in groupIDs)
        {
            [self postProcessLampGroupID: groupID];
        }
    });
}

-(void)getLampGroupNameReplyWithCode: (LSFResponseCode)rc groupID: (NSString*)groupID language: (NSString *)language andGroupName: (NSString*)name
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManagerCallback - getLampGroupNameReply() returned error code %i", rc);
    }

    dispatch_async(self.queue, ^{
        [self postUpdateLampGroupName: groupID andName: name];
    });
}

-(void)setLampGroupNameReplyWithCode: (LSFResponseCode)rc groupID: (NSString*)groupID andLanguage: (NSString *)language
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManagerCallback - setLampGroupNameReply() returned error code %i", rc);
    }
    
    dispatch_async(self.queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampGroupManager getLampGroupNameForID: groupID andLanguage: language];
    });
}

-(void)lampGroupsNameChanged: (NSArray *)groupIDs
{
    dispatch_async(self.queue, ^{
        BOOL containsNewIDs = NO;
        NSMutableDictionary *groups = ((LSFGroupModelContainer *)[LSFGroupModelContainer getGroupModelContainer]).groupContainer;
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *groupID in groupIDs)
        {
            if ([groups valueForKey: groupID] != nil)
            {
                [ajManager.lsfLampGroupManager getLampGroupNameForID: groupID];
            }
            else
            {
                containsNewIDs = YES;
            }
        }
        
        if (containsNewIDs)
        {
            [ajManager.lsfLampGroupManager getAllLampGroupIDs];
        }
    });
}

-(void)createLampGroupReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManagerCallback - createLampGroupNameReply() returned error code %i", rc);
    }

//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.3 * NSEC_PER_SEC)), self.queue, ^{
//        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
//        [ajManager.lsfLampGroupManager getAllLampGroupIDs];
//    });
}

-(void)lampGroupsCreated: (NSArray *)groupIDs
{
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.3 * NSEC_PER_SEC)), self.queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampGroupManager getAllLampGroupIDs];
    });
}

-(void)getLampGroupReplyWithCode: (LSFResponseCode)rc groupID: (NSString *)groupID andLampGroup: (LSFLampGroup *)group
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManagerCallback - getLampGroupReply() returned error code %i", rc);
    }
    else
    {
        dispatch_async(self.queue, ^{
            [self postUpdateLampGroup: groupID andLampGroup: group];
        });
    }
}

-(void)deleteLampGroupReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    NSLog(@"LSFSampleLampGroupManagerCallback - deleteLampGroupReply()");

    if (rc != LSF_OK)
    {
        NSLog(@"Delete lamp group returned an error code = %u", rc);
    }
}

-(void)lampGroupsDeleted: (NSArray *)groupIDs
{
    dispatch_async(self.queue, ^{
        [self postDeleteGroups: groupIDs];
    });
}

-(void)transitionLampGroupToStateReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    //TODO
}

-(void)pulseLampGroupWithStateReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    //TODO
}

-(void)pulseLampGroupWithPresetReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    //TODO
}

-(void)transitionLampGroupStateOnOffFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManagerCallback - transitionLampGroupStateOnOffFieldReply() returned error code %i", rc);
    }
    
    dispatch_async(self.queue, ^{
        [self postUpdateLampGroupMembersLamps: groupID];
    });
}

-(void)transitionLampGroupStateHueFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManagerCallback - transitionLampGroupStateHueFieldReply() returned error code %i", rc);
    }

    dispatch_async(self.queue, ^{
        [self postUpdateLampGroupMembersLamps: groupID];
    });
}

-(void)transitionLampGroupStateSaturationFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManagerCallback - transitionLampGroupStateSaturationFieldReply() returned error code %i", rc);
    }
    
    dispatch_async(self.queue, ^{
        [self postUpdateLampGroupMembersLamps: groupID];
    });
}

-(void)transitionLampGroupStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManagerCallback - transitionLampGroupStateBrightnessFieldReply() returned error code %i", rc);
    }
    
    dispatch_async(self.queue, ^{
        [self postUpdateLampGroupMembersLamps: groupID];
    });
}

-(void)transitionLampGroupStateColorTempFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString *)groupID
{
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManagerCallback - transitionLampGroupStateColorTempFieldReply() returned error code %i", rc);
    }
    
    dispatch_async(self.queue, ^{
        [self postUpdateLampGroupMembersLamps: groupID];
    });
}

-(void)resetLampGroupStateReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    //TODO
}

-(void)resetLampGroupStateOnOffFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    //TODO
}

-(void)resetLampGroupStateHueFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    //TODO
}

-(void)resetLampGroupStateSaturationFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    //TODO
}

-(void)resetLampGroupStateBrightnessFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    //TODO
}

-(void)resetLampGroupStateColorTempFieldReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    //TODO
}

-(void)updateLampGroupReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    //TODO
}

-(void)lampGroupsUpdated: (NSArray *)groupIDs
{
    dispatch_async(self.queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        
        for (NSString *groupID in groupIDs)
        {
            [ajManager.lsfLampGroupManager getLampGroupWithID: groupID];
        }
    });
}

-(void)transitionLampGroupStateToPresetReplyWithCode: (LSFResponseCode)rc andGroupID: (NSString*)groupID
{
    //TODO
}

/*
 * Private Functions
 */
-(void)postProcessLampGroupIDs: (NSArray *)groupIDs
{
    if ([groupIDs count] > 0)
    {
        self.flattenTriggerGroupID = [groupIDs objectAtIndex: ([groupIDs count] - 1)];
    }
    else
    {
        self.flattenTriggerGroupID = ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID;
    }
}

-(void)postProcessLampGroupID: (NSString *)groupID
{
    NSMutableDictionary *groups = ((LSFGroupModelContainer *)[LSFGroupModelContainer getGroupModelContainer]).groupContainer;
    LSFGroupModel *groupModel = [groups valueForKey: groupID];
    
    if (groupModel == nil)
    {
        [self postUpdateLampGroupID: groupID];
        
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampGroupManager getLampGroupNameForID: groupID];
        [ajManager.lsfLampGroupManager getLampGroupWithID: groupID];
    }
    
    if ([groupID isEqualToString: self.flattenTriggerGroupID])
    {
        [self postFlattenLampGroups];
    }
}

-(void)postUpdateLampGroupID: (NSString *)groupID
{
    NSMutableDictionary *groups = ((LSFGroupModelContainer *)[LSFGroupModelContainer getGroupModelContainer]).groupContainer;
    LSFGroupModel *groupModel = [groups valueForKey: groupID];

    if (groupModel == nil)
    {
        groupModel = [[LSFGroupModel alloc] initWithGroupID: groupID];
        [groups setValue: groupModel forKey: groupID];
    }

    //[self updateGroupWithID: groupID];
}

-(void)postUpdateLampGroupName: (NSString *)groupID andName: (NSString *)name
{
    NSMutableDictionary *groups = ((LSFGroupModelContainer *)[LSFGroupModelContainer getGroupModelContainer]).groupContainer;
    LSFGroupModel *groupModel = [groups valueForKey: groupID];
    
    if (groupModel != nil)
    {
        groupModel.name = name;
    }
    
    [self updateGroupWithID: groupID];
}

-(void)postUpdateLampGroup: (NSString *)groupID andLampGroup: (LSFLampGroup *)lampGroup
{
    NSMutableDictionary *groups = ((LSFGroupModelContainer *)[LSFGroupModelContainer getGroupModelContainer]).groupContainer;
    LSFGroupModel *groupModel = [groups valueForKey: groupID];
    
    if (groupModel != nil)
    {
        groupModel.members = lampGroup;
    }
    
    if ([groupID isEqualToString: self.flattenTriggerGroupID])
    {
        [self postFlattenLampGroups];
    }

    [self updateGroupWithID: groupID];
}

-(void)postFlattenLampGroups
{
    NSMutableDictionary *groups = ((LSFGroupModelContainer *)[LSFGroupModelContainer getGroupModelContainer]).groupContainer;
    [([[LSFGroupsFlattener alloc] init]) flattenGroups: groups];
    
    for (LSFGroupModel *groupModel in [groups allValues])
    {
        [self postUpdateLampGroupState: groupModel];
    }
}

-(void)postUpdateLampGroupState: (LSFGroupModel *)groupModel
{
    NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
    
    BOOL powerOn = NO;
    uint64_t brightness = 0;
    uint64_t hue = 0;
    uint64_t saturation = 0;
    uint64_t colorTemp = 0;
    
    uint64_t countDim = 0;
    uint64_t countColor = 0;
    uint64_t countTemp = 0;
    LSFCapabilityData *capability = [[LSFCapabilityData alloc] init];
    
    for (NSString *lampID in groupModel.lamps)
    {
        LSFLampModel *lampModel = [lamps valueForKey: lampID];
        
        if (lampModel != nil)
        {
            [capability includeData: lampModel.capability];
            
            powerOn = powerOn || lampModel.state.onOff;
            
            if (lampModel.lampDetails.dimmable)
            {
                brightness = brightness + lampModel.state.brightness;
                countDim++;
            }
            
            if (lampModel.lampDetails.color)
            {
                hue = hue + lampModel.state.hue;
                saturation = saturation + lampModel.state.saturation;
                countColor++;
            }
            
            if (lampModel.lampDetails.variableColorTemp)
            {
                colorTemp = colorTemp + lampModel.state.colorTemp;
                countTemp++;
            }
        }
        else
        {
            //NSLog(@"postUpdateLampGroupState - missing lamp");
        }
    }
    
    double divisorDim = countDim > 0 ? countDim : 1.0;
    double divisorColor = countColor > 0 ? countColor : 1.0;
    double divisorTemp = countTemp > 0 ? countTemp : 1.0;
    
    groupModel.state.onOff = powerOn;
    groupModel.state.brightness = (uint32_t)round(brightness / divisorDim);
    groupModel.state.hue = (uint32_t)round(hue / divisorColor);
    groupModel.state.saturation = (uint32_t)round(saturation / divisorColor);
    groupModel.state.colorTemp = (uint32_t)round(colorTemp / divisorTemp);
    groupModel.capability = capability;

//    NSLog(@"OnOff = %@", powerOn ? @"On" : @"Off");
//    NSLog(@"Brightness = %u", (uint32_t)round(brightness / divisorDim));
//    NSLog(@"Hue = %u", (uint32_t)round(hue / divisorColor));
//    NSLog(@"Saturation = %u", (uint32_t)round(saturation / divisorColor));
//    NSLog(@"Color Temp = %u", (uint32_t)round(colorTemp / divisorTemp));
//    NSLog(@"Group Model State - %@", groupModel.name);
//    NSLog(@"OnOff = %@", groupModel.state.onOff ? @"On" : @"Off");
//    NSLog(@"Brightness = %u", groupModel.state.brightness);
//    NSLog(@"Hue = %u", groupModel.state.hue);
//    NSLog(@"Saturation = %u", groupModel.state.saturation);
//    NSLog(@"Color Temp = %u", groupModel.state.colorTemp);

    [self updateGroupWithID: groupModel.theID];
}

-(void)postUpdateLampGroupMembersLamps: (NSString *)groupID
{
    NSMutableDictionary *groups = ((LSFGroupModelContainer *)[LSFGroupModelContainer getGroupModelContainer]).groupContainer;
    LSFGroupModel *groupModel = [groups valueForKey: groupID];
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
    
    if (groupModel != nil)
    {
        for (NSString *lampID in groupModel.lamps)
        {
            [ajManager.lsfLampManager getLampStateForID: lampID];
        }
    }
}

-(void)postDeleteGroups: (NSArray *)groupIDs
{
    NSMutableArray *groupNames = [[NSMutableArray alloc] init];
    NSMutableDictionary *groups = ((LSFGroupModelContainer *)[LSFGroupModelContainer getGroupModelContainer]).groupContainer;
    
    for (int i = 0; i < groupIDs.count; i++)
    {
        NSString *groupID = [groupIDs objectAtIndex: i];
        LSFGroupModel *model = [groups valueForKey: groupID];

        [groupNames insertObject: model.name atIndex: i];
        [groups removeObjectForKey: groupID];
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.reloadGroupsDelegate != nil)
        {
            [self.reloadGroupsDelegate deleteGroupsWithIDs: groupIDs andNames: [NSArray arrayWithArray: groupNames]];
        }
    });
}

-(void)updateGroupWithID: (NSString *)groupID
{
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.reloadGroupsDelegate != nil)
        {
            [self.reloadGroupsDelegate reloadGroupWithID: groupID];
        }
    });
}

@end
