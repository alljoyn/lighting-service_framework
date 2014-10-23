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

#import "LSFGroupsMembersTableViewController.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFGroupModelContainer.h"
#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"
#import "LSFCapabilityData.h"
#import "LSFEnums.h"

@interface LSFGroupsMembersTableViewController ()

@property (nonatomic, strong) LSFGroupModel *groupModel;
@property (nonatomic, strong) LSFLampGroup *lampGroup;
@property (nonatomic, strong) NSMutableArray *lampsGroupsArray;
@property (nonatomic, strong) NSMutableArray *selectedRows;

-(void)controllerNotificationReceived: (NSNotification *)notification;
-(void)groupNotificationReceived: (NSNotification *)notification;
-(void)reloadGroupWithID: (NSString *)groupID;
-(void)deleteGroupsWithIDs: (NSArray *)groupIDs andNames: (NSArray *)groupNames;
-(void)buildTableArray;
-(void)modifyAllRows: (BOOL)isSelected;
-(void)processSelectedRows;
-(void)checkGroupCapability: (LSFCapabilityData *)capability;
-(void)createLampGroup;
-(BOOL)isParentGroup: (LSFGroupModel *)groupModel;
-(NSArray *)sortLampsGroupsData: (NSArray *)data;

@end

@implementation LSFGroupsMembersTableViewController

@synthesize groupID = _groupID;
@synthesize groupModel = _groupModel;
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
    
    //Initialize selected rows array
    self.selectedRows = [[NSMutableArray alloc] init];
    
    //Set notification handler
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(controllerNotificationReceived:) name: @"ControllerNotification" object: nil];
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(groupNotificationReceived:) name: @"GroupNotification" object: nil];
    
    //Grab the group model using the passed in group ID
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = container.groupContainer;
    self.groupModel = [groups valueForKey: self.groupID];
    
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
        [self dismissViewControllerAnimated: NO completion: nil];
    }
}

/*
 * GroupNotification Handler
 */
-(void)groupNotificationReceived: (NSNotification *)notification
{
    NSString *groupID = [notification.userInfo valueForKey: @"groupID"];
    NSNumber *callbackOp = [notification.userInfo valueForKey: @"operation"];
    NSArray *groupIDs = [notification.userInfo valueForKey: @"groupIDs"];
    NSArray *groupNames = [notification.userInfo valueForKey: @"groupNames"];

    if ([self.groupID isEqualToString: groupID] || [groupIDs containsObject: self.groupID])
    {
        switch (callbackOp.intValue)
        {
            case GroupCreated:
            case GroupNameUpdated:
            case GroupStateUpdated:
                [self reloadGroupWithID: groupID];
                break;
            case GroupDeleted:
                [self deleteGroupsWithIDs: groupIDs andNames: groupNames];
                break;
            default:
                NSLog(@"Operation not found - Taking no action");
                break;
        }
    }
}

-(void)reloadGroupWithID: (NSString *)groupID
{
    if ([self.groupID isEqualToString: groupID])
    {
        //Grab the group model using the passed in group ID
        LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
        NSMutableDictionary *groups = container.groupContainer;
        self.groupModel = [groups valueForKey: self.groupID];

        //Load UI
        [self buildTableArray];
        [self.tableView reloadData];
    }
}

-(void)deleteGroupsWithIDs: (NSArray *)groupIDs andNames: (NSArray *)groupNames
{
    if ([groupIDs containsObject: self.groupID])
    {
        int index = [groupIDs indexOfObject: self.groupID];

        [self dismissViewControllerAnimated: YES completion: nil];

        dispatch_async(dispatch_get_main_queue(), ^{
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Group Not Found"
                                                            message: [NSString stringWithFormat: @"The group \"%@\" no longer exists.", [groupNames objectAtIndex: index]]
                                                           delegate: nil
                                                  cancelButtonTitle: @"OK"
                                                  otherButtonTitles: nil];
            [alert show];
        });
    }
}

/*
 * UITableViewDataSource Protocol Implementation
 */
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [self.lampsGroupsArray count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    id model = [self.lampsGroupsArray objectAtIndex: [indexPath row]];
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: @"ModifyLampsGroup" forIndexPath:indexPath];
    
    if ([model isKindOfClass: [LSFGroupModel class]])
    {
        LSFGroupModel *gm = (LSFGroupModel *)model;
        cell.textLabel.text = gm.name;
        cell.imageView.image = [UIImage imageNamed:@"groups_off_icon.png"];
        
        if ([self.groupModel.members.lampGroups containsObject: gm.theID])
        {
            cell.accessoryType = UITableViewCellAccessoryCheckmark;
            [self.selectedRows addObject: indexPath];
        }
        else
        {
            cell.accessoryType = UITableViewCellAccessoryNone;
        }
    }
    if ([model isKindOfClass: [LSFLampModel class]])
    {
        LSFLampModel *lm = (LSFLampModel *)model;
        cell.textLabel.text = lm.name;
        cell.imageView.image = [UIImage imageNamed:@"lamps_off_icon.png"];
        
        if ([self.groupModel.members.lamps containsObject: lm.theID])
        {
            cell.accessoryType = UITableViewCellAccessoryCheckmark;
            [self.selectedRows addObject: indexPath];
        }
        else
        {
            cell.accessoryType = UITableViewCellAccessoryNone;
        }
    }
    
    return cell;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath: (NSIndexPath *)indexPath
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
 * Button Event Handlers
 */
-(IBAction)cancelButtonPressed: (id)sender
{
    [self dismissViewControllerAnimated: YES completion: nil];
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

-(IBAction)selectAllButtonPressed: (id)sender
{
    [self modifyAllRows: YES];
}

-(IBAction)clearAllButtonPressed: (id)sender
{
    [self modifyAllRows: NO];
}

/*
 * Private functions
 */
-(void)buildTableArray
{
    LSFGroupModelContainer *groupContainer = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = groupContainer.groupContainer;
    NSMutableArray *groupsArray = [NSMutableArray arrayWithArray: [groups allValues]];
    
    for (LSFGroupModel *groupModel in [groupsArray copy])
    {
        if ([groupModel.theID isEqualToString: self.groupModel.theID] || [groupModel.theID isEqualToString: @"!!all_lamps!!"] || [self isParentGroup:groupModel])
        {
            [groupsArray removeObject: groupModel];
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
        [groupManager updateLampGroupWithID: self.groupID andLampGroup: self.lampGroup];
    });
    
    [self dismissViewControllerAnimated: YES completion: nil];
}

-(BOOL)isParentGroup: (LSFGroupModel *)groupModel
{
    if (!([groupModel.theID isEqualToString: self.groupModel.theID]) && [[groupModel groups] containsObject:self.groupModel.theID])
    {
        NSLog(@"%@ is a parent of %@", groupModel.name, self.groupModel.name);
        return YES;
    }
    return NO;
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
