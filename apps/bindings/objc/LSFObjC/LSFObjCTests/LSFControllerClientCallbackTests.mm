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

#import "LSFControllerClientCallbackTests.h"
#import "MockControllerClientCallbackDelegateHandler.h"
#import "LSFObjC/LSFControllerClientCallback.h"

@interface LSFControllerClientCallbackTests()

@property (nonatomic) MockControllerClientCallbackDelegateHandler *cccdh;
@property (nonatomic) LSFControllerClientCallback *ccc;
@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation LSFControllerClientCallbackTests

@synthesize cccdh = _cccdh;
@synthesize ccc = _ccc;
@synthesize dataArray = _dataArray;

-(void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    self.cccdh = [[MockControllerClientCallbackDelegateHandler alloc] init];
    self.ccc = new LSFControllerClientCallback(self.cccdh);
    self.dataArray = [[NSMutableArray alloc] init];
}

-(void)tearDown
{
    delete self.ccc;
    self.cccdh = nil;
    
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testConnectedToControllerService
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"connectedToControllerService";
    NSString *controllerServiceID = @"TestContollerServiceID";
    NSString *controllerServiceName = @"TestControllerServiceName";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: controllerServiceID];
    [self.dataArray addObject: controllerServiceName];
    
    //Call callback method
    std::string csID([controllerServiceID UTF8String]);
    std::string csName([controllerServiceName UTF8String]);
    self.ccc->ConnectedToControllerServiceCB(csID, csName);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.cccdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testConnectToControllerServiceFailed
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"connectToControllerServiceFailed";
    NSString *controllerServiceID = @"TestContollerServiceID";
    NSString *controllerServiceName = @"TestControllerServiceName";
    
    [self.dataArray addObject: controllerServiceName];
    [self.dataArray addObject: controllerServiceID];
    [self.dataArray addObject: functionName];
    
    //Call callback method
    std::string csID([controllerServiceID UTF8String]);
    std::string csName([controllerServiceName UTF8String]);
    self.ccc->ConnectToControllerServiceFailedCB(csID, csName);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.cccdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testDisconnectedFromControllerService
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"disconnectedFromControllerService";
    NSString *controllerServiceID = @"TestContollerServiceID";
    NSString *controllerServiceName = @"TestControllerServiceName";
    
    [self.dataArray addObject: controllerServiceName];
    [self.dataArray addObject: controllerServiceID];
    [self.dataArray addObject: functionName];
    
    //Call callback method
    std::string csID([controllerServiceID UTF8String]);
    std::string csName([controllerServiceName UTF8String]);
    self.ccc->DisconnectedFromControllerServiceCB(csID, csName);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.cccdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testControllerClientError
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"controllerClientError";
    NSNumber *num1 = [[NSNumber alloc] initWithInt: ERROR_NONE];
    NSNumber *num2 = [[NSNumber alloc] initWithInt: ERROR_NO_ACTIVE_CONTROLLER_SERVICE_FOUND];
    NSNumber *num3 = [[NSNumber alloc] initWithInt: ERROR_REGISTERING_SIGNAL_HANDLERS];
    NSNumber *num4 = [[NSNumber alloc] initWithInt: ERROR_ALLJOYN_METHOD_CALL_TIMEOUT];
    NSArray *errorCodeArray = [[NSArray alloc] initWithObjects: num1, num2, num3, num4, nil];
    
    [self.dataArray addObject: errorCodeArray];
    [self.dataArray addObject: functionName];
    
    //Call callback method
    ErrorCodeList errorCodeList;
    errorCodeList.push_back(ERROR_NONE);
    errorCodeList.push_back(ERROR_NO_ACTIVE_CONTROLLER_SERVICE_FOUND);
    errorCodeList.push_back(ERROR_REGISTERING_SIGNAL_HANDLERS);
    errorCodeList.push_back(ERROR_ALLJOYN_METHOD_CALL_TIMEOUT);
    self.ccc->ControllerClientErrorCB(errorCodeList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.cccdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

@end
