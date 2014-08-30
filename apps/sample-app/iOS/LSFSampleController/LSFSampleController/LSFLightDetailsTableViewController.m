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

@interface LSFLightDetailsTableViewController ()

@property (nonatomic, strong) NSArray *data;
@property (nonatomic, strong) NSArray *detailsFields;
@property (nonatomic, strong) NSArray *aboutFields;

@end

@implementation LSFLightDetailsTableViewController

@synthesize lampModel = _lampModel;
@synthesize data = _data;
@synthesize detailsFields = _detailsFields;
@synthesize aboutFields = _aboutFields;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    LSFConstants *constants = [LSFConstants getConstants];
    self.detailsFields = constants.lampDetailsFields;
    self.aboutFields = constants.aboutFields;
    self.data = [[NSArray alloc] initWithObjects: self.detailsFields, self.aboutFields, nil];
    
    [self.tableView reloadData];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source
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
                cell.detailTextLabel.text = [NSString stringWithFormat: @"%u", self.lampModel.lampDetails.maxVoltage];
                break;
            case 11:
                cell.detailTextLabel.text = [NSString stringWithFormat: @"%u", self.lampModel.lampDetails.minVoltage];
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

@end
