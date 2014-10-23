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

#import "LSFColorAverager.h"

@interface LSFColorAverager()

@property (nonatomic) long long sum;
@property (nonatomic) long long count;
@property (nonatomic) long long initial;
@property (nonatomic) BOOL uniform;

@end

@implementation LSFColorAverager

@synthesize sum = _sum;
@synthesize count = _count;
@synthesize initial = _initial;
@synthesize uniform = _uniform;

-(id)init
{
    self = [super init];

    if (self)
    {
        [self reset];
    }

    return self;
}

-(void)reset
{
    self.sum = 0;
    self.count = 0;
    self.initial = -1;
    self.uniform = YES;
}

-(void)add: (long long)value
{
    self.sum += value;
    self.count++;

    if (self.initial < 0)
    {
        self.initial = value;
    }
    else if (self.uniform)
    {
        self.uniform = (value == self.initial);
    }
}

-(long long)getSum
{
    return self.sum;
}

-(long long)getCount
{
    return self.count;
}

-(long long)getAverage
{
    double divisor = self.count > 0 ? self.count : 1.0;
    return round((double)self.sum / divisor);
}

-(BOOL)isUniform
{
    return self.uniform;
}

@end
