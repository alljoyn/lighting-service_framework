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

#import "LSFGroupsAddLampsTableViewController.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFGroupModelContainer.h"
#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"
#import "LSFCapabilityData.h"
#import "LSFEnums.h"

@interface LSFGroupsAddLampsTableViewController ()

@property (nonatomic, strong) LSFLampGroup *lampGroup;
@property (nonatomic, strong) NSMutableArray *lampsGroupsArray;
@property (nonatomic, strong) NSMutableArray *selectedRows;

-(void)controllerNotificationReceived: (NSNotification *)notification;
-(void)buildTableArray;
-(void)modifyAllRows: (BOOL)isSelected;
-(void)processSelectedRows;
-(void)checkGroupCapability: (LSFCapabilityData *)capability;
-(void)createLampGroup;
-(NSArray *)sortLampsGroupsData: (NSArray *)data;

@end

@implementation LSFGroupsAddLampsTableViewController

@synthesize groupName = _groupName;
@synthesize lampGroup = _lampGroup;
@synthesize lampsGroupsArray = _lampsGroupsArray;
@synthesize selectedRows = _selectedRows;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    //Set notification handler
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(controllerNotificationReceived:) name: @"ControllerNotification" object: nil];
    
    //Initialize selected rows array
    self.selectedRows = [[NSMutableArray alloc] init];
    
    //Load UI
    [self buildTableArray];
    [self.tableView reloadData];
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
 * UITableViewDatasource Protocol Implementation
 */
-(NSInteger)tableView: (UITableView *)tableView numberOfRowsInSection: (NSInteger)section
{
    return [self.lampsGroupsArray count];
}

-(UITableViewCell *)tableView: (UITableView *)tableView cellForRowAtIndexPath: (NSIndexPath *)indexPath
{
    id model = [self.lampsGroupsArray objectAtIndex: [indexPath row]];
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: @"AddLampsGroup" forIndexPath:indexPath];
    
    if ([model isKindOfClass: [LSFGroupModel class]])
    {
        cell.textLabel.text = ((LSFGroupModel *)model).name;
        cell.imageView.image = [UIImage imageNamed:@"groups_off_icon.png"];
        cell.accessoryType = UITableViewCellAccessoryNone;
    }
    if ([model isKindOfClass: [LSFLampModel class]])
    {
        cell.textLabel.text = ((LSFLampModel *)model).name;
        cell.imageView.image = [UIImage imageNamed:@"lamps_off_icon.png"];
        cell.accessoryType = UITableViewCellAccessoryNone;
    }
    
    return cell;
}

-(void)tableView: (UITableView *)tableView didSelectRowAtIndexPath: (NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
    
    if (cell.accessoryType == UITableViewCellAccessoryCheckmark)
    {
        cell.accessoryType = UITableViewCellAccessoryNone;
        [self.selectedRows removeObject: indexPath];
    }
    else
    {
        cell.accessoryType = UITableViewCellAccessoryCheckmark;
        [self.selectedRows addObject: indexPath];
    }
    
    [self.tableView deselectRowAtIndexPath: indexPath animated: YES];
}

-(NSString *)tableView: (UITableView *)tableView titleForHeaderInSection: (NSInteger)section
{
    return @"Select the lamps in this group";
}

/*
 * Button action handlers
 */
-(IBAction)selectAllButtonPressed: (id)sender
{
    [self modifyAllRows: YES];
}

-(IBAction)clearAllButtonPressed: (id)sender
{
    [self modifyAllRows: NO];
}

