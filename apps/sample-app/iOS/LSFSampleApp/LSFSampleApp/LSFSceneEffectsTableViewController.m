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

#import "LSFSceneEffectsTableViewController.h"
#import "LSFNoEffectTableViewController.h"
#import "LSFTransitionEffectTableViewController.h"
#import "LSFPulseEffectTableViewController.h"
#import "LSFConstants.h"
#import "LSFLampModelContainer.h"
#import "LSFGroupModelContainer.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"
#import "LSFUtilityFunctions.h"

@interface LSFSceneEffectsTableViewController ()

@property (nonatomic, strong) NSIndexPath *selectedIndex;

@end

@implementation LSFSceneEffectsTableViewController

@synthesize selectedIndex = _selectedIndex;
@synthesize sceneModel = _sceneModel;
@synthesize sceneElement = _sceneElement;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

#pragma mark - Table view data source

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 3;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: @"EffectCells" forIndexPath:indexPath];
    cell.textLabel.text = [([LSFConstants getConstants]).supportedEffects objectAtIndex: [indexPath row]];
    cell.imageView.image = [UIImage imageNamed: [([LSFConstants getConstants]).effectImages objectAtIndex: [indexPath row]]];

    if (indexPath.row == 0)
    {
        cell.accessoryType = UITableViewCellAccessoryCheckmark;
        self.selectedIndex = indexPath;
    }
    else
    {
        cell.accessoryType = UITableViewCellAccessoryNone;
    }

    return cell;
}

-(void)tableView: (UITableView *)tableView didSelectRowAtIndexPath: (NSIndexPath *)indexPath
{
    if (![self.selectedIndex isEqual: indexPath])
    {
        UITableViewCell *previousSelectedCell = [tableView cellForRowAtIndexPath: self.selectedIndex];
        previousSelectedCell.accessoryType = UITableViewCellAccessoryNone;
        [self.tableView deselectRowAtIndexPath: self.selectedIndex animated: NO];
    }

    if ([self.selectedIndex isEqual: indexPath])
    {
        //self.selectedIndex = nil;
        [self.tableView deselectRowAtIndexPath: self.selectedIndex animated: NO];
    }
    else
    {
        UITableViewCell* cell = [tableView cellForRowAtIndexPath: indexPath];
        cell.accessoryType = UITableViewCellAccessoryCheckmark;
        [self.tableView deselectRowAtIndexPath: indexPath animated: NO];
        self.selectedIndex = indexPath;
    }
}

-(NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    return [NSString stringWithFormat: @"Choose an optional effect for %@", [LSFUtilityFunctions buildSectionTitleString: self.sceneElement]];
}

/*
 * Button event handler
 */
-(IBAction)nextButtonPressed: (id)sender
{
    switch (self.selectedIndex.row)
    {
        case 0:
            [self performSegueWithIdentifier: @"NoEffect" sender: self];
            break;
        case 1:
            [self performSegueWithIdentifier: @"TransitionEffect" sender: self];
            break;
        case 2:
            [self performSegueWithIdentifier: @"PulseEffect" sender: self];
            break;
    }
}

/*
 * Segue method
 */
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString: @"NoEffect"])
    {
        LSFNoEffectDataModel *nedm = [[LSFNoEffectDataModel alloc] init];
        nedm.members = self.sceneElement.members;
        nedm.capability = self.sceneElement.capability;

        LSFNoEffectTableViewController *netvc = [segue destinationViewController];
        netvc.sceneModel = self.sceneModel;
        netvc.nedm = nedm;
    }
    else if ([segue.identifier isEqualToString: @"TransitionEffect"])
    {
        LSFTransitionEffectDataModel *tedm = [[LSFTransitionEffectDataModel alloc] init];
        tedm.members = self.sceneElement.members;
        tedm.capability = self.sceneElement.capability;

        LSFTransitionEffectTableViewController *tetvc = [segue destinationViewController];
        tetvc.sceneModel = self.sceneModel;
        tetvc.tedm = tedm;
    }
    else if ([segue.identifier isEqualToString: @"PulseEffect"])
    {
        LSFPulseEffectDataModel *pedm = [[LSFPulseEffectDataModel alloc] init];
        pedm.members = self.sceneElement.members;
        pedm.capability = self.sceneElement.capability;

        LSFPulseEffectTableViewController *petvc = [segue destinationViewController];
        petvc.sceneModel = self.sceneModel;
        petvc.pedm = pedm;
    }
}

@end