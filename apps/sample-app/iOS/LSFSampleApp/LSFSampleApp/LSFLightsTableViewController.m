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
#import "LSFGarbageCollector.h"
#import "LSFUtilityFunctions.h"

@interface LSFLightsTableViewController ()

@property (nonatomic, strong) NSMutableArray *lampData;

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

@synthesize lampData = _lampData;

-(void)viewDidLoad
{
    NSLog(@"LSFLightsTableViewController - viewDidLoad()");

    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    NSLog(@"LSFLightsTableViewController - viewWillAppear()");

    [super viewWillAppear: animated];
    [self.navigationItem setHidesBackButton:YES];

    //Set lamps callback delegate
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.slmc setReloadLightsDelegate: self];
    });

    //Set the content of the default lamp data array
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    self.lampData = [[NSMutableArray alloc] initWithArray: [container.lampContainer allValues]];
    [self sortLampsByName];

    //Reload the table
    [self.tableView reloadData];
}

-(void)viewWillDisappear: (BOOL)animated
{
    NSLog(@"LSFLightsTableViewController - viewWillDisappear()");

    [super viewWillDisappear: animated];

    //Clear lamps callback delegate
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.slmc setReloadLightsDelegate: nil];
    });

    self.tableView.backgroundView = nil;
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LSFReloadLightsCallbackDelegate Implementation
 */
-(void)reloadLampWithID: (NSString *)lampID
{
    @synchronized(self.lampData)
    {
        LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
        NSMutableDictionary *lamps = container.lampContainer;
        LSFLampModel *model = [lamps valueForKey: lampID];

        if (model != nil)
        {
            NSUInteger existingIndex = [self checkIfLampModelExists: model];

            if (existingIndex == NSNotFound)
            {
                NSUInteger insertIndex = [self findInsertionIndexInArray: model];
                [self.lampData insertObject: model atIndex: insertIndex];

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

                    [self.lampData removeObjectAtIndex: existingIndex];
                    [self.lampData insertObject: model atIndex: insertIndex];

                    [self moveObjectFromIndex: existingIndex toIndex: insertIndex];
                }
            }
        }
        else
        {
            NSLog(@"LSFLightsTableViewController - reloadLampWithID() model is nil");
        }
    }
}

-(void)deleteLampWithID:(NSString *)lampID andName: (NSString *)lampName
{
    @synchronized(self.lampData)
    {
        LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
        NSMutableDictionary *lamps = container.lampContainer;
        LSFLampModel *model = [lamps valueForKey: lampID];

        if (model == nil)
        {
            int modelArrayIndex = [self findIndexInModelOfID: lampID];

            if (modelArrayIndex != -1)
            {
                [self.lampData removeObjectAtIndex: modelArrayIndex];
                [self deleteRowInTableAtIndex: modelArrayIndex];
            }
        }
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
        if (self.lampData.count == 0)
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
        
        return self.lampData.count;
    }
}

-(UITableViewCell *)tableView: (UITableView *)tableView cellForRowAtIndexPath: (NSIndexPath *)indexPath
{
    LSFLampModel *data = [self.lampData objectAtIndex: [indexPath row]];
    
    LSFLightsCell *cell = [tableView dequeueReusableCellWithIdentifier: @"LightCell" forIndexPath: indexPath];
    cell.lampID = data.theID;
    cell.nameLabel.text = data.name;

    cell.colorIndicatorImage.layer.rasterizationScale = [UIScreen mainScreen].scale;
    cell.colorIndicatorImage.layer.shouldRasterize = YES;
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

/*
 * Segue method
 */
-(void)prepareForSegue: (UIStoryboardSegue *)segue sender: (id)sender
{
    if ([segue.identifier isEqualToString: @"LampInfo"])
    {
        NSIndexPath *indexPath = [self.tableView indexPathForCell: sender];
        
        LSFLightInfoTableViewController *litvc = [segue destinationViewController];
        litvc.lampID = ((LSFLampModel *)[self.lampData objectAtIndex: [indexPath row]]).theID;
    }
}

/*
 * Private Functions
 */
-(NSUInteger)checkIfLampModelExists: (LSFLampModel *)lampModel
{
    for (int i = 0; i < self.lampData.count; i++)
    {
        LSFLampModel *model = (LSFLampModel *)[self.lampData objectAtIndex: i];

        if ([lampModel.theID isEqualToString: model.theID])
        {
            return i;
        }
    }

    return NSNotFound;
}

-(NSUInteger)findInsertionIndexInArray: (LSFLampModel *)lampModel
{
    return [self.lampData indexOfObject: lampModel inSortedRange: (NSRange){0, [self.lampData count]} options: (NSBinarySearchingFirstEqual | NSBinarySearchingInsertionIndex) usingComparator:
            ^NSComparisonResult(id a, id b) {
                NSString *first = [(LSFLampModel *)a name];
                NSString *second = [(LSFLampModel *)b name];
                return [first localizedCaseInsensitiveCompare: second];
            }];
}

-(void)sortLampsByName
{
    NSMutableArray *sortedArray = [NSMutableArray arrayWithArray: [self.lampData sortedArrayUsingComparator: ^NSComparisonResult(id a, id b) {
        NSString *first = [(LSFLampModel *)a name];
        NSString *second = [(LSFLampModel *)b name];
        return [first localizedCaseInsensitiveCompare: second];
    }]];

    self.lampData = [NSMutableArray arrayWithArray: sortedArray];
}

-(int)findIndexInModelOfID: (NSString *)theID
{
    for (int i = 0; i < self.lampData.count; i++)
    {
        LSFLampModel *model = [self.lampData objectAtIndex: i];

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
