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
#import "LSFConstants.h"

@interface LSFGroupsTableViewController ()

@property (nonatomic, strong) NSMutableArray *groupData;
@property (nonatomic, strong) UIBarButtonItem *myEditButton;
@property (nonatomic, strong) UIBarButtonItem *settingsButton;
@property (nonatomic, strong) UIBarButtonItem *addButton;

-(void)plusButtonPressed;
-(void)settingsButtonPressed;
-(NSUInteger)checkIfGroupModelExists: (LSFGroupModel *)groupModel;
-(NSUInteger)findInsertionIndexInArray: (LSFGroupModel *)groupModel;
-(void)sortGroupsByName;
-(void)addObjectToTableAtIndex: (NSUInteger)insertIndex;
-(void)moveObjectFromIndex: (NSUInteger)fromIndex toIndex: (NSUInteger)toIndex;
-(void)refreshRowInTableAtIndex: (NSUInteger)index;
-(void)deleteRowsInTableAtIndex: (NSArray *)cellIndexPaths;

@end

@implementation LSFGroupsTableViewController

@synthesize groupData = _groupData;
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
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.slgmc setReloadGroupsDelegate: self];
    });
    
    //Set the content of the default group data array
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    self.groupData = [[NSMutableArray alloc] initWithArray: [container.groupContainer allValues]];

    if (self.groupData.count > 0)
    {
        [self sortGroupsByName];
    }

    //Reload the table
    [self.tableView reloadData];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    //Clear lamps callback delegate
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.slgmc setReloadGroupsDelegate: nil];
    });
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LSFReloadGroupsCallbackDelegate Implementation
 */
-(void)reloadGroupWithID: (NSString *)groupID
{
    @synchronized(self.groupData)
    {
        LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
        NSMutableDictionary *groups = container.groupContainer;
        LSFGroupModel *model = [groups valueForKey: groupID];

        if (model != nil)
        {
            NSUInteger existingIndex = [self checkIfGroupModelExists: model];

            if (existingIndex == NSNotFound)
            {
                NSUInteger insertIndex = [self findInsertionIndexInArray: model];
                [self.groupData insertObject: model atIndex: insertIndex];

                [self addObjectToTableAtIndex: insertIndex];
            }
            else
            {
                NSUInteger insertIndex = [self findInsertionIndexInArray: model];

                if (existingIndex == insertIndex)
                {
                    [self refreshRowInTableAtIndex: insertIndex];
                }
                else
                {
                    if (existingIndex < insertIndex)
                    {
                        insertIndex--;
                    }

                    [self.groupData removeObjectAtIndex: existingIndex];
                    [self.groupData insertObject: model atIndex: insertIndex];
                    
                    [self moveObjectFromIndex: existingIndex toIndex: insertIndex];
                }
            }
        }
        else
        {
            NSLog(@"LSFGroupsTableViewController - reloadGroupWithID() model is nil");
        }
    }
}

-(void)deleteGroupsWithIDs: (NSArray *)groupIDs andNames: (NSArray *)groupNames
{
    @synchronized(self.groupData)
    {
        LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
        NSMutableDictionary *groups = container.groupContainer;

        NSMutableArray *deleteIndexPaths = [[NSMutableArray alloc] init];

        for (NSString *groupID in groupIDs)
        {
            LSFGroupModel *model = [groups valueForKey: groupID];

            if (model == nil)
            {
                int modelArrayIndex = [self findIndexInModelOfID: groupID];

                if (modelArrayIndex != -1)
                {
                    [self.groupData removeObjectAtIndex: modelArrayIndex];
                    [deleteIndexPaths addObject: [NSIndexPath indexPathForRow: modelArrayIndex inSection:0]];
                }
            }
        }
        
        [self deleteRowsInTableAtIndex: deleteIndexPaths];
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
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
    LSFConstants *constants = [LSFConstants getConstants];

    if (!ajManager.isConnectedToController)
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
        return self.groupData.count;
    }
}


