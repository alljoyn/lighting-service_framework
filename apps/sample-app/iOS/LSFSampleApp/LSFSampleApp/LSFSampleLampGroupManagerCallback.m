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
#import "LSFTabManager.h"
#import "LSFEnums.h"
#import "LSFColorAverager.h"

@interface LSFSampleLampGroupManagerCallback()

@property (nonatomic, strong) dispatch_queue_t queue;
@property (nonatomic, strong) NSString *flattenTriggerGroupID;
@property (nonatomic, strong) LSFColorAverager *averageBrightness;
@property (nonatomic, strong) LSFColorAverager *averageHue;
@property (nonatomic, strong) LSFColorAverager *averageSaturation;
@property (nonatomic, strong) LSFColorAverager *averageColorTemp;

-(void)postProcessLampGroupIDs: (NSArray *)groupIDs;
-(void)postProcessLampGroupID: (NSString *)groupID;
-(void)postUpdateLampGroupName: (NSString *)groupID andName: (NSString *)name;
-(void)postUpdateLampGroup: (NSString *)groupID andLampGroup: (LSFLampGroup *)lampGroup;
-(void)postFlattenLampGroups;
-(void)postUpdateLampGroupState: (LSFGroupModel *)groupModel;
-(void)postUpdateLampGroupMembersLamps: (NSString *)groupID;
-(void)postDeleteGroups: (NSArray *)groupIDs;
-(void)updateGroupWithID: (NSString *)groupID andCallbackOperation: (GroupCallbackOperation)callbackOp;

@end

@implementation LSFSampleLampGroupManagerCallback

@synthesize queue = _queue;
@synthesize flattenTriggerGroupID = _flattenTriggerGroupID;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.queue = ([LSFDispatchQueue getDispatchQueue]).queue;
        self.averageBrightness = [[LSFColorAverager alloc] init];
        self.averageHue = [[LSFColorAverager alloc] init];
        self.averageSaturation = [[LSFColorAverager alloc] init];
        self.averageColorTemp = [[LSFColorAverager alloc] init];
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

    dispatch_async(self.queue, ^{
        [self postProcessLampGroupID: groupID];
    });
}

-(void)lampGroupsCreated: (NSArray *)groupIDs
{
    //NSLog(@"Lamp Groups Created Callback");

    dispatch_async(self.queue, ^{
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
    if (rc != LSF_OK)
    {
        NSLog(@"LSFSampleLampGroupManagerCallback - transitionLampGroupToStateReply() returned error code %i", rc);
    }

    dispatch_async(self.queue, ^{
        [self postUpdateLampGroupMembersLamps: groupID];
    });
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
        groupModel = [[LSFGroupModel alloc] initWithGroupID: groupID];
        [groups setValue: groupModel forKey: groupID];

        [self updateGroupWithID: groupID andCallbackOperation: GroupCreated];

        dispatch_async(self.queue, ^{
            LSFTabManager *tabManager = [LSFTabManager getTabManager];
            [tabManager updateGroupsTab];
        });
        
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfLampGroupManager getLampGroupNameForID: groupID];
        [ajManager.lsfLampGroupManager getLampGroupWithID: groupID];
    }
    
    if ([groupID isEqualToString: self.flattenTriggerGroupID])
    {
        [self postFlattenLampGroups];
    }
}

-(void)postUpdateLampGroupName: (NSString *)groupID andName: (NSString *)name
{
    NSMutableDictionary *groups = ((LSFGroupModelContainer *)[LSFGroupModelContainer getGroupModelContainer]).groupContainer;
    LSFGroupModel *groupModel = [groups valueForKey: groupID];
    
    if (groupModel != nil)
    {
        groupModel.name = name;
    }
    
    [self updateGroupWithID: groupID andCallbackOperation: GroupNameUpdated];
}

