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

#import "LSFStatePulseEffectTests.h"
#import "LSFObjC/LSFStatePulseEffect.h"

@interface LSFStatePulseEffectTests()

@property (nonatomic) LSFStatePulseEffect *statePulseEffect;

@end

@implementation LSFStatePulseEffectTests

@synthesize statePulseEffect = _statePulseEffect;

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

-(void)testStatePulseEffectWithoutFromState
{
    //Create data objects for LSFStatePulseEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    
    LSFLampState *initialToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 2700];
    
    unsigned int initialPeriod = 15;
    unsigned int initialDuration = 60;
    unsigned int initialNumPulses = 10;
    
    //Create LSFStatePulseEffect
    self.statePulseEffect = [[LSFStatePulseEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs toState: initialToState period: initialPeriod duration: initialDuration andNumPulses: initialNumPulses];
    
    //Test extracting the data
    unsigned int period = self.statePulseEffect.period;
    unsigned int duration = self.statePulseEffect.duration;
    unsigned int numPulses = self.statePulseEffect.numPulses;
    XCTAssertTrue(initialPeriod == period, @"Period should be equal to 15");
    XCTAssertTrue(initialDuration == duration, @"Duration should be equal to 60");
    XCTAssertTrue(initialNumPulses == numPulses, @"NumPulses should be equal to 10");
    
    NSArray *lampIDs = self.statePulseEffect.lamps;
    NSSet *initialLampsSet = [NSSet setWithArray: initialLampIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    NSArray *lampGroupIDs = self.statePulseEffect.lampGroups;
    NSSet *initialLampGroupsSet = [NSSet setWithArray: initialLampGroupIDs];
    NSSet *lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    LSFLampState *fromState = self.statePulseEffect.fromLampState;
    XCTAssertFalse(fromState.onOff, @"State should be on");
    XCTAssertTrue(fromState.brightness == 0, @"Brightness should be 0");
    XCTAssertTrue(fromState.hue == 0, @"Hue should be 0");
    XCTAssertTrue(fromState.saturation == 0, @"Saturation should be 0");
    XCTAssertTrue(fromState.colorTemp == 0, @"Color Temp should be 0");
    
    LSFLampState *toState = self.statePulseEffect.toLampState;
    XCTAssertTrue(toState.onOff, @"State should be on");
    XCTAssertTrue(toState.brightness == 100, @"Brightness should be 100");
    XCTAssertTrue(toState.hue == 360, @"Hue should be 360");
    XCTAssertTrue(toState.saturation == 100, @"Saturation should be 100");
    XCTAssertTrue(toState.colorTemp == 2700, @"Color Temp should be 2700");
}

-(void)testStatePulseEffectWithFromState
{
    //Create data objects for LSFStatePulseEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    
    LSFLampState *initialFromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 50 hue: 180 saturation: 50 colorTemp: 2700];
    LSFLampState *initialToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int initialPeriod = 15;
    unsigned int initialDuration = 60;
    unsigned int initialNumPulses = 10;
    
    //Create LSFStatePulseEffect
    self.statePulseEffect = [[LSFStatePulseEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs fromState: initialFromState toState: initialToState period: initialPeriod duration: initialDuration andNumPulses: initialNumPulses];
    
    //Test extracting the data
    unsigned int period = self.statePulseEffect.period;
    unsigned int duration = self.statePulseEffect.duration;
    unsigned int numPulses = self.statePulseEffect.numPulses;
    XCTAssertTrue(initialPeriod == period, @"Period should be equal to 15");
    XCTAssertTrue(initialDuration == duration, @"Duration should be equal to 60");
    XCTAssertTrue(initialNumPulses == numPulses, @"NumPulses should be equal to 10");
    
    NSArray *lampIDs = self.statePulseEffect.lamps;
    NSSet *initialLampsSet = [NSSet setWithArray: initialLampIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    NSArray *lampGroupIDs = self.statePulseEffect.lampGroups;
    NSSet *initialLampGroupsSet = [NSSet setWithArray: initialLampGroupIDs];
    NSSet *lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    LSFLampState *fromState = self.statePulseEffect.fromLampState;
    XCTAssertTrue(fromState.onOff, @"State should be on");
    XCTAssertTrue(fromState.brightness == 50, @"Brightness should be 0");
    XCTAssertTrue(fromState.hue == 180, @"Hue should be 0");
    XCTAssertTrue(fromState.saturation == 50, @"Saturation should be 0");
    XCTAssertTrue(fromState.colorTemp == 2700, @"Color Temp should be 0");
    
    LSFLampState *toState = self.statePulseEffect.toLampState;
    XCTAssertTrue(toState.onOff, @"State should be on");
    XCTAssertTrue(toState.brightness == 100, @"Brightness should be 100");
    XCTAssertTrue(toState.hue == 360, @"Hue should be 360");
    XCTAssertTrue(toState.saturation == 100, @"Saturation should be 100");
    XCTAssertTrue(toState.colorTemp == 9000, @"Color Temp should be 2700");
}

-(void)testSetGetLamps
{
    //Create data objects for LSFStatePulseEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    
    LSFLampState *initialFromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 50 hue: 180 saturation: 50 colorTemp: 2700];
    LSFLampState *initialToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int initialPeriod = 15;
    unsigned int initialDuration = 60;
    unsigned int initialNumPulses = 10;
    
    //Create LSFStatePulseEffect
    self.statePulseEffect = [[LSFStatePulseEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs fromState: initialFromState toState: initialToState period: initialPeriod duration: initialDuration andNumPulses: initialNumPulses];
    
    //Set lamps with new array
    NSString *lampID4 = @"lampID4";
    NSString *lampID5 = @"lampID5";
    NSArray *newLampIDs = [NSArray arrayWithObjects: lampID4, lampID5, nil];
    
    self.statePulseEffect.lamps = newLampIDs;
    
    //Get lamps
    NSArray *lampIDs = self.statePulseEffect.lamps;
    NSSet *initialLampsSet = [NSSet setWithArray: newLampIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testSetGetLampGroups
{
    //Create data objects for LSFStatePulseEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    
    LSFLampState *initialFromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 50 hue: 180 saturation: 50 colorTemp: 2700];
    LSFLampState *initialToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int initialPeriod = 15;
    unsigned int initialDuration = 60;
    unsigned int initialNumPulses = 10;
    
    //Create LSFStatePulseEffect
    self.statePulseEffect = [[LSFStatePulseEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs fromState: initialFromState toState: initialToState period: initialPeriod duration: initialDuration andNumPulses: initialNumPulses];
    
    //Set lamp groups with new array
    NSString *lampGroupID3 = @"lampGroupID3";
    NSArray *newLampGroupIDs = [NSArray arrayWithObjects: lampGroupID3, nil];
    
    self.statePulseEffect.lampGroups = newLampGroupIDs;
    
    //Get lamp groups
    NSArray *lampIDs = self.statePulseEffect.lampGroups;
    NSSet *initialLampsSet = [NSSet setWithArray: newLampGroupIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testSetGetPeriod
{
    //Create data objects for LSFStatePulseEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    
    LSFLampState *initialFromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 50 hue: 180 saturation: 50 colorTemp: 2700];
    LSFLampState *initialToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int initialPeriod = 15;
    unsigned int initialDuration = 60;
    unsigned int initialNumPulses = 10;
    
    //Create LSFStatePulseEffect
    self.statePulseEffect = [[LSFStatePulseEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs fromState: initialFromState toState: initialToState period: initialPeriod duration: initialDuration andNumPulses: initialNumPulses];
    
    //Set period
    self.statePulseEffect.period = 30;
    
    //Get period
    unsigned int newPeriod = self.statePulseEffect.period;
    XCTAssertTrue(newPeriod == 30, @"Period should be equal to 30");
}

-(void)testSetGetDuration
{
    //Create data objects for LSFStatePulseEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    
    LSFLampState *initialFromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 50 hue: 180 saturation: 50 colorTemp: 2700];
    LSFLampState *initialToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int initialPeriod = 15;
    unsigned int initialDuration = 60;
    unsigned int initialNumPulses = 10;
    
    //Create LSFStatePulseEffect
    self.statePulseEffect = [[LSFStatePulseEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs fromState: initialFromState toState: initialToState period: initialPeriod duration: initialDuration andNumPulses: initialNumPulses];
    
    //Set duration
    self.statePulseEffect.duration = 120;
    
    //Get duration
    unsigned int newDuration = self.statePulseEffect.duration;
    XCTAssertTrue(newDuration == 120, @"Duration should be equal to 120");
}

-(void)testSetGetNumPulses
{
    //Create data objects for LSFStatePulseEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    
    LSFLampState *initialFromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 50 hue: 180 saturation: 50 colorTemp: 2700];
    LSFLampState *initialToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int initialPeriod = 15;
    unsigned int initialDuration = 60;
    unsigned int initialNumPulses = 10;
    
    //Create LSFStatePulseEffect
    self.statePulseEffect = [[LSFStatePulseEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs fromState: initialFromState toState: initialToState period: initialPeriod duration: initialDuration andNumPulses: initialNumPulses];
    
    //Set num pulses
    self.statePulseEffect.numPulses = 20;
    
    //Get num pulses
    unsigned int newNumPulses = self.statePulseEffect.numPulses;
    XCTAssertTrue(newNumPulses == 20, @"Num Pulses should be equal to 20");
}

-(void)testSetGetFromLampState
{
    //Create data objects for LSFStatePulseEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    
    LSFLampState *initialFromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 50 hue: 180 saturation: 50 colorTemp: 2700];
    LSFLampState *initialToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int initialPeriod = 15;
    unsigned int initialDuration = 60;
    unsigned int initialNumPulses = 10;
    
    //Create LSFStatePulseEffect
    self.statePulseEffect = [[LSFStatePulseEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs fromState: initialFromState toState: initialToState period: initialPeriod duration: initialDuration andNumPulses: initialNumPulses];
    
    //Set from lamp state with new data
    LSFLampState *newFromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    self.statePulseEffect.fromLampState = newFromState;
    
    //Get from lamp state
    LSFLampState *fromState = self.statePulseEffect.fromLampState;
    XCTAssertTrue(fromState.onOff, @"State should be on");
    XCTAssertTrue(fromState.brightness == 100, @"Brightness should be 100");
    XCTAssertTrue(fromState.hue == 360, @"Hue should be 360");
    XCTAssertTrue(fromState.saturation == 100, @"Saturation should be 100");
    XCTAssertTrue(fromState.colorTemp == 9000, @"Color Temp should be 2700");
}

-(void)testSetGetToLampState
{
    //Create data objects for LSFStatePulseEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSArray *initialLampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *initialLampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    
    LSFLampState *initialFromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 50 hue: 180 saturation: 50 colorTemp: 2700];
    LSFLampState *initialToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int initialPeriod = 15;
    unsigned int initialDuration = 60;
    unsigned int initialNumPulses = 10;
    
    //Create LSFStatePulseEffect
    self.statePulseEffect = [[LSFStatePulseEffect alloc] initWithLampIDs: initialLampIDs lampGroupIDs: initialLampGroupIDs fromState: initialFromState toState: initialToState period: initialPeriod duration: initialDuration andNumPulses: initialNumPulses];
    
    //Set to lamp state with new data
    LSFLampState *newToState = [[LSFLampState alloc] initWithOnOff: NO brightness: 50 hue: 180 saturation: 50 colorTemp: 2700];
    self.statePulseEffect.toLampState = newToState;
    
    //Get to lamp state
    LSFLampState *toState = self.statePulseEffect.toLampState;
    XCTAssertFalse(toState.onOff, @"State should be on");
    XCTAssertTrue(toState.brightness == 50, @"Brightness should be 50");
    XCTAssertTrue(toState.hue == 180, @"Hue should be 180");
    XCTAssertTrue(toState.saturation == 50, @"Saturation should be 50");
    XCTAssertTrue(toState.colorTemp == 2700, @"Color Temp should be 2700");
}

@end
