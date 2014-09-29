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

#import "LSFGroupsInfoTableViewController.h"
#import "LSFGroupsChangeNameViewController.h"
#import "LSFGroupsPresetsTableViewController.h"
#import "LSFGroupsMembersTableViewController.h"
#import "LSFLampState.h"
#import "LSFPresetModel.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFConstants.h"
#import "LSFGroupModelContainer.h"
#import "LSFPresetModelContainer.h"
#import "LSFPresetModel.h"
#import "LSFUtilityFunctions.h"

@interface LSFGroupsInfoTableViewController ()

@property (nonatomic, strong) LSFGroupModel *groupModel;
@property (nonatomic) BOOL isDimmable;
@property (nonatomic) BOOL hasColor;
@property (nonatomic) BOOL hasVariableColorTemp;

-(void)brightnessSliderTapped: (UIGestureRecognizer *)gr;
-(void)hueSliderTapped: (UIGestureRecognizer *)gr;
-(void)saturationSliderTapped: (UIGestureRecognizer *)gr;
-(void)colorTempSliderTapped: (UIGestureRecognizer *)gr;
-(BOOL)checkIfLampState: (LSFLampState *) state matchesPreset: (LSFPresetModel *)data;
-(void)showAlertDialog;

@end

@implementation LSFGroupsInfoTableViewController

@synthesize groupID = _groupID;
@synthesize groupModel = _groupModel;
@synthesize nameLabel = _nameLabel;
@synthesize powerSwitch = _powerSwitch;
@synthesize presetButton = _presetButton;
@synthesize brightnessSlider = _brightnessSlider;
@synthesize brightnessLabel = _brightnessLabel;
@synthesize hueSlider = _hueSlider;
@synthesize hueLabel = _huePercentageLabel;
@synthesize saturationSlider = _saturationSlider;
@synthesize saturationLabel = _saturationLabel;
@synthesize colorTempSlider = _colorTempSlider;
@synthesize colorTempLabel = _colorTempLabel;
@synthesize isDimmable = _isDimmable;
@synthesize hasColor = _hasColor;
@synthesize hasVariableColorTemp = _hasVariableColorTemp;

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    UITapGestureRecognizer *brightnessTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(brightnessSliderTapped:)];
    [self.brightnessSlider addGestureRecognizer: brightnessTGR];
    UITapGestureRecognizer *hueTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(hueSliderTapped:)];
    [self.hueSlider addGestureRecognizer: hueTGR];
    UITapGestureRecognizer *saturationTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(saturationSliderTapped:)];
    [self.saturationSlider addGestureRecognizer: saturationTGR];
    UITapGestureRecognizer *colorTempTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(colorTempSliderTapped:)];
    [self.colorTempSlider addGestureRecognizer: colorTempTGR];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    //Set groups callback delegate
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.slgmc setReloadGroupsDelegate: self];
        [ajManager.spmc setReloadPresetsDelegate: self];
    });
    
    //Reload the table
    [self reloadGroupWithID: self.groupID];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    //Clear groups callback delegate
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.slgmc setReloadGroupsDelegate: nil];
        [ajManager.spmc setReloadPresetsDelegate: nil];
    });
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LSFReloadGroupsCallbackDelegate Implementation
 */
