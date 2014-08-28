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

#import "LSFLampGroupTests.h"
#import "LSFObjC/LSFLampGroup.h"

@interface LSFLampGroupTests()

@property (nonatomic) LSFLampGroup *lampGroup;

@end

@implementation LSFLampGroupTests

@synthesize lampGroup = _lampGroup;

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    self.lampGroup = [[LSFLampGroup alloc] init];
}

- (void)tearDown
{
    self.lampGroup = nil;
    
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testLamps
{
    //Test lamp ID set
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    
    NSArray *setLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    [self.lampGroup setLamps: setLampIDs];
    
    NSArray *getlampIDs = [self.lampGroup lamps];
   
    NSSet *startData = [[NSSet alloc] initWithArray: setLampIDs];
    NSSet *endData = [[NSSet alloc] initWithArray: getlampIDs];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGroups
{
    //Test lamp group ID set
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    
    NSArray *setLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    [self.lampGroup setLampGroups: setLampGroupIDs];
    
    NSArray *getlampGroupIDs = [self.lampGroup lampGroups];
    
    NSSet *startData = [[NSSet alloc] initWithArray: setLampGroupIDs];
    NSSet *endData = [[NSSet alloc] initWithArray: getlampGroupIDs];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testIsDependentGroup;
{
    //Test lamp group ID set
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *nonDependentGroupID = @"nonDependentGroupID";
    
    NSArray *setLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    [self.lampGroup setLampGroups: setLampGroupIDs];
    
    LSFResponseCode code;
    code = [self.lampGroup isDependentLampGroup: lampGroupID1];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Response code should be equal to LSF_ERR_DEPENDENCY");
    
    code = [self.lampGroup isDependentLampGroup: lampGroupID2];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Response code should be equal to LSF_ERR_DEPENDENCY");
    
    code = [self.lampGroup isDependentLampGroup: nonDependentGroupID];
    XCTAssertTrue(code == LSF_OK, @"Response code should be equal to LSF_OK");
}

@end
