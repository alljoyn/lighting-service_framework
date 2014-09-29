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

#import "LSFScenesTableViewController.h"
#import "LSFSceneInfoTableViewController.h"
#import "LSFScenesEnterNameViewController.h"
#import "LSFMasterScenesEnterNameViewController.h"
#import "LSFMasterScenesInfoTableViewController.h"
#import "LSFLampModelContainer.h"
#import "LSFGroupModelContainer.h"
#import "LSFSceneModelContainer.h"
#import "LSFMasterSceneModelContainer.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"
#import "LSFSceneDataModel.h"
#import "LSFMasterSceneDataModel.h"
#import "LSFSceneCell.h"
#import "LSFNoEffectDataModel.h"
#import "LSFTransitionEffectDataModel.h"
#import "LSFPulseEffectDataModel.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFConstants.h"

@interface LSFScenesTableViewController ()

@property (nonatomic, strong) NSMutableArray *sceneData;
@property (nonatomic, strong) UIBarButtonItem *myEditButton;
@property (nonatomic, strong) UIBarButtonItem *settingsButton;
@property (nonatomic, strong) UIBarButtonItem *addButton;

-(void)plusButtonPressed;
-(void)settingsButtonPressed;
-(NSString *)buildSceneDetailsString: (LSFSceneDataModel *)sceneModel;
-(NSString *)buildMasterSceneDetailsString: (LSFMasterSceneDataModel *)masterSceneModel;
-(void)appendLampNames: (NSArray *)lampIDs toString: (NSMutableString *)detailsString;
-(void)appendGroupNames: (NSArray *)groupIDs toString: (NSMutableString *)detailsString;
-(void)appendSceneNames: (NSArray *)sceneIDs toString: (NSMutableString *)detailsString;
-(NSUInteger)checkIfModelExists: (LSFDataModel *)sceneModel;
-(NSUInteger)findInsertionIndexInArray: (LSFDataModel *)sceneModel;
-(void)sortScenesByName;
-(int)findIndexInModelOfID: (NSString *)theID;
-(void)addObjectToTableAtIndex: (NSUInteger)insertIndex;
-(void)moveObjectFromIndex: (NSUInteger)fromIndex toIndex: (NSUInteger)toIndex;
-(void)refreshRowInTableAtIndex: (NSUInteger)index;
-(void)deleteRowsInTableAtIndex: (NSArray *)cellIndexPaths;

@end

@implementation LSFScenesTableViewController

@synthesize myEditButton = _myEditButton;
@synthesize settingsButton = _settingsButton;
@synthesize addButton = _addButton;

-(void)viewDidLoad
{
    [super viewDidLoad];
    [self.navigationItem setHidesBackButton:YES];

    self.addButton = [[UIBarButtonItem alloc]
                      initWithBarButtonSystemItem:UIBarButtonSystemItemAdd
                      target: self
                      action: @selector(plusButtonPressed)];

    self.settingsButton = [[UIBarButtonItem alloc]
                           initWithImage: [UIImage imageNamed: @"nav_settings_regular_icon.png"]
                           style:UIBarButtonItemStylePlain
                           target: self
                           action: @selector(settingsButtonPressed)];

    NSArray *actionButtonItems = @[self.settingsButton, self.addButton];
    self.navigationItem.rightBarButtonItems = actionButtonItems;
    self.navigationItem.leftBarButtonItem = self.editButtonItem;
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.ssmc setReloadScenesDelegate: self];
        [ajManager.smsmc setReloadMasterScenesDelegate: self];
    });

    //Set the content of the default scene data array
    LSFSceneModelContainer *scenesContainer = [LSFSceneModelContainer getSceneModelContainer];
    LSFMasterSceneModelContainer *masterScenesContainer = [LSFMasterSceneModelContainer getMasterSceneModelContainer];
    self.sceneData = [NSMutableArray arrayWithArray: [scenesContainer.sceneContainer allValues]];
    [self.sceneData addObjectsFromArray: [masterScenesContainer.masterScenesContainer allValues]];

    if (self.sceneData.count > 0)
    {
        [self sortScenesByName];
    }

    [self.tableView reloadData];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.ssmc setReloadScenesDelegate: nil];
        [ajManager.smsmc setReloadMasterScenesDelegate: nil];
    });
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LSFReloadScenesCallbackDelegate Implementation
 */
