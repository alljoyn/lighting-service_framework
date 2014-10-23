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

#import "LSFGroupsTableViewController.h"
#import "LSFGroupsInfoTableViewController.h"
#import "LSFGroupsAddNameViewController.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFGroupsCell.h"
#import "LSFGroupModelContainer.h"
#import "LSFGroupModel.h"
#import "LSFSceneModelContainer.h"
#import "LSFSceneDataModel.h"
#import "LSFNoEffectDataModel.h"
#import "LSFTransitionEffectDataModel.h"
#import "LSFPulseEffectDataModel.h"
#import "LSFConstants.h"
#import "LSFWifiMonitor.h"
#import "LSFEnums.h"

@interface LSFGroupsTableViewController ()

@property (nonatomic, strong) UIBarButtonItem *myEditButton;
@property (nonatomic, strong) UIBarButtonItem *settingsButton;
@property (nonatomic, strong) UIBarButtonItem *addButton;

-(void)groupNotificationReceived: (NSNotification *)notification;
-(void)addNewGroup: (NSString *)groupID;
-(void)reorderGroup: (NSString *)groupID;
-(void)refreshGroup: (NSString *)groupID;
-(void)removeGroup: (NSArray *)groupIDs;
-(void)plusButtonPressed;
-(void)settingsButtonPressed;
-(NSUInteger)checkIfGroupModelExists: (LSFGroupModel *)groupModel;
-(NSUInteger)findInsertionIndexInArray: (LSFGroupModel *)groupModel;
-(void)sortGroupsByName;
-(void)refreshRowInTableAtIndex: (NSUInteger)index;
-(NSArray *)isGroupContainedInGroupsOrScenes: (NSString *)groupID;

@end

@implementation LSFGroupsTableViewController

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
    
    //Set groups notification handler
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(groupNotificationReceived:) name: @"GroupNotification" object: nil];
    
    //Set the content of the default group data array
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    self.data = [[NSMutableArray alloc] initWithArray: [container.groupContainer allValues]];

    if (self.data.count > 0)
    {
        [self sortGroupsByName];
    }

    //Reload the table
    [self.tableView reloadData];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    //Clear groups notification handler
    [[NSNotificationCenter defaultCenter] removeObserver: self];
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * GroupNotification Handler
 */
-(void)groupNotificationReceived: (NSNotification *)notification
{
    @synchronized(self.data)
    {
        NSString *groupID = [notification.userInfo valueForKey: @"groupID"];
        NSNumber *callbackOp = [notification.userInfo valueForKey: @"operation"];
        NSArray *groupIDs = [notification.userInfo valueForKey: @"groupIDs"];

        switch (callbackOp.intValue)
        {
            case GroupCreated:
                [self addNewGroup: groupID];
                break;
            case GroupNameUpdated:
                [self reorderGroup: groupID];
                break;
            case GroupStateUpdated:
                [self refreshGroup: groupID];
                break;
            case GroupDeleted:
                [self removeGroup: groupIDs];
                break;
            default:
                NSLog(@"Operation not found - Taking no action");
                break;
        }
    }
}

-(void)addNewGroup: (NSString *)groupID
{
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = container.groupContainer;
    LSFGroupModel *model = [groups valueForKey: groupID];

    long long currentTmestamp = (long long)([[NSDate date] timeIntervalSince1970] * 1000);

    if ((model.timestamp + model.delay) <= currentTmestamp)
    {
        @synchronized (model)
        {
            [self.data addObject: model];
            [self sortGroupsByName];
            [self.tableView reloadData];
        }
    }
}

-(void)refreshGroup: (NSString *)groupID
{
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = container.groupContainer;
    LSFGroupModel *model = [groups valueForKey: groupID];

    long long currentTmestamp = (long long)([[NSDate date] timeIntervalSince1970] * 1000);

    if ((model.timestamp + model.delay) <= currentTmestamp)
    {
        @synchronized (model)
        {
            if (model != nil)
            {
                NSUInteger existingIndex = [self findIndexInModelOfID: groupID];

                if (existingIndex != -1)
                {
                    [self refreshRowInTableAtIndex: existingIndex];
                }
            }
            else
            {
                NSLog(@"LSFGroupsTableViewController - refreshLamp() model is nil");
            }
        }
    }
}

