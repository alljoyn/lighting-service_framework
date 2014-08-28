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

#import "LSFPulseEffectTableViewController.h"
#import "LSFSceneElementEffectPropertiesViewController.h"
#import "LSFPulseEffectDataModel.h"
#import "LSFConstants.h"
#import "LSFAllJoynManager.h"

@interface LSFPulseEffectTableViewController ()

@property (nonatomic, strong) LSFPulseEffectDataModel *pedm;

-(void)endBrightnessSliderTapped: (UIGestureRecognizer *)gr;
-(void)endHueSliderTapped: (UIGestureRecognizer *)gr;
-(void)endSaturationSliderTapped: (UIGestureRecognizer *)gr;
-(void)endColorTempSliderTapped: (UIGestureRecognizer *)gr;
-(void)showWarning;

@end

@implementation LSFPulseEffectTableViewController

@synthesize startPropertiesSwitch = _startPropertiesSwitch;
@synthesize endPresetButton = _endPresetButton;
@synthesize endBrightnessSlider = _endBrightnessSlider;
@synthesize brightnessButton = _brightnessButton;
@synthesize endBrightnessLabel = _endBrightnessLabel;
@synthesize endHueSlider = _endHueSlider;
@synthesize hueButton = _hueButton;
@synthesize endHueLabel = _endHueLabel;
@synthesize endSaturationSlider = _endSaturationSlider;
@synthesize saturationButton = _saturationButton;
@synthesize endSaturationLabel = _endSaturationLabel;
@synthesize endColorTempSlider = _endColorTempSlider;
@synthesize colorTempButton = _colorTempButton;
@synthesize endColorTempLabel = _endColorTempLabel;
@synthesize durationLabel = _durationLabel;
@synthesize periodLabel = _periodLabel;
@synthesize numPulsesLabel = _numPulsesLabel;
@synthesize pedm = _pedm;
@synthesize sceneModel = _sceneModel;
@synthesize sceneElement = _sceneElement;


-(void)viewDidLoad
{
    [super viewDidLoad];

    UITapGestureRecognizer *endBrightnessTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(endBrightnessSliderTapped:)];
    [self.endBrightnessSlider addGestureRecognizer: endBrightnessTGR];
    UITapGestureRecognizer *endHueTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(endHueSliderTapped:)];
    [self.endHueSlider addGestureRecognizer: endHueTGR];
    UITapGestureRecognizer *endSaturationTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(endSaturationSliderTapped:)];
    [self.endSaturationSlider addGestureRecognizer: endSaturationTGR];
    UITapGestureRecognizer *endColorTempTGR = [[UITapGestureRecognizer alloc] initWithTarget: self action: @selector(endColorTempSliderTapped:)];
    [self.endColorTempSlider addGestureRecognizer: endColorTempTGR];

    self.pedm = [[LSFPulseEffectDataModel alloc] init];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    self.durationLabel.text = [NSString stringWithFormat: @"%.8g seconds", ((double)self.pedm.duration / 1000.0)];
    self.periodLabel.text = [NSString stringWithFormat: @"%.8g seconds", ((double)self.pedm.period / 1000.0)];
    self.numPulsesLabel.text = [NSString stringWithFormat: @"%u", self.pedm.numPulses];
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * Various event handlers
 */
-(IBAction)endBrightnessSliderValueChanged: (UISlider *)sender
{
    NSString *endBrightnessText = [NSString stringWithFormat: @"%i%%", (uint32_t)sender.value];
    self.endBrightnessLabel.text = endBrightnessText;
}

-(IBAction)endHueSliderValueChanged: (UISlider *)sender
{
    NSString *endHueText = [NSString stringWithFormat: @"%i°", (uint32_t)sender.value];
    self.endHueLabel.text = endHueText;
}

-(IBAction)endSaturationSliderValueChanged: (UISlider *)sender
{
    NSString *endSaturationText = [NSString stringWithFormat: @"%i%%", (uint32_t)sender.value];
    self.endSaturationLabel.text = endSaturationText;
}

-(IBAction)endColorTempSliderValueChanged: (UISlider *)sender
{
    NSString *endColorTempText = [NSString stringWithFormat: @"%iK", (uint32_t)sender.value];
    self.endColorTempLabel.text = endColorTempText;
}

-(IBAction)startPropertiesSwitchValueChanged: (UISwitch *)sender
{
    if (sender.on)
    {
        self.brightnessSlider.enabled = NO;
        self.hueSlider.enabled = NO;
        self.saturationSlider.enabled = NO;
        self.colorTempSlider.enabled = NO;

        self.brightnessLabel.text = @"N/A";
        self.hueLabel.text = @"N/A";
        self.saturationLabel.text = @"N/A";
        self.colorTempLabel.text = @"N/A";

        self.brightnessButton.enabled = YES;
        self.hueButton.enabled = YES;
        self.saturationButton.enabled = YES;
        self.colorTempButton.enabled = YES;
    }
    else
    {
        self.brightnessSlider.enabled = YES;
        self.hueSlider.enabled = YES;
        self.saturationSlider.enabled = YES;
        self.colorTempSlider.enabled = YES;

        self.brightnessLabel.text = [NSString stringWithFormat: @"%i%%", (uint32_t)self.brightnessSlider.value];
        self.hueLabel.text = [NSString stringWithFormat: @"%i°", (uint32_t)self.hueSlider.value];
        self.saturationLabel.text = [NSString stringWithFormat: @"%i%%", (uint32_t)self.saturationSlider.value];
        self.colorTempLabel.text = [NSString stringWithFormat: @"%iK", (uint32_t)self.colorTempSlider.value];

        self.brightnessButton.enabled = NO;
        self.hueButton.enabled = NO;
        self.saturationButton.enabled = NO;
        self.colorTempButton.enabled = NO;
    }
}

