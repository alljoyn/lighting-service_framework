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

#import "LSFObject.h"
#import "LampValues.h"

@interface LSFLampDetails : LSFObject

@property (nonatomic) LampMake lampMake;
@property (nonatomic) LampModel lampModel;
@property (nonatomic) DeviceType deviceType;
@property (nonatomic) LampType lampType;
@property (nonatomic) BaseType baseType;
@property (nonatomic) unsigned int lampBeamAngle;
@property (nonatomic) BOOL dimmable;
@property (nonatomic) BOOL color;
@property (nonatomic) BOOL variableColorTemp;
@property (nonatomic) BOOL hasEffects;
@property (nonatomic) unsigned int maxVoltage;
@property (nonatomic) unsigned int minVoltage;
@property (nonatomic) unsigned int wattage;
@property (nonatomic) unsigned int incandescentEquivalent;
@property (nonatomic) unsigned int maxLumens;
@property (nonatomic) unsigned int minTemperature;
@property (nonatomic) unsigned int maxTemperature;
@property (nonatomic) unsigned int colorRenderingIndex;
@property (nonatomic, strong) NSString *lampID;

-(id)init;

@end