-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    LSFGroupModel *data = [self.groupData objectAtIndex: [indexPath row]];

    NSLog(@"Group Model State - %@", data.name);
    NSLog(@"OnOff = %@", data.state.onOff ? @"On" : @"Off");
    NSLog(@"Brightness = %u", data.state.brightness);
    NSLog(@"Hue = %u", data.state.hue);
    NSLog(@"Saturation = %u", data.state.saturation);
    NSLog(@"Color Temp = %u", data.state.colorTemp);

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

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 70;
}

-(CGFloat) tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 1.0f;
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
        LSFGroupModel *data = [self.groupData objectAtIndex: [indexPath row]];

        int modelArrayIndex = [self findIndexInModelOfID: data.theID];
        [self.groupData removeObjectAtIndex: modelArrayIndex];

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
    [self performSegueWithIdentifier: @"GroupsSettings" sender: self];
}

-(NSUInteger)checkIfGroupModelExists: (LSFGroupModel *)groupModel
{
    for (int i = 0; i < self.groupData.count; i++)
    {
        LSFGroupModel *model = (LSFGroupModel *)[self.groupData objectAtIndex: i];

        if ([groupModel.theID isEqualToString: model.theID])
        {
            return i;
        }
    }

    return NSNotFound;
}

-(NSUInteger)findInsertionIndexInArray: (LSFGroupModel *)groupModel
{
    return [self.groupData indexOfObject: groupModel inSortedRange: (NSRange){0, [self.groupData count]} options: (NSBinarySearchingFirstEqual | NSBinarySearchingInsertionIndex) usingComparator:
            ^NSComparisonResult(id a, id b) {
                NSString *first = [(LSFGroupModel *)a name];
                NSString *second = [(LSFGroupModel *)b name];
                return [first localizedCaseInsensitiveCompare: second];
            }];
}

-(void)sortGroupsByName
{
    NSMutableArray *sortedArray = [NSMutableArray arrayWithArray: [self.groupData sortedArrayUsingComparator: ^NSComparisonResult(id a, id b) {
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
    self.groupData = [NSMutableArray arrayWithArray: sortedArray];
}

-(int)findIndexInModelOfID: (NSString *)theID
{
    for (int i = 0; i < self.groupData.count; i++)
    {
        LSFGroupModel *model = [self.groupData objectAtIndex: i];

        if ([theID isEqualToString: model.theID])
        {
            return i;
        }
    }

    return -1;
}

-(void)addObjectToTableAtIndex: (NSUInteger)insertIndex
{
    //NSLog(@"Add: Index = %u", insertIndex);
    NSArray *insertIndexPaths = [NSArray arrayWithObjects: [NSIndexPath indexPathForRow: insertIndex inSection:0], nil];

    [self.tableView beginUpdates];
    [self.tableView insertRowsAtIndexPaths: insertIndexPaths withRowAnimation: UITableViewRowAnimationFade];
    [self.tableView endUpdates];
}

-(void)moveObjectFromIndex: (NSUInteger)fromIndex toIndex: (NSUInteger)toIndex
{
    //NSLog(@"Move: FromIndex = %u; ToIndex = %u", fromIndex, toIndex);
    NSIndexPath *fromIndexPath = [NSIndexPath indexPathForRow: fromIndex inSection: 0];
    NSIndexPath *toIndexPath = [NSIndexPath indexPathForRow: toIndex inSection: 0];

    [self.tableView beginUpdates];
    [self.tableView moveRowAtIndexPath: fromIndexPath toIndexPath: toIndexPath];
    [self.tableView endUpdates];

    [self refreshRowInTableAtIndex: toIndex];
}

-(void)refreshRowInTableAtIndex: (NSUInteger)index
{
    //NSLog(@"Refresh: Index = %u", index);
    NSArray *refreshIndexPaths = [NSArray arrayWithObjects: [NSIndexPath indexPathForRow: index inSection:0], nil];

    [self.tableView beginUpdates];
    [self.tableView reloadRowsAtIndexPaths: refreshIndexPaths withRowAnimation: UITableViewRowAnimationNone];
    [self.tableView endUpdates];
}

-(void)deleteRowsInTableAtIndex: (NSArray *)cellIndexPaths
{
    [self.tableView beginUpdates];
    [self.tableView deleteRowsAtIndexPaths: cellIndexPaths withRowAnimation: UITableViewRowAnimationFade];
    [self.tableView endUpdates];
}

@end
