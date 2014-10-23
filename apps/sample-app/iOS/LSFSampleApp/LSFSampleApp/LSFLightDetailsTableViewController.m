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

#import "LSFLightDetailsTableViewController.h"
#import "LSFConstants.h"
#import "LSFEnumConversions.h"
#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"
#import "LSFAllJoynManager.h"
#import "LSFEnums.h"

@interface LSFLightDetailsTableViewController ()

@property (nonatomic, strong) LSFLampModel *lampModel;
@property (nonatomic, strong) NSArray *data;
@property (nonatomic, strong) NSArray *detailsFields;
@property (nonatomic, strong) NSArray *aboutFields;
@property (strong, atomic) UIAlertView *loadingAV;

-(void)controllerNotificationReceived: (NSNotification *)notification;
-(void)lampNotificationReceived: (NSNotification *)notification;
-(void)deleteLampWithID: (NSString *)lampID andName: (NSString *)lampName;

@end

@implementation LSFLightDetailsTableViewController

@synthesize lampID = _lampID;
@synthesize lampModel = _lampModel;
@synthesize data = _data;
@synthesize detailsFields = _detailsFields;
@synthesize aboutFields = _aboutFields;

-(void)viewDidLoad
{
    [super viewDidLoad];

    LSFConstants *constants = [LSFConstants getConstants];
    self.detailsFields = constants.lampDetailsFields;
    self.aboutFields = constants.aboutFields;
    self.data = [[NSArray alloc] initWithObjects: self.detailsFields, self.aboutFields, nil];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    //Set notification handler
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(controllerNotificationReceived:) name: @"ControllerNotification" object: nil];
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(lampNotificationReceived:) name: @"LampNotification" object: nil];

    [self showLoadingAlert:@"Fetching lamp details..."];

    [[LSFAllJoynManager getAllJoynManager] getAboutDataForLampID: self.lampID];
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = container.lampContainer;
    self.lampModel = [lamps valueForKey: self.lampID];
    [self.tableView reloadData];

    [self dismissLoadingAlert];
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
 * LampNotification Handler
 */
-(void)lampNotificationReceived: (NSNotification *)notification
{
    NSString *lampID = [notification.userInfo valueForKey: @"lampID"];
    NSNumber *callbackOp = [notification.userInfo valueForKey: @"operation"];

    if ([self.lampID isEqualToString: lampID])
    {
        switch (callbackOp.intValue)
        {
            case LampDeleted:
                [self deleteLampWithID: lampID andName: [notification.userInfo valueForKey: @"lampName"]];
                break;
            default:
                NSLog(@"Operation not found - Taking no action");
                break;
        }
    }
}

-(void)deleteLampWithID: (NSString *)lampID andName: (NSString *)lampName
{
    if ([self.lampID isEqualToString: lampID])
    {
        [self.navigationController popToRootViewControllerAnimated: YES];

        dispatch_async(dispatch_get_main_queue(), ^{
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Connection Lost"
                                                            message: [NSString stringWithFormat: @"Unable to connect to \"%@\".", lampName]
                                                           delegate: nil
                                                  cancelButtonTitle: @"OK"
                                                  otherButtonTitles: nil];
            [alert show];
        });
    }
}

