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

#import "LSFLampParametersTests.h"
#import "LSFObjC/LSFLampParameters.h"

@interface LSFLampParametersTests()

@property (nonatomic) LSFLampParameters *lampParams;

@end

@implementation LSFLampParametersTests

@synthesize lampParams = _lampParams;

-(void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    self.lampParams = [[LSFLampParameters alloc] init];
}

-(void)tearDown
{
    self.lampParams = nil;
    
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testEnergyUsageMilliwatts
{
    int eum = self.lampParams.energyUsageMilliwatts;
    XCTAssertTrue((eum == 0), @"Energy Usage Milliwatts should be zero");
}

-(void)testLumens
{
    int lumens = self.lampParams.lumens;
    XCTAssertTrue((lumens == 0), @"Lumens should be zero");
}

@end
