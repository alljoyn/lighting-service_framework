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

#import "LSFLightInfoTableViewController.h"
#import "LSFLightDetailsTableViewController.h"
#import "LSFLightsPresetsTableViewController.h"
#import "LSFLightsChangeNameViewController.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFConstants.h"
#import "LSFPresetModel.h"
#import "LSFPresetModelContainer.h"
#import "LSFLampModelContainer.h"
#import "LSFUtilityFunctions.h"
#import "LSFEnums.h"
#import "LSFLampModel.h"

@interface LSFLightInfoTableViewController ()

@property (nonatomic, strong) LSFLampModel *lampModel;
@property (nonatomic) BOOL isDimmable;
@property (nonatomic) BOOL hasColor;
@property (nonatomic) BOOL hasVariableColorTemp;

-(void)controllerNotificationReceived: (NSNotification *)notification;
-(void)lampNotificationReceived: (NSNotification *)notification;
-(void)reloadLampWithID: (NSString *)lampID;
-(void)deleteLampWithID: (NSString *)lampID andName: (NSString *)lampName;
-(void)presetNotificationReceived: (NSNotification *)notification;
-(void)brightnessSliderTapped: (UIGestureRecognizer *)gr;
-(void)hueSliderTapped: (UIGestureRecognizer *)gr;
-(void)saturationSliderTapped: (UIGestureRecognizer *)gr;
-(void)colorTempSliderTapped: (UIGestureRecognizer *)gr;
-(BOOL)checkIfLampState: (LSFLampState *) state matchesPreset: (LSFPresetModel *)data;

@end

@implementation LSFLightInfoTableViewController

@synthesize lampID = _lampID;
@synthesize nameLabel = _nameLabel;
@synthesize powerSwitch = _powerSwitch;
@synthesize presetButton = _presetButton;
@synthesize brightnessSlider = _brightnessSlider;
@synthesize brightnessSliderButton = _brightnessSliderButton;
@synthesize brightnessLabel = _brightnessLabel;
@synthesize hueSlider = _hueSlider;
@synthesize hueSliderButton = _hueSliderButton;
@synthesize hueLabel = _huePercentageLabel;
@synthesize saturationSlider = _saturationSlider;
@synthesize saturationSliderButton = _saturationSliderButton;
@synthesize saturationLabel = _saturationLabel;
@synthesize colorTempSlider = _colorTempSlider;
@synthesize colorTempSliderButton = _colorTempSliderButton;
@synthesize colorTempLabel = _colorTempLabel;
@synthesize lumensLabel = _lumensLabel;
@synthesize energyUsageLabel = _energyUsageLabel;
@synthesize isDimmable = _isDimmable;
@synthesize hasColor = _hasColor;
@synthesize hasVariableColorTemp = _hasVariableColorTemp;

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    UITapGestureRecognizer *brightnessTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(brightnessSliderTapped:)];
    [self.brightnessSlider addGestureRecognizer: brightnessTGR];
    [self.brightnessSlider setThumbImage: [UIImage imageNamed: @"power_slider_normal_icon.png"] forState: UIControlStateNormal];
    [self.brightnessSlider setThumbImage: [UIImage imageNamed: @"power_slider_pressed_icon.png"] forState: UIControlStateHighlighted];

    UITapGestureRecognizer *hueTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(hueSliderTapped:)];
    [self.hueSlider addGestureRecognizer: hueTGR];
    [self.hueSlider setThumbImage: [UIImage imageNamed: @"power_slider_normal_icon.png"] forState: UIControlStateNormal];
    [self.hueSlider setThumbImage: [UIImage imageNamed: @"power_slider_pressed_icon.png"] forState: UIControlStateHighlighted];

    UITapGestureRecognizer *saturationTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(saturationSliderTapped:)];
    [self.saturationSlider addGestureRecognizer: saturationTGR];
    [self.saturationSlider setThumbImage: [UIImage imageNamed: @"power_slider_normal_icon.png"] forState: UIControlStateNormal];
    [self.saturationSlider setThumbImage: [UIImage imageNamed: @"power_slider_pressed_icon.png"] forState: UIControlStateHighlighted];

    UITapGestureRecognizer *colorTempTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(colorTempSliderTapped:)];
    [self.colorTempSlider addGestureRecognizer: colorTempTGR];
    [self.colorTempSlider setThumbImage: [UIImage imageNamed: @"power_slider_normal_icon.png"] forState: UIControlStateNormal];
    [self.colorTempSlider setThumbImage: [UIImage imageNamed: @"power_slider_pressed_icon.png"] forState: UIControlStateHighlighted];

    self.colorIndicatorImage.layer.rasterizationScale = [UIScreen mainScreen].scale;
    self.colorIndicatorImage.layer.shouldRasterize = YES;
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    //Set notification handler
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(controllerNotificationReceived:) name: @"ControllerNotification" object: nil];
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(lampNotificationReceived:) name: @"LampNotification" object: nil];
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(presetNotificationReceived:) name: @"PresetNotification" object: nil];
    
    //Reload the table
    [self reloadLampWithID: self.lampID];
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
            case LampNameUpdated:
            case LampDetailsUpdated:
            case LampStateUpdated:
            case LampParametersUpdated:
                [self reloadLampWithID: lampID];
                break;
            default:
                NSLog(@"Operation not found - Taking no action");
                break;
        }
    }
}

