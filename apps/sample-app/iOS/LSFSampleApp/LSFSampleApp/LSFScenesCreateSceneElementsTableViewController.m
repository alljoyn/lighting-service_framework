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

#import "LSFScenesCreateSceneElementsTableViewController.h"
#import "LSFScenesMembersTableViewController.h"
#import "LSFLampModelContainer.h"
#import "LSFGroupModelContainer.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"
#import "LSFAllJoynManager.h"
#import "LSFEnums.h"

@interface LSFScenesCreateSceneElementsTableViewController ()

@property (nonatomic, strong) UIBarButtonItem *addButton;
@property (nonatomic, strong) UIBarButtonItem *saveButton;
@property (nonatomic, strong) NSMutableArray *dataArray;

-(void)controllerNotificationReceived: (NSNotification *)notification;
-(void)plusButtonPressed;
-(void)saveButtonPressed;
-(NSString *)buildMemberString: (LSFSceneElementDataModel *)sceneElement;

@end

@implementation LSFScenesCreateSceneElementsTableViewController

@synthesize addButton = _addButton;
@synthesize saveButton = _saveButton;
@synthesize dataArray = _dataArray;

-(void)viewDidLoad
{
    [super viewDidLoad];

    self.dataArray = [[NSMutableArray alloc] init];

    self.addButton = [[UIBarButtonItem alloc]
                      initWithBarButtonSystemItem:UIBarButtonSystemItemAdd
                      target: self
                      action: @selector(plusButtonPressed)];

    self.saveButton = [[UIBarButtonItem alloc]
                       initWithBarButtonSystemItem: UIBarButtonSystemItemSave
                       target: self
                       action: @selector(saveButtonPressed)];

    NSArray *actionButtonItems = @[self.saveButton, self.addButton];
    self.navigationItem.rightBarButtonItems = actionButtonItems;
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    //Set notification handler
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(controllerNotificationReceived:) name: @"ControllerNotification" object: nil];

    [self.dataArray removeAllObjects];

    if (self.sceneModel != nil)
    {
        [self.dataArray addObjectsFromArray: self.sceneModel.noEffects];
        [self.dataArray addObjectsFromArray: self.sceneModel.transitionEffects];
        [self.dataArray addObjectsFromArray: self.sceneModel.pulseEffects];
    }

    [self.tableView reloadData];
}

-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear: animated];

    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
    if (!ajManager.isConnectedToController)
    {
        [self dismissViewControllerAnimated: YES completion: nil];
    }
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
        [self dismissViewControllerAnimated: YES completion: nil];
    }
}

/*
 * UITableViewDataSource Implementation
 */
-(NSInteger)numberOfSectionsInTableView: (UITableView *)tableView
{
    return 1;
}

-(NSString *)tableView: (UITableView *)tableView titleForHeaderInSection: (NSInteger)section
{
    return @"Tap + to add a new scene element";
}

-(NSInteger)tableView: (UITableView *)tableView numberOfRowsInSection: (NSInteger)section
{
    return self.dataArray.count;
}

-(UITableViewCell *)tableView: (UITableView *)tableView cellForRowAtIndexPath: (NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: @"SceneElement" forIndexPath: indexPath];

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

-(BOOL)tableView: (UITableView *)tableView canEditRowAtIndexPath: (NSIndexPath *)indexPath
{
    return NO;
}

/*
 * Segue Function
 */
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString: @"AddSceneElement"])
    {
        UINavigationController *nc = (UINavigationController *)[segue destinationViewController];
        LSFScenesMembersTableViewController *smtvc = (LSFScenesMembersTableViewController *)nc.topViewController;
        smtvc.sceneModel = self.sceneModel;
        smtvc.usesCancel = YES;
    }
}

/*
 * Private Functions
 */
-(void)plusButtonPressed
{
    [self performSegueWithIdentifier: @"AddSceneElement" sender: self];
}

-(void)saveButtonPressed
{
    if ((self.sceneModel.noEffects.count + self.sceneModel.transitionEffects.count + self.sceneModel.pulseEffects.count) > 0)
    {
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.lsfSceneManager createScene: [self.sceneModel toScene] andSceneName: self.sceneModel.name];

        [self dismissViewControllerAnimated: YES completion: nil];
    }
    else
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"No Scene Elements"
                                                        message: [NSString stringWithFormat: @"You must create at least one scene element before saving the scene"]
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];
    }
}

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

@end
