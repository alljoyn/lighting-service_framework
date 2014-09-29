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

#import "LSFScenesMembersTableViewController.h"
#import "LSFSceneEffectsTableViewController.h"
#import "LSFGroupModelContainer.h"
#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"
#import "LSFSceneElementDataModel.h"
#import "LSFNoEffectTableViewController.h"

@interface LSFScenesMembersTableViewController ()

@property (nonatomic, strong) LSFSceneElementDataModel *sceneElement;
@property (nonatomic, strong) UIBarButtonItem *cancelButton;

-(void)cancelButtonPressed;
-(BOOL)checkIfEffectsAreSupported;

@end

@implementation LSFScenesMembersTableViewController

@synthesize sceneModel = _sceneModel;
@synthesize sceneElement = _sceneElement;
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
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    id model = [self.dataArray objectAtIndex: [indexPath row]];
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: @"Members" forIndexPath:indexPath];

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

-(NSString *)tableView: (UITableView *)tableView titleForHeaderInSection: (NSInteger)section
{
    return @"Select one or more lamps or groups to be in this scene";
}

/*
 * Override public functions in LSFMembersTableViewController
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

    self.dataArray = [NSMutableArray arrayWithArray: groupsArray];
    [self.dataArray addObjectsFromArray: lampsArray];
}

-(void)processSelectedRows
{
    //NSLog(@"LSFScenesMembersTableViewController - processSelectedRows() executing");

    LSFCapabilityData *capabilityData = [[LSFCapabilityData alloc] init];
    NSMutableArray *groupIDs = [[NSMutableArray alloc] init];
    NSMutableArray *lampIDs = [[NSMutableArray alloc] init];

    for (NSIndexPath *indexPath in self.selectedRows)
    {
        id model = [self.dataArray objectAtIndex: indexPath.row];

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

    LSFLampGroup *lampGroup = [[LSFLampGroup alloc] init];
    lampGroup.lamps = lampIDs;
    lampGroup.lampGroups = groupIDs;

    self.sceneElement = [[LSFSceneElementDataModel alloc] initWithEffectType: Unknown andName: @""];
    self.sceneElement.members = lampGroup;
    self.sceneElement.capability = capabilityData;
}

/*
 * Next Button Event Handler
 */
-(IBAction)nextButtonPressed: (UIBarButtonItem *)sender
{
    NSLog(@"LSFScenesMembersTableViewController - nextButtonPressed() executing");

    [self processSelectedRows];

    if ([self checkIfEffectsAreSupported])
    {
        [self performSegueWithIdentifier: @"ChooseSceneEffect" sender: nil];
    }
    else
    {
        [self performSegueWithIdentifier: @"JumpToNoEffect" sender: nil];
    }
}

/*
 * Cancel Button Event Handler
 */
-(void)cancelButtonPressed
{
    [self dismissViewControllerAnimated: YES completion: nil];
}

-(BOOL)checkIfEffectsAreSupported
{
    LSFLampModelContainer *lampContainer = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = lampContainer.lampContainer;

    for (NSString *lampID in self.sceneElement.members.lamps)
    {
        NSLog(@"Checking LampID: %@", lampID);
        LSFLampModel *lampModel = [lamps valueForKey: lampID];

        if (lampModel.lampDetails.hasEffects)
        {
            return YES;
        }
    }

    LSFGroupModelContainer *groupContainer = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = groupContainer.groupContainer;

    for (NSString *groupID in self.sceneElement.members.lampGroups)
    {
        NSLog(@"Checking GroupID: %@", groupID);
        LSFGroupModel *groupModel = [groups valueForKey: groupID];

        for (NSString *lampID in groupModel.lamps)
        {
            NSLog(@"Checking LampID: %@ within GroupID: %@", lampID, groupID);
            LSFLampModel *lampModel = [lamps valueForKey: lampID];

            if (lampModel.lampDetails.hasEffects)
            {
                return YES;
            }
        }
    }

    return NO;
}

/*
 * Segue functions
 */
-(BOOL)shouldPerformSegueWithIdentifier: (NSString *)identifier sender: (id)sender
{
    if ([self.selectedRows count] == 0)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                        message: @"You must select at least one lamp or group to create a scene element."
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];

        return NO;
    }

    return YES;
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString: @"ChooseSceneEffect"])
    {
        LSFSceneEffectsTableViewController *setvc = [segue destinationViewController];
        setvc.sceneModel = self.sceneModel;
        setvc.sceneElement = self.sceneElement;
    }
    else if ([segue.identifier isEqualToString: @"JumpToNoEffect"])
    {
        LSFNoEffectDataModel *nedm = [[LSFNoEffectDataModel alloc] init];
        nedm.members = self.sceneElement.members;
        nedm.capability = self.sceneElement.capability;

        LSFNoEffectTableViewController *netvc = [segue destinationViewController];
        netvc.sceneModel = self.sceneModel;
        netvc.nedm = nedm;
    }
}

@end
