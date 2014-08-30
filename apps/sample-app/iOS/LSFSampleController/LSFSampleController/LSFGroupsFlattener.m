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

#import "LSFGroupsFlattener.h"

@implementation LSFGroupsFlattener

@synthesize groupIDSet = _groupIDSet;
@synthesize lampIDSet = _lampIDSet;
@synthesize duplicates = _duplicates;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.groupIDSet = [[NSMutableSet alloc] init];
        self.lampIDSet = [[NSMutableSet alloc] init];
        self.duplicates = 0;
    }
    
    return self;
}

-(void)flattenGroups: (NSMutableDictionary *)groups
{
    for (LSFGroupModel *groupModel in [groups allValues])
    {
        self.groupIDSet = [[NSMutableSet alloc] init];
        self.lampIDSet = [[NSMutableSet alloc] init];
        self.duplicates = 0;
        
        [self flattenGroups: groups withGroupModel: groupModel];
        [self flattenLamps: groups];
        
//        NSLog(@"LSFGroupsFlattener - Printing groups and lamps sets");
//        
//        for (NSString *groupID in self.groupIDSet)
//        {
//            NSLog(@"%@", groupID);
//        }
//        
//        for (NSString *lampID in self.lampIDSet)
//        {
//            NSLog(@"%@", lampID);
//        }

        groupModel.groups = self.groupIDSet;
        groupModel.lamps = self.lampIDSet;
        groupModel.duplicates = self.duplicates;
    }
}

-(void)flattenGroups: (NSMutableDictionary *)groups withGroupModel: (LSFGroupModel *)parentModel
{
    if (![self.groupIDSet containsObject: parentModel.theID])
    {
        [self.groupIDSet addObject: parentModel.theID];
        
        for (NSString *childGroupID in parentModel.members.lampGroups)
        {
            LSFGroupModel *childModel = [groups valueForKey: childGroupID];
            
            if (childModel != nil)
            {
                [self flattenGroups: groups withGroupModel: childModel];
            }
        }
    }
    else
    {
        self.duplicates++;
    }
}

-(void)flattenLamps: (NSMutableDictionary *)groups
{
    for (NSString *groupID in self.groupIDSet)
    {
        LSFGroupModel *groupModel = [groups valueForKey: groupID];
        
        if (groupModel != nil)
        {
            //NSArray *lamps = groupModel.members.lamps;
            [self.lampIDSet addObjectsFromArray: groupModel.members.lamps];
        }
    }
}

@end
