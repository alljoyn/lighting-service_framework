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

#import "LSFGroupsPresetsTableViewController.h"
#import "LSFGroupsCreatePresetViewController.h"
#import "LSFGroupModelContainer.h"
#import "LSFPresetModelContainer.h"
#import "LSFGroupModel.h"
#import "LSFPresetModel.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFConstants.h"
#import "LSFEnums.h"

@interface LSFGroupsPresetsTableViewController ()

@property (nonatomic, strong) LSFGroupModel *groupModel;
@property (nonatomic, strong) NSArray *presetData;
@property (nonatomic, strong) NSMutableArray *presetDataSorted;

-(void)controllerNotificationReceived: (NSNotification *)notification;
-(void)groupNotificationReceived: (NSNotification *)notification;
-(void)deleteGroupsWithIDs: (NSArray *)groupIDs andNames: (NSArray *)groupNames;
-(void)presetNotificationReceived: (NSNotification *)notification;
-(void)reloadPresets;
-(BOOL)checkIfLampStateMatchesPreset: (LSFPresetModel *)data;

@end

@implementation LSFGroupsPresetsTableViewController

@synthesize groupID = _groupID;
@synthesize groupModel = _groupModel;
@synthesize presetData = _presetData;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];
    self.navigationItem.rightBarButtonItem = self.editButtonItem;

    //Set notification handler
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(controllerNotificationReceived:) name: @"ControllerNotification" object: nil];
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(groupNotificationReceived:) name: @"GroupNotification" object: nil];
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(presetNotificationReceived:) name: @"PresetNotification" object: nil];

    [self reloadPresets];
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
            case GroupStateUpdated:
                [self reloadPresets];
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

-(void)deleteGroupsWithIDs: (NSArray *)groupIDs andNames: (NSArray *)groupNames
{
    int index = [groupIDs indexOfObject: self.groupID];

    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Group Not Found"
                                                    message: [NSString stringWithFormat: @"The group \"%@\" no longer exists.", [groupNames objectAtIndex: index]]
                                                   delegate: nil
                                          cancelButtonTitle: @"OK"
                                          otherButtonTitles: nil];
    [alert show];

    [self.navigationController popToRootViewControllerAnimated: YES];
}

/*
 * PresetNotification Handler
 */
-(void)presetNotificationReceived: (NSNotification *)notification
{
    [self reloadPresets];
}

-(void)reloadPresets
{
    LSFGroupModelContainer *groupsContainer = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = groupsContainer.groupContainer;
    self.groupModel = [groups valueForKey: self.groupID];

    LSFPresetModelContainer *container = [LSFPresetModelContainer getPresetModelContainer];
    self.presetData = [container.presetContainer allValues];
    [self sortPresetData];

    [self.tableView reloadData];
}

/*
 * UITableViewDataSource Implementation
 */
-(UITableViewCell *)tableView: (UITableView *)tableView cellForRowAtIndexPath: (NSIndexPath *)indexPath
{
    if (indexPath.section == 0)
    {
        UITableViewCell *cell = [[UITableViewCell alloc] init];
        cell.textLabel.text = @"Save New Preset";
        cell.textLabel.textColor = [UIColor colorWithRed: 0 green: 0.478431 blue: 1.0 alpha: 1.0];
        return cell;
    }
    else
    {
        LSFPresetModel *data = [self.presetDataSorted objectAtIndex: [indexPath row]];
        BOOL stateMatchesPreset = [self checkIfLampStateMatchesPreset: data];

        UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: @"GroupPresetCell" forIndexPath: indexPath];
        cell.textLabel.text = data.name;

        if (stateMatchesPreset)
        {
            cell.accessoryType = UITableViewCellAccessoryCheckmark;
        }
        else
        {
            cell.accessoryType = UITableViewCellAccessoryNone;
        }

        return cell;
    }
}

