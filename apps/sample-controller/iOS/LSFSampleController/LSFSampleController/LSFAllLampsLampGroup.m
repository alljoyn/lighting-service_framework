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

#import "LSFAllLampsLampGroup.h"
#import "LSFLampModelContainer.h"

@implementation LSFAllLampsLampGroup

-(void)setLamps: (NSArray *)lamps
{
    NSLog(@"LSFAllLampsLampGroup - Invalid attempt to set lamp members of the all-lamps lamp group");
}

-(NSArray *)lamps
{
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    return [container.lampContainer allKeys];
}

-(void)setLampGroups: (NSArray *)lampGroups
{
    NSLog(@"LSFAllLampsLampGroup - Invalid attempt to set group members of the all-lamps lamp group");
}

-(NSArray *)lampGroups
{
    return [[NSArray alloc] init];
}

-(LSFResponseCode)isDependentLampGroup: (NSString *)groupID
{
    return [super isDependentLampGroup: groupID];
}

@end