-(void)reorderGroup: (NSString *)groupID
{
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = container.groupContainer;
    LSFGroupModel *model = [groups valueForKey: groupID];

    long long currentTmestamp = (long long)([[NSDate date] timeIntervalSince1970] * 1000);

    if ((model.timestamp + model.delay) <= currentTmestamp)
    {
        @synchronized (model)
        {
            if (model != nil)
            {
                [self sortGroupsByName];
                [self.tableView reloadData];
            }
            else
            {
                NSLog(@"LSFGroupsTableViewController - reorderLamp() model is nil");
            }
        }
    }
}

-(void)removeGroup: (NSArray *)groupIDs
{
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = container.groupContainer;

    for (NSString *groupID in groupIDs)
    {
        LSFGroupModel *model = [groups valueForKey: groupID];

        if (model == nil)
        {
            int modelArrayIndex = [self findIndexInModelOfID: groupID];

            if (modelArrayIndex != -1)
            {
                [self.data removeObjectAtIndex: modelArrayIndex];
                [self.tableView reloadData];
            }
        }
        else
        {
            NSLog(@"LSFGroupsTableViewController - removeLamp() model is not nil");
        }
    }
}

/*
 * UITableViewDataSource Protocol Implementation
 */
-(NSInteger)numberOfSectionsInTableView: (UITableView *)tableView
{
    return 1;
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    LSFWifiMonitor *wifiMonitor = [LSFWifiMonitor getWifiMonitor];
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
    LSFConstants *constants = [LSFConstants getConstants];

    if (!wifiMonitor.isWifiConnected)
    {
        CGRect frame = CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height);
        UIView *customView = [[UIView alloc] initWithFrame: frame];
        UILabel *messageLabel = [[UILabel alloc] init];
        [customView addSubview: messageLabel];

        frame.origin.x = 30;
        frame.size.width = self.view.bounds.size.width - frame.origin.x;

        messageLabel.frame = frame;
        messageLabel.text = [NSString stringWithFormat: @"No Wi-Fi Connection.\n\nPlease check that Wi-Fi is enabled in this device's settings and that the device is connected to a Wi-Fi network.\n\nWaiting for Wi-Fi to become available..."];
        messageLabel.textColor = [UIColor blackColor];
        messageLabel.numberOfLines = 0;
        messageLabel.textAlignment = NSTextAlignmentLeft;

        self.tableView.backgroundView = customView;
        return 0;
    }
    else if (!ajManager.isConnectedToController)
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
        self.tableView.backgroundView = nil;
        return self.data.count;
    }
}


-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    LSFGroupModel *data = [self.data objectAtIndex: [indexPath row]];

//    NSLog(@"Group Model State - %@", data.name);
//    NSLog(@"OnOff = %@", data.state.onOff ? @"On" : @"Off");
//    NSLog(@"Brightness = %u", data.state.brightness);
//    NSLog(@"Hue = %u", data.state.hue);
//    NSLog(@"Saturation = %u", data.state.saturation);
//    NSLog(@"Color Temp = %u", data.state.colorTemp);

    LSFGroupsCell *cell = [tableView dequeueReusableCellWithIdentifier: @"GroupCell" forIndexPath:indexPath];
    cell.groupID = data.theID;
    cell.nameLabel.text = data.name;
    
    if (data.capability.dimmable > NONE)
    {
        if (data.state.onOff && data.state.brightness == 0)
        {
            dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
                LSFLampGroupManager *groupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
                [groupManager transitionLampGroupID: cell.groupID onOffField: NO];
            });
        }
        
        cell.brightnessSlider.enabled = YES;
        cell.brightnessSlider.value = data.state.brightness;
        cell.brightnessSliderButton.enabled = NO;

        if (data.uniformity.brightness)
        {
            [cell.brightnessSlider setThumbImage: [UIImage imageNamed: @"power_slider_normal_icon.png"] forState: UIControlStateNormal];
            [cell.brightnessSlider setThumbImage: [UIImage imageNamed: @"power_slider_pressed_icon.png"] forState: UIControlStateHighlighted];
        }
        else
        {
            [cell.brightnessSlider setThumbImage: [UIImage imageNamed: @"power_slider_midstate_icon.png"] forState: UIControlStateNormal];
            [cell.brightnessSlider setThumbImage: [UIImage imageNamed: @"power_slider_pressed_icon.png"] forState: UIControlStateHighlighted];
        }
    }
    else
    {
        cell.brightnessSlider.enabled = NO;
        cell.brightnessSlider.value = 0;
        cell.brightnessSliderButton.enabled = YES;
    }

    if (data.state.onOff)
    {
        if (data.uniformity.power)
        {
            cell.powerImage.image = [UIImage imageNamed: @"light_power_btn_on.png"];
        }
        else
        {
            cell.powerImage.image = [UIImage imageNamed: @"light_power_btn_other.png"];
        }
    }
    else
    {
        cell.powerImage.image = [UIImage imageNamed: @"light_power_btn_off.png"];
    }

    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 70;
}