-(void)tableView: (UITableView *)tableView didSelectRowAtIndexPath: (NSIndexPath *)indexPath
{
    if (indexPath.section == 0)
    {
        [self performSegueWithIdentifier: @"SaveGroupPreset" sender: nil];
        return;
    }

    UITableViewCell *cell = [tableView cellForRowAtIndexPath: indexPath];

    if (cell != nil)
    {
        if (cell.accessoryType == UITableViewCellAccessoryNone)
        {
            cell.accessoryType = UITableViewCellAccessoryCheckmark;
            LSFPresetModel *data = [self.presetDataSorted objectAtIndex: [indexPath row]];

            dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
                LSFLampGroupManager *lampGroupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
                [lampGroupManager transitionLampGroupID: self.groupModel.theID toPreset: data.theID];
            });

            [self.navigationController popViewControllerAnimated: YES];
        }
        else
        {
            [self.navigationController popViewControllerAnimated: YES];
        }
    }
    else
    {
        //NSLog(@"Cell is nil");
    }
}

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
        return self.presetDataSorted.count;
    }
}

-(NSString *)tableView: (UITableView *)tableView titleForHeaderInSection: (NSInteger)section
{
    if (section == 1)
    {
        if (![self tableView:self.tableView numberOfRowsInSection:1])
        {
            return @"No presets have been saved yet";
        }
        else
        {
            return @"Presets";
        }
    }

    return @" ";
}

-(BOOL)tableView: (UITableView *)tableView canEditRowAtIndexPath: (NSIndexPath *)indexPath
{
    return indexPath.section == 1 ? YES : NO;
}

-(void)tableView: (UITableView *)tableView commitEditingStyle: (UITableViewCellEditingStyle)editingStyle forRowAtIndexPath: (NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete)
    {
        LSFPresetModel *data = [self.presetDataSorted objectAtIndex: [indexPath row]];
        [self.presetDataSorted removeObjectAtIndex: indexPath.row];
        
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
        
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFPresetManager *presetManager = ([LSFAllJoynManager getAllJoynManager]).lsfPresetManager;
            [presetManager deletePresetWithID: data.theID];
        });
    }
}

-(CGFloat)tableView: (UITableView *)tableView heightForHeaderInSection: (NSInteger)section
{
    if (section == 0)
    {
        return 20.0f;
    }

    return UITableViewAutomaticDimension;
}

/*
 * Private functions
 */
-(BOOL)checkIfLampStateMatchesPreset: (LSFPresetModel *)data
{
    BOOL returnValue = NO;
    
    LSFConstants *constants = [LSFConstants getConstants];
    unsigned int scaledBrightness = [constants scaleLampStateValue: self.groupModel.state.brightness withMax: 100];
    unsigned int scaledHue = [constants scaleLampStateValue: self.groupModel.state.hue withMax: 360];
    unsigned int scaledSaturation = [constants scaleLampStateValue: self.groupModel.state.saturation withMax: 100];
    unsigned int scaledColorTemp = [constants scaleColorTemp: self.groupModel.state.colorTemp];
    
    if ((self.groupModel.state.onOff == data.state.onOff) && (scaledBrightness == data.state.brightness) && (scaledHue == data.state.hue) && (scaledSaturation == data.state.saturation) && (scaledColorTemp == data.state.colorTemp))
    {
        returnValue = YES;
    }
    
    return returnValue;
}

-(void)sortPresetData
{
    NSSortDescriptor *sortDesc = [NSSortDescriptor sortDescriptorWithKey:@"name" ascending:YES comparator:^NSComparisonResult(id obj1, id obj2) {
        return [(NSString *)obj1 compare:(NSString *)obj2 options:NSCaseInsensitiveSearch];
    }];
    self.presetDataSorted = [[NSMutableArray alloc] initWithArray: [self.presetData sortedArrayUsingDescriptors:[NSArray arrayWithObject:sortDesc]]];
}

/*
 * Segue Function
 */
- (void)prepareForSegue: (UIStoryboardSegue *)segue sender: (id)sender
{
    if ([segue.identifier isEqualToString: @"SaveGroupPreset"])
    {
        LSFGroupsCreatePresetViewController *gcpvc = [segue destinationViewController];
        gcpvc.lampState = self.groupModel.state;
        gcpvc.groupID = self.groupID;
    }
}

@end
