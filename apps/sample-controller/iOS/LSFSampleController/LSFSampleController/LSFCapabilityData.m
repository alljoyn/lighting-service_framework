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

#import "LSFCapabilityData.h"

@interface LSFCapabilityData()

-(int)combineA: (int)a withB: (int)b;

@end

@implementation LSFCapabilityData

@synthesize dimmable = _dimmable;
@synthesize color = _color;
@synthesize temp = _temp;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.dimmable = UNSET;
        self.color = UNSET;
        self.temp = UNSET;
    }
    
    return self;
}

-(id)initWithDimmable: (BOOL)isDimmable color: (BOOL)hasColor andTemp: (BOOL)hasTemp
{
    self = [super init];
    
    if (self)
    {
        self.dimmable = isDimmable ? ALL : NONE;
        self.color = hasColor ? ALL : NONE;
        self.temp = hasTemp ? ALL : NONE;
    }
    
    return self;
}

-(void)includeData: (LSFCapabilityData *)data
{
    if (data != nil)
    {
        self.dimmable = [self combineA: self.dimmable withB: data.dimmable];
        self.color = [self combineA: self.color withB: data.color];
        self.temp = [self combineA: self.temp withB: data.temp];
    }
}

-(BOOL)isMixed
{
    return ((self.dimmable == SOME) || (self.color == SOME) || (self.temp == SOME));
}

-(int)combineA: (int)a withB: (int)b
{
    switch (a)
    {
        case UNSET:
            //No data in a, use b
            return b;
        case NONE:
            //None if b is none or unset, otherwise some
            return (b == NONE || b == UNSET) ? NONE : SOME;
        case SOME:
            return SOME;
        case ALL:
            //all if b is all or unset, otherwise some
            return (b == ALL || b == UNSET) ? ALL : SOME;
        default:
            return UNSET;
    }
}

-(NSString *)print
{
    return [NSString stringWithFormat: @"[dimmable: %i color: %i temp: %i]", self.dimmable, self.color, self.temp];
}

@end