-(void)reloadSceneWithID: (NSString *)sceneID
{
    @synchronized(self.sceneData)
    {
        LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
        NSMutableDictionary *scenes = container.sceneContainer;
        LSFSceneDataModel *model = [scenes valueForKey: sceneID];

        if (model != nil)
        {
            NSUInteger existingIndex = [self checkIfModelExists: (LSFDataModel *)model];

            if (existingIndex == NSNotFound)
            {
                NSUInteger insertIndex = [self findInsertionIndexInArray: (LSFDataModel *)model];
                [self.sceneData insertObject: model atIndex: insertIndex];

                [self addObjectToTableAtIndex: insertIndex];
            }
            else
            {
                NSUInteger insertIndex = [self findInsertionIndexInArray: (LSFDataModel *)model];

                if (existingIndex == insertIndex)
                {
                    [self refreshRowInTableAtIndex: insertIndex];
                }
                else
                {
                    if (existingIndex < insertIndex)
                    {
                        insertIndex--;
                    }

                    [self.sceneData removeObjectAtIndex: existingIndex];
                    [self.sceneData insertObject: model atIndex: insertIndex];

                    [self moveObjectFromIndex: existingIndex toIndex: insertIndex];
                }
            }
        }
        else
        {
            NSLog(@"LSFScenesTableViewController - reloadSceneWithID() model is nil");
        }
    }
}

-(void)deleteScenesWithIDs: (NSArray *)sceneIDs andNames: (NSArray *)sceneNames
{
    @synchronized(self.sceneData)
    {
        LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
        NSMutableDictionary *scenes = container.sceneContainer;

        NSMutableArray *deleteIndexPaths = [[NSMutableArray alloc] init];

        for (NSString *sceneID in sceneIDs)
        {
            LSFSceneDataModel *model = [scenes valueForKey: sceneID];

            if (model == nil)
            {
                int modelArrayIndex = [self findIndexInModelOfID: sceneID];

                if (modelArrayIndex != -1)
                {
                    [self.sceneData removeObjectAtIndex: modelArrayIndex];
                    [deleteIndexPaths addObject: [NSIndexPath indexPathForRow: modelArrayIndex inSection:0]];
                }
            }
        }
        
        [self deleteRowsInTableAtIndex: deleteIndexPaths];
    }
}

/*
 * LSFReloadMasterScenesCallbackDelegate Implementation
 */
-(void)reloadMasterSceneWithID: (NSString *)masterSceneID
{
    @synchronized(self.sceneData)
    {
        LSFMasterSceneModelContainer *container = [LSFMasterSceneModelContainer getMasterSceneModelContainer];
        NSMutableDictionary *masterScenes = container.masterScenesContainer;
        LSFMasterSceneDataModel *model = [masterScenes valueForKey: masterSceneID];

        if (model != nil)
        {
            NSUInteger existingIndex = [self checkIfModelExists: (LSFDataModel *)model];

            if (existingIndex == NSNotFound)
            {
                NSUInteger insertIndex = [self findInsertionIndexInArray: (LSFDataModel *)model];
                [self.sceneData insertObject: model atIndex: insertIndex];

                [self addObjectToTableAtIndex: insertIndex];
            }
            else
            {
                NSUInteger insertIndex = [self findInsertionIndexInArray: (LSFDataModel *)model];

                if (existingIndex == insertIndex)
                {
                    [self refreshRowInTableAtIndex: insertIndex];
                }
                else
                {
                    if (existingIndex < insertIndex)
                    {
                        insertIndex--;
                    }

                    [self.sceneData removeObjectAtIndex: existingIndex];
                    [self.sceneData insertObject: model atIndex: insertIndex];

                    [self moveObjectFromIndex: existingIndex toIndex: insertIndex];
                }
            }
        }
        else
        {
            NSLog(@"LSFScenesTableViewController - reloadMasterSceneWithID() model is nil");
        }
    }
}

