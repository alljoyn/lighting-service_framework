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

#import "LSFControllerServiceManagerCallbackTests.h"
#import "MockControllerServiceManagerCallbackDelegateHandler.h"
#import "LSFObjC/LSFControllerServiceManagerCallback.h"

@interface LSFControllerServiceManagerCallbackTests()

@property (nonatomic) MockControllerServiceManagerCallbackDelegateHandler *csmcdh;
@property (nonatomic) LSFControllerServiceManagerCallback *csmc;
@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation LSFControllerServiceManagerCallbackTests

@synthesize csmcdh = _csmcdh;
@synthesize csmc = _csmc;
@synthesize dataArray = _dataArray;

-(void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    self.csmcdh = [[MockControllerServiceManagerCallbackDelegateHandler alloc] init];
    self.csmc = new LSFControllerServiceManagerCallback(self.csmcdh);
    self.dataArray = [[NSMutableArray alloc] init];
}

-(void)tearDown
{
    delete self.csmc;
    self.csmcdh = nil;
    
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testGetControllerServiceVersion
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    unsigned int version = 75;
    NSNumber *csVersion = [[NSNumber alloc] initWithUnsignedInt: version];
    NSString *functionName = @"getControllerServiceVersion";
    
    [self.dataArray addObject: csVersion];
    [self.dataArray addObject: functionName];
    
    //Call method
    self.csmc->GetControllerServiceVersionReplyCB(version);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.csmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testLightingResetControllerService
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"lightingResetControllerService";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    
    //Call method
    self.csmc->LightingResetControllerServiceReplyCB(code);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.csmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testControllerServiceLightingReset
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"controllerServiceLightingReset";
    [self.dataArray addObject: functionName];
    
    //Call method
    self.csmc->ControllerServiceLightingResetCB();
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.csmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testControllerServiceNameChanged
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *controllerServiceID = @"ControllerServiceID1";
    NSString *controllerServiceName = @"ControllerServiceName";
    NSString *functionName = @"controllerServiceNameChanged";
    [self.dataArray addObject: controllerServiceID];
    [self.dataArray addObject: controllerServiceName];
    [self.dataArray addObject: functionName];

    //Call callback method
    std::string csID([controllerServiceID UTF8String]);
    std::string csName([controllerServiceName UTF8String]);
    self.csmc->ControllerServiceNameChangedCB(csID, csName);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.csmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

@end
