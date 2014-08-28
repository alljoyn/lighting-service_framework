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

#import "LSFPresetManagerCallbackTests.h"
#import "MockPresetManagerCallbackDelegateHandler.h"
#import "LSFObjC/LSFPresetManagerCallback.h"

@interface LSFPresetManagerCallbackTests()

@property (nonatomic) MockPresetManagerCallbackDelegateHandler *pmcdh;
@property (nonatomic) LSFPresetManagerCallback *pmc;
@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation LSFPresetManagerCallbackTests

@synthesize pmcdh = _pmcdh;
@synthesize pmc = _pmc;
@synthesize dataArray = _dataArray;

-(void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    self.pmcdh = [[MockPresetManagerCallbackDelegateHandler alloc] init];
    self.pmc = new LSFPresetManagerCallback(self.pmcdh);
    self.dataArray = [[NSMutableArray alloc] init];
}

-(void)tearDown
{
    delete self.pmc;
    self.pmcdh = nil;
    
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testGetPreset
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *presetID = @"presetID1";
    NSString *functionName = @"getPreset";
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
    [self.dataArray addObject: presetID];
    
    //Call callback method
    lsf::LampState *state = new lsf::LampState(true, 100, 200, 300, 400);
    std::string pid([presetID UTF8String]);
    self.pmc->GetPresetReplyCB(code, pid, *state);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetAllPresetIDs
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *functionName = @"getAllPresetIDs";
    NSString *presetID1 = @"presetID1";
    NSString *presetID2 = @"presetID2";
    NSString *presetID3 = @"presetID3";
    NSString *presetID4 = @"presetID4";
    NSArray *lampIDsArray = [[NSArray alloc] initWithObjects: presetID1, presetID2, presetID3, presetID4, nil];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampIDsArray];
    
    //Call callback method
    LSFStringList presetIDList;
    std::string id1([presetID1 UTF8String]);
    presetIDList.push_back(id1);
    std::string id2([presetID2 UTF8String]);
    presetIDList.push_back(id2);
    std::string id3([presetID3 UTF8String]);
    presetIDList.push_back(id3);
    std::string id4([presetID4 UTF8String]);
    presetIDList.push_back(id4);
    self.pmc->GetAllPresetIDsReplyCB(code, presetIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetPresetName
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *presetID = @"presetID1";
    NSString *functionName = @"getPresetName";
    NSString *language = @"en";
    NSString *name = @"testPresetName";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: language];
    [self.dataArray addObject: presetID];
    [self.dataArray addObject: name];
    
    //Call callback method
    std::string pid([presetID UTF8String]);
    std::string lang([language UTF8String]);
    std::string presetName([name UTF8String]);
    self.pmc->GetPresetNameReplyCB(code, pid, lang, presetName);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testSetPresetName
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *presetID = @"presetID1";
    NSString *functionName = @"setPresetName";
    NSString *language = @"en";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: language];
    [self.dataArray addObject: presetID];
    
    //Call callback method
    std::string pid([presetID UTF8String]);
    std::string lang([language UTF8String]);
    self.pmc->SetPresetNameReplyCB(code, pid, lang);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testPresetsNameChanged
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"presetsNameChanged";
    NSString *presetID1 = @"presetID1";
    NSString *presetID2 = @"presetID2";
    NSString *presetID3 = @"presetID3";
    NSString *presetID4 = @"presetID4";
    NSArray *lampIDsArray = [[NSArray alloc] initWithObjects: presetID1, presetID2, presetID3, presetID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampIDsArray];
    
    //Call callback method
    LSFStringList presetIDList;
    std::string id1([presetID1 UTF8String]);
    presetIDList.push_back(id1);
    std::string id2([presetID2 UTF8String]);
    presetIDList.push_back(id2);
    std::string id3([presetID3 UTF8String]);
    presetIDList.push_back(id3);
    std::string id4([presetID4 UTF8String]);
    presetIDList.push_back(id4);
    self.pmc->PresetsNameChangedCB(presetIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testCreatePreset
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *presetID = @"presetID1";
    NSString *functionName = @"createPreset";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: presetID];
    
    //Call callback method
    std::string pid([presetID UTF8String]);
    self.pmc->CreatePresetReplyCB(code, pid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testPresetsCreated
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"presetsCreated";
    NSString *presetID1 = @"presetID1";
    NSString *presetID2 = @"presetID2";
    NSString *presetID3 = @"presetID3";
    NSString *presetID4 = @"presetID4";
    NSArray *lampIDsArray = [[NSArray alloc] initWithObjects: presetID1, presetID2, presetID3, presetID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampIDsArray];
    
    //Call callback method
    LSFStringList presetIDList;
    std::string id1([presetID1 UTF8String]);
    presetIDList.push_back(id1);
    std::string id2([presetID2 UTF8String]);
    presetIDList.push_back(id2);
    std::string id3([presetID3 UTF8String]);
    presetIDList.push_back(id3);
    std::string id4([presetID4 UTF8String]);
    presetIDList.push_back(id4);
    self.pmc->PresetsCreatedCB(presetIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testUpdatePreset
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *presetID = @"presetID1";
    NSString *functionName = @"updatePreset";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: presetID];
    
    //Call callback method
    std::string pid([presetID UTF8String]);
    self.pmc->UpdatePresetReplyCB(code, pid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testPresetsUpdated
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"presetsUpdated";
    NSString *presetID1 = @"presetID1";
    NSString *presetID2 = @"presetID2";
    NSString *presetID3 = @"presetID3";
    NSString *presetID4 = @"presetID4";
    NSArray *lampIDsArray = [[NSArray alloc] initWithObjects: presetID1, presetID2, presetID3, presetID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampIDsArray];
    
    //Call callback method
    LSFStringList presetIDList;
    std::string id1([presetID1 UTF8String]);
    presetIDList.push_back(id1);
    std::string id2([presetID2 UTF8String]);
    presetIDList.push_back(id2);
    std::string id3([presetID3 UTF8String]);
    presetIDList.push_back(id3);
    std::string id4([presetID4 UTF8String]);
    presetIDList.push_back(id4);
    self.pmc->PresetsUpdatedCB(presetIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testDeletePreset
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *presetID = @"presetID1";
    NSString *functionName = @"deletePreset";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: presetID];
    
    //Call callback method
    std::string pid([presetID UTF8String]);
    self.pmc->DeletePresetReplyCB(code, pid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testPresetsDeleted
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"presetsDeleted";
    NSString *presetID1 = @"presetID1";
    NSString *presetID2 = @"presetID2";
    NSString *presetID3 = @"presetID3";
    NSString *presetID4 = @"presetID4";
    NSArray *lampIDsArray = [[NSArray alloc] initWithObjects: presetID1, presetID2, presetID3, presetID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampIDsArray];
    
    //Call callback method
    LSFStringList presetIDList;
    std::string id1([presetID1 UTF8String]);
    presetIDList.push_back(id1);
    std::string id2([presetID2 UTF8String]);
    presetIDList.push_back(id2);
    std::string id3([presetID3 UTF8String]);
    presetIDList.push_back(id3);
    std::string id4([presetID4 UTF8String]);
    presetIDList.push_back(id4);
    self.pmc->PresetsDeletedCB(presetIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetDefaultLampState
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *functionName = @"getDefaultLampState";
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
    
    //Call callback method
    lsf::LampState *state = new lsf::LampState(true, 100, 200, 300, 400);
    self.pmc->GetDefaultLampStateReplyCB(code, *state);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testSetDefaultLampState
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *functionName = @"setDefaultLampState";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    
    //Call callback method
    self.pmc->SetDefaultLampStateReplyCB(code);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testDefaultLampStateChanged
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"defaultLampStateChanged";
    [self.dataArray addObject: functionName];
    
    //Call callback method
    self.pmc->DefaultLampStateChangedCB();
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.pmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

@end