-(IBAction)brightnessSliderTouchedWhileDisabled: (UIButton *)sender
{
    [self showWarning];
}

-(IBAction)hueSliderTouchedWhileDisabled: (UIButton *)sender
{
    [self showWarning];
}

-(IBAction)saturationSliderTouchedWhileDisabled: (UIButton *)sender
{
    [self showWarning];
}

-(IBAction)colorTempSliderTouchedWhileDisabled: (UIButton *)sender
{
    [self showWarning];
}

-(IBAction)doneButtonPressed: (id)sender
{
    LSFConstants *constants = [LSFConstants getConstants];
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    //Get Scene Members
    self.pedm.members = self.sceneElement.members;

    //Get Capability
    self.pedm.capability = self.sceneElement.capability;

    //Get Lamp State
    if (!self.startPropertiesSwitch.on)
    {
        NSLog(@"Switch for start properties is off, using start state");

        unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)self.brightnessSlider.value withMax: 100];
        unsigned int scaledHue = [constants scaleLampStateValue: (uint32_t)self.hueSlider.value withMax: 360];
        unsigned int scaledSaturation = [constants scaleLampStateValue: (uint32_t)self.saturationSlider.value withMax: 100];
        unsigned int scaledColorTemp = [constants scaleColorTemp: (uint32_t)self.colorTempSlider.value];

        self.pedm.state.brightness = scaledBrightness;
        self.pedm.state.hue = scaledHue;
        self.pedm.state.saturation = scaledSaturation;
        self.pedm.state.colorTemp = scaledColorTemp;
    }
    else
    {
        NSLog(@"Switch for start properties is on, not using start state");
    }

    unsigned int scaledEndBrightness = [constants scaleLampStateValue: (uint32_t)self.endBrightnessSlider.value withMax: 100];
    unsigned int scaledEndHue = [constants scaleLampStateValue: (uint32_t)self.endHueSlider.value withMax: 360];
    unsigned int scaledEndSaturation = [constants scaleLampStateValue: (uint32_t)self.endSaturationSlider.value withMax: 100];
    unsigned int scaledEndColorTemp = [constants scaleColorTemp: (uint32_t)self.endColorTempSlider.value];

    self.pedm.endState.brightness = scaledEndBrightness;
    self.pedm.endState.hue = scaledEndHue;
    self.pedm.endState.saturation = scaledEndSaturation;
    self.pedm.endState.colorTemp = scaledEndColorTemp;

    [self.sceneModel updatePulseEffect: self.pedm];

    if (self.sceneModel.theID != nil && ![self.sceneModel.theID isEqualToString: @""])
    {
        NSLog(@"Scene Model ID is not nil or empty, call update scene");
    }
    else
    {
        NSLog(@"Scene Model ID is nil or empty, call create scene");

        [ajManager.lsfSceneManager createScene: [self.sceneModel toScene] andSceneName: self.sceneModel.name];
    }

    [self dismissViewControllerAnimated: YES completion: nil];
}

/*
 * Private functions
 */
-(void)endBrightnessSliderTapped: (UIGestureRecognizer *)gr
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

    NSString *endBrightnessText = [NSString stringWithFormat: @"%i%%", (uint32_t)value];
    self.endBrightnessLabel.text = endBrightnessText;
    self.endBrightnessSlider.value = value;
}

-(void)endHueSliderTapped: (UIGestureRecognizer *)gr
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

    NSString *endHueText = [NSString stringWithFormat: @"%i°", (uint32_t)value];
    self.endHueLabel.text = endHueText;
    self.endHueSlider.value = value;
}

-(void)endSaturationSliderTapped: (UIGestureRecognizer *)gr
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

    NSString *endSaturationText = [NSString stringWithFormat: @"%i%%", (uint32_t)value];
    self.endSaturationLabel.text = endSaturationText;
    self.endSaturationSlider.value = value;
}

-(void)endColorTempSliderTapped: (UIGestureRecognizer *)gr
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

    NSString *endColorTempText = [NSString stringWithFormat: @"%iK", (uint32_t)value];
    self.endColorTempLabel.text = endColorTempText;
    self.endColorTempSlider.value = value;
}

-(void)showWarning
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                    message: @"You must switch the start properties switch off if you want to specify a start state."
                                                   delegate: nil
                                          cancelButtonTitle: @"OK"
                                          otherButtonTitles: nil];
    [alert show];
}

/*
 * Segue Methods
 */
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    LSFSceneElementEffectPropertiesViewController *seepvc = [segue destinationViewController];
    seepvc.pedm = self.pedm;

    if ([segue.identifier isEqualToString: @"PulseDuration"])
    {
        seepvc.effectProperty = PulseDuration;
    }
    else if ([segue.identifier isEqualToString: @"PulsePeriod"])
    {
        seepvc.effectProperty = PulsePeriod;
    }
    else if ([segue.identifier isEqualToString: @"PulseNumPulses"])
    {
        seepvc.effectProperty = PulseNumPulses;
    }
}

@end
