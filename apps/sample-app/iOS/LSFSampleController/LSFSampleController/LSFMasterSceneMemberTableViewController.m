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

#import "LSFMasterSceneMembersTableViewController.h"
#import "LSFSceneModelContainer.h"
#import "LSFSceneDataModel.h"
#import "LSFAllJoynManager.h"
#import "LSFDispatchQueue.h"

@interface LSFMasterSceneMembersTableViewController ()

@property (nonatomic, strong) UIBarButtonItem *cancelButton;

-(void)sortScenesByName: (NSArray *)scenes;

@end

@implementation LSFMasterSceneMembersTableViewController

@synthesize masterSceneModel = _masterSceneModel;
@synthesize usesCancel = _usesCancel;
@synthesize cancelButton = _cancelButton;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    if (self.usesCancel)
    {
        [self.navigationItem setHidesBackButton:YES];

        self.cancelButton = [[UIBarButtonItem alloc]
                             initWithBarButtonSystemItem:UIBarButtonSystemItemCancel
                             target: self
                             action: @selector(cancelButtonPressed)];

        self.navigationItem.leftBarButtonItem = self.cancelButton;
    }
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * Override table view delegate method so the cell knows how to draw itself
 */
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    LSFSceneDataModel *model = [self.dataArray objectAtIndex: [indexPath row]];
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: @"SceneMembers" forIndexPath:indexPath];

    cell.textLabel.text = model.name;
    cell.imageView.image = [UIImage imageNamed:@"scene_set_icon.png"];

    if ([self.masterSceneModel.masterScene.sceneIDs containsObject: model.theID])
    {
        cell.accessoryType = UITableViewCellAccessoryCheckmark;
        [self.selectedRows addObject: indexPath];
    }
    else
    {
        cell.accessoryType = UITableViewCellAccessoryNone;
    }

    return cell;
}

-(NSString *)tableView: (UITableView *)tableView titleForHeaderInSection: (NSInteger)section
{
    return @"Select the scenes in this master scene";
}

/*
 * Override public functions in LSFMembersTableViewController
 */
-(void)buildTableArray
{
    LSFSceneModelContainer *scenesContainer = [LSFSceneModelContainer getSceneModelContainer];
    NSMutableDictionary *scenes = scenesContainer.sceneContainer;
    NSMutableArray *scenesArray = [NSMutableArray arrayWithArray: [scenes allValues]];

    [self sortScenesByName: [NSMutableArray arrayWithArray: scenesArray]];
}

-(void)processSelectedRows
{
    NSLog(@"LSFMasterSceneMembersTableViewController - processSelectedRows() executing");

    NSMutableArray *sceneIDs = [[NSMutableArray alloc] init];

    for (NSIndexPath *indexPath in self.selectedRows)
    {
        LSFSceneDataModel *sceneDataModel = [self.dataArray objectAtIndex: indexPath.row];
        [sceneIDs addObject: sceneDataModel.theID];
    }

    self.masterSceneModel.masterScene.sceneIDs = sceneIDs;

    if ([self.masterSceneModel.theID isEqualToString: @""])
    {
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
            [ajManager.lsfMasterSceneManager createMasterScene: self.masterSceneModel.masterScene withName: self.masterSceneModel.name];
        });
    }
    else
    {
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
            [ajManager.lsfMasterSceneManager updateMasterSceneWithID: self.masterSceneModel.theID andMasterScene: self.masterSceneModel.masterScene];
        });
    }

    [self dismissViewControllerAnimated: YES completion: nil];
}

/*
 * Done Button Pressed Handler
 */
-(IBAction)doneButtonPressed: (id)sender
{
    NSLog(@"Done button pressed");

    if ([self.selectedRows count] == 0)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                        message: @"You must select at least one scene to create a master scene."
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];
    }
    else
    {
        [self processSelectedRows];
    }
}

/*
 * Cancel Button Event Handler
 */
-(void)cancelButtonPressed
{
    [self dismissViewControllerAnimated: YES completion: nil];
}

/*
 * Private Functions
 */
-(void)sortScenesByName: (NSArray *)scenes
{
    NSMutableArray *sortedArray = [NSMutableArray arrayWithArray: [scenes sortedArrayUsingComparator: ^NSComparisonResult(id a, id b) {
        NSString *first = [(LSFModel *)a name];
        NSString *second = [(LSFModel *)b name];
        return [first localizedCaseInsensitiveCompare: second];
    }]];

    self.dataArray = sortedArray;
}

@end
