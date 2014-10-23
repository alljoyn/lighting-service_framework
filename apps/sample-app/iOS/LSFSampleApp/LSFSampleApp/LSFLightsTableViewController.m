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

#import "LSFLightsTableViewController.h"
#import "LSFLightInfoTableViewController.h"
#import "LSFLightsCell.h"
#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFConstants.h"
#import "LSFUtilityFunctions.h"
#import "LSFWifiMonitor.h"
#import "LSFEnums.h"

@interface LSFLightsTableViewController ()

-(void)lampNotificationReceived: (NSNotification *)notification;
-(void)addNewLamp: (NSString *)lampID;
-(void)refreshLamp: (NSString *)lampID;
-(void)reorderLamp: (NSString *)lampID;
-(void)removeLamp: (NSString *)lampID;
-(NSUInteger)checkIfLampModelExists: (LSFLampModel *)lampModel;
-(NSUInteger)findInsertionIndexInArray: (LSFLampModel *)lampModel;
-(void)sortLampsByName;
-(int)findIndexInModelOfID: (NSString *)theID;
-(void)addObjectToTableAtIndex: (NSUInteger)insertIndex;
-(void)moveObjectFromIndex: (NSUInteger)fromIndex toIndex: (NSUInteger)toIndex;
-(void)refreshRowInTableAtIndex: (NSUInteger)index;
-(void)deleteRowInTableAtIndex: (NSUInteger)index;

@end

@implementation LSFLightsTableViewController

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];
    [self.navigationItem setHidesBackButton:YES];

    //Set lamps notification handler
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(lampNotificationReceived:) name: @"LampNotification" object: nil];

    //Set the content of the default lamp data array
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    self.data = [[NSMutableArray alloc] initWithArray: [container.lampContainer allValues]];
    [self sortLampsByName];

    //Reload the table
    [self.tableView reloadData];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    //Clear lamps notification handler
    [[NSNotificationCenter defaultCenter] removeObserver: self];

    self.tableView.backgroundView = nil;
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LampNotification Handler
 */
-(void)lampNotificationReceived: (NSNotification *)notification
{
    @synchronized(self.data)
    {
        NSString *lampID = [notification.userInfo valueForKey: @"lampID"];
        NSNumber *callbackOp = [notification.userInfo valueForKey: @"operation"];

        switch (callbackOp.intValue)
        {
            case LampFound:
                [self addNewLamp: lampID];
                break;
            case LampDeleted:
                [self removeLamp: lampID];
                break;
            case LampNameUpdated:
                [self reorderLamp: lampID];
                break;
            case LampDetailsUpdated:
            case LampStateUpdated:
                [self refreshLamp: lampID];
                break;
            default:
                NSLog(@"Operation not found - Taking no action");
                break;
        }
    }
}

-(void)addNewLamp: (NSString *)lampID
{
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = container.lampContainer;
    LSFLampModel *model = [lamps valueForKey: lampID];

    NSUInteger existingIndex = [self checkIfLampModelExists: model];

    if (existingIndex == NSNotFound)
    {
        NSUInteger insertIndex = [self findInsertionIndexInArray: model];
        [self.data insertObject: model atIndex: insertIndex];

        [self addObjectToTableAtIndex: insertIndex];
    }
    else
    {
        NSLog(@"LSFLightsTableViewController - addNewLamp() model is nil");
    }
}

-(void)refreshLamp: (NSString *)lampID
{
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = container.lampContainer;
    LSFLampModel *model = [lamps valueForKey: lampID];

    if (model != nil)
    {
        NSUInteger existingIndex = [self findIndexInModelOfID: lampID];

        if (existingIndex != -1)
        {
            [self refreshRowInTableAtIndex: existingIndex];
        }
    }
    else
    {
        NSLog(@"LSFLightsTableViewController - refreshLamp() model is nil");
    }
}

-(void)reorderLamp: (NSString *)lampID
{
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = container.lampContainer;
    LSFLampModel *model = [lamps valueForKey: lampID];

    if (model != nil)
    {
        NSUInteger existingIndex = [self checkIfLampModelExists: model];
        NSUInteger insertIndex = [self findInsertionIndexInArray: model];

        if (existingIndex == insertIndex)
        {
            NSLog(@"Even though name changed the cell does not have to be moved. Just refreshed.");
            [self refreshRowInTableAtIndex: insertIndex];
        }
        else
        {
            NSLog(@"Cell must be moved to preserve alphabetical order");

            if (existingIndex < insertIndex)
            {
                insertIndex--;
            }

            [self.data removeObjectAtIndex: existingIndex];
            [self.data insertObject: model atIndex: insertIndex];

            [self moveObjectFromIndex: existingIndex toIndex: insertIndex];
        }
    }
    else
    {
        NSLog(@"LSFLightsTableViewController - reorderLamp() model is nil");
    }
}

-(void)removeLamp: (NSString *)lampID
{
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = container.lampContainer;
    LSFLampModel *model = [lamps valueForKey: lampID];

    if (model == nil)
    {
        int modelArrayIndex = [self findIndexInModelOfID: lampID];

        if (modelArrayIndex != -1)
        {
            [self.data removeObjectAtIndex: modelArrayIndex];
            [self deleteRowInTableAtIndex: modelArrayIndex];
        }
    }
    else
    {
        NSLog(@"LSFLightsTableViewController - removeLamp() model is not nil");
    }
}

/*
 * UITableViewDataSource Protocol Implementation
 */
-(NSInteger)numberOfSectionsInTableView: (UITableView *)tableView
{
    return 1;
}

