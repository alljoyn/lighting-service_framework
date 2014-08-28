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

#import "LSFSceneTests.h"
#import "LSFObjC/LSFScene.h"

@interface LSFSceneTests()

@property (nonatomic, strong) LSFScene *scene;

@end

@implementation LSFSceneTests

@synthesize scene = _scene;

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    self.scene = [[LSFScene alloc] init];
}

- (void)tearDown
{
    self.scene = nil;
    
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testSetGetTransitionToStateComponent
{
    //Create LSFStateTransitionEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSArray *ste1LampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSArray *ste1LampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, nil];
    
    LSFLampState *ste1State = [[LSFLampState alloc] initWithOnOff: NO brightness: 50 hue: 180 saturation: 25 colorTemp: 2700];
    
    unsigned int ste1TransitionPeriod = 30;
    
    LSFStateTransitionEffect *stateTransitionEffect1 = [[LSFStateTransitionEffect alloc] initWithLampIDs: ste1LampIDs lampGroupIDs: ste1LampGroupIDs lampState: ste1State andTransitionPeriod: ste1TransitionPeriod];
    
    NSString *lampID3 = @"lampID3";
    NSArray *ste2LampIDs = [NSArray arrayWithObjects: lampID3, nil];
    
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSArray *ste2LampGroupIDs = [NSArray arrayWithObjects: lampGroupID2, lampGroupID3, nil];
    
    LSFLampState *ste2State = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int ste2TransitionPeriod = 15;
    
    LSFStateTransitionEffect *stateTransitionEffect2 = [[LSFStateTransitionEffect alloc] initWithLampIDs: ste2LampIDs lampGroupIDs: ste2LampGroupIDs lampState: ste2State andTransitionPeriod: ste2TransitionPeriod];
    
    NSArray *transitionToStateComponents = [NSArray arrayWithObjects: stateTransitionEffect1, stateTransitionEffect2, nil];
    
    //Set TransitionToStateComponents
    self.scene.transitionToStateComponent = transitionToStateComponents;
    
    //Get TransitionToStateComponents
    NSArray *ttsc = self.scene.transitionToStateComponent;
    XCTAssertTrue([ttsc count] == 2, @"Array should contain only one object");
    
    LSFStateTransitionEffect *ste = [ttsc objectAtIndex: 0];
    
    //Get lamps
    NSArray *lampIDs = ste.lamps;
    NSSet *initialLampsSet = [NSSet setWithArray: ste1LampIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get lamp groups
    NSArray *lampGroupIDs = ste.lampGroups;
    NSSet *initialLampGroupsSet = [NSSet setWithArray: ste1LampGroupIDs];
    NSSet *lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get transition period
    unsigned int newTransitionPeriod = ste.transitionPeriod;
    XCTAssertTrue(newTransitionPeriod == 30, @"Transition period should be 30");
    
    //Get lamp state
    LSFLampState *state = ste.lampState;
    XCTAssertFalse(state.onOff, @"State should be of");
    XCTAssertTrue(state.brightness == 50, @"Brightness should be 50");
    XCTAssertTrue(state.hue == 180, @"Hue should be 180");
    XCTAssertTrue(state.saturation == 25, @"Saturation should be 25");
    XCTAssertTrue(state.colorTemp == 2700, @"Color Temp should be 2700");
    
    ste = [ttsc objectAtIndex: 1];
    
    //Get lamps
    lampIDs = ste.lamps;
    initialLampsSet = [NSSet setWithArray: ste2LampIDs];
    lampsSet = [NSSet setWithArray: lampIDs];
    isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get lamp groups
    lampGroupIDs = ste.lampGroups;
    initialLampGroupsSet = [NSSet setWithArray: ste2LampGroupIDs];
    lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get transition period
    newTransitionPeriod = ste.transitionPeriod;
    XCTAssertTrue(newTransitionPeriod == 15, @"Transition period should be 15");
    
    //Get lamp state
    state = ste.lampState;
    XCTAssertTrue(state.onOff, @"State should be on");
    XCTAssertTrue(state.brightness == 100, @"Brightness should be 100");
    XCTAssertTrue(state.hue == 360, @"Hue should be 360");
    XCTAssertTrue(state.saturation == 100, @"Saturation should be 100");
    XCTAssertTrue(state.colorTemp == 9000, @"Color Temp should be 9000");
}

-(void)testSetGetTransitionToPresetComponent
{
    //Create LSFPresetTransitionEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSArray *pte1LampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSArray *pte1LampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, nil];
    
    NSString *pte1PresetID = @"presetID1";
    
    unsigned int pte1TransitionPeriod = 30;
    
    LSFPresetTransitionEffect *presetTransitionEffect1 = [[LSFPresetTransitionEffect alloc] initWithLampIDs: pte1LampIDs lampGroupIDs: pte1LampGroupIDs presetID: pte1PresetID andTransitionPeriod: pte1TransitionPeriod];
    
    NSString *lampID3 = @"lampID3";
    NSArray *pte2LampIDs = [NSArray arrayWithObjects: lampID3, nil];
    
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSArray *pte2LampGroupIDs = [NSArray arrayWithObjects: lampGroupID2, lampGroupID3, nil];
    
    NSString *pte2PresetID = @"presetID2";
    
    unsigned int pte2TransitionPeriod = 15;
    
    LSFPresetTransitionEffect *presetTransitionEffect2 = [[LSFPresetTransitionEffect alloc] initWithLampIDs: pte2LampIDs lampGroupIDs: pte2LampGroupIDs presetID: pte2PresetID andTransitionPeriod: pte2TransitionPeriod];
    
    NSArray *transitionToPresetComponents = [NSArray arrayWithObjects: presetTransitionEffect1, presetTransitionEffect2, nil];
    
    //Set TransitionToPresetComponents
    self.scene.transitionToPresetComponent = transitionToPresetComponents;
    
    //Get TransitionToPresetComponents
    NSArray *ttpc = self.scene.transitionToPresetComponent;
    XCTAssertTrue([ttpc count] == 2, @"Array should contain only one object");
    
    LSFPresetTransitionEffect *pte = [ttpc objectAtIndex: 0];
    
    //Get lamps
    NSArray *lampIDs = pte.lamps;
    NSSet *initialLampsSet = [NSSet setWithArray: pte1LampIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get lamp groups
    NSArray *lampGroupIDs = pte.lampGroups;
    NSSet *initialLampGroupsSet = [NSSet setWithArray: pte1LampGroupIDs];
    NSSet *lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get transition period
    unsigned int newTransitionPeriod = pte.transitionPeriod;
    XCTAssertTrue(newTransitionPeriod == 30, @"Transition period should be 30");
    
    //Get preset
    NSString *presetID = pte.presetID;
    XCTAssertTrue([presetID isEqualToString: pte1PresetID], @"Preset IDs should be equal");
    
    pte = [ttpc objectAtIndex: 1];
    
    //Get lamps
    lampIDs = pte.lamps;
    initialLampsSet = [NSSet setWithArray: pte2LampIDs];
    lampsSet = [NSSet setWithArray: lampIDs];
    isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get lamp groups
    lampGroupIDs = pte.lampGroups;
    initialLampGroupsSet = [NSSet setWithArray: pte2LampGroupIDs];
    lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get transition period
    newTransitionPeriod = pte.transitionPeriod;
    XCTAssertTrue(newTransitionPeriod == 15, @"Transition period should be 15");
    
    //Get preset
    presetID = pte.presetID;
    XCTAssertTrue([presetID isEqualToString: pte2PresetID], @"Preset IDs should be equal");
}

-(void)testSetGetPulseWithStateComponent
{
    //Create LSFStatePulseEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSArray *spe1LampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSArray *spe1LampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, nil];
    
    LSFLampState *spe1FromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 25 hue: 90 saturation: 25 colorTemp: 2700];
    LSFLampState *spe1ToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 50 hue: 180 saturation: 50 colorTemp: 5000];
    
    unsigned int spe1Period = 15;
    unsigned int spe1Duration = 60;
    unsigned int spe1NumPulses = 10;
    
    LSFStatePulseEffect *statePulseEffect1 = [[LSFStatePulseEffect alloc] initWithLampIDs: spe1LampIDs lampGroupIDs: spe1LampGroupIDs fromState: spe1FromState toState: spe1ToState period: spe1Period duration: spe1Duration andNumPulses: spe1NumPulses];
    
    NSString *lampID3 = @"lampID3";
    NSArray *spe2LampIDs = [NSArray arrayWithObjects: lampID3, nil];
    
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSArray *spe2LampGroupIDs = [NSArray arrayWithObjects: lampGroupID2, lampGroupID3, nil];
    
    LSFLampState *spe2FromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 75 hue: 270 saturation: 75 colorTemp: 5000];
    LSFLampState *spe2ToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int spe2Period = 30;
    unsigned int spe2Duration = 120;
    unsigned int spe2NumPulses = 20;
    
    LSFStatePulseEffect *statePulseEffect2 = [[LSFStatePulseEffect alloc] initWithLampIDs: spe2LampIDs lampGroupIDs: spe2LampGroupIDs fromState: spe2FromState toState: spe2ToState period: spe2Period duration: spe2Duration andNumPulses: spe2NumPulses];
    
    NSArray *pulseWithStateComponent = [NSArray arrayWithObjects: statePulseEffect1, statePulseEffect2, nil];
    
    //Set PulseWithStateComponents
    self.scene.pulseWithStateComponent = pulseWithStateComponent;
    
    //Get PulseWithStateComponents
    NSArray *pwsc = self.scene.pulseWithStateComponent;
    XCTAssertTrue([pwsc count] == 2, @"Array should contain only one object");
    
    LSFStatePulseEffect *spe = [pwsc objectAtIndex: 0];
    
    //Get lamps
    NSArray *lampIDs = spe.lamps;
    NSSet *initialLampsSet = [NSSet setWithArray: spe1LampIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get lamp groups
    NSArray *lampGroupIDs = spe.lampGroups;
    NSSet *initialLampGroupsSet = [NSSet setWithArray: spe1LampGroupIDs];
    NSSet *lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get period
    unsigned int newPeriod = spe.period;
    XCTAssertTrue(newPeriod == 15, @"Period should be 15");
    
    //Get duration
    unsigned int newDuration = spe.duration;
    XCTAssertTrue(newDuration == 60, @"Duration should be 60");
    
    //Get num pulses
    unsigned int newNumPulses = spe.numPulses;
    XCTAssertTrue(newNumPulses == 10, @"NumPulses should be 10");
    
    //Get from lamp state
    LSFLampState *state = spe.fromLampState;
    XCTAssertTrue(state.onOff, @"State should be of");
    XCTAssertTrue(state.brightness == 25, @"Brightness should be 25");
    XCTAssertTrue(state.hue == 90, @"Hue should be 90");
    XCTAssertTrue(state.saturation == 25, @"Saturation should be 25");
    XCTAssertTrue(state.colorTemp == 2700, @"Color Temp should be 2700");
    
    //Get to lamp state
    state = spe.toLampState;
    XCTAssertTrue(state.onOff, @"State should be of");
    XCTAssertTrue(state.brightness == 50, @"Brightness should be 50");
    XCTAssertTrue(state.hue == 180, @"Hue should be 180");
    XCTAssertTrue(state.saturation == 50, @"Saturation should be 50");
    XCTAssertTrue(state.colorTemp == 5000, @"Color Temp should be 5000");
    
    spe = [pwsc objectAtIndex: 1];
    
    //Get lamps
    lampIDs = spe.lamps;
    initialLampsSet = [NSSet setWithArray: spe2LampIDs];
    lampsSet = [NSSet setWithArray: lampIDs];
    isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get lamp groups
    lampGroupIDs = spe.lampGroups;
    initialLampGroupsSet = [NSSet setWithArray: spe2LampGroupIDs];
    lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get period
    newPeriod = spe.period;
    XCTAssertTrue(newPeriod == 30, @"Period should be 30");
    
    //Get duration
    newDuration = spe.duration;
    XCTAssertTrue(newDuration == 120, @"Duration should be 120");
    
    //Get num pulses
    newNumPulses = spe.numPulses;
    XCTAssertTrue(newNumPulses == 20, @"NumPulses should be 20");
    
    //Get from lamp state
    state = spe.fromLampState;
    XCTAssertTrue(state.onOff, @"State should be of");
    XCTAssertTrue(state.brightness == 75, @"Brightness should be 75");
    XCTAssertTrue(state.hue == 270, @"Hue should be 270");
    XCTAssertTrue(state.saturation == 75, @"Saturation should be 75");
    XCTAssertTrue(state.colorTemp == 5000, @"Color Temp should be 5000");
    
    //Get to lamp state
    state = spe.toLampState;
    XCTAssertTrue(state.onOff, @"State should be of");
    XCTAssertTrue(state.brightness == 100, @"Brightness should be 100");
    XCTAssertTrue(state.hue == 360, @"Hue should be 360");
    XCTAssertTrue(state.saturation == 100, @"Saturation should be 100");
    XCTAssertTrue(state.colorTemp == 9000, @"Color Temp should be 9000");
}

-(void)testSetGetPulseWithPresetComponent
{
    //Create LSFPresetPulseEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSArray *ppe1LampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSArray *ppe1LampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, nil];
    
    NSString *ppe1FromPresetID = @"fromPresetID1";
    NSString *ppe1ToPresetID = @"toPresetID1";
    
    unsigned int ppe1Period = 15;
    unsigned int ppe1Duration = 60;
    unsigned int ppe1NumPulses = 10;
    
    LSFPresetPulseEffect *presetPulseEffect1 = [[LSFPresetPulseEffect alloc] initWithLampIDs: ppe1LampIDs lampGroupIDs: ppe1LampGroupIDs fromPresetID: ppe1FromPresetID toPresetID: ppe1ToPresetID period: ppe1Period duration: ppe1Duration andNumPulses: ppe1NumPulses];
    
    NSString *lampID3 = @"lampID3";
    NSArray *ppe2LampIDs = [NSArray arrayWithObjects: lampID3, nil];
    
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSArray *ppe2LampGroupIDs = [NSArray arrayWithObjects: lampGroupID2, lampGroupID3, nil];
    
    NSString *ppe2FromPresetID = @"fromPresetID2";
    NSString *ppe2ToPresetID = @"toPresetID2";
    
    unsigned int ppe2Period = 30;
    unsigned int ppe2Duration = 120;
    unsigned int ppe2NumPulses = 20;
    
    LSFPresetPulseEffect *presetPulseEffect2 = [[LSFPresetPulseEffect alloc] initWithLampIDs: ppe2LampIDs lampGroupIDs: ppe2LampGroupIDs fromPresetID: ppe2FromPresetID toPresetID: ppe2ToPresetID period: ppe2Period duration: ppe2Duration andNumPulses: ppe2NumPulses];
    
    NSArray *pulseWithPresetComponent = [NSArray arrayWithObjects: presetPulseEffect1, presetPulseEffect2, nil];
    
    //Set PulseWithPresetComponents
    self.scene.pulseWithPresetComponent = pulseWithPresetComponent;
    
    //Get PulseWithPresetComponents
    NSArray *pwpc = self.scene.pulseWithPresetComponent;
    XCTAssertTrue([pwpc count] == 2, @"Array should contain only one object");
    
    LSFPresetPulseEffect *ppe = [pwpc objectAtIndex: 0];
    
    //Get lamps
    NSArray *lampIDs = ppe.lamps;
    NSSet *initialLampsSet = [NSSet setWithArray: ppe1LampIDs];
    NSSet *lampsSet = [NSSet setWithArray: lampIDs];
    BOOL isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get lamp groups
    NSArray *lampGroupIDs = ppe.lampGroups;
    NSSet *initialLampGroupsSet = [NSSet setWithArray: ppe1LampGroupIDs];
    NSSet *lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get period
    unsigned int newPeriod = ppe.period;
    XCTAssertTrue(newPeriod == 15, @"Period should be 15");
    
    //Get duration
    unsigned int newDuration = ppe.duration;
    XCTAssertTrue(newDuration == 60, @"Duration should be 60");
    
    //Get num pulses
    unsigned int newNumPulses = ppe.numPulses;
    XCTAssertTrue(newNumPulses == 10, @"NumPulses should be 10");
    
    //Get from preset
    NSString *fromPresetID = ppe.fromPresetID;
    XCTAssertTrue([fromPresetID isEqualToString: ppe1FromPresetID], @"From preset IDs should be equal");
    
    //Get to preset
    NSString *toPresetID = ppe.toPresetID;
    XCTAssertTrue([toPresetID isEqualToString: ppe1ToPresetID], @"To preset IDs should be equal");
    
    ppe = [pwpc objectAtIndex: 1];
    
    //Get lamps
    lampIDs = ppe.lamps;
    initialLampsSet = [NSSet setWithArray: ppe2LampIDs];
    lampsSet = [NSSet setWithArray: lampIDs];
    isSetsEqual = [initialLampsSet isEqualToSet: lampsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get lamp groups
    lampGroupIDs = ppe.lampGroups;
    initialLampGroupsSet = [NSSet setWithArray: ppe2LampGroupIDs];
    lampGroupsSet = [NSSet setWithArray: lampGroupIDs];
    isSetsEqual = [initialLampGroupsSet isEqualToSet: lampGroupsSet];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
    
    //Get period
    newPeriod = ppe.period;
    XCTAssertTrue(newPeriod == 30, @"Period should be 30");
    
    //Get duration
    newDuration = ppe.duration;
    XCTAssertTrue(newDuration == 120, @"Duration should be 120");
    
    //Get num pulses
    newNumPulses = ppe.numPulses;
    XCTAssertTrue(newNumPulses == 20, @"NumPulses should be 20");
    
    //Get from preset
    fromPresetID = ppe.fromPresetID;
    XCTAssertTrue([fromPresetID isEqualToString: ppe2FromPresetID], @"From preset IDs should be equal");
    
    //Get to preset
    toPresetID = ppe.toPresetID;
    XCTAssertTrue([toPresetID isEqualToString: ppe2ToPresetID], @"To preset IDs should be equal");
}

-(void)testIsDependentOnPreset
{
    //Create LSFPresetTransitionEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSArray *pte1LampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSArray *pte1LampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, nil];
    NSString *pte1PresetID = @"presetID1";
    
    unsigned int pte1TransitionPeriod = 30;
    
    LSFPresetTransitionEffect *presetTransitionEffect1 = [[LSFPresetTransitionEffect alloc] initWithLampIDs: pte1LampIDs lampGroupIDs: pte1LampGroupIDs presetID: pte1PresetID andTransitionPeriod: pte1TransitionPeriod];
    
    NSString *lampID3 = @"lampID3";
    NSArray *pte2LampIDs = [NSArray arrayWithObjects: lampID3, nil];
    
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSArray *pte2LampGroupIDs = [NSArray arrayWithObjects: lampGroupID2, lampGroupID3, nil];
    NSString *pte2PresetID = @"presetID2";
    
    unsigned int pte2TransitionPeriod = 15;
    
    LSFPresetTransitionEffect *presetTransitionEffect2 = [[LSFPresetTransitionEffect alloc] initWithLampIDs: pte2LampIDs lampGroupIDs: pte2LampGroupIDs presetID: pte2PresetID andTransitionPeriod: pte2TransitionPeriod];
    
    NSArray *transitionToPresetComponents = [NSArray arrayWithObjects: presetTransitionEffect1, presetTransitionEffect2, nil];
    
    //Set TransitionToPresetComponents
    self.scene.transitionToPresetComponent = transitionToPresetComponents;

    //Create LSFPresetPulseEffect
    NSArray *ppe1LampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    NSArray *ppe1LampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, nil];
    NSString *ppe1FromPresetID = @"fromPresetID1";
    NSString *ppe1ToPresetID = @"toPresetID1";
    
    unsigned int ppe1Period = 15;
    unsigned int ppe1Duration = 60;
    unsigned int ppe1NumPulses = 10;
    
    LSFPresetPulseEffect *presetPulseEffect1 = [[LSFPresetPulseEffect alloc] initWithLampIDs: ppe1LampIDs lampGroupIDs: ppe1LampGroupIDs fromPresetID: ppe1FromPresetID toPresetID: ppe1ToPresetID period: ppe1Period duration: ppe1Duration andNumPulses: ppe1NumPulses];
    
    NSArray *ppe2LampIDs = [NSArray arrayWithObjects: lampID3, nil];
    NSArray *ppe2LampGroupIDs = [NSArray arrayWithObjects: lampGroupID2, lampGroupID3, nil];
    NSString *ppe2FromPresetID = @"fromPresetID2";
    NSString *ppe2ToPresetID = @"toPresetID2";
    
    unsigned int ppe2Period = 30;
    unsigned int ppe2Duration = 120;
    unsigned int ppe2NumPulses = 20;
    
    LSFPresetPulseEffect *presetPulseEffect2 = [[LSFPresetPulseEffect alloc] initWithLampIDs: ppe2LampIDs lampGroupIDs: ppe2LampGroupIDs fromPresetID: ppe2FromPresetID toPresetID: ppe2ToPresetID period: ppe2Period duration: ppe2Duration andNumPulses: ppe2NumPulses];
    
    NSArray *pulseWithPresetComponent = [NSArray arrayWithObjects: presetPulseEffect1, presetPulseEffect2, nil];
    
    //Set PulseWithPresetComponents
    self.scene.pulseWithPresetComponent = pulseWithPresetComponent;
    
    //Test for preset dependency
    LSFResponseCode code = [self.scene isDependentOnPreset: @"presetID1"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Preset should be dependent");
    
    code = [self.scene isDependentOnPreset: @"presetID2"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Preset should be dependent");
    
    code = [self.scene isDependentOnPreset: @"fromPresetID1"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"From preset should be dependent");
    
    code = [self.scene isDependentOnPreset: @"fromPresetID2"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"From preset should be dependent");
    
    code = [self.scene isDependentOnPreset: @"toPresetID1"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"To preset should be dependent");
    
    code = [self.scene isDependentOnPreset: @"toPresetID2"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"To preset should be dependent");
    
    code = [self.scene isDependentOnPreset: @"BadPresetID"];
    XCTAssertTrue(code == LSF_OK, @"Preset should not be dependent");
}

-(void)testIsDependentOnLampGroup
{
    //Create LSFStateTransitionEffect
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSArray *ste1LampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSArray *ste1LampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, nil];
    
    LSFLampState *ste1State = [[LSFLampState alloc] initWithOnOff: NO brightness: 50 hue: 180 saturation: 25 colorTemp: 2700];
    
    unsigned int ste1TransitionPeriod = 30;
    
    LSFStateTransitionEffect *stateTransitionEffect1 = [[LSFStateTransitionEffect alloc] initWithLampIDs: ste1LampIDs lampGroupIDs: ste1LampGroupIDs lampState: ste1State andTransitionPeriod: ste1TransitionPeriod];
    
    NSString *lampID3 = @"lampID3";
    NSArray *ste2LampIDs = [NSArray arrayWithObjects: lampID3, nil];
    
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSArray *ste2LampGroupIDs = [NSArray arrayWithObjects: lampGroupID2, lampGroupID3, nil];
    
    LSFLampState *ste2State = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int ste2TransitionPeriod = 15;
    
    LSFStateTransitionEffect *stateTransitionEffect2 = [[LSFStateTransitionEffect alloc] initWithLampIDs: ste2LampIDs lampGroupIDs: ste2LampGroupIDs lampState: ste2State andTransitionPeriod: ste2TransitionPeriod];
    
    NSArray *transitionToStateComponents = [NSArray arrayWithObjects: stateTransitionEffect1, stateTransitionEffect2, nil];
    
    //Set TransitionToStateComponents
    self.scene.transitionToStateComponent = transitionToStateComponents;
    
    //Create LSFPresetTransitionEffect
    NSArray *pte1LampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID4 = @"lampGroupID4";
    NSArray *pte1LampGroupIDs = [NSArray arrayWithObjects: lampGroupID4, nil];
    
    NSString *pte1PresetID = @"presetID1";
    
    unsigned int pte1TransitionPeriod = 30;
    
    LSFPresetTransitionEffect *presetTransitionEffect1 = [[LSFPresetTransitionEffect alloc] initWithLampIDs: pte1LampIDs lampGroupIDs: pte1LampGroupIDs presetID: pte1PresetID andTransitionPeriod: pte1TransitionPeriod];
    
    NSArray *pte2LampIDs = [NSArray arrayWithObjects: lampID3, nil];
    
    NSString *lampGroupID5 = @"lampGroupID5";
    NSString *lampGroupID6 = @"lampGroupID6";
    NSArray *pte2LampGroupIDs = [NSArray arrayWithObjects: lampGroupID5, lampGroupID6, nil];
    
    NSString *pte2PresetID = @"presetID2";
    
    unsigned int pte2TransitionPeriod = 15;
    
    LSFPresetTransitionEffect *presetTransitionEffect2 = [[LSFPresetTransitionEffect alloc] initWithLampIDs: pte2LampIDs lampGroupIDs: pte2LampGroupIDs presetID: pte2PresetID andTransitionPeriod: pte2TransitionPeriod];
    
    NSArray *transitionToPresetComponents = [NSArray arrayWithObjects: presetTransitionEffect1, presetTransitionEffect2, nil];
    
    //Set TransitionToPresetComponents
    self.scene.transitionToPresetComponent = transitionToPresetComponents;
    
    //Create LSFStatePulseEffect
    NSArray *spe1LampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID7 = @"lampGroupID7";
    NSArray *spe1LampGroupIDs = [NSArray arrayWithObjects: lampGroupID7, nil];
    
    LSFLampState *spe1FromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 25 hue: 90 saturation: 25 colorTemp: 2700];
    LSFLampState *spe1ToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 50 hue: 180 saturation: 50 colorTemp: 5000];
    
    unsigned int spe1Period = 15;
    unsigned int spe1Duration = 60;
    unsigned int spe1NumPulses = 10;
    
    LSFStatePulseEffect *statePulseEffect1 = [[LSFStatePulseEffect alloc] initWithLampIDs: spe1LampIDs lampGroupIDs: spe1LampGroupIDs fromState: spe1FromState toState: spe1ToState period: spe1Period duration: spe1Duration andNumPulses: spe1NumPulses];
    
    NSArray *spe2LampIDs = [NSArray arrayWithObjects: lampID3, nil];
    
    NSString *lampGroupID8 = @"lampGroupID8";
    NSString *lampGroupID9 = @"lampGroupID9";
    NSArray *spe2LampGroupIDs = [NSArray arrayWithObjects: lampGroupID8, lampGroupID9, nil];
    
    LSFLampState *spe2FromState = [[LSFLampState alloc] initWithOnOff: YES brightness: 75 hue: 270 saturation: 75 colorTemp: 5000];
    LSFLampState *spe2ToState = [[LSFLampState alloc] initWithOnOff: YES brightness: 100 hue: 360 saturation: 100 colorTemp: 9000];
    
    unsigned int spe2Period = 30;
    unsigned int spe2Duration = 120;
    unsigned int spe2NumPulses = 20;
    
    LSFStatePulseEffect *statePulseEffect2 = [[LSFStatePulseEffect alloc] initWithLampIDs: spe2LampIDs lampGroupIDs: spe2LampGroupIDs fromState: spe2FromState toState: spe2ToState period: spe2Period duration: spe2Duration andNumPulses: spe2NumPulses];
    
    NSArray *pulseWithStateComponent = [NSArray arrayWithObjects: statePulseEffect1, statePulseEffect2, nil];
    
    //Set PulseWithStateComponents
    self.scene.pulseWithStateComponent = pulseWithStateComponent;
    
    //Create LSFPresetPulseEffect
    NSArray *ppe1LampIDs = [NSArray arrayWithObjects: lampID1, lampID2, nil];
    
    NSString *lampGroupID10 = @"lampGroupID10";
    NSArray *ppe1LampGroupIDs = [NSArray arrayWithObjects: lampGroupID10, nil];
    
    NSString *ppe1FromPresetID = @"fromPresetID1";
    NSString *ppe1ToPresetID = @"toPresetID1";
    
    unsigned int ppe1Period = 15;
    unsigned int ppe1Duration = 60;
    unsigned int ppe1NumPulses = 10;
    
    LSFPresetPulseEffect *presetPulseEffect1 = [[LSFPresetPulseEffect alloc] initWithLampIDs: ppe1LampIDs lampGroupIDs: ppe1LampGroupIDs fromPresetID: ppe1FromPresetID toPresetID: ppe1ToPresetID period: ppe1Period duration: ppe1Duration andNumPulses: ppe1NumPulses];
    
    NSArray *ppe2LampIDs = [NSArray arrayWithObjects: lampID3, nil];
    
    NSString *lampGroupID11 = @"lampGroupID11";
    NSString *lampGroupID12 = @"lampGroupID12";
    NSArray *ppe2LampGroupIDs = [NSArray arrayWithObjects: lampGroupID11, lampGroupID12, nil];
    
    NSString *ppe2FromPresetID = @"fromPresetID2";
    NSString *ppe2ToPresetID = @"toPresetID2";
    
    unsigned int ppe2Period = 30;
    unsigned int ppe2Duration = 120;
    unsigned int ppe2NumPulses = 20;
    
    LSFPresetPulseEffect *presetPulseEffect2 = [[LSFPresetPulseEffect alloc] initWithLampIDs: ppe2LampIDs lampGroupIDs: ppe2LampGroupIDs fromPresetID: ppe2FromPresetID toPresetID: ppe2ToPresetID period: ppe2Period duration: ppe2Duration andNumPulses: ppe2NumPulses];
    
    NSArray *pulseWithPresetComponent = [NSArray arrayWithObjects: presetPulseEffect1, presetPulseEffect2, nil];
    
    //Set PulseWithPresetComponents
    self.scene.pulseWithPresetComponent = pulseWithPresetComponent;
    
    //Test for lamp group dependency
    LSFResponseCode code = [self.scene isDependentOnLampGroup: @"lampGroupID1"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"lampGroupID2"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"lampGroupID3"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"lampGroupID4"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"lampGroupID5"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"lampGroupID6"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"lampGroupID7"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"lampGroupID8"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"lampGroupID9"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"lampGroupID10"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"lampGroupID11"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"lampGroupID12"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Lamp group should be dependent");
    
    code = [self.scene isDependentOnLampGroup: @"BadLampGroupID"];
    XCTAssertTrue(code == LSF_OK, @"Lamp group should not be dependent");
}

@end
