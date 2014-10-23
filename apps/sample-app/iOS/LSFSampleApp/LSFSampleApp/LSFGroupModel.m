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

#import "LSFGroupModel.h"

@interface LSFGroupModel()

@end

@implementation LSFGroupModel

@synthesize members = _members;
@synthesize lamps = _lamps;
@synthesize groups = _groups;
@synthesize duplicates = _duplicates;
@synthesize delay = _delay;

-(id)initWithGroupID: (NSString *)groupID
{
    self = [super initWithID: groupID andName: @"[Loading group info...]"];
    
    if (self)
    {
        self.members = [[LSFLampGroup alloc] init];
    }
    
    return self;
}

-(void)setLamps: (NSSet *)lamps
{
    _lamps = lamps;
    self.capability = [[LSFCapabilityData alloc] init];
}

-(NSSet *)lamps
{
    return _lamps;
}

-(void)setGroups: (NSSet *)groups
{
    _groups = groups;
    self.capability = [[LSFCapabilityData alloc] init];
}

-(NSSet *)groups
{
    return _groups;
}

@end