-(CGFloat) tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 0.00000000001f;
}

-(BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ([indexPath row] == 0)
    {
        return NO;
    }
    else
    {
        return YES;
    }
}

-(UITableViewCellEditingStyle)tableView: (UITableView *)tableView editingStyleForRowAtIndexPath: (NSIndexPath *)indexPath
{
    if (self.tableView.editing)
    {
        return UITableViewCellEditingStyleDelete;
    }

    return UITableViewCellEditingStyleNone;
}

-(void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete)
    {
        LSFGroupModel *data = [self.data objectAtIndex: [indexPath row]];

        NSArray *names = [self isGroupContainedInGroupsOrScenes: data.theID];
        if (names.count > 0)
        {
            NSMutableString *nameString = [[NSMutableString alloc] init];

            for (int i = 0; i < names.count; i++)
            {
                [nameString appendString: [NSString stringWithFormat: @"\"%@\", ", [names objectAtIndex: i]]];

                if (i == (names.count - 2))
                {
                    [nameString appendString: @"and "];
                }
            }

            [nameString deleteCharactersInRange: NSMakeRange(nameString.length - 2, 2)];

            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Can't Delete Group"
                                                            message: [NSString stringWithFormat: @"The group \"%@\" is being used by the following groups or scenes: %@. It cannot be deleted until it is removed from those items, or those items are deleted.", data.name, nameString]
                                                           delegate: nil
                                                  cancelButtonTitle: @"OK"
                                                  otherButtonTitles: nil];
            [alert show];
        }
        else
        {
            int modelArrayIndex = [self findIndexInModelOfID: data.theID];
            [self.data removeObjectAtIndex: modelArrayIndex];

            // Delete the row from the data source
            [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];

            dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
                LSFLampGroupManager *groupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
                [groupManager deleteLampGroupWithID: data.theID];
            });
        }
    }
}

/*
 * Segue method
 */
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString: @"GroupInfo"])
    {
        NSIndexPath *indexPath = [self.tableView indexPathForCell: sender];
        
        LSFGroupsInfoTableViewController *gitvc = [segue destinationViewController];
        gitvc.groupID = [NSString stringWithString: ((LSFGroupModel *)[self.data objectAtIndex: [indexPath row]]).theID];
    }
}

/*
 * Private functions
 */
-(void)plusButtonPressed
{
    LSFWifiMonitor *wifiMonitor = [LSFWifiMonitor getWifiMonitor];
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    if (wifiMonitor.isWifiConnected && ajManager.isConnectedToController)
    {
        [self performSegueWithIdentifier: @"AddGroup" sender: nil];
    }
    else
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                        message: @"Must be connected to a controller before creating a group."
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];
    }
};

-(void)settingsButtonPressed
{
    [self performSegueWithIdentifier: @"GroupsSettings" sender: self];
}

-(NSUInteger)checkIfGroupModelExists: (LSFGroupModel *)groupModel
{
    for (int i = 0; i < self.data.count; i++)
    {
        LSFGroupModel *model = (LSFGroupModel *)[self.data objectAtIndex: i];

        if ([groupModel.theID isEqualToString: model.theID])
        {
            return i;
        }
    }

    return NSNotFound;
}

