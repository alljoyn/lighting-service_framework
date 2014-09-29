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

#import "LSFSceneElementDataModel.h"

static int nextID = 1;

@implementation LSFSceneElementDataModel

@synthesize type = _type;
@synthesize members = _members;

-(id)initWithEffectType: (EffectType)type andName: (NSString *)name
{
    self = [super initWithID: [NSString stringWithFormat: @"%i", nextID++] andName: name];

    if (self)
    {
        self.type = type;
        self.members = [[LSFLampGroup alloc] init];

        // Lamp state should always be set to "ON". Turning the lamp off as part
        // of an effect requires setting the brightness to zero.
        self.state.onOff = YES;

        self.capability.dimmable = ALL;
        self.capability.color = ALL;
        self.capability.temp = ALL;
    }

    return self;
}

@end