-(void)deleteMasterScenesWithIDs: (NSArray *)masterSceneIDs andNames: (NSArray *)masterSceneNames
{
    @synchronized(self.sceneData)
    {
        LSFMasterSceneModelContainer *container = [LSFMasterSceneModelContainer getMasterSceneModelContainer];
        NSMutableDictionary *masterScenes = container.masterScenesContainer;

        NSMutableArray *deleteIndexPaths = [[NSMutableArray alloc] init];

        for (NSString *masterSceneID in masterSceneIDs)
        {
            LSFMasterSceneDataModel *model = [masterScenes valueForKey: masterSceneID];

            if (model == nil)
            {
                int modelArrayIndex = [self findIndexInModelOfID: masterSceneID];

                if (modelArrayIndex != -1)
                {
                    [self.sceneData removeObjectAtIndex: modelArrayIndex];
                    [deleteIndexPaths addObject: [NSIndexPath indexPathForRow: modelArrayIndex inSection:0]];
                }
            }
        }
        
        [self deleteRowsInTableAtIndex: deleteIndexPaths];
    }
}

/*
 * UITableViewDataSource Implementation
 */
-(UITableViewCell *)tableView: (UITableView *)tableView cellForRowAtIndexPath: (NSIndexPath *)indexPath
{
    id data = [self.sceneData objectAtIndex: [indexPath row]];

    LSFSceneCell *cell = [tableView dequeueReusableCellWithIdentifier: @"SceneCell" forIndexPath: indexPath];

    if ([data isKindOfClass: [LSFSceneDataModel class]])
    {
        LSFSceneDataModel *sceneModel = (LSFSceneDataModel *)data;

        cell.sceneType = Basic;
        cell.sceneID = sceneModel.theID;
        cell.sceneImageView.image = [UIImage imageNamed: @"scene_set_icon.png"];
        cell.nameLabel.text = sceneModel.name;
        cell.detailsLabel.text = [self buildSceneDetailsString: sceneModel];
    }
    else if ([data isKindOfClass: [LSFMasterSceneDataModel class]])
    {
        LSFMasterSceneDataModel *masterSceneModel = (LSFMasterSceneDataModel *)data;

        cell.sceneType = Master;
        cell.masterSceneID = masterSceneModel.theID;
        cell.sceneImageView.image = [UIImage imageNamed: @"master_scene_set_icon.png"];
        cell.nameLabel.text = masterSceneModel.name;
        cell.detailsLabel.text = [self buildMasterSceneDetailsString: masterSceneModel];
    }
    
    return cell;
}

-(void)tableView: (UITableView *)tableView didSelectRowAtIndexPath: (NSIndexPath *)indexPath
{
    id data = [self.sceneData objectAtIndex: indexPath.row];

    if ([data isKindOfClass: [LSFSceneDataModel class]])
    {
        LSFSceneDataModel *sceneDataModel = (LSFSceneDataModel *)data;

        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFSceneManager *sceneManager = ([LSFAllJoynManager getAllJoynManager]).lsfSceneManager;
            [sceneManager applySceneWithID: sceneDataModel.theID];
        });
    }
    else if ([data isKindOfClass: [LSFMasterSceneDataModel class]])
    {
        LSFMasterSceneDataModel *masterSceneDataModel = (LSFMasterSceneDataModel *)data;

        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFMasterSceneManager *masterSceneManager = ([LSFAllJoynManager getAllJoynManager]).lsfMasterSceneManager;
            [masterSceneManager applyMasterSceneWithID: masterSceneDataModel.theID];
        });
    }
}

