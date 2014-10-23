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

#import "LSFSceneInfoTableViewController.h"
#import "LSFScenesMembersTableViewController.h"
#import "LSFScenesModifyMembersTableViewController.h"
#import "LSFScenesChangeNameViewController.h"
#import "LSFLampModelContainer.h"
#import "LSFGroupModelContainer.h"
#import "LSFSceneModelContainer.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"
#import "LSFSceneDataModel.h"
#import "LSFSceneElementDataModel.h"
#import "LSFNoEffectDataModel.h"
#import "LSFTransitionEffectDataModel.h"
#import "LSFPulseEffectDataModel.h"
#import "LSFAllJoynManager.h"
#import "LSFDispatchQueue.h"
#import "LSFEnums.h"

@interface LSFSceneInfoTableViewController ()

@property (nonatomic, strong) LSFSceneDataModel *sceneModel;
@property (nonatomic, strong) NSMutableArray *dataArray;
@property (nonatomic, strong) UIBarButtonItem *addButton;

-(void)controllerNotificationReceived: (NSNotification *)notification;
-(void)sceneNotificationReceived: (NSNotification *)notification;
-(void)reloadSceneWithID: (NSString *)sceneID;
-(void)deleteScenesWithIDs: (NSArray *)sceneIDs andNames: (NSArray *)sceneNames;
-(NSString *)buildMemberString: (LSFSceneElementDataModel *)sceneElement;
-(void)plusButtonPressed;

@end

@implementation LSFSceneInfoTableViewController

@synthesize sceneID = _sceneID;
@synthesize sceneModel = _sceneModel;
@synthesize dataArray = _dataArray;
@synthesize addButton = _addButton;

-(void)viewDidLoad
{
    [super viewDidLoad];

    self.dataArray = [[NSMutableArray alloc] init];

    self.addButton = [[UIBarButtonItem alloc]
                      initWithBarButtonSystemItem:UIBarButtonSystemItemAdd
                      target: self
                      action: @selector(plusButtonPressed)];

    NSArray *actionButtonItems = @[self.editButtonItem, self.addButton];
    self.navigationItem.rightBarButtonItems = actionButtonItems;
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    if (!ajManager.isConnectedToController)
    {
        [self.navigationController popToRootViewControllerAnimated: YES];
    }

    //Set notification handler
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(controllerNotificationReceived:) name: @"ControllerNotification" object: nil];
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(sceneNotificationReceived:) name: @"SceneNotification" object: nil];

    [self reloadSceneWithID: self.sceneID];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    //Clear notification handler
    [[NSNotificationCenter defaultCenter] removeObserver: self];
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * ControllerNotification Handler
 */
-(void)controllerNotificationReceived: (NSNotification *)notification
{
    NSDictionary *userInfo = notification.userInfo;
    NSNumber *controllerStatus = [userInfo valueForKey: @"status"];

    if (controllerStatus.intValue == Disconnected)
    {
        [self.navigationController popToRootViewControllerAnimated: YES];
    }
}

/*
 * SceneNotification Handler
 */
-(void)sceneNotificationReceived: (NSNotification *)notification
{
    NSString *sceneID = [notification.userInfo valueForKey: @"sceneID"];
    NSNumber *callbackOp = [notification.userInfo valueForKey: @"operation"];
    NSArray *sceneIDs = [notification.userInfo valueForKey: @"sceneIDs"];
    NSArray *sceneNames = [notification.userInfo valueForKey: @"sceneNames"];

    if ([self.sceneID isEqualToString: sceneID] || [sceneIDs containsObject: self.sceneID])
    {
        switch (callbackOp.intValue)
        {
            case SceneNameUpdated:
            case SceneUpdated:
                [self reloadSceneWithID: sceneID];
                break;
            case SceneDeleted:
                [self deleteScenesWithIDs: sceneIDs andNames: sceneNames];
                break;
            default:
                break;
        }
    }
}