-(NSUInteger)findInsertionIndexInArray: (LSFGroupModel *)groupModel
{
    NSUInteger insertionIndex = 1;

    if (self.data.count <= 1)
    {
        NSLog(@"Group Data count is 1 or less so returning 1");
        insertionIndex = 1;
    }
    else
    {
        for (int i = 1; i < self.data.count; i++)
        {
            LSFGroupModel *model = (LSFGroupModel *)[self.data objectAtIndex: i];
            NSLog(@"GroupModel Name = %@; Model Name = %@", groupModel.name, model.name);

            if ([model.name localizedCaseInsensitiveCompare: groupModel.name] == NSOrderedAscending || [model.name localizedCaseInsensitiveCompare: groupModel.name] == NSOrderedSame)
            {
                NSLog(@"InsertionIndex = %i", i);
                insertionIndex = i;
                break;
            }
        }
    }

    return insertionIndex;
}

-(void)sortGroupsByName
{
    NSMutableArray *sortedArray = [NSMutableArray arrayWithArray: [self.data sortedArrayUsingComparator: ^NSComparisonResult(id a, id b) {
        NSString *first = [(LSFGroupModel *)a name];
        NSString *second = [(LSFGroupModel *)b name];
        return [first localizedCaseInsensitiveCompare: second];
    }]];
    
    LSFGroupModel *allLampsGroupModel;
    for (LSFGroupModel *model in sortedArray)
    {
        if ([model.name isEqualToString: @"All Lamps"])
        {
            allLampsGroupModel = model;
            [sortedArray removeObject: model];
            break;
        }
    }

    if (allLampsGroupModel != nil)
    {
        [sortedArray insertObject: allLampsGroupModel atIndex: 0];
    }
    
    self.data = [NSMutableArray arrayWithArray: sortedArray];
}

-(int)findIndexInModelOfID: (NSString *)theID
{
    for (int i = 0; i < self.data.count; i++)
    {
        LSFGroupModel *model = [self.data objectAtIndex: i];

        if ([theID isEqualToString: model.theID])
        {
            return i;
        }
    }

    return -1;
}

-(void)refreshRowInTableAtIndex: (NSUInteger)index
{
    //NSLog(@"Refresh: Index = %u", index);
    NSArray *refreshIndexPaths = [NSArray arrayWithObjects: [NSIndexPath indexPathForRow: index inSection:0], nil];

    [self.tableView beginUpdates];
    [self.tableView reloadRowsAtIndexPaths: refreshIndexPaths withRowAnimation: UITableViewRowAnimationNone];
    [self.tableView endUpdates];
}

-(NSArray *)isGroupContainedInGroupsOrScenes: (NSString *)groupID
{
    NSMutableArray *names = [[NSMutableArray alloc] init];

    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableArray *groups = [[NSMutableArray alloc] initWithArray: [container.groupContainer allValues]];

    for (LSFGroupModel *model in groups)
    {
        if (![groupID isEqualToString: model.theID])
        {
            if ([model.members.lampGroups containsObject: groupID])
            {
                NSLog(@"Group %@ contains group ID %@", model.name, groupID);
                [names addObject: [NSString stringWithString: model.name]];
            }
        }
    }

    LSFSceneModelContainer *sceneContainer = [LSFSceneModelContainer getSceneModelContainer];
    NSMutableArray *scenes = [[NSMutableArray alloc] initWithArray: [sceneContainer.sceneContainer allValues]];

    for (LSFSceneDataModel *model in scenes)
    {
        for (LSFNoEffectDataModel *nedm in model.noEffects)
        {
            if ([nedm.members.lampGroups containsObject: groupID])
            {
                [names addObject: [NSString stringWithString: model.name]];
            }
        }

        for (LSFTransitionEffectDataModel *tedm in model.transitionEffects)
        {
            if ([tedm.members.lampGroups containsObject: groupID])
            {
                [names addObject: [NSString stringWithString: model.name]];
            }
        }

        for (LSFPulseEffectDataModel *pedm in model.pulseEffects)
        {
            if ([pedm.members.lampGroups containsObject: groupID])
            {
                [names addObject: [NSString stringWithString: model.name]];
            }
        }
    }

    return names;
}

@end