-(NSInteger)tableView: (UITableView *)tableView numberOfRowsInSection: (NSInteger)section
{
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
    LSFConstants *constants = [LSFConstants getConstants];

    if (!ajManager.isConnectedToController)
    {
        CGRect frame = CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height);
        UIView *customView = [[UIView alloc] initWithFrame: frame];
        UILabel *messageLabel = [[UILabel alloc] init];
        [customView addSubview: messageLabel];

        frame.origin.x = 30;
        frame.size.width = self.view.bounds.size.width - frame.origin.x;

        messageLabel.frame = frame;
        messageLabel.text = [NSString stringWithFormat: @"No controller service is available on the network %@.\n\nSearching for controller service...", [constants currentWifiSSID]];
        messageLabel.textColor = [UIColor blackColor];
        messageLabel.numberOfLines = 0;
        messageLabel.textAlignment = NSTextAlignmentLeft;

        self.tableView.backgroundView = customView;
        return 0;
    }
    else
    {
        if (self.sceneData.count == 0)
        {
            // Display a message when the table is empty
            UILabel *messageLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height)];

            messageLabel.text = @"There are no scenes available.\n\nTo create a scene from the current light settings, tap the + button above.";
            messageLabel.textColor = [UIColor blackColor];
            messageLabel.numberOfLines = 0;
            messageLabel.textAlignment = NSTextAlignmentCenter;
            [messageLabel sizeToFit];

            self.tableView.backgroundView = messageLabel;
        }
        else
        {
            self.tableView.backgroundView = nil;
        }
        
        return self.sceneData.count;
    }
}

-(CGFloat)tableView: (UITableView *)tableView heightForRowAtIndexPath: (NSIndexPath *)indexPath
{
    return 70;
}

-(CGFloat) tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 1.0f;
}

-(BOOL)tableView: (UITableView *)tableView canEditRowAtIndexPath: (NSIndexPath *)indexPath
{
    return YES;
}

-(void)tableView: (UITableView *)tableView commitEditingStyle: (UITableViewCellEditingStyle)editingStyle forRowAtIndexPath: (NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete)
    {
        id data = [self.sceneData objectAtIndex: indexPath.row];

        if ([data isKindOfClass: [LSFSceneDataModel class]])
        {
            LSFSceneDataModel *sceneDataModel = (LSFSceneDataModel *)data;

            int modelArrayIndex = [self findIndexInModelOfID: sceneDataModel.theID];
            [self.sceneData removeObjectAtIndex: modelArrayIndex];

            // Delete the row from the data source
            [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation: UITableViewRowAnimationFade];

            dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
                LSFSceneManager *sceneManager = ([LSFAllJoynManager getAllJoynManager]).lsfSceneManager;
                [sceneManager deleteSceneWithID: sceneDataModel.theID];
            });
        }
        else if ([data isKindOfClass: [LSFMasterSceneDataModel class]])
        {
            LSFMasterSceneDataModel *masterSceneDataModel = (LSFMasterSceneDataModel *)data;

            int modelArrayIndex = [self findIndexInModelOfID: masterSceneDataModel.theID];
            [self.sceneData removeObjectAtIndex: modelArrayIndex];

            // Delete the row from the data source
            [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation: UITableViewRowAnimationFade];

            dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
                LSFMasterSceneManager *masterSceneManager = ([LSFAllJoynManager getAllJoynManager]).lsfMasterSceneManager;
                [masterSceneManager deleteMasterSceneWithID: masterSceneDataModel.theID];
            });
        }
    }
}

-(void)tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(NSIndexPath *)indexPath
{
    id data = [self.sceneData objectAtIndex: indexPath.row];

    if ([data isKindOfClass: [LSFSceneDataModel class]])
    {
        [self performSegueWithIdentifier: @"SceneInfo" sender: indexPath];
    }
    else if ([data isKindOfClass: [LSFMasterSceneDataModel class]])
    {
        [self performSegueWithIdentifier: @"MasterSceneInfo" sender: indexPath];
    }
}

/*
 * Segue Method
 */
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString: @"CreateScene"])
    {
        UINavigationController *nc = (UINavigationController *)[segue destinationViewController];
        LSFScenesEnterNameViewController *senvc = (LSFScenesEnterNameViewController *)nc.topViewController;
        senvc.sceneModel = [[LSFSceneDataModel alloc] init];
    }
    else if ([segue.identifier isEqualToString: @"CreateMasterScene"])
    {
        UINavigationController *nc = (UINavigationController *)[segue destinationViewController];
        LSFMasterScenesEnterNameViewController *msenvc = (LSFMasterScenesEnterNameViewController *)nc.topViewController;
        msenvc.masterSceneModel = [[LSFMasterSceneDataModel alloc] init];
    }
    else if ([segue.identifier isEqualToString: @"SceneInfo"])
    {
        NSIndexPath *indexPath = (NSIndexPath *)sender;

        LSFSceneInfoTableViewController *sitvc = [segue destinationViewController];
        sitvc.sceneID = [NSString stringWithString: ((LSFSceneDataModel *)[self.sceneData objectAtIndex: [indexPath row]]).theID];
    }
    else if ([segue.identifier isEqualToString: @"MasterSceneInfo"])
    {
        NSIndexPath *indexPath = (NSIndexPath *)sender;

        LSFMasterScenesInfoTableViewController *msitvc = [segue destinationViewController];
        msitvc.masterSceneID = [NSString stringWithString: ((LSFMasterSceneDataModel *)[self.sceneData objectAtIndex: [indexPath row]]).theID];
    }
}

