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

#import "LSFPresetTransitionEffectTests.h"
#import "LSFObjC/LSFPresetTransitionEffect.h"

@interface LSFPresetTransitionEffectTests()

@property (nonatomic) LSFPresetTransitionEffect *presetTransitionEffect;

@end

@implementation LSFPresetTransitionEffectTests

@synthesize presetTransitionEffect = _presetTransitionEffect;

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testStateTransitionEffectWithoutTransitionPeriod
{
    //Create data objects for LSFPresetTransitionEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    
    NSString *initialPresetID = @"presetID1";
    
    //Create LSFPresetTransitionEffect
    self.presetTransitionEffect = [[LSFPresetTransitionEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs andPresetID: initialPresetID];
    
    //Test extracting the data
    unsigned int transtionPeriod = self.presetTransitionEffect.transitionPeriod;
    XCTAssertTrue(transtionPeriod == 0, @"Transition period should be zero");
    
    NSArray *lampIDs = self.presetTransitionEffect.lamps;
    NSSet *initialLampsSet = [NSSet setWithArray: initialLampIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    NSArray *lampGroupIDs = self.presetTransitionEffect.lampGroups;
    NSSet *initialLampGroupsSet = [NSSet setWithArray: initialLampGroupIDs];
    NSSet *lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    NSString *presetID = self.presetTransitionEffect.presetID;
    XCTAssertTrue([presetID isEqualToString: initialPresetID], @"Preset IDs should be equal");
}

-(void)testStateTransitionEffectWithTransitionPeriod
{
    //Create data objects for LSFPresetTransitionEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID2, nil];
    
    NSString *initialPresetID = @"presetID1";
    
    unsigned int initialTransitionPeriod = 30;
    
    //Create LSFPresetTransitionEffect
    self.presetTransitionEffect = [[LSFPresetTransitionEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs presetID: initialPresetID andTransitionPeriod: initialTransitionPeriod];
    
    //Test extracting the data
    unsigned int transtionPeriod = self.presetTransitionEffect.transitionPeriod;
    XCTAssertTrue(transtionPeriod == 30, @"Transition period should be 30");
    
    NSArray *lampIDs = self.presetTransitionEffect.lamps;
    NSSet *initialLampsSet = [NSSet setWithArray: initialLampIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    NSArray *lampGroupIDs = self.presetTransitionEffect.lampGroups;
    NSSet *initialLampGroupsSet = [NSSet setWithArray: initialLampGroupIDs];
    NSSet *lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    NSString *presetID = self.presetTransitionEffect.presetID;
    XCTAssertTrue([presetID isEqualToString: initialPresetID], @"Preset IDs should be equal");
}

-(void)testSetGetLamps
{
    //Create data objects for LSFPresetTransitionEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID2, nil];
    
    NSString *initialPresetID = @"presetID1";
    
    unsigned int initialTransitionPeriod = 30;
    
    //Create LSFPresetTransitionEffect
    self.presetTransitionEffect = [[LSFPresetTransitionEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs presetID: initialPresetID andTransitionPeriod: initialTransitionPeriod];
    
    //Set lamps with new array
    NSString *lampID3 = @"lampID3";
    NSString *lampID4 = @"lampID4";
    NSArray *newLampIDs = [NSArray arrayWithObjects: lampID3, lampID4, nil];
    
    self.presetTransitionEffect.lamps = newLampIDs;
    
    //Get lamps
    NSArray *lampIDs = self.presetTransitionEffect.lamps;
    NSSet *initialLampsSet = [NSSet setWithArray: newLampIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testSetGetLampGroups
{
    //Create data objects for LSFPresetTransitionEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, nil];
    
    NSString *initialPresetID = @"presetID1";
    
    unsigned int initialTransitionPeriod = 30;
    
    //Create LSFPresetTransitionEffect
    self.presetTransitionEffect = [[LSFPresetTransitionEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs presetID: initialPresetID andTransitionPeriod: initialTransitionPeriod];
    
    //Set lamp groups with new array
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSArray *newLampGroupIDs = [NSArray arrayWithObjects: lampGroupID2, lampGroupID3, nil];
    
    self.presetTransitionEffect.lampGroups = newLampGroupIDs;
    
    //Get lamp groups
    NSArray *lampIDs = self.presetTransitionEffect.lampGroups;
    NSSet *initialLampsSet = [NSSet setWithArray: newLampGroupIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testSetGetTransitionPeriod
{
    //Create data objects for LSFPresetTransitionEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, nil];
    
    NSString *initialPresetID = @"presetID1";
    
    unsigned int initialTransitionPeriod = 30;
    
    //Create LSFPresetTransitionEffect
    self.presetTransitionEffect = [[LSFPresetTransitionEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs presetID: initialPresetID andTransitionPeriod: initialTransitionPeriod];
    
    //Set transition period
    self.presetTransitionEffect.transitionPeriod = 15;
    
    //Get transition period
    unsigned int newTransitionPeriod = self.presetTransitionEffect.transitionPeriod;
    XCTAssertTrue(newTransitionPeriod == 15, @"Transition period should be 15");
}

-(void)testSetGetPresetID
{
    //Create data objects for LSFPresetTransitionEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, nil];
    
    NSString *initialPresetID = @"presetID1";
    
    unsigned int initialTransitionPeriod = 30;
    
    //Create LSFPresetTransitionEffect
    self.presetTransitionEffect = [[LSFPresetTransitionEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs presetID: initialPresetID andTransitionPeriod: initialTransitionPeriod];
    
    //Set preset ID
    NSString *newPresetID = @"newPresetID";
    self.presetTransitionEffect.presetID = newPresetID;
    
    //Get preset ID
    NSString *presetID = self.presetTransitionEffect.presetID;
    XCTAssertTrue([presetID isEqualToString: newPresetID], @"Preset IDs should be equal");
}

@end
