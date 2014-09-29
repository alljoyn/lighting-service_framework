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

#import "LSFNoEffectTableViewController.h"
#import "LSFConstants.h"
#import "LSFAllJoynManager.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"
#import "LSFLampModelContainer.h"
#import "LSFGroupModelContainer.h"
#import "LSFUtilityFunctions.h"
#import "LSFScenesPresetsTableViewController.h"
#import "LSFUtilityFunctions.h"
#import "LSFPresetModelContainer.h"
#import "LSFPresetModel.h"

@implementation LSFNoEffectTableViewController

@synthesize sceneModel = _sceneModel;
@synthesize nedm = _nedm;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    LSFConstants *constants = [LSFConstants getConstants];

    if (self.nedm != nil)
    {
        unsigned int brightness = [constants unscaleLampStateValue: self.nedm.state.brightness withMax: 100];
        self.brightnessSlider.value = brightness;
        self.brightnessLabel.text = [NSString stringWithFormat: @"%u%%", brightness];

        unsigned int hue = [constants unscaleLampStateValue: self.nedm.state.hue withMax: 360];
        self.hueSlider.value = hue;
        self.hueLabel.text = [NSString stringWithFormat: @"%u°", hue];

        unsigned int saturation = [constants unscaleLampStateValue: self.nedm.state.saturation withMax: 100];
        self.saturationSlider.value = saturation;
        self.saturationLabel.text = [NSString stringWithFormat: @"%u%%", saturation];

        unsigned int colorTemp = [constants unscaleColorTemp: self.nedm.state.colorTemp];
        self.colorTempSlider.value = colorTemp;
        self.colorTempLabel.text = [NSString stringWithFormat: @"%iK", colorTemp];

        [self checkSaturationValue:self.nedm.state];
    }

    LSFLampState *lstate = [[LSFLampState alloc] initWithOnOff:YES brightness: self.brightnessSlider.value hue: self.hueSlider.value saturation: self.saturationSlider.value colorTemp: self.colorTempSlider.value];
    [LSFUtilityFunctions colorIndicatorSetup: self.colorIndicatorImage dataState: lstate];

    [self presetButtonSetup: self.nedm.state];
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * Table View Data Source Methods
 */
-(NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    if (section == 0)
    {
        return [NSString stringWithFormat: @"Set the properties that %@ will change to", [LSFUtilityFunctions buildSectionTitleString: self.nedm]];
    }

    return @"";
}

/*
 * Button Event Handlers
 */
-(IBAction)doneButtonPressed: (id)sender
{
    LSFConstants *constants = [LSFConstants getConstants];
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    //Get Lamp State
    unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)self.brightnessSlider.value withMax: 100];
    unsigned int scaledHue = [constants scaleLampStateValue: (uint32_t)self.hueSlider.value withMax: 360];
    unsigned int scaledSaturation = [constants scaleLampStateValue: (uint32_t)self.saturationSlider.value withMax: 100];
    unsigned int scaledColorTemp = [constants scaleColorTemp: (uint32_t)self.colorTempSlider.value];

    self.nedm.state.brightness = scaledBrightness;
    self.nedm.state.hue = scaledHue;
    self.nedm.state.saturation = scaledSaturation;
    self.nedm.state.colorTemp = scaledColorTemp;

    [self.sceneModel updateNoEffect: self.nedm];

    if (self.sceneModel.theID != nil && ![self.sceneModel.theID isEqualToString: @""])
    {
        [ajManager.lsfSceneManager updateSceneWithID: self.sceneModel.theID withScene: [self.sceneModel toScene]];
    }
    else
    {
        [ajManager.lsfSceneManager createScene: [self.sceneModel toScene] andSceneName: self.sceneModel.name];
    }

    [self dismissViewControllerAnimated: YES completion: nil];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString: @"ScenePresets"])
    {
        LSFScenesPresetsTableViewController *sptvc = [segue destinationViewController];
        LSFConstants *constants = [LSFConstants getConstants];

        //Get Lamp State
        unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)self.brightnessSlider.value withMax: 100];
        unsigned int scaledHue = [constants scaleLampStateValue: (uint32_t)self.hueSlider.value withMax: 360];
        unsigned int scaledSaturation = [constants scaleLampStateValue: (uint32_t)self.saturationSlider.value withMax: 100];
        unsigned int scaledColorTemp = [constants scaleColorTemp: (uint32_t)self.colorTempSlider.value];

        LSFLampState* scaledLampState = [[LSFLampState alloc] initWithOnOff: self.nedm.state.onOff brightness:scaledBrightness hue:scaledHue saturation:scaledSaturation colorTemp:scaledColorTemp];

        sptvc.lampState = scaledLampState;
        sptvc.effectSender = self;
    }
}

-(void)presetButtonSetup: (LSFLampState*)state
{
    LSFPresetModelContainer *container = [LSFPresetModelContainer getPresetModelContainer];
    NSArray *presets = [container.presetContainer allValues];

    BOOL presetMatched = NO;
    for (LSFPresetModel *data in presets)
    {
        BOOL matchesPreset = [self checkIfLampState: state matchesPreset: data];

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
}

-(BOOL)checkIfLampState: (LSFLampState *) state matchesPreset: (LSFPresetModel *)data
{
    BOOL returnValue = NO;

    if ((state.onOff == data.state.onOff) && (state.brightness == data.state.brightness) && (state.hue == data.state.hue) && (state.saturation == data.state.saturation) && (state.colorTemp == data.state.colorTemp))
    {
        returnValue = YES;
    }

    return returnValue;
}

-(void)checkSaturationValue:(LSFLampState *)state
{
    LSFConstants *constants = [LSFConstants getConstants];

    if (state.saturation == 0)
    {
        self.hueSlider.enabled = NO;
        self.hueLabel.text = @"N/A";
        self.hueSliderButton.enabled = YES;
    }
    else
    {
        self.hueSlider.enabled = YES;
        self.hueSliderButton.enabled = NO;
        unsigned int hue = [constants unscaleLampStateValue: state.hue withMax: 360];
        self.hueLabel.text = [NSString stringWithFormat: @"%i°", hue];
    }

    if (state.saturation == 100)
    {
        self.colorTempSlider.enabled = NO;
        self.colorTempLabel.text = @"N/A";
        self.colorTempSliderButton.enabled = YES;
    }
    else
    {
        self.colorTempSlider.enabled = YES;
        self.colorTempSliderButton.enabled = NO;
        unsigned int colorTemp = [constants unscaleColorTemp: state.colorTemp];
        self.colorTempLabel.text = [NSString stringWithFormat: @"%iK", colorTemp];
    }
}
@end