-(void)reloadGroupWithID: (NSString *)groupID
{
    if ([self.groupID isEqualToString: groupID])
    {
        LSFGroupModelContainer *groupContainer = [LSFGroupModelContainer getGroupModelContainer];
        NSMutableDictionary *groups = groupContainer.groupContainer;
        self.groupModel = [groups valueForKey: self.groupID];

        self.nameLabel.text = self.groupModel.name;
        self.powerSwitch.on = self.groupModel.state.onOff;

        if (self.groupModel.capability.dimmable >= SOME)
        {
            if (self.groupModel.state.onOff && self.groupModel.state.brightness == 0)
            {
                dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
                    LSFLampGroupManager *lampGroupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
                    [lampGroupManager transitionLampGroupID: self.groupModel.theID onOffField: NO];
                });
            }

            self.brightnessSlider.enabled = YES;
            self.brightnessSlider.value = self.groupModel.state.brightness;
            self.brightnessLabel.text = [NSString stringWithFormat: @"%i%%", self.groupModel.state.brightness];
            self.brightnessSliderButton.enabled = NO;
            self.isDimmable = YES;

            if (self.groupModel.capability.dimmable == SOME)
            {
                self.brightnessAsterisk.hidden = NO;
                [self performSelectorOnMainThread:@selector(updateNotSupportedLabel) withObject:nil waitUntilDone:NO];
            }
        }
        else
        {
            self.brightnessSlider.value = 0;
            self.brightnessSlider.enabled = NO;
            self.brightnessLabel.text = @"N/A";
            self.brightnessSliderButton.enabled = YES;
            self.isDimmable = NO;
            self.brightnessAsterisk.hidden = YES;
        }

        if (self.groupModel.capability.color >= SOME)
        {
            if (self.groupModel.state.saturation == 0)
            {
                self.hueSlider.value = self.groupModel.state.hue;
                self.hueSlider.enabled = NO;
                self.hueLabel.text = @"N/A";
                self.hueSliderButton.enabled = YES;
            }
            else
            {
                self.hueSlider.value = self.groupModel.state.hue;
                self.hueSlider.enabled = YES;
                self.hueLabel.text = [NSString stringWithFormat: @"%i°", self.groupModel.state.hue];
                self.hueSliderButton.enabled = NO;
            }

            if (self.groupModel.capability.color == SOME)
            {
                self.hueAsterisk.hidden = NO;
                self.saturationAsterisk.hidden = NO;
                [self performSelectorOnMainThread:@selector(updateNotSupportedLabel) withObject:nil waitUntilDone:NO];
            }

            self.saturationSlider.enabled = YES;
            self.saturationSlider.value = self.groupModel.state.saturation;
            self.saturationLabel.text = [NSString stringWithFormat: @"%i%%", self.groupModel.state.saturation];
            self.saturationSliderButton.enabled = NO;
            self.hasColor = YES;
        }
        else
        {
            self.hueSlider.value = 0;
            self.hueSlider.enabled = NO;
            self.hueLabel.text = @"N/A";
            self.hueSliderButton.enabled = YES;

            self.saturationSlider.value = 0;
            self.saturationSlider.enabled = NO;
            self.saturationLabel.text = @"N/A";
            self.saturationSliderButton.enabled = YES;

            self.hasColor = NO;
            self.hueAsterisk.hidden = YES;
            self.saturationAsterisk.hidden = YES;
        }

        if (self.groupModel.capability.temp >= SOME)
        {
            if (self.groupModel.state.saturation == 100)
            {
                self.colorTempSlider.value = self.groupModel.state.colorTemp;
                self.colorTempSlider.enabled = NO;
                self.colorTempLabel.text = @"N/A";
                self.colorTempSliderButton.enabled = YES;
            }
            else
            {
                self.colorTempSlider.value = self.groupModel.state.colorTemp;
                self.colorTempSlider.enabled = YES;
                self.colorTempLabel.text = [NSString stringWithFormat: @"%iK", self.groupModel.state.colorTemp];
                self.colorTempSliderButton.enabled = NO;
            }

            if (self.groupModel.capability.temp == SOME)
            {
                self.colorTempAsterisk.hidden = NO;
                [self performSelectorOnMainThread:@selector(updateNotSupportedLabel) withObject:nil waitUntilDone:NO];
            }

            self.hasVariableColorTemp = YES;
        }
        else
        {
            self.colorTempSlider.value = 0;
            self.colorTempSlider.enabled = NO;
            self.colorTempLabel.text = @"N/A";
            self.colorTempSliderButton.enabled = YES;
            self.hasVariableColorTemp = NO;
            self.colorTempAsterisk.hidden = YES;
        }

        LSFPresetModelContainer *presetContainer = [LSFPresetModelContainer getPresetModelContainer];
        NSArray *presets = [presetContainer.presetContainer allValues];

        BOOL presetMatched = NO;
        for (LSFPresetModel *data in presets)
        {
            BOOL matchesPreset = [self checkIfLampState: self.groupModel.state matchesPreset: data];
            
            if (matchesPreset)
            {
                [self.presetButton setTitle: data.name forState: UIControlStateNormal];
                presetMatched = YES;
            }
        }
        
        if (!presetMatched)
        {
            [self.presetButton setTitle: @"Save New Preset" forState: UIControlStateNormal];
        }
        
        if (!self.isDimmable && !self.hasColor && !self.hasVariableColorTemp)
        {
            self.presetButton.enabled = NO;
        }
    }

    [LSFUtilityFunctions colorIndicatorSetup: self.colorIndicatorImage dataState: self.groupModel.state];
}