-(NSInteger)tableView: (UITableView *)tableView numberOfRowsInSection: (NSInteger)section
{
    LSFWifiMonitor *wifiMonitor = [LSFWifiMonitor getWifiMonitor];
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
    LSFConstants *constants = [LSFConstants getConstants];

    if (!wifiMonitor.isWifiConnected)
    {
        CGRect frame = CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height);
        UIView *customView = [[UIView alloc] initWithFrame: frame];
        UILabel *messageLabel = [[UILabel alloc] init];
        [customView addSubview: messageLabel];

        frame.origin.x = 30;
        frame.size.width = self.view.bounds.size.width - frame.origin.x;

        messageLabel.frame = frame;
        messageLabel.text = [NSString stringWithFormat: @"No Wi-Fi Connection.\n\nPlease check that Wi-Fi is enabled in this device's settings and that the device is connected to a Wi-Fi network.\n\nWaiting for Wi-Fi to become available..."];
        messageLabel.textColor = [UIColor blackColor];
        messageLabel.numberOfLines = 0;
        messageLabel.textAlignment = NSTextAlignmentLeft;

        self.tableView.backgroundView = customView;
        return 0;
    }
    else if (!ajManager.isConnectedToController)
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
        if (self.data.count == 0)
        {
            CGRect frame = CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height);
            UIView *customView = [[UIView alloc] initWithFrame: frame];
            UILabel *messageLabel = [[UILabel alloc] init];
            [customView addSubview: messageLabel];

            frame.origin.x = 30;
            frame.size.width = self.view.bounds.size.width - frame.origin.x;

            messageLabel.frame = frame;
            messageLabel.text = [NSString stringWithFormat: @"No lamps have been detected on the network %@.\n\nSearching for lamps...", [constants currentWifiSSID]];
            messageLabel.textColor = [UIColor blackColor];
            messageLabel.numberOfLines = 0;
            messageLabel.textAlignment = NSTextAlignmentLeft;

            self.tableView.backgroundView = customView;
        }
        else
        {
            self.tableView.backgroundView = nil;
        }
        
        return self.data.count;
    }
}

-(UITableViewCell *)tableView: (UITableView *)tableView cellForRowAtIndexPath: (NSIndexPath *)indexPath
{
    LSFLampModel *data = [self.data objectAtIndex: [indexPath row]];
    
    LSFLightsCell *cell = [tableView dequeueReusableCellWithIdentifier: @"LightCell" forIndexPath: indexPath];
    cell.lampID = data.theID;
    cell.nameLabel.text = data.name;

    [LSFUtilityFunctions colorIndicatorSetup: cell.colorIndicatorImage dataState: data.state];

    if (data.lampDetails.dimmable)
    {
        if (data.state.onOff && data.state.brightness == 0)
        {
            dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
                LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
                [lampManager transitionLampID: cell.lampID onOffField: NO];
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
    
    if (data.state.onOff)
    {
        cell.powerImage.image = [UIImage imageNamed: @"light_power_btn_on.png"];
    }
    else
    {
        cell.powerImage.image = [UIImage imageNamed: @"light_power_btn_off.png"];
    }

    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 70;
}

-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return 0.00000000001f;
}

/*
 * Segue method
 */
-(void)prepareForSegue: (UIStoryboardSegue *)segue sender: (id)sender
{
    if ([segue.identifier isEqualToString: @"LampInfo"])
    {
        NSIndexPath *indexPath = [self.tableView indexPathForCell: sender];
        
        LSFLightInfoTableViewController *litvc = [segue destinationViewController];
        litvc.lampID = ((LSFLampModel *)[self.data objectAtIndex: [indexPath row]]).theID;
    }
}

/*
 * Private Functions
 */
-(NSUInteger)checkIfLampModelExists: (LSFLampModel *)lampModel
{
    for (int i = 0; i < self.data.count; i++)
    {
        LSFLampModel *model = (LSFLampModel *)[self.data objectAtIndex: i];

        if ([lampModel.theID isEqualToString: model.theID])
        {
            return i;
        }
    }

    return NSNotFound;
}

-(NSUInteger)findInsertionIndexInArray: (LSFLampModel *)lampModel
{
    return [self.data indexOfObject: lampModel inSortedRange: (NSRange){0, [self.data count]} options: (NSBinarySearchingFirstEqual | NSBinarySearchingInsertionIndex) usingComparator:
            ^NSComparisonResult(id a, id b) {
                NSString *first = [(LSFLampModel *)a name];
                NSString *second = [(LSFLampModel *)b name];
                return [first localizedCaseInsensitiveCompare: second];
            }];
}

-(void)sortLampsByName
{
    NSMutableArray *sortedArray = [NSMutableArray arrayWithArray: [self.data sortedArrayUsingComparator: ^NSComparisonResult(id a, id b) {
        NSString *first = [(LSFLampModel *)a name];
        NSString *second = [(LSFLampModel *)b name];
        return [first localizedCaseInsensitiveCompare: second];
    }]];

    self.data = [NSMutableArray arrayWithArray: sortedArray];
}

-(int)findIndexInModelOfID: (NSString *)theID
{
    for (int i = 0; i < self.data.count; i++)
    {
        LSFLampModel *model = [self.data objectAtIndex: i];

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

-(void)deleteRowInTableAtIndex: (NSUInteger)index
{
    //NSLog(@"Delete Index = %u", index);
    NSArray *deleteIndexPaths = [NSArray arrayWithObjects: [NSIndexPath indexPathForRow: index inSection:0],nil];

    [self.tableView beginUpdates];
    [self.tableView deleteRowsAtIndexPaths: deleteIndexPaths withRowAnimation: UITableViewRowAnimationFade];
    [self.tableView endUpdates];
}

@end