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

#import "LSFSceneManagerCallbackTests.h"
#import "MockSceneManagerCallbackDelegateHandler.h"
#import "LSFObjC/LSFSceneManagerCallback.h"

@interface LSFSceneManagerCallbackTests()

@property (nonatomic) MockSceneManagerCallbackDelegateHandler *smcdh;
@property (nonatomic) LSFSceneManagerCallback *smc;
@property (nonatomic) NSMutableArray *dataArray;

@end

@implementation LSFSceneManagerCallbackTests

@synthesize smcdh = _smcdh;
@synthesize smc = _smc;
@synthesize dataArray = _dataArray;

-(void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    self.smcdh = [[MockSceneManagerCallbackDelegateHandler alloc] init];
    self.smc = new LSFSceneManagerCallback(self.smcdh);
    self.dataArray = [[NSMutableArray alloc] init];
}

-(void)tearDown
{
    delete self.smc;
    self.smcdh = nil;
    
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testGetAllSceneIDs
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *functionName = @"getAllSceneIDs";
    NSString *sceneID1 = @"sceneID1";
    NSString *sceneID2 = @"sceneID2";
    NSString *sceneID3 = @"sceneID3";
    NSString *sceneID4 = @"sceneID4";
    NSArray *sceneIDsArray = [[NSArray alloc] initWithObjects: sceneID1, sceneID2, sceneID3, sceneID4, nil];
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: sceneIDsArray];
    
    //Call callback method
    LSFStringList sceneIDList;
    std::string id1([sceneID1 UTF8String]);
    sceneIDList.push_back(id1);
    std::string id2([sceneID2 UTF8String]);
    sceneIDList.push_back(id2);
    std::string id3([sceneID3 UTF8String]);
    sceneIDList.push_back(id3);
    std::string id4([sceneID4 UTF8String]);
    sceneIDList.push_back(id4);
    self.smc->GetAllSceneIDsReplyCB(code, sceneIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetSceneName
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *sceneID = @"sceneID1";
    NSString *functionName = @"getSceneName";
    NSString *language = @"en";
    NSString *name = @"testPresetName";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: language];
    [self.dataArray addObject: sceneID];
    [self.dataArray addObject: name];
    
    //Call callback method
    std::string sid([sceneID UTF8String]);
    std::string lang([language UTF8String]);
    std::string sceneName([name UTF8String]);
    self.smc->GetSceneNameReplyCB(code, sid, lang, sceneName);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testSetSceneName
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *sceneID = @"sceneID1";
    NSString *functionName = @"setSceneName";
    NSString *language = @"en";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: language];
    [self.dataArray addObject: sceneID];
    
    //Call callback method
    std::string sid([sceneID UTF8String]);
    std::string lang([language UTF8String]);
    self.smc->SetSceneNameReplyCB(code, sid, lang);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testScenesNameChanged
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"scenesNameChanged";
    NSString *sceneID1 = @"sceneID1";
    NSString *sceneID2 = @"sceneID2";
    NSString *sceneID3 = @"sceneID3";
    NSString *sceneID4 = @"sceneID4";
    NSArray *sceneIDsArray = [[NSArray alloc] initWithObjects: sceneID1, sceneID2, sceneID3, sceneID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: sceneIDsArray];
    
    //Call callback method
    LSFStringList sceneIDList;
    std::string id1([sceneID1 UTF8String]);
    sceneIDList.push_back(id1);
    std::string id2([sceneID2 UTF8String]);
    sceneIDList.push_back(id2);
    std::string id3([sceneID3 UTF8String]);
    sceneIDList.push_back(id3);
    std::string id4([sceneID4 UTF8String]);
    sceneIDList.push_back(id4);
    self.smc->ScenesNameChangedCB(sceneIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testCreateScene
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *sceneID = @"sceneID1";
    NSString *functionName = @"createScene";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: sceneID];
    
    //Call callback method
    std::string sid([sceneID UTF8String]);
    self.smc->CreateSceneReplyCB(code, sid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testScenesCreated
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"scenesCreated";
    NSString *sceneID1 = @"sceneID1";
    NSString *sceneID2 = @"sceneID2";
    NSString *sceneID3 = @"sceneID3";
    NSString *sceneID4 = @"sceneID4";
    NSArray *sceneIDsArray = [[NSArray alloc] initWithObjects: sceneID1, sceneID2, sceneID3, sceneID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: sceneIDsArray];
    
    //Call callback method
    LSFStringList sceneIDList;
    std::string id1([sceneID1 UTF8String]);
    sceneIDList.push_back(id1);
    std::string id2([sceneID2 UTF8String]);
    sceneIDList.push_back(id2);
    std::string id3([sceneID3 UTF8String]);
    sceneIDList.push_back(id3);
    std::string id4([sceneID4 UTF8String]);
    sceneIDList.push_back(id4);
    self.smc->ScenesCreatedCB(sceneIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testUpdateScene
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *sceneID = @"sceneID1";
    NSString *functionName = @"updateScene";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: sceneID];
    
    //Call callback method
    std::string sid([sceneID UTF8String]);
    self.smc->UpdateSceneReplyCB(code, sid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testScenesUpdated
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"scenesUpdated";
    NSString *sceneID1 = @"sceneID1";
    NSString *sceneID2 = @"sceneID2";
    NSString *sceneID3 = @"sceneID3";
    NSString *sceneID4 = @"sceneID4";
    NSArray *sceneIDsArray = [[NSArray alloc] initWithObjects: sceneID1, sceneID2, sceneID3, sceneID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: sceneIDsArray];
    
    //Call callback method
    LSFStringList sceneIDList;
    std::string id1([sceneID1 UTF8String]);
    sceneIDList.push_back(id1);
    std::string id2([sceneID2 UTF8String]);
    sceneIDList.push_back(id2);
    std::string id3([sceneID3 UTF8String]);
    sceneIDList.push_back(id3);
    std::string id4([sceneID4 UTF8String]);
    sceneIDList.push_back(id4);
    self.smc->ScenesUpdatedCB(sceneIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testDeleteScene
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *sceneID = @"sceneID1";
    NSString *functionName = @"deleteScene";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: sceneID];
    
    //Call callback method
    std::string sid([sceneID UTF8String]);
    self.smc->DeleteSceneReplyCB(code, sid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testScenesDeleted
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"scenesDeleted";
    NSString *sceneID1 = @"sceneID1";
    NSString *sceneID2 = @"sceneID2";
    NSString *sceneID3 = @"sceneID3";
    NSString *sceneID4 = @"sceneID4";
    NSArray *sceneIDsArray = [[NSArray alloc] initWithObjects: sceneID1, sceneID2, sceneID3, sceneID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: sceneIDsArray];
    
    //Call callback method
    LSFStringList sceneIDList;
    std::string id1([sceneID1 UTF8String]);
    sceneIDList.push_back(id1);
    std::string id2([sceneID2 UTF8String]);
    sceneIDList.push_back(id2);
    std::string id3([sceneID3 UTF8String]);
    sceneIDList.push_back(id3);
    std::string id4([sceneID4 UTF8String]);
    sceneIDList.push_back(id4);
    self.smc->ScenesDeletedCB(sceneIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testGetScene
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *sceneID = @"sceneID1";
    NSString *functionName = @"getScene";
    
    NSString *lampID1 = @"lampID1";
    NSString *lampID2 = @"lampID2";
    NSString *lampID3 = @"lampID3";
    NSArray *lampIDs = [NSArray arrayWithObjects: lampID1, lampID2, lampID3, nil];
    
    NSString *lampGroupID1 = @"lampGroupID1";
    NSString *lampGroupID2 = @"lampGroupID2";
    NSArray *lampGroupIDs = [NSArray arrayWithObjects: lampGroupID1, lampGroupID2, nil];
    
    NSString *presetID = @"testPresetID";
    unsigned int transtionPeriod = 30;
    NSNumber *tp = [[NSNumber alloc] initWithInt: transtionPeriod];
    
    LSFPresetTransitionEffect *pte = [[LSFPresetTransitionEffect alloc] initWithLampIDs: lampIDs lampGroupIDs: lampGroupIDs presetID: presetID andTransitionPeriod:transtionPeriod];
    
    NSArray *transitionToPresetComponent = [NSArray arrayWithObjects: pte, nil];
    
    LSFScene *scene = [[LSFScene alloc] init];
    scene.transitionToPresetComponent = transitionToPresetComponent;
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: lampIDs];
    [self.dataArray addObject: lampGroupIDs];
    [self.dataArray addObject: presetID];
    [self.dataArray addObject: tp];
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: sceneID];
    
    //Call callback method
    std::string sid([sceneID UTF8String]);
    self.smc->GetSceneReplyCB(code, sid, *(static_cast<lsf::Scene*>(scene.handle)));
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testApplyScene
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    LSFResponseCode code = LSF_ERR_INVALID;
    NSNumber *responseCode = [[NSNumber alloc] initWithInt: code];
    NSString *sceneID = @"sceneID1";
    NSString *functionName = @"applyScene";
    
    [self.dataArray addObject: responseCode];
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: sceneID];
    
    //Call callback method
    std::string sid([sceneID UTF8String]);
    self.smc->ApplySceneReplyCB(code, sid);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

-(void)testScenesApplied
{
    //Ensure array is empty
    [self.dataArray removeAllObjects];
    
    //Populate array with test data
    NSString *functionName = @"scenesApplied";
    NSString *sceneID1 = @"sceneID1";
    NSString *sceneID2 = @"sceneID2";
    NSString *sceneID3 = @"sceneID3";
    NSString *sceneID4 = @"sceneID4";
    NSArray *sceneIDsArray = [[NSArray alloc] initWithObjects: sceneID1, sceneID2, sceneID3, sceneID4, nil];
    
    [self.dataArray addObject: functionName];
    [self.dataArray addObject: sceneIDsArray];
    
    //Call callback method
    LSFStringList sceneIDList;
    std::string id1([sceneID1 UTF8String]);
    sceneIDList.push_back(id1);
    std::string id2([sceneID2 UTF8String]);
    sceneIDList.push_back(id2);
    std::string id3([sceneID3 UTF8String]);
    sceneIDList.push_back(id3);
    std::string id4([sceneID4 UTF8String]);
    sceneIDList.push_back(id4);
    self.smc->ScenesAppliedCB(sceneIDList);
    
    //Test the data using NSSet
    NSSet *startData = [[NSSet alloc] initWithArray: self.dataArray];
    NSSet *endData = [[NSSet alloc] initWithArray: [self.smcdh getCallbackData]];
    BOOL isSetsEqual = [startData isEqualToSet: endData];
    XCTAssertTrue(isSetsEqual, @"Start and end data should be equal");
}

@end
