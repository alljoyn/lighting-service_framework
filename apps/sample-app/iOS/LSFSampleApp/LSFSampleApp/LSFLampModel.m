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

#import "LSFLampModel.h"

@implementation LSFLampModel

@synthesize lampDetails = _lampDetails;
@synthesize lampParameters = _lampParameters;
@synthesize aboutData = _aboutData;

-(id)initWithLampID: (NSString *)lampID
{
    self = [super initWithID: lampID andName: @"[Loading lamp info...]"];
    
    if (self)
    {
        self.lampDetails = [[LSFLampDetails alloc] init];
        self.lampParameters = [[LSFLampParameters alloc] init];
        self.aboutData = [[LSFAboutData alloc] init];
    }
    
    return self;
}

@end