-(void)reloadSceneWithID: (NSString *)sceneID
{
    if ([self.sceneID isEqualToString: sceneID])
    {
        [self.dataArray removeAllObjects];

        LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
        NSMutableDictionary *sceneDictionary = container.sceneContainer;
        self.sceneModel = [sceneDictionary valueForKey: self.sceneID];

        if (self.sceneModel != nil)
        {
            [self.dataArray addObjectsFromArray: self.sceneModel.noEffects];
            [self.dataArray addObjectsFromArray: self.sceneModel.transitionEffects];
            [self.dataArray addObjectsFromArray: self.sceneModel.pulseEffects];
        }
        else
        {
            [self.navigationController popToRootViewControllerAnimated: YES];
        }
        
        [self.tableView reloadData];
    }
}

-(void)deleteScenesWithIDs: (NSArray *)sceneIDs andNames: (NSArray *)sceneNames
{
    if ([sceneIDs containsObject: self.sceneID])
    {
        int index = [sceneIDs indexOfObject: self.sceneID];

        [self.navigationController popToRootViewControllerAnimated: YES];

        dispatch_async(dispatch_get_main_queue(), ^{
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Scene Not Found"
                                                            message: [NSString stringWithFormat: @"The scene \"%@\" no longer exists.", [sceneNames objectAtIndex: index]]
                                                           delegate: nil
                                                  cancelButtonTitle: @"OK"
                                                  otherButtonTitles: nil];
            [alert show];
        });
    }
}

/*
 * UITableViewDataSource Implementation
 */
-(NSInteger)numberOfSectionsInTableView: (UITableView *)tableView
{
    return 2;
}

-(NSInteger)tableView: (UITableView *)tableView numberOfRowsInSection: (NSInteger)section
{
    if (section == 0)
    {
        return 1;
    }
    else
    {
        return (self.sceneModel.noEffects.count + self.sceneModel.transitionEffects.count + self.sceneModel.pulseEffects.count);
    }
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 0)
    {
        UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle: UITableViewCellStyleValue1 reuseIdentifier: nil];
        cell.textLabel.text = @"Scene Name";
        cell.detailTextLabel.text = self.sceneModel.name;
        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        return cell;
    }
    else
    {
        UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: @"SceneElementCell" forIndexPath: indexPath];

        id sceneElement = [self.dataArray objectAtIndex: indexPath.row];
        cell.textLabel.text = [self buildMemberString: (LSFSceneElementDataModel *)sceneElement];

        if ([sceneElement isKindOfClass: [LSFNoEffectDataModel class]])
        {
            cell.imageView.image = [UIImage imageNamed: @"list_constant_icon.png"];
            cell.detailTextLabel.text = @"";
        }
        else if ([sceneElement isKindOfClass: [LSFTransitionEffectDataModel class]])
        {
            cell.imageView.image = [UIImage imageNamed: @"list_transition_icon.png"];
            cell.detailTextLabel.text = @"Transition";
        }
        else if ([sceneElement isKindOfClass: [LSFPulseEffectDataModel class]])
        {
            cell.imageView.image = [UIImage imageNamed: @"list_pulse_icon.png"];
            cell.detailTextLabel.text = @"Pulse";
        }

        return cell;
    }
}

-(NSString *)tableView: (UITableView *)tableView titleForHeaderInSection: (NSInteger)section
{
    if (section == 1)
    {
        return @"Scene Elements";
    }

    return @"";
}

-(BOOL)tableView: (UITableView *)tableView canEditRowAtIndexPath: (NSIndexPath *)indexPath
{
    if (indexPath.section == 1)
    {
        return YES;
    }
    else
    {
        return NO;
    }
}

