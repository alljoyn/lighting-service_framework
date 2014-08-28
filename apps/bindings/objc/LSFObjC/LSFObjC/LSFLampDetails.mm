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

#import "LSFLampDetails.h"
#import "LSFTypes.h"

@interface LSFLampDetails()

@property (nonatomic, readonly) lsf::LampDetails *lampDetails;

//-(void)populateWithData;

@end

@implementation LSFLampDetails

@synthesize lampDetails = _lampDetails;

-(id)init
{
    self = [super init];
    
    if (self)
    {
        self.handle = new lsf::LampDetails();
        
        //TODO - remove
        //[self populateWithData];
    }
    
    return self;
}

-(void)setLampMake: (LampMake)lampMake
{
    self.lampDetails->make = lampMake;
}

-(LampMake)lampMake
{
    return self.lampDetails->make;
}

-(void)setLampModel: (LampModel)lampModel
{
    self.lampDetails->model = lampModel;
}

-(LampModel)lampModel
{
    return self.lampDetails->model;
}

-(void)setDeviceType: (DeviceType)deviceType
{
    self.lampDetails->type = deviceType;
}

-(DeviceType)deviceType
{
    return self.lampDetails->type;
}

-(void)setLampType: (LampType)lampType
{
    self.lampDetails->lampType = lampType;
}

-(LampType)lampType
{
    return self.lampDetails->lampType;
}

-(void)setBaseType: (BaseType)baseType
{
    self.lampDetails->lampBaseType = baseType;
}

-(BaseType)baseType
{
    return self.lampDetails->lampBaseType;
}

-(void)setLampBeamAngle: (unsigned int)lampBeamAngle
{
    self.lampDetails->lampBeamAngle = lampBeamAngle;
}

-(unsigned int)lampBeamAngle
{
    return self.lampDetails->lampBeamAngle;
}

-(void)setDimmable: (BOOL)dimmable
{
    if (dimmable)
    {
        self.lampDetails->dimmable = true;
    }
    else
    {
        self.lampDetails->dimmable = false;
    }
}

-(BOOL)dimmable
{
    bool dimmable = self.lampDetails->dimmable;
    
    if (dimmable)
    {
        return YES;
    }
    else
    {
        return NO;
    }
}

-(void)setColor: (BOOL)color
{
    if (color)
    {
        self.lampDetails->color = true;
    }
    else
    {
        self.lampDetails->color = false;
    }
}

-(BOOL)color
{
    bool color = self.lampDetails->color;
    
    if (color)
    {
        return YES;
    }
    else
    {
        return NO;
    }
}

-(void)setVariableColorTemp: (BOOL)variableColorTemp
{
    if (variableColorTemp)
    {
        self.lampDetails->variableColorTemp = true;
    }
    else
    {
        self.lampDetails->variableColorTemp = false;
    }
}

-(BOOL)variableColorTemp
{
    bool vct = self.lampDetails->variableColorTemp;
    
    if (vct)
    {
        return YES;
    }
    else
    {
        return NO;
    }
}

-(void)setHasEffects: (BOOL)hasEffects
{
    if (hasEffects)
    {
        self.lampDetails->hasEffects = true;
    }
    else
    {
        self.lampDetails->hasEffects = false;
    }
}

-(BOOL)hasEffects
{
    bool hasEffects = self.lampDetails->hasEffects;
    
    if (hasEffects)
    {
        return YES;
    }
    else
    {
        return NO;
    }
}

-(void)setMaxVoltage: (unsigned int)maxVoltage
{
    self.lampDetails->maxVoltage = maxVoltage;
}

-(unsigned int)maxVoltage
{
    return self.lampDetails->maxVoltage;
}

-(void)setMinVoltage: (unsigned int)minVoltage
{
    self.lampDetails->minVoltage = minVoltage;
}

-(unsigned int)minVoltage
{
    return self.lampDetails->minVoltage;
}

-(void)setWattage: (unsigned int)wattage
{
    self.lampDetails->wattage = wattage;
}

-(unsigned int)wattage
{
    return self.lampDetails->wattage;
}

-(void)setIncandescentEquivalent: (unsigned int)incandescentEquivalent
{
    self.lampDetails->incandescentEquivalent = incandescentEquivalent;
}

-(unsigned int)incandescentEquivalent
{
    return self.lampDetails->incandescentEquivalent;
}

-(void)setMaxLumens: (unsigned int)maxLumens
{
    self.lampDetails->maxLumens = maxLumens;
}

-(unsigned int)maxLumens
{
    return self.lampDetails->maxLumens;
}

-(void)setMinTemperature: (unsigned int)minTemperature
{
    self.lampDetails->minTemperature = minTemperature;
}

-(unsigned int)minTemperature
{
    return self.lampDetails->minTemperature;
}

-(void)setMaxTemperature: (unsigned int)maxTemperature
{
    self.lampDetails->maxTemperature = maxTemperature;
}

-(unsigned int)maxTemperature
{
    return self.lampDetails->maxTemperature;
}

-(void)setColorRenderingIndex: (unsigned int)colorRenderingIndex
{
    self.lampDetails->colorRenderingIndex = colorRenderingIndex;
}

-(unsigned int)colorRenderingIndex
{
    return self.lampDetails->colorRenderingIndex;
}

-(void)setLampID: (NSString *)lampID
{
    std::string lid([lampID UTF8String]);
    self.lampDetails->lampID = lid;
}

-(NSString *)lampID
{
    return [NSString stringWithUTF8String: (self.lampDetails->lampID).c_str()];
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
- (lsf::LampDetails *)lampDetails
{
    return static_cast<lsf::LampDetails*>(self.handle);
}

/*
 * Private function
 */
//-(void)populateWithData
//{
//    self.lampDetails->make = MAKE_LIFX;
//    self.lampDetails->model = MODEL_LED;
//    self.lampDetails->type = TYPE_LAMP;
//    self.lampDetails->lampType = LAMPTYPE_PAR20;
//    self.lampDetails->lampBaseType = BASETYPE_E11;
//    self.lampDetails->lampBeamAngle = 100;
//    self.lampDetails->dimmable = true;
//    self.lampDetails->color = true;
//    self.lampDetails->variableColorTemp = true;
//    self.lampDetails->hasEffects = true;
//    self.lampDetails->maxVoltage = 120;
//    self.lampDetails->minVoltage = 120;
//    self.lampDetails->wattage = 60;
//    self.lampDetails->incandescentEquivalent = 9;
//    self.lampDetails->maxLumens = 600;
//    self.lampDetails->minTemperature = 2300;
//    self.lampDetails->maxTemperature = 5000;
//    self.lampDetails->colorRenderingIndex = 93;    
//}

@end