/*
 * UIActionSheetDelegate implementation
 */
-(void)actionSheet: (UIActionSheet *)actionSheet clickedButtonAtIndex: (NSInteger)buttonIndex
{
    switch (buttonIndex)
    {
        case 0:
            [self performSegueWithIdentifier: @"CreateScene" sender: self];
            break;
        case 1:
            [self performSegueWithIdentifier: @"CreateMasterScene" sender: self];
            break;
        case 2:
            //NSLog(@"Cancel button pressed");
            break;
     }
}

/*
 * Private functions
 */
-(void)plusButtonPressed
{
    UIActionSheet *actionSheet = [[UIActionSheet alloc] initWithTitle: nil delegate: self cancelButtonTitle: @"Cancel" destructiveButtonTitle: nil otherButtonTitles: @"Add Scene", @"Add Master Scene", nil];
    [actionSheet showInView: self.view];
};

-(void)settingsButtonPressed
{
    [self performSegueWithIdentifier: @"ScenesSettings" sender: self];
}

-(NSString *)buildSceneDetailsString: (LSFSceneDataModel *)sceneModel
{
    NSMutableString *detailsString = [[NSMutableString alloc] init];

    for (LSFNoEffectDataModel *nedm in sceneModel.noEffects)
    {
        [self appendLampNames: nedm.members.lamps toString: detailsString];
        [self appendGroupNames: nedm.members.lampGroups toString: detailsString];
    }

    for (LSFTransitionEffectDataModel *tedm in sceneModel.transitionEffects)
    {
        [self appendLampNames: tedm.members.lamps toString: detailsString];
        [self appendGroupNames: tedm.members.lampGroups toString: detailsString];
    }

    for (LSFPulseEffectDataModel *pedm in sceneModel.pulseEffects)
    {
        [self appendLampNames: pedm.members.lamps toString: detailsString];
        [self appendGroupNames: pedm.members.lampGroups toString: detailsString];
    }

    NSString *finalString;
    if (detailsString.length > 0)
    {
        finalString = [detailsString substringToIndex: detailsString.length - 2];
    }

    return finalString;
}

-(void)appendLampNames: (NSArray *)lampIDs toString: (NSMutableString *)detailsString
{
    LSFLampModelContainer *lampContainer = [LSFLampModelContainer getLampModelContainer];

    for (NSString *lampID in lampIDs)
    {
        LSFLampModel *lampModel = [lampContainer.lampContainer valueForKey: lampID];

        if (lampModel != nil)
        {
            [detailsString appendString: [NSString stringWithFormat: @"%@, ", lampModel.name]];
        }
    }
}

-(void)appendGroupNames: (NSArray *)groupIDs toString: (NSMutableString *)detailsString
{
    LSFGroupModelContainer *groupContainer = [LSFGroupModelContainer getGroupModelContainer];

    for (NSString *groupID in groupIDs)
    {
        LSFGroupModel *groupModel = [groupContainer.groupContainer valueForKey: groupID];

        if (groupModel != nil)
        {
            [detailsString appendString: [NSString stringWithFormat: @"%@, ", groupModel.name]];
        }
    }
}

-(NSString *)buildMasterSceneDetailsString: (LSFMasterSceneDataModel *)masterSceneModel
{
    NSMutableString *detailsString = [[NSMutableString alloc] init];
    [self appendSceneNames: masterSceneModel.masterScene.sceneIDs toString: detailsString];

    NSString *finalString;
    if (detailsString.length > 0)
    {
        finalString = [detailsString substringToIndex: detailsString.length - 2];
    }

    return finalString;
}

