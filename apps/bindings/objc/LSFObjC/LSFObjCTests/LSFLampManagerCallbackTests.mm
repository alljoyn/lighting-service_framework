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

#import "LSFLampManagerCallbackTests.h"
#import "MockLampManagerCallbackDelegateHandler.h"
#import "LSFObjC/LSFLampManagerCallback.h"

@interface LSFLampManagerCallbackTests()

@property (nonatomic) MockLampManagerCallbackDelegateHandler *lmcdh;
@property (nonatomic) LSFLampManagerCallback *lmc;
@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation LSFLampManagerCallbackTests

@synthesize lmcdh = _lmcdh;
@synthesize lmc = _lmc;
@synthesize dataArray = _dataArray;

-(void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    self.lmcdh = [[MockLampManagerCallbackDelegateHandler alloc] init];
    self.lmc = new LSFLampManagerCallback(self.lmcdh);
    self.dataArray = [[NSMutableArray alloc] init];
}

-(void)tearDown
{
    delete self.lmc;
    self.lmcdh = nil;
    
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testGetAllLampIDs
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *functionName = @"getAllLampIDs";
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSString *lampID4 = @"lampID4";
    NSArray *lampIDsArray = [[NSArray alloc] initWithObjects: lampID1, lampID2, lampID3, lampID4, nil];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampIDsArray];
    
    //Call callback method
    LSFStringList lampIDList;
    std::string id1([lampID1 UTF8String]);
    lampIDList.push_back(id1);
    std::string id2([lampID2 UTF8String]);
    lampIDList.push_back(id2);
    std::string id3([lampID3 UTF8String]);
    lampIDList.push_back(id3);
    std::string id4([lampID4 UTF8String]);
    lampIDList.push_back(id4);
    self.lmc->GetAllLampIDsReplyCB(code, lampIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampName
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampName";
    NSString *language = @"en";
    NSString *name = @"testLampName";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: language];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: name];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    std::string lang([language UTF8String]);
    std::string lampName([name UTF8String]);
    self.lmc->GetLampNameReplyCB(code, lid, lang, lampName);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampManufacturer
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampManufacturer";
    NSString *language = @"en";
    NSString *manufacturer = @"testManufacturer";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: language];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: manufacturer];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    std::string lang([language UTF8String]);
    std::string manu([manufacturer UTF8String]);
    self.lmc->GetLampManufacturerReplyCB(code, lid, lang, manu);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testSetLampName
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"setLampName";
    NSString *language = @"en";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: language];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    std::string lang([language UTF8String]);
    self.lmc->SetLampNameReplyCB(code, lid, lang);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testLampsNameChanged
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"lampsNameChanged";
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSString *lampID4 = @"lampID4";
    NSArray *lampIDsArray = [[NSArray alloc] initWithObjects: lampID1, lampID2, lampID3, lampID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampIDsArray];
    
    //Call callback method
    LSFStringList lampIDList;
    std::string id1([lampID1 UTF8String]);
    lampIDList.push_back(id1);
    std::string id2([lampID2 UTF8String]);
    lampIDList.push_back(id2);
    std::string id3([lampID3 UTF8String]);
    lampIDList.push_back(id3);
    std::string id4([lampID4 UTF8String]);
    lampIDList.push_back(id4);
    self.lmc->LampsNameChangedCB(lampIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testLampsFound
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];

    //Populate array with test data
    NSString *functionName = @"lampsFound";
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSString *lampID4 = @"lampID4";
    NSArray *lampIDsArray = [[NSArray alloc] initWithObjects: lampID1, lampID2, lampID3, lampID4, nil];

    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampIDsArray];

    //Call callback method
    LSFStringList lampIDList;
    std::string id1([lampID1 UTF8String]);
    lampIDList.push_back(id1);
    std::string id2([lampID2 UTF8String]);
    lampIDList.push_back(id2);
    std::string id3([lampID3 UTF8String]);
    lampIDList.push_back(id3);
    std::string id4([lampID4 UTF8String]);
    lampIDList.push_back(id4);
    self.lmc->LampsFoundCB(lampIDList);

    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testLampsLost
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];

    //Populate array with test data
    NSString *functionName = @"lampsLost";
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSString *lampID4 = @"lampID4";
    NSArray *lampIDsArray = [[NSArray alloc] initWithObjects: lampID1, lampID2, lampID3, lampID4, nil];

    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampIDsArray];

    //Call callback method
    LSFStringList lampIDList;
    std::string id1([lampID1 UTF8String]);
    lampIDList.push_back(id1);
    std::string id2([lampID2 UTF8String]);
    lampIDList.push_back(id2);
    std::string id3([lampID3 UTF8String]);
    lampIDList.push_back(id3);
    std::string id4([lampID4 UTF8String]);
    lampIDList.push_back(id4);
    self.lmc->LampsLostCB(lampIDList);

    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testPingLamp
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];

    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"pingLamp";

    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];

    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->PingLampReplyCB(code, lid);

    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampDetails //(LSFResponseCode)rc lampID: (NSString *)lampID andLampDetails: (LSFLampDetails *)details
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampDetails";
    NSNumber *make = [[NSNumber alloc] initWithInt: MAKE_LIFX];
    NSNumber *model = [[NSNumber alloc] initWithInt: MODEL_LED];
    NSNumber *type = [[NSNumber alloc] initWithInt: TYPE_LAMP];
    NSNumber *lampType = [[NSNumber alloc] initWithInt: LAMPTYPE_A15];
    NSNumber *lampBaseType = [[NSNumber alloc] initWithInt: BASETYPE_E5];
    NSNumber *lampBeamAngle = [[NSNumber alloc] initWithUnsignedInt: 100];
    NSNumber *dimmable = [[NSNumber alloc] initWithBool: YES];
    NSNumber *color = [[NSNumber alloc] initWithBool: YES];
    NSNumber *variableColorTemp = [[NSNumber alloc] initWithBool: NO];
    NSNumber *hasEffects = [[NSNumber alloc] initWithBool: NO];
    NSNumber *maxVoltage = [[NSNumber alloc] initWithUnsignedInt: 120];
    NSNumber *minVoltage = [[NSNumber alloc] initWithUnsignedInt: 120];
    NSNumber *wattage = [[NSNumber alloc] initWithUnsignedInt: 100];
    NSNumber *incandescentEquivalent = [[NSNumber alloc] initWithUnsignedInt: 9];
    NSNumber *maxLumens = [[NSNumber alloc] initWithUnsignedInt: 100];
    NSNumber *minTemp = [[NSNumber alloc] initWithUnsignedInt: 2400];
    NSNumber *maxTemp = [[NSNumber alloc] initWithUnsignedInt: 5000];
    NSNumber *cri = [[NSNumber alloc] initWithUnsignedInt: 93];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: make];
    [self.dataArray addObject: model];
    [self.dataArray addObject: type];
    [self.dataArray addObject: lampType];
    [self.dataArray addObject: lampBaseType];
    [self.dataArray addObject: lampBeamAngle];
    [self.dataArray addObject: dimmable];
    [self.dataArray addObject: color];
    [self.dataArray addObject: variableColorTemp];
    [self.dataArray addObject: hasEffects];
    [self.dataArray addObject: maxVoltage];
    [self.dataArray addObject: minVoltage];
    [self.dataArray addObject: wattage];
    [self.dataArray addObject: incandescentEquivalent];
    [self.dataArray addObject: maxLumens];
    [self.dataArray addObject: minTemp];
    [self.dataArray addObject: maxTemp];
    [self.dataArray addObject: cri];
    
    //Call callback method
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
    std::string lid([lampID UTF8String]);
    ld->lampID = lid;
    self.lmc->GetLampDetailsReplyCB(code, lid, *ld);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampParameters
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampParameters";
    NSNumber *eum = [[NSNumber alloc] initWithUnsignedInt: 0];
    NSNumber *lume = [[NSNumber alloc] initWithUnsignedInt: 0];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: eum];
    [self.dataArray addObject: lume];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    lsf::LampParameters *params = new lsf::LampParameters();
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampParametersReplyCB(code, lid, *params);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampParametersEnergyUsageMilliwattsField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampParametersEnergyUsageMilliwatts";
    NSNumber *eum = [[NSNumber alloc] initWithUnsignedInt: 15];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: eum];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampParametersEnergyUsageMilliwattsFieldReplyCB(code, lid, 15);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampParametersLumensField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampParametersLumens";
    NSNumber *lume = [[NSNumber alloc] initWithUnsignedInt: 100];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lume];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampParametersLumensFieldReplyCB(code, lid, 100);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampState
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampState";
    NSNumber *onOff = [[NSNumber alloc] initWithBool: YES];
    NSNumber *brightness = [[NSNumber alloc] initWithUnsignedInt: 100];
    NSNumber *hue = [[NSNumber alloc] initWithUnsignedInt: 200];
    NSNumber *saturation = [[NSNumber alloc] initWithUnsignedInt: 300];
    NSNumber *colorTemp = [[NSNumber alloc] initWithUnsignedInt: 400];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: onOff];
    [self.dataArray addObject: brightness];
    [self.dataArray addObject: hue];
    [self.dataArray addObject: saturation];
    [self.dataArray addObject: colorTemp];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    lsf::LampState *state = new lsf::LampState(true, 100, 200, 300, 400);
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampStateReplyCB(code, lid, *state);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampStateOnOffField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampStateOnOffField";
    NSNumber *onOff = [[NSNumber alloc] initWithBool: YES];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: onOff];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampStateOnOffFieldReplyCB(code, lid, true);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampStateHueField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampStateHueField";
    NSNumber *hue = [[NSNumber alloc] initWithUnsignedInt: 100];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: hue];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampStateHueFieldReplyCB(code, lid, 100);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampStateSaturationField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampStateSaturationField";
    NSNumber *saturation = [[NSNumber alloc] initWithUnsignedInt: 100];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: saturation];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampStateSaturationFieldReplyCB(code, lid, 100);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampStateBrightnessField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampStateBrightnessField";
    NSNumber *brightness = [[NSNumber alloc] initWithUnsignedInt: 100];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: brightness];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampStateBrightnessFieldReplyCB(code, lid, 100);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampStateColorTempField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampStateColorTempField";
    NSNumber *colorTemp = [[NSNumber alloc] initWithUnsignedInt: 100];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: colorTemp];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampStateColorTempFieldReplyCB(code, lid, 100);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampState
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"resetLampState";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->ResetLampStateReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testLampsStateChanged
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"lampsStateChanged";
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSString *lampID4 = @"lampID4";
    NSArray *lampIDsArray = [[NSArray alloc] initWithObjects: lampID1, lampID2, lampID3, lampID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampIDsArray];
    
    //Call callback method
    LSFStringList lampIDList;
    std::string id1([lampID1 UTF8String]);
    lampIDList.push_back(id1);
    std::string id2([lampID2 UTF8String]);
    lampIDList.push_back(id2);
    std::string id3([lampID3 UTF8String]);
    lampIDList.push_back(id3);
    std::string id4([lampID4 UTF8String]);
    lampIDList.push_back(id4);
    self.lmc->LampsStateChangedCB(lampIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampState
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"transitionLampState";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->TransitionLampStateReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testPulseLampWithState
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"pulseWithState";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->PulseLampWithStateReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testPulseLampWithPreset
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"pulseWithPreset";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->PulseLampWithPresetReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampStateOnOffField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"transitionLampStateOnOffField";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->TransitionLampStateOnOffFieldReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampStateHueField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"transitionLampStateHueField";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->TransitionLampStateHueFieldReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampStateSaturationField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"transitionLampStateSaturationField";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->TransitionLampStateSaturationFieldReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampStateBrightnessField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"transitionLampStateBrightnessField";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->TransitionLampStateBrightnessFieldReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampStateColorTempField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"transitionLampStateColorTempField";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->TransitionLampStateColorTempFieldReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampFaults //(LSFResponseCode)rc lampID: (NSString *)lampID andFaultCodes: (NSArray *)codes
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampFaults";
    NSNumber *code1 = [[NSNumber alloc] initWithUnsignedInt: 3];
    NSNumber *code2 = [[NSNumber alloc] initWithUnsignedInt: 5];
    NSArray *faultCodeArray = [[NSArray alloc] initWithObjects: code1, code2, nil];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: faultCodeArray];
    
    //Call callback method
    LampFaultCodeList codeList;
    codeList.push_back(3);
    codeList.push_back(5);
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampFaultsReplyCB(code, lid, codeList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampServiceVersion
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampServiceVersion";
    NSNumber *version = [[NSNumber alloc] initWithUnsignedInt: 2];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: version];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampServiceVersionReplyCB(code, lid, 2);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testClearLampFault
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"clearLampFault";
    NSNumber *faultCode = [[NSNumber alloc] initWithUnsignedInt: 10];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: faultCode];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->ClearLampFaultReplyCB(code, lid, 10);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampStateOnOffField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"resetLampStateOnOffField";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->ResetLampStateOnOffFieldReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampStateHueField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"resetLampStateHueField";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->ResetLampStateHueFieldReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampStateSaturationField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"resetLampStateSaturationField";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->ResetLampStateSaturationFieldReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampStateBrightnessField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"resetLampStateBrightnessField";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->ResetLampStateBrightnessFieldReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampStateColorTempField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"resetLampStateColorTempField";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->ResetLampStateColorTempFieldReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampStateToPreset
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"transitionLampStateToPreset";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    
    //Call callback method
    std::string lid([lampID UTF8String]);
    self.lmc->TransitionLampStateToPresetReplyCB(code, lid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampSupportedLanguages //(LSFResponseCode)rc lampID: (NSString*)lampID andSupportedLanguages: (NSArray*)supportedLanguages;
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampID = @"lampID1";
    NSString *functionName = @"getLampSupportedLanguages";
    NSString *language1 = @"en";
    NSString *language2 = @"fr";
    NSArray *languageArray = [[NSArray alloc] initWithObjects: language1, language2, nil];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampID];
    [self.dataArray addObject: languageArray];
    
    //Call callback method
    LSFStringList langList;
    std::string lang1([language1 UTF8String]);
    langList.push_back(lang1);
    std::string lang2([language2 UTF8String]);
    langList.push_back(lang2);
    std::string lid([lampID UTF8String]);
    self.lmc->GetLampSupportedLanguagesReplyCB(code, lid, langList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

@end