-(void)reloadLampWithID: (NSString *)lampID
{
    LSFLampModelContainer *lampsContainer = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = lampsContainer.lampContainer;
    self.lampModel = [lamps valueForKey: self.lampID];

    self.nameLabel.text = self.lampModel.name;
    self.powerSwitch.on = self.lampModel.state.onOff;

    if (self.lampModel.lampDetails.dimmable)
    {
        if (self.lampModel.state.onOff && self.lampModel.state.brightness == 0)
        {
            dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
                LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
                [lampManager transitionLampID: self.lampID onOffField: NO];
            });
        }

        self.brightnessSlider.enabled = YES;
        self.brightnessSlider.value = self.lampModel.state.brightness;
        self.brightnessLabel.text = [NSString stringWithFormat: @"%i%%", self.lampModel.state.brightness];
        self.brightnessSliderButton.enabled = NO;
        self.isDimmable = YES;
    }
    else
    {
        self.brightnessSlider.value = 0;
        self.brightnessSlider.enabled = NO;
        self.brightnessLabel.text = @"N/A";
        self.brightnessSliderButton.enabled = YES;
        self.isDimmable = NO;
    }

    if (self.lampModel.lampDetails.color)
    {
        if (self.lampModel.state.saturation == 0)
        {
            self.hueSlider.value = self.lampModel.state.hue;
            self.hueSlider.enabled = NO;
            self.hueLabel.text = @"N/A";
            self.hueSliderButton.enabled = YES;
        }
        else
        {
            self.hueSlider.value = self.lampModel.state.hue;
            self.hueSlider.enabled = YES;
            self.hueLabel.text = [NSString stringWithFormat: @"%i°", self.lampModel.state.hue];
            self.hueSliderButton.enabled = NO;
        }

        self.saturationSlider.enabled = YES;
        self.saturationSlider.value = self.lampModel.state.saturation;
        self.saturationLabel.text = [NSString stringWithFormat: @"%i%%", self.lampModel.state.saturation];
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
    }

    if (self.lampModel.lampDetails.variableColorTemp)
    {
        if (self.lampModel.state.saturation == 100)
        {
            self.colorTempSlider.value = self.lampModel.state.colorTemp;
            self.colorTempSlider.enabled = NO;
            self.colorTempLabel.text = @"N/A";
            self.colorTempSliderButton.enabled = YES;
        }
        else
        {
            self.colorTempSlider.value = self.lampModel.state.colorTemp;
            self.colorTempSlider.enabled = YES;
            self.colorTempLabel.text = [NSString stringWithFormat: @"%iK", self.lampModel.state.colorTemp];
            self.colorTempSliderButton.enabled = NO;
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
    }

    if (!self.isDimmable || !self.hasColor || !self.hasVariableColorTemp)
    {
        [self.presetButton setTitle: @"Save New Preset" forState: UIControlStateNormal];
        self.presetButton.enabled = NO;
    }
    else
    {
        self.presetButton.enabled = YES;

        LSFPresetModelContainer *container = [LSFPresetModelContainer getPresetModelContainer];
        NSArray *presets = [container.presetContainer allValues];

        NSMutableArray *presetsArray = [[NSMutableArray alloc] init];
        BOOL presetMatched = NO;
        for (LSFPresetModel *data in presets)
        {
            BOOL matchesPreset = [self checkIfLampState: self.lampModel.state matchesPreset: data];

            if (matchesPreset)
            {
                [presetsArray addObject: data.name];
                presetMatched = YES;
            }
        }

        if (presetMatched)
        {
            NSArray *sortedArray = [presetsArray sortedArrayUsingSelector: @selector(localizedCaseInsensitiveCompare:)];
            NSMutableString *presetsMatched = [[NSMutableString alloc] init];

            for (NSString *presetName in sortedArray)
            {
                [presetsMatched appendString: [NSString stringWithFormat:@"%@, ", presetName]];
            }

            [presetsMatched deleteCharactersInRange: NSMakeRange(presetsMatched.length - 2, 2)];
            [self.presetButton setTitle: presetsMatched forState: UIControlStateNormal];
        }
        else
        {
            [self.presetButton setTitle: @"Save New Preset" forState: UIControlStateNormal];
        }
    }

    self.lumensLabel.text = [NSString stringWithFormat: @"%i", self.lampModel.lampParameters.lumens];
    self.energyUsageLabel.text = [NSString stringWithFormat: @"%i mW", self.lampModel.lampParameters.energyUsageMilliwatts];

    [LSFUtilityFunctions colorIndicatorSetup: self.colorIndicatorImage dataState: self.lampModel.state];
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
 * PresetNotification Handler
 */
-(void)presetNotificationReceived: (NSNotification *)notification
{
    //Reload the table
    [self reloadLampWithID: self.lampID];
}

/*
 * Various event handlers
 */
-(IBAction)powerSwitchPressed: (UISwitch *)sender
{
    //NSLog(@"Power switch pressed. Switch is now %@", sender.isOn ? @"ON" : @"OFF");
    
    LSFConstants *constants = [LSFConstants getConstants];
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        BOOL brightnessChanged = NO;
        LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        
        if (sender.isOn && self.isDimmable && self.lampModel.state.brightness == 0)
        {
            brightnessChanged = YES;
            unsigned int scaledBrightness = [constants scaleLampStateValue: 25 withMax: 100];
            [lampManager transitionLampID: self.lampID brightnessField: scaledBrightness];
        }
        
        [lampManager transitionLampID: self.lampID onOffField: sender.isOn];
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

/*
 * Segue method
 */
-(void)prepareForSegue: (UIStoryboardSegue *)segue sender: (id)sender
{
    if ([segue.identifier isEqualToString: @"LampDetails"])
    {
        LSFLightDetailsTableViewController *ldtvc = [segue destinationViewController];
        ldtvc.lampID = self.lampID;
    }
    else if ([segue.identifier isEqualToString: @"ChangeLightName"])
    {
        LSFLightsChangeNameViewController *clnvc = [segue destinationViewController];
        clnvc.lampID = self.lampID;
    }
    else if ([segue.identifier isEqualToString: @"LampPresets"])
    {
        LSFLightsPresetsTableViewController *lptvc = [segue destinationViewController];
        lptvc.lampID = self.lampID;
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
        LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)value withMax: 100];
        [lampManager transitionLampID: self.lampID brightnessField: scaledBrightness];
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
        LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        unsigned int scaledHue = [constants scaleLampStateValue: (uint32_t)value withMax: 360];
        [lampManager transitionLampID: self.lampID hueField: scaledHue];
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
        LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        unsigned int scaledSaturation = [constants scaleLampStateValue: (uint32_t)value withMax: 100];
        [lampManager transitionLampID: self.lampID saturationField: scaledSaturation];
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
        LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        unsigned int scaledColorTemp = [constants scaleColorTemp: (uint32_t)value];
        [lampManager transitionLampID: self.lampID colorTempField: scaledColorTemp];
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

-(void)updateBrightnessValue:(id)sender
{
    LSFConstants *constants = [LSFConstants getConstants];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)((UISlider *)sender).value withMax: 100];
        [lampManager transitionLampID: self.lampID brightnessField: scaledBrightness];
    });
}

