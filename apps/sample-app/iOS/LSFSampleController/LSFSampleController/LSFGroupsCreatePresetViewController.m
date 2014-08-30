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

#import "LSFGroupsCreatePresetViewController.h"
#import "LSFPresetModelContainer.h"
#import "LSFPresetModel.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFConstants.h"
#import "LSFUtilityFunctions.h"

@interface LSFGroupsCreatePresetViewController ()

@property (nonatomic) BOOL doneButtonPressed;

@end

@implementation LSFGroupsCreatePresetViewController

@synthesize lampState = _lampState;
@synthesize presetNameTextField = _presetNameTextField;
@synthesize doneButtonPressed = _doneButtonPressed;

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    LSFPresetModelContainer *container = [LSFPresetModelContainer getPresetModelContainer];
    int numPresets = [container.presetContainer count];
    
    [self.presetNameTextField becomeFirstResponder];
    self.presetNameTextField.text = [NSString stringWithFormat: @"Preset %i", ++numPresets];
    self.doneButtonPressed = NO;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(BOOL)textFieldShouldReturn: (UITextField *)textField
{
    if (![LSFUtilityFunctions checkNameLength: self.presetNameTextField.text entity: @"Preset Name"])
    {
        return NO;
    }

    if (![LSFUtilityFunctions checkWhiteSpaces:self.self.presetNameTextField.text entity: @"Preset Name"])
    {
        return NO;
    }

    BOOL nameMatchFound = [self checkForDuplicateName: self.presetNameTextField.text];
    
    if (!nameMatchFound)
    {
        self.doneButtonPressed = YES;
        [textField resignFirstResponder];
        
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFConstants *constants = [LSFConstants getConstants];
            
            unsigned int scaledBrightness = [constants scaleLampStateValue: self.lampState.brightness withMax: 100];
            unsigned int scaledHue = [constants scaleLampStateValue: self.lampState.hue withMax: 360];
            unsigned int scaledSaturation = [constants scaleLampStateValue: self.lampState.saturation withMax: 100];
            unsigned int scaledColorTemp = [constants scaleColorTemp: self.lampState.colorTemp];
            
            LSFLampState *scaledState = [[LSFLampState alloc] initWithOnOff: self.lampState.onOff brightness: scaledBrightness hue: scaledHue saturation: scaledSaturation colorTemp: scaledColorTemp];
            
            LSFPresetManager *presetManager = ([LSFAllJoynManager getAllJoynManager]).lsfPresetManager;
            [presetManager createPresetWithState: scaledState andPresetName: self.presetNameTextField.text];
        });
        
        return YES;
    }
    
    return NO;
}

-(void)textFieldDidEndEditing: (UITextField *)textField
{
    if (self.doneButtonPressed)
    {
        [self.navigationController popViewControllerAnimated: YES];
    }
}

/*
 * UIAlertViewDelegate implementation
 */
-(void)alertView: (UIAlertView *)alertView clickedButtonAtIndex: (NSInteger)buttonIndex
{
    if (buttonIndex == 0)
    {
        [alertView dismissWithClickedButtonIndex: 0 animated: YES];
    }
    
    if (buttonIndex == 1)
    {
        [alertView dismissWithClickedButtonIndex: 1 animated: YES];
        
        self.doneButtonPressed = YES;
        [self.presetNameTextField resignFirstResponder];
        
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFPresetManager *presetManager = ([LSFAllJoynManager getAllJoynManager]).lsfPresetManager;
            [presetManager createPresetWithState: self.lampState andPresetName: self.presetNameTextField.text];
        });
    }
}

/*
 * Private functions
 */
-(BOOL)checkForDuplicateName: (NSString *)name
{
    LSFPresetModelContainer *container = [LSFPresetModelContainer getPresetModelContainer];
    NSDictionary *presets = container.presetContainer;
    
    for (LSFPresetModel *model in [presets allValues])
    {
        if ([name isEqualToString: model.name])
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Duplicate Name"
                                                            message: [NSString stringWithFormat: @"Warning: there is already a preset named \"%@.\" Although it's possible to use the same name for more than one preset, it's better to give each preset a unique name.\n\nKeep duplicate preset name \"%@\"?", name, name]
                                                           delegate: self
                                                  cancelButtonTitle: @"NO"
                                                  otherButtonTitles: @"YES", nil];
            [alert show];
            
            return YES;
        }
    }
    
    return NO;
}

@end
