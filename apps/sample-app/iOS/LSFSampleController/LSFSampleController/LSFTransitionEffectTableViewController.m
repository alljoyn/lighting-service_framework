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

#import "LSFTransitionEffectTableViewController.h"
#import "LSFSceneElementEffectPropertiesViewController.h"
#import "LSFTransitionEffectDataModel.h"
#import "LSFConstants.h"
#import "LSFAllJoynManager.h"
#import "LSFUtilityFunctions.h"

@interface LSFTransitionEffectTableViewController ()

@property (nonatomic, strong) LSFTransitionEffectDataModel *tedm;

@end

@implementation LSFTransitionEffectTableViewController

@synthesize durationLabel = _durationLabel;
@synthesize tedm = _tedm;

-(void)viewDidLoad
{
    [super viewDidLoad];

    if (self.sceneElement.type != Unknown)
    {
        self.tedm = (LSFTransitionEffectDataModel *)self.sceneElement;
    }
    else
    {
        self.tedm = [[LSFTransitionEffectDataModel alloc] init];
    }

    LSFConstants *constants = [LSFConstants getConstants];

    if (self.sceneElement != nil)
    {
        unsigned int brightness = [constants unscaleLampStateValue: self.tedm.state.brightness withMax: 100];
        self.brightnessSlider.value = brightness;
        self.brightnessLabel.text = [NSString stringWithFormat: @"%u%%", brightness];

        unsigned int hue = [constants unscaleLampStateValue: self.tedm.state.hue withMax: 360];
        self.hueSlider.value = hue;
        self.hueLabel.text = [NSString stringWithFormat: @"%u°", hue];

        unsigned int saturation = [constants unscaleLampStateValue: self.tedm.state.saturation withMax: 100];
        self.saturationSlider.value = saturation;
        self.saturationLabel.text = [NSString stringWithFormat: @"%u%%", saturation];

        unsigned int colorTemp = [constants unscaleColorTemp: self.tedm.state.colorTemp];
        self.colorTempSlider.value = colorTemp;
        self.colorTempLabel.text = [NSString stringWithFormat: @"%iK", colorTemp];
        
    }
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    self.durationLabel.text = [NSString stringWithFormat: @"%.8g seconds", ((double)self.tedm.duration / 1000.0)];
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
        return [NSString stringWithFormat: @"Set the properties that %@ will transition to", [LSFUtilityFunctions buildSectionTitleString: self.sceneElement]];
    }

    return @"";
}

/*
 * Button event handler
 */
-(IBAction)doneButtonPressed: (id)sender
{
    LSFConstants *constants = [LSFConstants getConstants];
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];

    //Get Scene Members
    self.tedm.members = self.sceneElement.members;

    //Get Capability
    self.tedm.capability = self.sceneElement.capability;

    //Get Lamp State
    unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)self.brightnessSlider.value withMax: 100];
    unsigned int scaledHue = [constants scaleLampStateValue: (uint32_t)self.hueSlider.value withMax: 360];
    unsigned int scaledSaturation = [constants scaleLampStateValue: (uint32_t)self.saturationSlider.value withMax: 100];
    unsigned int scaledColorTemp = [constants scaleColorTemp: (uint32_t)self.colorTempSlider.value];

    self.tedm.state.brightness = scaledBrightness;
    self.tedm.state.hue = scaledHue;
    self.tedm.state.saturation = scaledSaturation;
    self.tedm.state.colorTemp = scaledColorTemp;

    [self.sceneModel updateTransitionEffect: self.tedm];

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

/*
 * Segue Methods
 */
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString: @"TransitionDuration"])
    {
        LSFSceneElementEffectPropertiesViewController *seepvc = [segue destinationViewController];
        seepvc.tedm = self.tedm;
        seepvc.effectProperty = TransitionDuration;
    }
}

@end
