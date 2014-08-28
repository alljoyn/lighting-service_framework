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

#import "LSFSceneElementEffectPropertiesViewController.h"

@interface LSFSceneElementEffectPropertiesViewController ()

@end

@implementation LSFSceneElementEffectPropertiesViewController

@synthesize effectProperty = _effectProperty;
@synthesize tedm = _tedm;
@synthesize pedm = _pedm;
@synthesize durationTextField = _durationTextField;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];
    [self.durationTextField becomeFirstResponder];

    switch (self.effectProperty)
    {
        case TransitionDuration:
            self.durationTextField.placeholder = @"Enter transition duration in seconds";
            break;
        case PulseDuration:
            self.durationTextField.placeholder = @"Enter pulse duration in seconds";
            break;
        case PulsePeriod:
            self.durationTextField.placeholder = @"Enter pulse period in seconds";
            break;
        case PulseNumPulses:
            self.durationTextField.placeholder = @"Enter number of pulses";
            break;
    }
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * Done Button Event Handler
 */
-(IBAction)doneButtonPressed: (id)sender
{
    [self.durationTextField resignFirstResponder];

    switch (self.effectProperty)
    {
        case TransitionDuration:
            self.tedm.duration = ([self.durationTextField.text doubleValue] * 1000.0);
            break;
        case PulseDuration:
            self.pedm.duration = ([self.durationTextField.text doubleValue] * 1000.0);
            break;
        case PulsePeriod:
            self.pedm.period = ([self.durationTextField.text doubleValue] * 1000.0);
            break;
        case PulseNumPulses:
            self.pedm.numPulses = [self.durationTextField.text intValue];
            break;
    }

    [self.navigationController popViewControllerAnimated: YES];
}

@end
