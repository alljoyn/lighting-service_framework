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

#import "LSFScenesPresetsTableViewController.h"
#import "LSFAllJoynManager.h"
#import "LSFDispatchQueue.h"
#import "LSFPresetModelContainer.h"
#import "LSFPresetModel.h"
#import "LSFScenesCreatePresetViewController.h"
#import "LSFConstants.h"
#import "LSFNoEffectTableViewController.h"
#import "LSFTransitionEffectTableViewController.h"
#import "LSFPulseEffectTableViewController.h"

@interface LSFScenesPresetsTableViewController ()

@property (nonatomic, strong) NSArray *presetData;
@property (nonatomic, strong) NSArray *presetDataSorted;

@end

@implementation LSFScenesPresetsTableViewController

@synthesize lampState = _lampState;
@synthesize effectSender = _effectSender;
@synthesize endStateFlag = _endStateFlag;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];
    self.navigationItem.rightBarButtonItem = self.editButtonItem;

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.spmc setReloadPresetsDelegate: self];
    });

    [self reloadPresets];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    if([self.effectSender isKindOfClass:[LSFNoEffectTableViewController class]])
    {
        ((LSFNoEffectTableViewController*)self.effectSender).nedm.state = self.lampState;
    }
    else if ([self.effectSender isKindOfClass:[LSFTransitionEffectTableViewController class]])
    {
        ((LSFTransitionEffectTableViewController*)self.effectSender).tedm.state = self.lampState;
    }
    else if ([self.effectSender isKindOfClass:[LSFPulseEffectTableViewController class]])
    {
        if (self.endStateFlag)
        {
            ((LSFPulseEffectTableViewController*)self.effectSender).pedm.endState = self.lampState;
        }
        else
        {
            ((LSFPulseEffectTableViewController*)self.effectSender).pedm.state = self.lampState;
        }
    }

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.spmc setReloadPresetsDelegate: nil];
    });
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LSFReloadPresetsCallbackDelegate Implementation
 */
-(void)reloadPresets
{
    LSFPresetModelContainer *container = [LSFPresetModelContainer getPresetModelContainer];
    self.presetData = [container.presetContainer allValues];
    [self sortPresetData];

    [self.tableView reloadData];
}

/*
 * UITableViewDataSource Implementation
 */
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
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

        UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: @"ScenePresetCell" forIndexPath:indexPath];
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
        [self performSegueWithIdentifier: @"SaveScenePreset" sender: nil];
        return;
    }

    UITableViewCell *cell = [tableView cellForRowAtIndexPath: indexPath];

    if (cell != nil)
    {
        if (cell.accessoryType == UITableViewCellAccessoryNone)
        {
            cell.accessoryType = UITableViewCellAccessoryCheckmark;
            LSFPresetModel *data = [self.presetDataSorted objectAtIndex: [indexPath row]];

            self.lampState.brightness = data.state.brightness;
            self.lampState.hue = data.state.hue;
            self.lampState.saturation = data.state.saturation;
            self.lampState.colorTemp = data.state.colorTemp;


            if([self.effectSender isKindOfClass:[LSFNoEffectTableViewController class]])
            {
                ((LSFNoEffectTableViewController*)self.effectSender).nedm.state = self.lampState;
            }
            else if ([self.effectSender isKindOfClass:[LSFTransitionEffectTableViewController class]])
            {
                ((LSFTransitionEffectTableViewController*)self.effectSender).tedm.state = self.lampState;
            }
            else if ([self.effectSender isKindOfClass:[LSFPulseEffectTableViewController class]])
            {
                if (self.endStateFlag)
                {
                    ((LSFPulseEffectTableViewController*)self.effectSender).pedm.endState = self.lampState;
                }
                else
                {
                    ((LSFPulseEffectTableViewController*)self.effectSender).pedm.state = self.lampState;
                }
            }

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

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}

/*
 * UITableViewDataSource Implementation
 */
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (section == 0)
    {
        return 1;
    }
    else
    {
        LSFPresetModelContainer *container = [LSFPresetModelContainer getPresetModelContainer];
        return [container.presetContainer count];
    }
}

-(NSString *)tableView: (UITableView *)tableView titleForHeaderInSection: (NSInteger)section
{
    if (section == 1)
    {
        if (![self tableView: self.tableView numberOfRowsInSection:1])
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
        //UITableViewCell *cell = [tableView cellForRowAtIndexPath: indexPath];
        //NSLog(@"Clicked delete on %@", cell.textLabel.text);

        LSFPresetModel *data = [self.presetDataSorted objectAtIndex: [indexPath row]];

        LSFPresetModelContainer *container = [LSFPresetModelContainer getPresetModelContainer];
        [container.presetContainer removeObjectForKey: data.theID];

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

    if (((self.lampState.onOff == data.state.onOff) && (self.lampState.brightness == data.state.brightness) && (self.lampState.hue == data.state.hue) && (self.lampState.saturation == data.state.saturation) && self.lampState.colorTemp == data.state.colorTemp))
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
    self.presetDataSorted = [self.presetData sortedArrayUsingDescriptors:[NSArray arrayWithObject:sortDesc]];
}

/*
 * Segue Function
 */
- (void)prepareForSegue: (UIStoryboardSegue *)segue sender: (id)sender
{
    if ([segue.identifier isEqualToString: @"SaveScenePreset"])
    {
        LSFScenesCreatePresetViewController *scpvc = [segue destinationViewController];
        scpvc.lampState = self.lampState;
    }
}

@end