-(void)deleteGroupsWithIDs: (NSArray *)groupIDs andNames: (NSArray *)groupNames
{
    if ([groupIDs containsObject: self.groupID])
    {
        int index = [groupIDs indexOfObject: self.groupID];

        [self.navigationController popToRootViewControllerAnimated: YES];

        dispatch_async(dispatch_get_main_queue(), ^{
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Group Not Found"
                                                            message: [NSString stringWithFormat: @"The group \"%@\" no longer exists.", [groupNames objectAtIndex: index]]
                                                           delegate: nil
                                                  cancelButtonTitle: @"OK"
                                                  otherButtonTitles: nil];
            [alert show];
        });
    }
}

/*
 * LSFReloadPresetsCallbackDelegate Implementation
 */
-(void)reloadPresets
{
    //Reload the table
    [self reloadGroupWithID: self.groupID];
}

/*
 * Various Event Handlers
 */
-(IBAction)powerSwitchPressed: (UISwitch *)sender
{
    //NSLog(@"Power switch is now %@", self.powerSwitch.isOn ? @"On" : @"Off");
    
    LSFConstants *constants = [LSFConstants getConstants];
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampGroupManager *lampGroupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        
        if (sender.isOn && self.isDimmable && self.groupModel.state.brightness == 0)
        {
            unsigned int scaledBrightness = [constants scaleLampStateValue: 25 withMax: 100];
            [lampGroupManager transitionLampGroupID: self.groupModel.theID brightnessField: scaledBrightness];
        }
        
        [lampGroupManager transitionLampGroupID: self.groupModel.theID onOffField: sender.isOn];
    });
}

-(IBAction)brightnessSliderValueChanged: (UISlider *)sender
{
    NSString *brightnessText = [NSString stringWithFormat: @"%i%%", (uint32_t)sender.value];
    self.brightnessLabel.text = brightnessText;
}

-(IBAction)brightnessSliderTouchUpInside:(id)sender
{
    [self updateBrightnessValue: sender];
}

-(IBAction)brightnessSliderTouchUpOutside:(id)sender
{
    [self updateBrightnessValue: sender];
}

-(IBAction)brightnessSliderTouchedWhileDisabled: (UIButton *)sender
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                    message: @"This group is not able to change its brightness."
                                                   delegate: nil
                                          cancelButtonTitle: @"OK"
                                          otherButtonTitles: nil];
    [alert show];
}

-(IBAction)hueSliderValueChanged: (UISlider *)sender
{
    NSString *hueText = [NSString stringWithFormat: @"%i°", (uint32_t)sender.value];
    self.hueLabel.text = hueText;
}

-(IBAction)hueSliderTouchUpInside:(id)sender
{
    [self updateHueValue: sender];
}

-(IBAction)hueSliderTouchUpOutside:(id)sender
{
    [self updateHueValue: sender];
}

-(IBAction)hueSliderTouchedWhileDisabled: (UIButton *)sender
{
    if (self.hasColor)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                        message: @"Hue has no effect when saturation is zero. Set saturation to greater than zero to enable the hue slider."
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];
    }
    else
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                        message: @"This group is not able to change its hue."
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];
    }
}

-(IBAction)saturationSliderValueChanged: (UISlider *)sender
{
    NSString *saturationText = [NSString stringWithFormat: @"%i%%", (uint32_t)sender.value];
    self.saturationLabel.text = saturationText;
}

-(IBAction)saturationSliderTouchUpInside:(id)sender
{
    [self updateSaturationValue:sender];
}

-(IBAction)saturationSliderTouchUpOutside:(id)sender
{
    [self updateSaturationValue:sender];
}

-(IBAction)saturationSliderTouchedWhileDisabled: (UIButton *)sender
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                    message: @"This group is not able to change its saturation."
                                                   delegate: nil
                                          cancelButtonTitle: @"OK"
                                          otherButtonTitles: nil];
    [alert show];
}

-(IBAction)colorTempSliderValueChanged: (UISlider *)sender
{
    NSString *colorTempText = [NSString stringWithFormat: @"%iK", (uint32_t)sender.value];
    self.colorTempLabel.text = colorTempText;
}

-(IBAction)colorSliderTouchUpInside:(id)sender
{
    [self updateColorValue: sender];
}