-(IBAction)doneButtonPressed: (id)sender
{
    if ([self.selectedRows count] == 0)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                        message: @"You must select at least one lamp or group to create a group."
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
 * Private functions
 */
-(void)buildTableArray
{
    LSFGroupModelContainer *groupContainer = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = groupContainer.groupContainer;
    NSMutableArray *groupsArray = [NSMutableArray arrayWithArray: [groups allValues]];
    
    for (LSFGroupModel *groupModel in groupsArray)
    {
        if ([groupModel.theID isEqualToString: @"!!all_lamps!!"])
        {
            [groupsArray removeObject: groupModel];
            break;
        }
    }
    
    LSFLampModelContainer *lampsContainer = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = lampsContainer.lampContainer;
    NSMutableArray *lampsArray = [NSMutableArray arrayWithArray: [lamps allValues]];
    
    self.lampsGroupsArray = [NSMutableArray arrayWithArray: [self sortLampsGroupsData: groupsArray]];
    [self.lampsGroupsArray addObjectsFromArray: [self sortLampsGroupsData: lampsArray]];
}

-(void)modifyAllRows: (BOOL)isSelected
{
    [self.selectedRows removeAllObjects];
    
    for (int i = 0; i < [self.tableView numberOfSections]; i++)
    {
        for (int j = 0; j < [self.tableView numberOfRowsInSection: i]; j++)
        {
            NSUInteger ints[2] = {i, j};
            NSIndexPath *indexPath = [NSIndexPath indexPathWithIndexes: ints length: 2];
            
            UITableViewCell *cell = [self.tableView cellForRowAtIndexPath: indexPath];
            
            if (isSelected)
            {
                cell.accessoryType = UITableViewCellAccessoryCheckmark;
                [self.selectedRows addObject: indexPath];
            }
            else
            {
                cell.accessoryType = UITableViewCellAccessoryNone;
            }
        }
    }
}

-(void)processSelectedRows
{
    LSFCapabilityData *capabilityData = [[LSFCapabilityData alloc] init];
    NSMutableArray *groupIDs = [[NSMutableArray alloc] init];
    NSMutableArray *lampIDs = [[NSMutableArray alloc] init];
    
    for (NSIndexPath *indexPath in self.selectedRows)
    {
        id model = [self.lampsGroupsArray objectAtIndex: indexPath.row];
        
        if ([model isKindOfClass: [LSFGroupModel class]])
        {
            [groupIDs addObject: ((LSFModel *)model).theID];
            [capabilityData includeData: ((LSFDataModel *)model).capability];
        }
        else if ([model isKindOfClass: [LSFLampModel class]])
        {
            [lampIDs addObject: ((LSFModel *)model).theID];
            [capabilityData includeData: ((LSFDataModel *)model).capability];
        }
    }
    
    self.lampGroup = [[LSFLampGroup alloc] init];
    self.lampGroup.lamps = lampIDs;
    self.lampGroup.lampGroups = groupIDs;
    
    [self checkGroupCapability: capabilityData];
}

-(void)checkGroupCapability: (LSFCapabilityData *)capability
{
    if ([capability isMixed])
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Mixing Lamp Types"
                                                        message: @"Warning! You are creating a group that contains lamps of different types. Some properties may not be supported for all lamps in the group.\n\nFor example, you may add both color and white lamps to a group. The Sample App will allow you to set the color for the group, but this won't have any effect on the white lamps."
                                                       delegate: self
                                              cancelButtonTitle: @"No"
                                              otherButtonTitles: @"Yes", nil];
        [alert show];
    }
    else
    {
        [self createLampGroup];
    }
}

-(void)createLampGroup
{
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampGroupManager *groupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        [groupManager createLampGroup: self.lampGroup withName: self.groupName];
    });
    
    [self dismissViewControllerAnimated: YES completion: nil];
}

-(NSArray *)sortLampsGroupsData: (NSArray *)data
{
    NSSortDescriptor *sortDesc = [NSSortDescriptor sortDescriptorWithKey:@"name" ascending:YES comparator:^NSComparisonResult(id obj1, id obj2) {
        return [(NSString *)obj1 compare:(NSString *)obj2 options:NSCaseInsensitiveSearch];
    }];


    return [data sortedArrayUsingDescriptors:[NSArray arrayWithObject:sortDesc]];
}

/*
 * UIAlertViewDelegate implementation
 */
-(void)alertView: (UIAlertView *)alertView clickedButtonAtIndex: (NSInteger)buttonIndex
{
    if (buttonIndex == 0)
    {
        [alertView dismissWithClickedButtonIndex: 0 animated: NO];
    }
    
    if (buttonIndex == 1)
    {
        [alertView dismissWithClickedButtonIndex: 1 animated: NO];
        [self createLampGroup];
    }
}

@end
