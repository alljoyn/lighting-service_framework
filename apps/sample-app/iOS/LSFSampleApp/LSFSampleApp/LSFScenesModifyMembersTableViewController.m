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

#import "LSFScenesModifyMembersTableViewController.h"
#import "LSFNoEffectTableViewController.h"
#import "LSFTransitionEffectTableViewController.h"
#import "LSFPulseEffectTableViewController.h"
#import "LSFGroupModelContainer.h"
#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"

@interface LSFScenesModifyMembersTableViewController ()

@property (nonatomic, strong) UIBarButtonItem *cancelButton;

-(void)cancelButtonPressed;

@end

@implementation LSFScenesModifyMembersTableViewController

@synthesize sceneModel = _sceneModel;
@synthesize sceneElement = _sceneElement;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    [self.navigationItem setHidesBackButton:YES];

    self.cancelButton = [[UIBarButtonItem alloc]
                         initWithBarButtonSystemItem:UIBarButtonSystemItemCancel
                         target: self
                         action: @selector(cancelButtonPressed)];

    self.navigationItem.leftBarButtonItem = self.cancelButton;
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
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: @"ModifyMembers" forIndexPath:indexPath];

    if ([model isKindOfClass: [LSFGroupModel class]])
    {
        LSFGroupModel *groupModel = (LSFGroupModel *)model;

        cell.textLabel.text = groupModel.name;
        cell.imageView.image = [UIImage imageNamed:@"groups_off_icon.png"];

        if ([self.sceneElement.members.lampGroups containsObject: groupModel.theID])
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
        LSFLampModel *lampModel = (LSFLampModel *)model;

        cell.textLabel.text = lampModel.name;
        cell.imageView.image = [UIImage imageNamed:@"lamps_off_icon.png"];

        if ([self.sceneElement.members.lamps containsObject: lampModel.theID])
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

/*
 * Override public functions in LSFMembersTableViewController
 */
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

    self.sceneElement.members = lampGroup;
    self.sceneElement.capability = capabilityData;

    if ([self.sceneElement isKindOfClass: [LSFNoEffectDataModel class]])
    {
        [self performSegueWithIdentifier: @"ModifyNoEffect" sender: nil];
    }
    else if ([self.sceneElement isKindOfClass: [LSFTransitionEffectDataModel class]])
    {
        [self performSegueWithIdentifier: @"ModifyTransition" sender: nil];
    }
    else if ([self.sceneElement isKindOfClass: [LSFPulseEffectDataModel class]])
    {
        [self performSegueWithIdentifier: @"ModifyPulse" sender: nil];
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
 * Next Button Clicked Handler
 */
-(IBAction)nextButtonPressed: (id)sender
{
    if ([self.selectedRows count] == 0)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                        message: @"You must select at least one lamp or group to modify a scene element."
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];
        return;
    }
    else
    {
        [self processSelectedRows];
    }
}

/*
 * Segue Function
 */
-(void)prepareForSegue: (UIStoryboardSegue *)segue sender: (id)sender
{
    if ([segue.identifier isEqualToString: @"ModifyNoEffect"])
    {
        LSFNoEffectTableViewController *netvc = [segue destinationViewController];
        netvc.sceneModel = self.sceneModel;
        netvc.nedm = (LSFNoEffectDataModel *)self.sceneElement;
    }
    else if ([segue.identifier isEqualToString: @"ModifyTransition"])
    {
        LSFTransitionEffectTableViewController *tetvc = [segue destinationViewController];
        tetvc.sceneModel = self.sceneModel;
        tetvc.tedm = (LSFTransitionEffectDataModel *)self.sceneElement;
    }
    else if ([segue.identifier isEqualToString: @"ModifyPulse"])
    {
        LSFPulseEffectTableViewController *petvc = [segue destinationViewController];
        petvc.sceneModel = self.sceneModel;
        petvc.pedm = (LSFPulseEffectDataModel *)self.sceneElement;
    }
}

@end
