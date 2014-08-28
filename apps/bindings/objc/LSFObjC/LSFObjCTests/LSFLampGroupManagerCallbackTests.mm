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

#import "LSFLampGroupManagerCallbackTests.h"
#import "MockLampGroupManageCallbackDelegateHandler.h"
#import "LSFObjC/LSFLampGroupManagerCallback.h"

@interface LSFLampGroupManagerCallbackTests()

@property (nonatomic) MockLampGroupManageCallbackDelegateHandler *lgmcdh;
@property (nonatomic) LSFLampGroupManagerCallback *lgmc;
@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation LSFLampGroupManagerCallbackTests

@synthesize lgmcdh = _lgmcdh;
@synthesize lgmc = _lgmc;
@synthesize dataArray = _dataArray;

-(void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    self.lgmcdh = [[MockLampGroupManageCallbackDelegateHandler alloc] init];
    self.lgmc = new LSFLampGroupManagerCallback(self.lgmcdh);
    self.dataArray = [[NSMutableArray alloc] init];
}

-(void)tearDown
{
    delete self.lgmc;
    self.lgmcdh = nil;
    
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testGetAllLampGroupIDs
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"getAllLampGroupIDs";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSString *lampGroupID4 = @"lampGroupID4";
    NSArray *lampGroupIDsArray = [[NSArray alloc] initWithObjects: lampGroupID1, lampGroupID2, lampGroupID3, lampGroupID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupIDsArray];
    
    //Call callback method
    LSFStringList lampGroupIDList;
    std::string id1([lampGroupID1 UTF8String]);
    lampGroupIDList.push_back(id1);
    std::string id2([lampGroupID2 UTF8String]);
    lampGroupIDList.push_back(id2);
    std::string id3([lampGroupID3 UTF8String]);
    lampGroupIDList.push_back(id3);
    std::string id4([lampGroupID4 UTF8String]);
    lampGroupIDList.push_back(id4);
    self.lgmc->GetAllLampGroupIDsReplyCB(code, lampGroupIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampGroupName
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"getLampGroupName";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    NSString *lampGroupName = @"lampGroupName";
    NSString *language = @"en";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    [self.dataArray addObject: lampGroupName];
    [self.dataArray addObject: language];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    std::string gname([lampGroupName UTF8String]);
    std::string lang([language UTF8String]);
    self.lgmc->GetLampGroupNameReplyCB(code, gid, lang, gname);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testSetLampGroupName
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"setLampGroupName";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    NSString *language = @"en";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    [self.dataArray addObject: language];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    std::string lang([language UTF8String]);
    self.lgmc->SetLampGroupNameReplyCB(code, gid, lang);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testLampGroupsNameChanged
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"lampGroupsNameChanged";
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSString *lampGroupID4 = @"lampGroupID4";
    NSArray *lampGroupIDsArray = [[NSArray alloc] initWithObjects: lampGroupID1, lampGroupID2, lampGroupID3, lampGroupID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampGroupIDsArray];
    
    //Call callback method
    LSFStringList lampGroupIDList;
    std::string id1([lampGroupID1 UTF8String]);
    lampGroupIDList.push_back(id1);
    std::string id2([lampGroupID2 UTF8String]);
    lampGroupIDList.push_back(id2);
    std::string id3([lampGroupID3 UTF8String]);
    lampGroupIDList.push_back(id3);
    std::string id4([lampGroupID4 UTF8String]);
    lampGroupIDList.push_back(id4);
    self.lgmc->LampGroupsNameChangedCB(lampGroupIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testCreateLampGroup
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"createLampGroup";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->CreateLampGroupReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testLampGroupsCreated
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"lampGroupsCreated";
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSString *lampGroupID4 = @"lampGroupID4";
    NSArray *lampGroupIDsArray = [[NSArray alloc] initWithObjects: lampGroupID1, lampGroupID2, lampGroupID3, lampGroupID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampGroupIDsArray];
    
    //Call callback method
    LSFStringList lampGroupIDList;
    std::string id1([lampGroupID1 UTF8String]);
    lampGroupIDList.push_back(id1);
    std::string id2([lampGroupID2 UTF8String]);
    lampGroupIDList.push_back(id2);
    std::string id3([lampGroupID3 UTF8String]);
    lampGroupIDList.push_back(id3);
    std::string id4([lampGroupID4 UTF8String]);
    lampGroupIDList.push_back(id4);
    self.lgmc->LampGroupsCreatedCB(lampGroupIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetLampGroup
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"getLampGroup";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSString *lampID4 = @"lampID4";
    NSArray *lampIDsArray = [[NSArray alloc] initWithObjects: lampID1, lampID2, lampID3, lampID4, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSString *lampGroupID4 = @"lampGroupID4";
    NSArray *lampGroupIDsArray = [[NSArray alloc] initWithObjects: lampGroupID1, lampGroupID2, lampGroupID3, lampGroupID4, nil];
    
    LSFLampGroup *lampGroup = [[LSFLampGroup alloc] init];
    [lampGroup setLamps: lampIDsArray];
    [lampGroup setLampGroups: lampGroupIDsArray];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    [self.dataArray addObject: lampIDsArray];
    [self.dataArray addObject: lampGroupIDsArray];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->GetLampGroupReplyCB(code, gid, *static_cast<lsf::LampGroup*>(lampGroup.handle));
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testDeleteLampGroup
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"deleteLampGroup";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->DeleteLampGroupReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testLampGroupsDeleted
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"lampGroupsDeleted";
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSString *lampGroupID4 = @"lampGroupID4";
    NSArray *lampGroupIDsArray = [[NSArray alloc] initWithObjects: lampGroupID1, lampGroupID2, lampGroupID3, lampGroupID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampGroupIDsArray];
    
    //Call callback method
    LSFStringList lampGroupIDList;
    std::string id1([lampGroupID1 UTF8String]);
    lampGroupIDList.push_back(id1);
    std::string id2([lampGroupID2 UTF8String]);
    lampGroupIDList.push_back(id2);
    std::string id3([lampGroupID3 UTF8String]);
    lampGroupIDList.push_back(id3);
    std::string id4([lampGroupID4 UTF8String]);
    lampGroupIDList.push_back(id4);
    self.lgmc->LampGroupsDeletedCB(lampGroupIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampGroupToState
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"transitionLampGroupToState";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->TransitionLampGroupStateReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testPulseLampGroupWithState
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"pulseLampGroupWithState";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->PulseLampGroupWithStateReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testPulseLampGroupWithPreset
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"pulseLampGroupWithPreset";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->PulseLampGroupWithPresetReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampGroupStateOnOffField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"transitionLampGroupStateOnOffField";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->TransitionLampGroupStateOnOffFieldReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampGroupStateHueField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"transitionLampGroupStateHueField";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->TransitionLampGroupStateHueFieldReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampGroupStateSaturationField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"transitionLampGroupStateSaturationField";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->TransitionLampGroupStateSaturationFieldReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampGroupStateBrightnessField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"transitionLampGroupStateBrightnessField";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->TransitionLampGroupStateBrightnessFieldReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampGroupStateColorTempField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"transitionLampGroupStateColorTempField";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->TransitionLampGroupStateColorTempFieldReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampGroupState
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"resetLampGroupState";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->ResetLampGroupStateReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampGroupStateOnOffField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"resetLampGroupStateOnOffField";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->ResetLampGroupStateOnOffFieldReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampGroupStateHueField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"resetLampGroupStateHueField";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->ResetLampGroupStateHueFieldReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampGroupStateSaturationField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"resetLampGroupStateSaturationField";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->ResetLampGroupStateSaturationFieldReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampGroupStateBrightnessField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"resetLampGroupStateBrightnessField";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->ResetLampGroupStateBrightnessFieldReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testResetLampGroupStateColorTempField
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"resetLampGroupStateColorTempField";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->ResetLampGroupStateColorTempFieldReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testUpdateLampGroup
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"updateLampGroup";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->UpdateLampGroupReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testLampGroupsUpdated
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"lampGroupsUpdated";
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSString *lampGroupID3 = @"lampGroupID3";
    NSString *lampGroupID4 = @"lampGroupID4";
    NSArray *lampGroupIDsArray = [[NSArray alloc] initWithObjects: lampGroupID1, lampGroupID2, lampGroupID3, lampGroupID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampGroupIDsArray];
    
    //Call callback method
    LSFStringList lampGroupIDList;
    std::string id1([lampGroupID1 UTF8String]);
    lampGroupIDList.push_back(id1);
    std::string id2([lampGroupID2 UTF8String]);
    lampGroupIDList.push_back(id2);
    std::string id3([lampGroupID3 UTF8String]);
    lampGroupIDList.push_back(id3);
    std::string id4([lampGroupID4 UTF8String]);
    lampGroupIDList.push_back(id4);
    self.lgmc->LampGroupsUpdatedCB(lampGroupIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testTransitionLampGroupStateToPreset
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"transitionLampGroupStateToPreset";
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *lampGroupID = @"lampGroupID";
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: lampGroupID];
    
    //Call callback method
    std::string gid([lampGroupID UTF8String]);
    self.lgmc->TransitionLampGroupStateToPresetReplyCB(code, gid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.lgmcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

@end
