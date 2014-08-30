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
#import "LSFNoEffectDataModel.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"
#import "LSFLampModelContainer.h"
#import "LSFGroupModelContainer.h"
#import "LSFUtilityFunctions.h"

@interface LSFNoEffectTableViewController ()

@end

@implementation LSFNoEffectTableViewController

@synthesize sceneModel = _sceneModel;
@synthesize sceneElement = _sceneElement;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    LSFConstants *constants = [LSFConstants getConstants];

    if (self.sceneElement != nil)
    {
        unsigned int brightness = [constants unscaleLampStateValue: self.sceneElement.state.brightness withMax: 100];
        self.brightnessSlider.value = brightness;
        self.brightnessLabel.text = [NSString stringWithFormat: @"%u%%", brightness];

        unsigned int hue = [constants unscaleLampStateValue: self.sceneElement.state.hue withMax: 360];
        self.hueSlider.value = hue;
        self.hueLabel.text = [NSString stringWithFormat: @"%u°", hue];

        unsigned int saturation = [constants unscaleLampStateValue: self.sceneElement.state.saturation withMax: 100];
        self.saturationSlider.value = saturation;
        self.saturationLabel.text = [NSString stringWithFormat: @"%u%%", saturation];

        unsigned int colorTemp = [constants unscaleColorTemp: self.sceneElement.state.colorTemp];
        self.colorTempSlider.value = colorTemp;
        self.colorTempLabel.text = [NSString stringWithFormat: @"%iK", colorTemp];

    }
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
        return [NSString stringWithFormat: @"Set the properties that %@ will change to", [LSFUtilityFunctions buildSectionTitleString: self.sceneElement]];
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

    LSFNoEffectDataModel *nedm;

    if (self.sceneElement.type != Unknown)
    {
        nedm = (LSFNoEffectDataModel *)self.sceneElement;
    }
    else
    {
        nedm = [[LSFNoEffectDataModel alloc] init];

//        NSLog(@"OnOff = %@", nedm.state.onOff ? @"ON" : @"OFF");
//        NSLog(@"Brightness = %u", nedm.state.brightness);
//        NSLog(@"Hue = %u", nedm.state.hue);
//        NSLog(@"Saturation = %u", nedm.state.saturation);
//        NSLog(@"ColorTemp = %u", nedm.state.colorTemp);
    }

    //Get Scene Members
    nedm.members = self.sceneElement.members;

    //Get Capability
    nedm.capability = self.sceneElement.capability;

    //Get Lamp State
    unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)self.brightnessSlider.value withMax: 100];
    unsigned int scaledHue = [constants scaleLampStateValue: (uint32_t)self.hueSlider.value withMax: 360];
    unsigned int scaledSaturation = [constants scaleLampStateValue: (uint32_t)self.saturationSlider.value withMax: 100];
    unsigned int scaledColorTemp = [constants scaleColorTemp: (uint32_t)self.colorTempSlider.value];

    nedm.state.brightness = scaledBrightness;
    nedm.state.hue = scaledHue;
    nedm.state.saturation = scaledSaturation;
    nedm.state.colorTemp = scaledColorTemp;

    [self.sceneModel updateNoEffect: nedm];

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

@end
