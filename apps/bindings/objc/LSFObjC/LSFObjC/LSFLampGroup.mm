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

#import "LSFLampGroup.h"
#import "LSFTypes.h"

@interface LSFLampGroup()

@property (nonatomic, readonly) lsf::LampGroup *lampGroup;

@end

@implementation LSFLampGroup

@synthesize lampGroup = _lampGroup;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.handle = new lsf::LampGroup();
    }
    
    return self;
}

-(void)setLamps: (NSArray *)lampIDs
{
    LSFStringList lids;
    
    for (NSString *lid in lampIDs)
    {
        lids.push_back([lid UTF8String]);
    }
    
    self.lampGroup->lamps = lids;
}

-(NSArray *)lamps
{
    LSFStringList lamps = self.lampGroup->lamps;
    NSMutableArray *lampIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = lamps.begin(); iter != lamps.end(); ++iter)
    {
        [lampIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    return lampIDsArray;
}

-(void)setLampGroups: (NSArray *)groupIDs
{
    LSFStringList gids;
    
    for (NSString *gid in groupIDs)
    {
        gids.push_back([gid UTF8String]);
    }
    
    self.lampGroup->lampGroups = gids;
}

-(NSArray *)lampGroups
{
    LSFStringList lampGroups = self.lampGroup->lampGroups;
    NSMutableArray *groupIDsArray = [[NSMutableArray alloc] init];
    
    for (std::list<LSFString>::const_iterator iter = lampGroups.begin(); iter != lampGroups.end(); ++iter)
    {
        [groupIDsArray addObject: [NSString stringWithUTF8String: (*iter).c_str()]];
    }
    
    return groupIDsArray;
}

-(LSFResponseCode)isDependentLampGroup: (NSString *)groupID
{
    std::string gid([groupID UTF8String]);
    return self.lampGroup->IsDependentLampGroup(gid);
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
- (lsf::LampGroup *)lampGroup
{
    return static_cast<lsf::LampGroup*>(self.handle);
}

@end