-(void)tableView: (UITableView *)tableView commitEditingStyle: (UITableViewCellEditingStyle)editingStyle forRowAtIndexPath: (NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete)
    {
        LSFSceneElementDataModel *data = [self.dataArray objectAtIndex: indexPath.row];
        BOOL removeSuccessful = [self.sceneModel removeElement: data.theID];

        if (removeSuccessful)
        {
            //NSLog(@"Removing scene element for SceneDataModel was succesful");

            dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
                LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
                [ajManager.lsfSceneManager updateSceneWithID: self.sceneModel.theID withScene: [self.sceneModel toScene]];
            });

            // Delete the row from the data source
            [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation: UITableViewRowAnimationFade];
        }
    }
}

-(void)tableView: (UITableView *)tableView didSelectRowAtIndexPath: (NSIndexPath *)indexPath
{
    if (indexPath.section == 1)
    {
        [self performSegueWithIdentifier: @"ModifySceneElement" sender: indexPath];
    }
    else
    {
        [self performSegueWithIdentifier: @"ChangeSceneName" sender: self];
    }
}

-(NSString *)tableView: (UITableView *)tableView titleForFooterInSection: (NSInteger)section
{
    if (section == 1)
    {
        return @"Tap + to add a new scene element";
    }

    return @"";
}

/*
 * Segue Function
 */
-(void)prepareForSegue: (UIStoryboardSegue *)segue sender: (id)sender
{
    if ([segue.identifier isEqualToString: @"CreateSceneElement"])
    {
        UINavigationController *nc = (UINavigationController *)[segue destinationViewController];
        LSFScenesMembersTableViewController *smtvc = (LSFScenesMembersTableViewController *)nc.topViewController;
        smtvc.sceneModel = self.sceneModel;
        smtvc.usesCancel = YES;
    }
    else if ([segue.identifier isEqualToString: @"ModifySceneElement"])
    {
        NSIndexPath *indexPath = (NSIndexPath *)sender;

        UINavigationController *nc = (UINavigationController *)[segue destinationViewController];
        LSFScenesModifyMembersTableViewController *smmtvc = (LSFScenesModifyMembersTableViewController *)nc.topViewController;
        smmtvc.sceneModel = self.sceneModel;
        smmtvc.sceneElement = [self.dataArray objectAtIndex: indexPath.row];
    }
    else if ([segue.identifier isEqualToString: @"ChangeSceneName"])
    {
        LSFScenesChangeNameViewController *scnvc = [segue destinationViewController];
        scnvc.sceneID = self.sceneID;
    }
}

/*
 * Private Functions
 */
-(NSString *)buildMemberString: (LSFSceneElementDataModel *)sceneElement
{
    BOOL firstNameAdded = NO;
    NSMutableString *memberString = [[NSMutableString alloc] init];

    LSFLampModelContainer *lampContainer = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lampsDictionary = lampContainer.lampContainer;

    for (int i = 0; !firstNameAdded && i < sceneElement.members.lamps.count; i++)
    {
        NSString *lampID = [sceneElement.members.lamps objectAtIndex: i];
        LSFLampModel *lampModel = [lampsDictionary valueForKey: lampID];

        if (lampModel != nil)
        {
            [memberString appendString: lampModel.name];
            firstNameAdded = YES;
        }
    }

    LSFGroupModelContainer *groupContainer = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groupsDictionary = groupContainer.groupContainer;

    for (int i = 0; !firstNameAdded && i < sceneElement.members.lampGroups.count; i++)
    {
        NSString *groupID = [sceneElement.members.lampGroups objectAtIndex: i];
        LSFGroupModel *groupModel = [groupsDictionary valueForKey: groupID];

        if (groupModel != nil)
        {
            [memberString appendString: groupModel.name];
            firstNameAdded = YES;
        }
    }

    int count = sceneElement.members.lamps.count + sceneElement.members.lampGroups.count - 1;

    if (count > 0)
    {
        [memberString appendString: [NSString stringWithFormat: @" (%i more)", count]];
    }

    return memberString;
}

-(void)plusButtonPressed
{
    [self performSegueWithIdentifier: @"CreateSceneElement" sender: self];
}

@end
