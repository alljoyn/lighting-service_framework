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

#import "LSFLampState.h"
#import "LSFTypes.h"

@interface LSFLampState()

@property (nonatomic, readonly) lsf::LampState *lampState;

@end

@implementation LSFLampState

@synthesize lampState = _lampState;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.handle = new lsf::LampState();
    }
    
    return self;
}

-(id)initWithOnOff: (BOOL)onOff brightness: (unsigned int)brightness hue: (unsigned int)hue saturation: (unsigned int)saturation colorTemp: (unsigned int)colorTemp;
{
    self = [super init];
    
    if (self)
    {
        self.handle = new lsf::LampState(onOff, hue, saturation, colorTemp, brightness);
    }
    
    return self;
}

-(void)setOnOff: (BOOL)onOff
{
    self.lampState->onOff = onOff;
}

-(BOOL)onOff
{
    bool onOff = self.lampState->onOff;
    
    if (onOff)
    {
        return YES;
    }
    else
    {
        return NO;
    }
}

-(void)setBrightness: (unsigned int)brightness
{
    self.lampState->brightness = brightness;
}

-(unsigned int)brightness
{
    return self.lampState->brightness;
}

-(void)setHue: (unsigned int)hue
{
    self.lampState->hue = hue;
}

-(unsigned int)hue
{
    return self.lampState->hue;
}

-(void)setSaturation: (unsigned int)saturation
{
    self.lampState->saturation = saturation;
}

-(unsigned int)saturation
{
    return self.lampState->saturation;
}

-(void)setColorTemp: (unsigned int)colorTemp
{
    self.lampState->colorTemp = colorTemp;
}

-(unsigned int)colorTemp
{
    return self.lampState->colorTemp;
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
- (lsf::LampState *)lampState
{
    return static_cast<lsf::LampState*>(self.handle);
}

@end