-(IBAction)colorSliderTouchUpOutside:(id)sender
{
    [self updateColorValue: sender];
}

-(IBAction)colorTempSliderTouchedWhileDisabled: (UIButton *)sender
{
    if (self.hasVariableColorTemp)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                        message: @"Color temperature has no effect when saturation is 100%. Set saturation to less than 100% to enable the color temperature slider."
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];
    }
    else
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                        message: @"This group is not able to change its color temperature."
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];
    }
}

/*
 * Segue method
 */
-(BOOL)shouldPerformSegueWithIdentifier: (NSString *)identifier sender: (id)sender
{
    if ([identifier isEqualToString: @"ChangeGroupName"])
    {
        if ([self.groupModel.theID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
        {
            UITableViewCell *cell = (UITableViewCell *)sender;
            cell.selectionStyle = UITableViewCellSelectionStyleNone;
            cell.selectionStyle = UITableViewCellSelectionStyleBlue;
            
            [self showAlertDialog];
            return NO;
        }
        else
        {
            return YES;
        }
    }
    else if ([identifier isEqualToString: @"ChangeGroupMembers"])
    {
        if ([self.groupModel.theID isEqualToString: ([LSFConstants getConstants]).ALL_LAMPS_GROUP_ID])
        {
            UITableViewCell *cell = (UITableViewCell *)sender;
            cell.selectionStyle = UITableViewCellSelectionStyleNone;
            cell.selectionStyle = UITableViewCellSelectionStyleBlue;
            
            [self showAlertDialog];
            return NO;
        }
        else
        {
            return YES;
        }
    }
    else
    {
        return YES;
    }
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString: @"ChangeGroupName"])
    {
        LSFGroupsChangeNameViewController *gcnvc = [segue destinationViewController];
        gcnvc.groupID = self.groupID;
    }
    else if ([segue.identifier isEqualToString: @"GroupPresets"])
    {
        LSFGroupsPresetsTableViewController *gptvc = [segue destinationViewController];
        gptvc.groupID = self.groupID;
    }
    else if ([segue.identifier isEqualToString: @"ChangeGroupMembers"])
    {
        UINavigationController *nc = (UINavigationController *)[segue destinationViewController];
        LSFGroupsMembersTableViewController *gmtvc = (LSFGroupsMembersTableViewController *)nc.topViewController;
        gmtvc.groupID = [NSString stringWithString: self.groupID];
    }
}

/*
 * Private functions
 */
-(void)brightnessSliderTapped: (UIGestureRecognizer *)gr
{
    UISlider *s = (UISlider *)gr.view;
    
    if (s.highlighted)
    {
        //tap on thumb, let slider deal with it
        return;
    }
    
    CGPoint pt = [gr locationInView: s];
    CGFloat percentage = pt.x / s.bounds.size.width;
    CGFloat delta = percentage * (s.maximumValue - s.minimumValue);
    CGFloat value = round(s.minimumValue + delta);
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFConstants *constants = [LSFConstants getConstants];
        LSFLampGroupManager *lampGroupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)value withMax: 100];
        if ((self.groupModel.state.onOff == NO) && (scaledBrightness > 0))
        {
            [lampGroupManager transitionLampGroupID: self.groupModel.theID onOffField: YES];
        }
        [lampGroupManager transitionLampGroupID: self.groupModel.theID brightnessField: scaledBrightness];
    });
}

-(void)hueSliderTapped: (UIGestureRecognizer *)gr
{
    UISlider *s = (UISlider *)gr.view;
    
    if (s.highlighted)
    {
        //tap on thumb, let slider deal with it
        return;
    }
    
    CGPoint pt = [gr locationInView: s];
    CGFloat percentage = pt.x / s.bounds.size.width;
    CGFloat delta = percentage * (s.maximumValue - s.minimumValue);
    CGFloat value = round(s.minimumValue + delta);
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFConstants *constants = [LSFConstants getConstants];
        LSFLampGroupManager *lampGroupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        unsigned int scaledHue = [constants scaleLampStateValue: (uint32_t)value withMax: 360];
        [lampGroupManager transitionLampGroupID: self.groupModel.theID hueField: scaledHue];
    });
}

