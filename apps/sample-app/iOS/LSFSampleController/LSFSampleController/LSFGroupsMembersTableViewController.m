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

@interface LSFGroupsMembersTableViewController ()

@property (nonatomic, strong) LSFGroupModel *groupModel;
@property (nonatomic, strong) LSFLampGroup *lampGroup;
@property (nonatomic, strong) NSMutableArray *lampsGroupsArray;
@property (nonatomic, strong) NSMutableArray *selectedRows;

-(void)buildTableArray;
-(void)modifyAllRows: (BOOL)isSelected;
-(void)processSelectedRows;
-(void)checkGroupCapability: (LSFCapabilityData *)capability;
-(void)createLampGroup;

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
    
    //NSLog(@"Set Group ID = %@", self.groupID);
    
    //Initialize selected rows array
    self.selectedRows = [[NSMutableArray alloc] init];
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager setReloadUIDelegate: self];
    });
    
    //Grab the group model using the passed in group ID
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = container.groupContainer;
    self.groupModel = [groups valueForKey: self.groupID];
    
    //Load UI
    [self buildTableArray];
    [self.tableView reloadData];
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LSFReloadUIDelegate Implementation
 */
-(void)reloadUI
{
    // Empty Callback
}

#pragma mark - Table view data source
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
    
    self.lampsGroupsArray = [NSMutableArray arrayWithArray: groupsArray];
    [self.lampsGroupsArray addObjectsFromArray: lampsArray];
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
//    NSLog(@"Printing lamp and group IDs that the user checked");
//    for (NSString *lampID in self.lampGroup.lamps)
//    {
//        NSLog(@"%@", lampID);
//    }
//    for (NSString *groupID in self.lampGroup.lampGroups)
//    {
//        NSLog(@"%@", groupID);
//    }

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

/*
 * UIAlertViewDelegate implementation
 */
-(void)alertView: (UIAlertView *)alertView clickedButtonAtIndex: (NSInteger)buttonIndex
{
    if (buttonIndex == 0)
    {
        [alertView dismissWithClickedButtonIndex: 0 animated: YES];
    }
    
    if (buttonIndex == 1)
    {
        [alertView dismissWithClickedButtonIndex: 1 animated: YES];
        [self createLampGroup];
    }
}

@end
