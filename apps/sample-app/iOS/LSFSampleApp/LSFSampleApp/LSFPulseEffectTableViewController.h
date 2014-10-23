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

#import "LSFEffectTableViewController.h"
#import "LSFSceneDataModel.h"
#import "LSFPulseEffectDataModel.h"

@interface LSFPulseEffectTableViewController : LSFEffectTableViewController

@property (nonatomic, weak) IBOutlet UISwitch *startPropertiesSwitch;
@property (nonatomic, weak) IBOutlet UIButton *endPresetButton;
@property (nonatomic, weak) IBOutlet UISlider *endBrightnessSlider;
@property (nonatomic, weak) IBOutlet UIButton *endBrightnessButton;
@property (nonatomic, weak) IBOutlet UILabel *endBrightnessLabel;
@property (nonatomic, weak) IBOutlet UISlider *endHueSlider;
@property (nonatomic, weak) IBOutlet UIButton *endHueButton;
@property (nonatomic, weak) IBOutlet UILabel *endHueLabel;
@property (nonatomic, weak) IBOutlet UISlider *endSaturationSlider;
@property (nonatomic, weak) IBOutlet UIButton *endSaturationButton;
@property (nonatomic, weak) IBOutlet UILabel *endSaturationLabel;
@property (nonatomic, weak) IBOutlet UISlider *endColorTempSlider;
@property (nonatomic, weak) IBOutlet UIButton *endColorTempButton;
@property (nonatomic, weak) IBOutlet UILabel *endColorTempLabel;
@property (nonatomic, weak) IBOutlet UILabel *durationLabel;
@property (nonatomic, weak) IBOutlet UILabel *periodLabel;
@property (nonatomic, weak) IBOutlet UILabel *numPulsesLabel;
@property (weak, nonatomic) IBOutlet UIImageView *endColorIndicatorImage;
@property (nonatomic, strong) LSFSceneDataModel *sceneModel;
@property (nonatomic, strong) LSFPulseEffectDataModel *pedm;
@property (nonatomic) BOOL shouldUpdateSceneAndDismiss;

-(IBAction)endBrightnessSliderValueChanged: (UISlider *)sender;
-(IBAction)endBrightnessSliderTouchUpInside: (UISlider *)sender;
-(IBAction)endBrightnessSliderTouchUpOutside: (UISlider *)sender;
-(IBAction)endHueSliderValueChanged: (UISlider *)sender;
-(IBAction)endHueSliderTouchUpInside: (UISlider *)sender;
-(IBAction)endHueSliderTouchUpOutside: (UISlider *)sender;
-(IBAction)endSaturationSliderValueChanged: (UISlider *)sender;
-(IBAction)endSaturationSliderTouchUpInside: (UISlider *)sender;
-(IBAction)endSaturationSliderTouchUpOutside: (UISlider *)sender;
-(IBAction)endColorTempSliderValueChanged: (UISlider *)sender;
-(IBAction)endColorTempSliderTouchUpInside: (UISlider *)sender;
-(IBAction)endColorTempSliderTouchUpOutside: (UISlider *)sender;
-(IBAction)doneButtonPressed: (id)sender;
-(IBAction)startPropertiesSwitchValueChanged: (UISwitch *)sender;
-(IBAction)startBrightnessSliderTouchedWhileDisabled: (UIButton *)sender;
-(IBAction)startHueSliderTouchedWhileDisabled: (UIButton *)sender;
-(IBAction)startSaturationSliderTouchedWhileDisabled: (UIButton *)sender;
-(IBAction)startColorTempSliderTouchedWhileDisabled: (UIButton *)sender;
-(IBAction)endBrightnessSliderTouchedWhileDisabled: (UIButton *)sender;
-(IBAction)endHueSliderTouchedWhileDisabled: (UIButton *)sender;
-(IBAction)endSaturationSliderTouchedWhileDisabled: (UIButton *)sender;
-(IBAction)endColorTempSliderTouchedWhileDisabled: (UIButton *)sender;


@end
