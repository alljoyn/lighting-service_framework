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

@interface LSFGroupsTableViewController ()

@property (nonatomic, strong) NSArray *groupData;
@property (nonatomic, strong) UIBarButtonItem *myEditButton;
@property (nonatomic, strong) UIBarButtonItem *settingsButton;
@property (nonatomic, strong) UIBarButtonItem *addButton;

-(void)plusButtonPressed;
-(void)settingsButtonPressed;
-(void)sortGroupsByName: (NSArray *)groups;

@end

@implementation LSFGroupsTableViewController

@synthesize groupData = _groupData;
@synthesize myEditButton = _myEditButton;
@synthesize settingsButton = _settingsButton;
@synthesize addButton = _addButton;

- (void)viewDidLoad
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

    NSMutableArray *vc = [NSMutableArray arrayWithArray: self.navigationController.viewControllers];
    [vc removeObjectAtIndex: 0];
    [self.navigationController setViewControllers: vc];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager setReloadUIDelegate: self];
    });
    
    [self reloadUI];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
 * LSFReloadUIDelegate Implementation
 */
-(void)reloadUI
{
    NSLog(@"LSFGroupsTableViewController - reloadUI() callback executing");
    
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    [self sortGroupsByName: [container.groupContainer allValues]];
    
    if (!self.tableView.isEditing)
    {
        [self.tableView reloadData];
    }
    else
    {
        NSLog(@"Skipping updating table since the table is in edit mode");
    }
}

#pragma mark - Table view data source
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    return [container.groupContainer count];
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    LSFGroupModel *data = [self.groupData objectAtIndex: [indexPath row]];
    
    LSFGroupsCell *cell = [tableView dequeueReusableCellWithIdentifier: @"GroupCell" forIndexPath:indexPath];
    cell.groupModel = data;
    cell.nameLabel.text = data.name;
    
    if (data.capability.dimmable > NONE)
    {
        if (cell.groupModel.state.onOff && cell.groupModel.state.brightness == 0)
        {
            dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
                LSFLampGroupManager *groupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
                [groupManager transitionLampGroupID: cell.groupModel.theID onOffField: NO];
            });
        }
        
        cell.brightnessSlider.enabled = YES;
        cell.brightnessSlider.value = data.state.brightness;
        cell.brightnessSliderButton.enabled = NO;
    }
    else
    {
        cell.brightnessSlider.enabled = NO;
        cell.brightnessSlider.value = 0;
        cell.brightnessSliderButton.enabled = YES;
    }
    
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget: cell action: @selector(sliderTapped:)];
    [cell.brightnessSlider addGestureRecognizer: tapGestureRecognizer];
    
    unsigned int c;
    NSString *color = @"f4f4f4";
    if ([color characterAtIndex: 0] == '#')
    {
        [[NSScanner scannerWithString: [color substringFromIndex: 1]] scanHexInt: &c];
    }
    else
    {
        [[NSScanner scannerWithString: color] scanHexInt: &c];
    }
    cell.backgroundColor = [UIColor colorWithRed:((c & 0xff0000) >> 16)/255.0 green:((c & 0xff00) >> 8)/255.0 blue:(c & 0xff)/255.0 alpha:1.0];
    
    if (data.state.onOff)
    {
        cell.powerImage.image = [UIImage imageNamed: @"light_power_on_btn.png"];
    }
    else
    {
        cell.powerImage.image = [UIImage imageNamed: @"light_power_off_btn.png"];
    }

    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 70;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
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

-(void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete)
    {
        UITableViewCell *cell = [tableView cellForRowAtIndexPath: indexPath];
        NSLog(@"Clicked delete on %@", cell.textLabel.text);
        
        LSFGroupModel *data = [self.groupData objectAtIndex: [indexPath row]];
        
        LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
        [container.groupContainer removeObjectForKey: data.theID];
        
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
        
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFLampGroupManager *groupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
            [groupManager deleteLampGroupWithID: data.theID];
        });
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
        gitvc.groupID = [NSString stringWithString: ((LSFGroupModel *)[self.groupData objectAtIndex: [indexPath row]]).theID];
    }
}

/*
 * Private functions
 */
-(void)plusButtonPressed
{
    [self performSegueWithIdentifier: @"AddGroup" sender: nil];
};

-(void)settingsButtonPressed
{
    NSLog(@"Settings Button pressed to display settings.");
    [self performSegueWithIdentifier: @"GroupsSettings" sender: self];
}

-(void)sortGroupsByName: (NSArray *)groups
{
    NSMutableArray *sortedArray = [NSMutableArray arrayWithArray: [groups sortedArrayUsingComparator: ^NSComparisonResult(id a, id b) {
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
    
    [sortedArray insertObject: allLampsGroupModel atIndex: 0];
    self.groupData = [NSArray arrayWithArray: sortedArray];
}

@end