/*
 * UITableViewDataSource Implementation
 */
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return [self.data count];
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    if (section == 0)
    {
        return @"Details";
    }
    else
    {
        return @"About";
    }
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (section == 0)
    {
        return [self.detailsFields count];
    }
    else
    {
        return [self.aboutFields count];
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier: @"DetailsCell"];
    
    if ([indexPath section] == 0)
    {
        cell.textLabel.text = [self.detailsFields objectAtIndex: [indexPath row]];
    }
    else
    {
        cell.textLabel.text = [self.aboutFields objectAtIndex: [indexPath row]];
    }
    
    if ([indexPath section] == 0)
    {
        switch ([indexPath row])
        {
            case 0:
                cell.detailTextLabel.text = [LSFEnumConversions convertLampMakeToString: self.lampModel.lampDetails.lampMake];
                break;
            case 1:
                cell.detailTextLabel.text = [LSFEnumConversions convertLampModelToString: self.lampModel.lampDetails.lampModel];
                break;
            case 2:
                cell.detailTextLabel.text = [LSFEnumConversions convertDeviceTypeToString: self.lampModel.lampDetails.deviceType];
                break;
            case 3:
                cell.detailTextLabel.text = [LSFEnumConversions convertLampTypeToString: self.lampModel.lampDetails.lampType];
                break;
            case 4:
                cell.detailTextLabel.text = [LSFEnumConversions convertBaseTypeToString: self.lampModel.lampDetails.baseType];
                break;
            case 5:
                cell.detailTextLabel.text = [NSString stringWithFormat: @"%u", self.lampModel.lampDetails.lampBeamAngle];
                break;
            case 6:
                cell.detailTextLabel.text = self.lampModel.lampDetails.dimmable ? @"YES" : @"NO";
                break;
            case 7:
                cell.detailTextLabel.text = self.lampModel.lampDetails.color ? @"YES" : @"NO";
                break;
            case 8:
                cell.detailTextLabel.text = self.lampModel.lampDetails.variableColorTemp ? @"YES" : @"NO";
                break;
            case 9:
                cell.detailTextLabel.text = self.lampModel.lampDetails.hasEffects ? @"YES" : @"NO";
                break;
            case 10:
                cell.detailTextLabel.text = [NSString stringWithFormat: @"%u", self.lampModel.lampDetails.minVoltage];
                break;
            case 11:
                cell.detailTextLabel.text = [NSString stringWithFormat: @"%u", self.lampModel.lampDetails.maxVoltage];
                break;
            case 12:
                cell.detailTextLabel.text = [NSString stringWithFormat: @"%u", self.lampModel.lampDetails.wattage];
                break;
            case 13:
                cell.detailTextLabel.text = [NSString stringWithFormat: @"%u", self.lampModel.lampDetails.incandescentEquivalent];
                break;
            case 14:
                cell.detailTextLabel.text = [NSString stringWithFormat: @"%u", self.lampModel.lampDetails.maxLumens];
                break;
            case 15:
                cell.detailTextLabel.text = [NSString stringWithFormat: @"%u", self.lampModel.lampDetails.minTemperature];
                break;
            case 16:
                cell.detailTextLabel.text = [NSString stringWithFormat: @"%u", self.lampModel.lampDetails.maxTemperature];
                break;
            case 17:
                cell.detailTextLabel.text = [NSString stringWithFormat: @"%u", self.lampModel.lampDetails.colorRenderingIndex];
                break;
        }
    }
    else
    {
        switch ([indexPath row])
        {
            case 0:
                cell.detailTextLabel.text = self.lampModel.aboutData.appID;
                break;
            case 1:
                cell.detailTextLabel.text = self.lampModel.aboutData.defaultLanguage;
                break;
            case 2:
                cell.detailTextLabel.text = self.lampModel.aboutData.deviceName;
                break;
            case 3:
                cell.detailTextLabel.text = self.lampModel.aboutData.deviceID;
                break;
            case 4:
                cell.detailTextLabel.text = self.lampModel.aboutData.appName;
                break;
            case 5:
                cell.detailTextLabel.text = self.lampModel.aboutData.manufacturer;
                break;
            case 6:
                cell.detailTextLabel.text = self.lampModel.aboutData.modelNumber;
                break;
            case 7:
                cell.detailTextLabel.text = self.lampModel.aboutData.supportedLanguages;
                break;
            case 8:
                cell.detailTextLabel.text = self.lampModel.aboutData.description;
                break;
            case 9:
                cell.detailTextLabel.text = self.lampModel.aboutData.dateOfManufacture;
                break;
            case 10:
                cell.detailTextLabel.text = self.lampModel.aboutData.softwareVersion;
                break;
            case 11:
                cell.detailTextLabel.text = self.lampModel.aboutData.ajSoftwareVersion;
                break;
            case 12:
                cell.detailTextLabel.text = self.lampModel.aboutData.hardwareVersion;
                break;
            case 13:
                cell.detailTextLabel.text = self.lampModel.aboutData.supportURL;
                break;
        }
    }
    
    return cell;
}

-(void)showLoadingAlert:(NSString *)message
{
    self.loadingAV = [[UIAlertView alloc] initWithTitle:@"" message:message delegate:nil cancelButtonTitle:nil otherButtonTitles:nil];
    UIActivityIndicatorView *activityIV = [[UIActivityIndicatorView alloc] initWithFrame:CGRectMake(0, 0, 80, 40)];
    activityIV.activityIndicatorViewStyle = UIActivityIndicatorViewStyleGray;
    [activityIV startAnimating];
    [self.loadingAV setValue:activityIV forKey:@"accessoryView"];
    [self.loadingAV show];
}

-(void)dismissLoadingAlert
{
    [self.loadingAV dismissWithClickedButtonIndex:0 animated:YES];
}

@end