-(void)saturationSliderTapped: (UIGestureRecognizer *)gr
{
    UISlider *s = (UISlider *)gr.view;
    
    if (s.highlighted)
    {
        //tap on thumb, let slider deal with it
        return;
    }
    
    CGPoint pt = [gr locationInView: s];
    CGFloat percentage = pt.x / s.bounds.size.width;
    CGFloat delta = percentage * (s.maximumValue - s.minimumValue);
    CGFloat value = round(s.minimumValue + delta);
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFConstants *constants = [LSFConstants getConstants];
        LSFLampGroupManager *lampGroupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        unsigned int scaledSaturation = [constants scaleLampStateValue: (uint32_t)value withMax: 100];
        [lampGroupManager transitionLampGroupID: self.groupModel.theID saturationField: scaledSaturation];
    });
}

-(void)colorTempSliderTapped: (UIGestureRecognizer *)gr
{
    UISlider *s = (UISlider *)gr.view;
    
    if (s.highlighted)
    {
        //tap on thumb, let slider deal with it
        return;
    }
    
    CGPoint pt = [gr locationInView: s];
    CGFloat percentage = pt.x / s.bounds.size.width;
    CGFloat delta = percentage * (s.maximumValue - s.minimumValue);
    CGFloat value = round(s.minimumValue + delta);
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFConstants *constants = [LSFConstants getConstants];
        LSFLampGroupManager *lampGroupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        unsigned int scaledColorTemp = [constants scaleColorTemp: (uint32_t)value];
        [lampGroupManager transitionLampGroupID: self.groupModel.theID colorTempField: scaledColorTemp];
    });
}

-(BOOL)checkIfLampState: (LSFLampState *) state matchesPreset: (LSFPresetModel *)data
{
    BOOL returnValue = NO;
    
    LSFConstants *constants = [LSFConstants getConstants];
    unsigned int scaledBrightness = [constants scaleLampStateValue: state.brightness withMax: 100];
    unsigned int scaledHue = [constants scaleLampStateValue: state.hue withMax: 360];
    unsigned int scaledSaturation = [constants scaleLampStateValue: state.saturation withMax: 100];
    unsigned int scaledColorTemp = [constants scaleColorTemp: state.colorTemp];
    
    if ((state.onOff == data.state.onOff) && (scaledBrightness == data.state.brightness) && (scaledHue == data.state.hue) && (scaledSaturation == data.state.saturation) && (scaledColorTemp == data.state.colorTemp))
    {
        returnValue = YES;
    }
    
    return returnValue;
}

-(void)showAlertDialog
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                    message: @"Cannot change the name of the \"All Lamps\" group"
                                                   delegate: nil
                                          cancelButtonTitle: @"OK"
                                          otherButtonTitles: nil];
    [alert show];
}

-(void)updateBrightnessValue:(id)sender
{
    LSFConstants *constants = [LSFConstants getConstants];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampGroupManager *lampGroupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)((UISlider *)sender).value withMax: 100];
        if ((self.groupModel.state.onOff == NO) && (scaledBrightness > 0))
        {
            [lampGroupManager transitionLampGroupID: self.groupModel.theID onOffField: YES];
        }
        [lampGroupManager transitionLampGroupID: self.groupModel.theID brightnessField: scaledBrightness];
    });
}

-(void)updateHueValue:(id)sender
{
    LSFConstants *constants = [LSFConstants getConstants];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampGroupManager *lampGroupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        unsigned int scaledHue = [constants scaleLampStateValue: (uint32_t)((UISlider *)sender).value withMax: 360];
        [lampGroupManager transitionLampGroupID: self.groupModel.theID hueField: scaledHue];
    });
}

-(void)updateSaturationValue:(id)sender
{
    LSFConstants *constants = [LSFConstants getConstants];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampGroupManager *lampGroupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        unsigned int scaledSaturation = [constants scaleLampStateValue: (uint32_t)((UISlider *)sender).value withMax: 100];
        [lampGroupManager transitionLampGroupID: self.groupModel.theID saturationField: scaledSaturation];
    });
}

-(void)updateColorValue:(id)sender
{
    LSFConstants *constants = [LSFConstants getConstants];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampGroupManager *lampGroupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        unsigned int scaledColorTemp = [constants scaleColorTemp: (uint32_t)((UISlider *)sender).value];
        [lampGroupManager transitionLampGroupID: self.groupModel.theID colorTempField: scaledColorTemp];
    });
}

-(void)updateNotSupportedLabel
{
    [self.tableView footerViewForSection:1].textLabel.text = @"* Not supported by some lamps in this group";
}

@end