-(void)postUpdateLampGroup: (NSString *)groupID andLampGroup: (LSFLampGroup *)lampGroup
{
    NSMutableDictionary *groups = ((LSFGroupModelContainer *)[LSFGroupModelContainer getGroupModelContainer]).groupContainer;
    LSFGroupModel *groupModel = [groups valueForKey: groupID];
    
    if (groupModel != nil)
    {
        groupModel.members = lampGroup;
    }

    [self postFlattenLampGroups];
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
    
//    BOOL powerOn = NO;
//    uint64_t brightness = 0;
//    uint64_t hue = 0;
//    uint64_t saturation = 0;
//    uint64_t colorTemp = 0;
//    
//    uint64_t countDim = 0;
//    uint64_t countColor = 0;
//    uint64_t countTemp = 0;

    LSFCapabilityData *capability = [[LSFCapabilityData alloc] init];
    int countOn = 0;
    int countOff = 0;

    [self.averageBrightness reset];
    [self.averageHue reset];
    [self.averageSaturation reset];
    [self.averageColorTemp reset];
    
    for (NSString *lampID in groupModel.lamps)
    {
        LSFLampModel *lampModel = [lamps valueForKey: lampID];
        
        if (lampModel != nil)
        {
            [capability includeData: lampModel.capability];

            if (lampModel.state.onOff)
            {
                countOn++;
            }
            else
            {
                countOff++;
            }

            if (lampModel.lampDetails.dimmable)
            {
                [self.averageBrightness add: lampModel.state.brightness];
            }

            if (lampModel.lampDetails.color)
            {
                [self.averageHue add: lampModel.state.hue];
                [self.averageSaturation add: lampModel.state.saturation];
            }

            if (lampModel.lampDetails.variableColorTemp)
            {
                [self.averageColorTemp add: lampModel.state.colorTemp];
            }
        }
        else
        {
            NSLog(@"postUpdateLampGroupState - missing lamp");
        }
    }

    groupModel.capability = capability;
    
    groupModel.state.onOff = (countOn > 0);
    groupModel.state.brightness = (uint32_t)[self.averageBrightness getAverage];
    groupModel.state.hue = (uint32_t)[self.averageHue getAverage];
    groupModel.state.saturation = (uint32_t)[self.averageSaturation getAverage];
    groupModel.state.colorTemp = (uint32_t)[self.averageColorTemp getAverage];

    groupModel.uniformity.power = ((countOn == 0) || (countOff == 0));
    groupModel.uniformity.brightness = [self.averageBrightness isUniform];
    groupModel.uniformity.hue = [self.averageHue isUniform];
    groupModel.uniformity.saturation = [self.averageSaturation isUniform];
    groupModel.uniformity.colorTemp = [self.averageColorTemp isUniform];

//    NSLog(@"Group Model State - %@", groupModel.name);
//    NSLog(@"OnOff = %@, Uniformity = %@", groupModel.state.onOff ? @"On" : @"Off", groupModel.uniformity.power ? @"True" : @"False");
//    NSLog(@"Brightness = %u, Uniformity = %@", groupModel.state.brightness, groupModel.uniformity.brightness ? @"True" : @"False");
//    NSLog(@"Hue = %u, Uniformity = %@", groupModel.state.hue, groupModel.uniformity.hue ? @"True" : @"False");
//    NSLog(@"Saturation = %u, Uniformity = %@", groupModel.state.saturation, groupModel.uniformity.saturation ? @"True" : @"False");
//    NSLog(@"Color Temp = %u, Uniformity = %@", groupModel.state.colorTemp, groupModel.uniformity.colorTemp ? @"True" : @"False");

    [self updateGroupWithID: groupModel.theID andCallbackOperation: GroupStateUpdated];
}

-(void)postUpdateLampGroupMembersLamps: (NSString *)groupID
{
    NSLog(@"postUpdateLampGroupMembersLamps() executing");

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

        dispatch_async(self.queue, ^{
            LSFTabManager *tabManager = [LSFTabManager getTabManager];
            [tabManager updateGroupsTab];
        });
    }

    dispatch_async(dispatch_get_main_queue(), ^{
        NSNumber *groupOp = [[NSNumber alloc] initWithInt: GroupDeleted];
        NSDictionary *userInfoDict = [[NSDictionary alloc] initWithObjects: [[NSArray alloc] initWithObjects: groupOp, groupIDs, [NSArray arrayWithArray: groupNames], nil] forKeys: [[NSArray alloc] initWithObjects: @"operation", @"groupIDs", @"groupNames", nil]];
        [[NSNotificationCenter defaultCenter] postNotificationName: @"GroupNotification" object: self userInfo: userInfoDict];
    });
}

-(void)updateGroupWithID: (NSString *)groupID andCallbackOperation: (GroupCallbackOperation)callbackOp
{
    dispatch_async(dispatch_get_main_queue(), ^{
        NSNumber *groupOp = [[NSNumber alloc] initWithInt: callbackOp];
        NSDictionary *userInfoDict = [[NSDictionary alloc] initWithObjects: [[NSArray alloc] initWithObjects: groupOp, groupID, nil] forKeys: [[NSArray alloc] initWithObjects: @"operation", @"groupID", nil]];
        [[NSNotificationCenter defaultCenter] postNotificationName: @"GroupNotification" object: self userInfo: userInfoDict];
    });
}

@end
