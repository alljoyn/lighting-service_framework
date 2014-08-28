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

#import "LSFMasterSceneTests.h"
#import "LSFObjC/LSFMasterScene.h"

@interface LSFMasterSceneTests()

@property (nonatomic, strong) LSFMasterScene *masterScene;

@end

@implementation LSFMasterSceneTests

@synthesize masterScene = _masterScene;

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
    
    self.masterScene = [[LSFMasterScene alloc] init];
}

- (void)tearDown
{
    self.masterScene = nil;
    
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

-(void)testInitWithSceneIDs
{
    //Create Scene IDs Array
    NSString *sceneID1 = @"sceneID1";
    NSString *sceneID2 = @"sceneID2";
    NSString *sceneID3 = @"sceneID3";
    NSArray *initialSceneIDs = [NSArray arrayWithObjects: sceneID1, sceneID2, sceneID3, nil];
    
    //Create Master Scene Object
    self.masterScene = [[LSFMasterScene alloc] initWithSceneIDs: initialSceneIDs];
    
    //Get Scene IDs
    NSArray *sceneIDs = self.masterScene.sceneIDs;
    
    //Test
    NSSet *initialSceneIDsSet = [NSSet setWithArray: initialSceneIDs];
    NSSet *sceneIDSet = [NSSet setWithArray: sceneIDs];
    BOOL isSetsEqual = [sceneIDSet isEqualToSet: initialSceneIDsSet];
    XCTAssertTrue(isSetsEqual, @"Sets should be equal");
}

-(void)testSetGetSceneIDs
{
    //Create Scene IDs Array
    NSString *sceneID1 = @"sceneID1";
    NSString *sceneID2 = @"sceneID2";
    NSString *sceneID3 = @"sceneID3";
    NSArray *initialSceneIDs = [NSArray arrayWithObjects: sceneID1, sceneID2, sceneID3, nil];
    
    //Set Scene IDs
    self.masterScene.sceneIDs = initialSceneIDs;
    
    //Get Scene IDs
    NSArray *sceneIDs = self.masterScene.sceneIDs;
    
    //Test
    NSSet *initialSceneIDsSet = [NSSet setWithArray: initialSceneIDs];
    NSSet *sceneIDSet = [NSSet setWithArray: sceneIDs];
    BOOL isSetsEqual = [sceneIDSet isEqualToSet: initialSceneIDsSet];
    XCTAssertTrue(isSetsEqual, @"Sets should be equal");
}

-(void)testIsDependentScene
{
    //Create Scene IDs Array
    NSString *sceneID1 = @"sceneID1";
    NSString *sceneID2 = @"sceneID2";
    NSString *sceneID3 = @"sceneID3";
    NSArray *initialSceneIDs = [NSArray arrayWithObjects: sceneID1, sceneID2, sceneID3, nil];
    
    //Set Scene IDs
    self.masterScene.sceneIDs = initialSceneIDs;
    
    //Test
    LSFResponseCode code = [self.masterScene isDependentOnScene: @"sceneID1"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Scene should be dependent");
    
    code = [self.masterScene isDependentOnScene: @"sceneID2"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Scene should be dependent");
    
    code = [self.masterScene isDependentOnScene: @"sceneID3"];
    XCTAssertTrue(code == LSF_ERR_DEPENDENCY, @"Scene should be dependent");
    
    code = [self.masterScene isDependentOnScene: @"BadSceneID"];
    XCTAssertTrue(code == LSF_OK, @"Scene should not be dependent");
}

@end