-(void)appendSceneNames: (NSArray *)sceneIDs toString: (NSMutableString *)detailsString
{
    LSFSceneModelContainer *sceneContainer = [LSFSceneModelContainer getSceneModelContainer];

    for (NSString *sceneID in sceneIDs)
    {
        LSFSceneDataModel *sceneModel = [sceneContainer.sceneContainer valueForKey: sceneID];

        if (sceneModel != nil)
        {
            [detailsString appendString: [NSString stringWithFormat: @"%@, ", sceneModel.name]];
        }
    }
}

-(NSUInteger)checkIfModelExists: (LSFDataModel *)sceneModel
{
    for (int i = 0; i < self.sceneData.count; i++)
    {
        LSFDataModel *model = [self.sceneData objectAtIndex: i];

        if ([sceneModel.theID isEqualToString: model.theID])
        {
            return i;
        }
    }

    return NSNotFound;
}

-(NSUInteger)findInsertionIndexInArray: (LSFDataModel *)sceneModel
{
    return [self.sceneData indexOfObject: sceneModel inSortedRange: (NSRange){0, [self.sceneData count]} options: (NSBinarySearchingFirstEqual | NSBinarySearchingInsertionIndex) usingComparator:
            ^NSComparisonResult(id a, id b) {
                NSString *first = [(LSFModel *)a name];
                NSString *second = [(LSFModel *)b name];
                return [first localizedCaseInsensitiveCompare: second];
            }];
}

-(void)sortScenesByName
{
    NSMutableArray *sortedArray = [NSMutableArray arrayWithArray: [self.sceneData sortedArrayUsingComparator: ^NSComparisonResult(id a, id b) {
        NSString *first = [(LSFModel *)a name];
        NSString *second = [(LSFModel *)b name];
        return [first localizedCaseInsensitiveCompare: second];
    }]];

    self.sceneData = [NSMutableArray arrayWithArray: sortedArray];
}

-(int)findIndexInModelOfID: (NSString *)theID
{
    for (int i = 0; i < self.sceneData.count; i++)
    {
        LSFDataModel *model = [self.sceneData objectAtIndex: i];

        if ([theID isEqualToString: model.theID])
        {
            return i;
        }
    }

    return -1;
}

-(void)addObjectToTableAtIndex: (NSUInteger)insertIndex
{
    NSLog(@"Add: Index = %u", insertIndex);

    NSArray *insertIndexPaths = [NSArray arrayWithObjects: [NSIndexPath indexPathForRow: insertIndex inSection:0], nil];

    [self.tableView beginUpdates];
    [self.tableView insertRowsAtIndexPaths: insertIndexPaths withRowAnimation: UITableViewRowAnimationFade];
    [self.tableView endUpdates];
}

-(void)moveObjectFromIndex: (NSUInteger)fromIndex toIndex: (NSUInteger)toIndex
{
    NSLog(@"Move: FromIndex = %u; ToIndex = %u", fromIndex, toIndex);

    NSIndexPath *fromIndexPath = [NSIndexPath indexPathForRow: fromIndex inSection: 0];
    NSIndexPath *toIndexPath = [NSIndexPath indexPathForRow: toIndex inSection: 0];
    [self.tableView moveRowAtIndexPath: fromIndexPath toIndexPath: toIndexPath];

    [self refreshRowInTableAtIndex: toIndex];
}

-(void)refreshRowInTableAtIndex: (NSUInteger)index
{
    NSLog(@"Refresh: Index = %u", index);

    NSArray *refreshIndexPaths = [NSArray arrayWithObjects: [NSIndexPath indexPathForRow: index inSection:0], nil];
    [self.tableView reloadRowsAtIndexPaths: refreshIndexPaths withRowAnimation: UITableViewRowAnimationNone];
}

-(void)deleteRowsInTableAtIndex: (NSArray *)cellIndexPaths
{
    [self.tableView deleteRowsAtIndexPaths: cellIndexPaths withRowAnimation: UITableViewRowAnimationFade];
}

@end
