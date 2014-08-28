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

@interface LSFLightsTableViewController ()

@property (nonatomic, strong) NSArray *lampData;

@end

@implementation LSFLightsTableViewController

@synthesize lampData = _lampData;

-(void)viewDidLoad
{
    [super viewDidLoad];

    NSMutableArray *vc = [NSMutableArray arrayWithArray: self.navigationController.viewControllers];
    [vc removeObjectAtIndex: 0];
    [self.navigationController setViewControllers: vc];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];
    [self.navigationItem setHidesBackButton:YES];
}

-(void)viewDidAppear: (BOOL)animated
{
    [super viewDidAppear: animated];
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager setReloadUIDelegate: self];
    });
    
    [self reloadUI];
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
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    self.lampData = [container.lampContainer allValues];
    
    [self.tableView reloadData];
}

-(void)colorIndicatorSetup:(UIImageView*)colorIndicatorImage dataState:(LSFLampState*) dataState
{
    CAShapeLayer *circleShape = [CAShapeLayer layer];

    [circleShape setPosition:CGPointMake([colorIndicatorImage bounds].size.width/2.0f, [colorIndicatorImage bounds].size.height/2.0f)];

    [circleShape setBounds:CGRectMake(0.0f, 0.0f, [colorIndicatorImage bounds].size.width, [colorIndicatorImage bounds].size.height)];

    [circleShape setPath:[[UIBezierPath bezierPathWithRoundedRect:CGRectMake(0.0f, 0.0f, 10.0f, 10.0f) cornerRadius:10.0f]  CGPath]];

    [circleShape setLineWidth:0.3f];

    [circleShape setStrokeColor:[[UIColor blackColor] CGColor]];

    UIColor* colorToFill = [UIColor colorWithHue: ((CGFloat)(dataState.hue) / 360) saturation: ((CGFloat)(dataState.saturation) / 100) brightness: ((CGFloat)(dataState.brightness) / 100) alpha: 1.000];

    [circleShape setFillColor: colorToFill.CGColor];

    [[colorIndicatorImage layer] addSublayer:circleShape];
}

#pragma mark Table View Data Source Methods
-(NSInteger)tableView: (UITableView *)tableView numberOfRowsInSection: (NSInteger)section
{
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    return [container.lampContainer count];
}

-(UITableViewCell *)tableView: (UITableView *)tableView cellForRowAtIndexPath: (NSIndexPath *)indexPath
{
    LSFLampModel *data = [self.lampData objectAtIndex: [indexPath row]];
    
    LSFLightsCell *cell = [tableView dequeueReusableCellWithIdentifier: @"LightCell" forIndexPath: indexPath];
    cell.lampModel = data;
    cell.nameLabel.text = data.name;

    [self colorIndicatorSetup:cell.colorIndicatorImage dataState:data.state];

    if (data.lampDetails.dimmable)
    {
        if (cell.lampModel.state.onOff && cell.lampModel.state.brightness == 0)
        {
            dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
                LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
                [lampManager transitionLampID: cell.lampModel.theID onOffField: NO];
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

/*
 * Segue method
 */
-(void)prepareForSegue: (UIStoryboardSegue *)segue sender: (id)sender
{
    if ([segue.identifier isEqualToString: @"LampInfo"])
    {
        NSIndexPath *indexPath = [self.tableView indexPathForCell: sender];
        
        LSFLightInfoTableViewController *litvc = [segue destinationViewController];
        litvc.lampModel = [self.lampData objectAtIndex: [indexPath row]];
    }
}

@end
