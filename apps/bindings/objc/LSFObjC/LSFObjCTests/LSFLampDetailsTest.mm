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

#import "LSFLampDetailsTest.h"
#import "LSFObjC/LSFLampDetails.h"
#import "LampValues.h"
#import "LSFTypes.h"

@interface LSFLampDetailsTest()

@property (nonatomic) LSFLampDetails *lampDetails;

@end

@implementation LSFLampDetailsTest

@synthesize lampDetails = _lampDetails;

-(void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    lsf::LampDetails *ld = new lsf::LampDetails();
    ld->make = MAKE_LIFX;
    ld->model = MODEL_LED;
    ld->type = TYPE_LAMP;
    ld->lampType = LAMPTYPE_A15;
    ld->lampBaseType = BASETYPE_E5;
    ld->lampBeamAngle = 100;
    ld->dimmable = true;
    ld->color = true;
    ld->variableColorTemp = false;
    ld->hasEffects = false;
    ld->maxVoltage = 120;
    ld->minVoltage = 120;
    ld->wattage = 100;
    ld->incandescentEquivalent = 9;
    ld->maxLumens = 100;
    ld->minTemperature = 2400;
    ld->maxTemperature = 5000;
    ld->colorRenderingIndex = 93;
    
    NSString *lampID = @"LampID1";
    std::string lid([lampID UTF8String]);
    
    ld->lampID = lid;
    
    self.lampDetails = [[LSFLampDetails alloc] initWithHandle: (LSFHandle)ld];
}

-(void)tearDown
{
    self.lampDetails = nil;
    
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testGetMake
{
    LampMake make = self.lampDetails.lampMake;
    XCTAssertTrue((MAKE_LIFX == make), @"Make should be equal to MAKE_LIFX");
}

-(void)testGetModel
{
    LampModel model = self.lampDetails.lampModel;
    XCTAssertTrue((MODEL_LED == model), @"Model should be equal to MODEL_LED");
}

-(void)testGetDeviceType
{
    DeviceType dt = self.lampDetails.deviceType;
    XCTAssertTrue((TYPE_LAMP == dt), @"Device type should be equal to TYPE_LAMP");
}

-(void)testGetLampType
{
    LampType lt = self.lampDetails.lampType;
    XCTAssertTrue((LAMPTYPE_A15 == lt), @"Lamp type should be equal to LAMPTYPE_A15");
}

-(void)testGetLampBaseType
{
    BaseType bt = self.lampDetails.baseType;
    XCTAssertTrue((BASETYPE_E5 == bt), @"Base type should be equal to BASETYPE_E5");
}

-(void)testGetLampBeamAngle
{
    unsigned int lba = self.lampDetails.lampBeamAngle;
    XCTAssertTrue((lba == 100), @"Lamp Beam Angle should be equal to 100");
}

-(void)testGetDimmable
{
    BOOL dimmable = self.lampDetails.dimmable;
    XCTAssertTrue(dimmable, @"Dimmable should be true");
}

-(void)testGetColor
{
    BOOL color = self.lampDetails.color;
    XCTAssertTrue(color, @"Color should be true");
}

-(void)testGetVariableColorTemp
{
    BOOL vct = self.lampDetails.variableColorTemp;
    XCTAssertFalse(vct, @"Variable color temp should be false");
}

-(void)testGetHasEffects
{
    BOOL hasEffects = self.lampDetails.hasEffects;
    XCTAssertFalse(hasEffects, @"Has Effects should be false");
}

-(void)testGetMaxVoltage
{
    unsigned int mv = self.lampDetails.maxVoltage;
    XCTAssertTrue((mv == 120), @"MaxVoltage should be equal to 120");
}

-(void)testGetMinVoltage
{
    unsigned int mv = self.lampDetails.minVoltage;
    XCTAssertTrue((mv == 120), @"MinVoltage should be equal to 120");
}

-(void)testGetWattage
{
    unsigned int wattage = self.lampDetails.wattage;
    XCTAssertTrue((wattage == 100), @"Wattage should be equal to 100");
}

-(void)testGetIncandescentEquivalent
{
    unsigned int ie = self.lampDetails.incandescentEquivalent;
    XCTAssertTrue((ie == 9), @"Incandescent Equivalent should be equal to 9");
}

-(void)testGetMaxLumens
{
    unsigned int ml = self.lampDetails.maxLumens;
    XCTAssertTrue((ml == 100), @"MaxLumens should be equal to 100");
}

-(void)testGetMaxTemp
{
    unsigned int mt = self.lampDetails.maxTemperature;
    XCTAssertTrue((mt == 5000), @"MaxTemp should be equal to 5000");
}

-(void)testGetMinTemp
{
    unsigned int mt = self.lampDetails.minTemperature;
    XCTAssertTrue((mt == 2400), @"MinTemp should be equal to 2400");
}

-(void)testGetColorRenderingIndex
{
    unsigned int cri = self.lampDetails.colorRenderingIndex;
    XCTAssertTrue((cri == 93), @"Color Rendering Index should be equal to 93");
}

-(void)testGetLampID
{
    NSString *lampID = self.lampDetails.lampID;
    XCTAssertTrue([lampID isEqualToString: @"LampID1"], @"Lamp ID should be equal to LampID1");
}

@end