-(void)updateHueValue:(id)sender
{
    LSFConstants *constants = [LSFConstants getConstants];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        unsigned int scaledHue = [constants scaleLampStateValue: (uint32_t)((UISlider *)sender).value withMax: 360];
        [lampManager transitionLampID: self.lampID hueField: scaledHue];
    });
}

-(void)updateSaturationValue:(id)sender
{
    LSFConstants *constants = [LSFConstants getConstants];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        unsigned int scaledSaturation = [constants scaleLampStateValue: (uint32_t)((UISlider *)sender).value withMax: 100];
        [lampManager transitionLampID: self.lampID saturationField: scaledSaturation];
    });
}

-(void)updateColorValue:(id)sender
{
    LSFConstants *constants = [LSFConstants getConstants];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        unsigned int scaledColorTemp = [constants scaleColorTemp: (uint32_t)((UISlider *)sender).value];
        [lampManager transitionLampID: self.lampID colorTempField: scaledColorTemp];
    });
}

/*
 * Button event handlers
 */
-(IBAction)brightnessSliderTouchedWhileDisabled: (UIButton *)sender;
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                    message: @"This Lamp is not able to change its brightness."
                                                   delegate: nil
                                          cancelButtonTitle: @"OK"
                                          otherButtonTitles: nil];
    [alert show];
}

-(IBAction)hueSliderTouchedWhileDisabled: (UIButton *)sender;
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
                                                        message: @"This Lamp is not able to change its hue."
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];
    }
}

-(IBAction)saturationSliderTouchedWhileDisabled: (UIButton *)sender;
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                    message: @"This Lamp is not able to change its saturation."
                                                   delegate: nil
                                          cancelButtonTitle: @"OK"
                                          otherButtonTitles: nil];
    [alert show];
}

-(IBAction)colorTempSliderTouchedWhileDisabled: (UIButton *)sender;
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
                                                        message: @"This Lamp is not able to change its color temperature."
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];
    }
}

@end
