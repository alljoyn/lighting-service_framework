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

#import "LSFLightsCell.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFConstants.h"
#import "LSFLampModelContainer.h"

@interface LSFLightsCell()

-(void)sliderTapped: (UIGestureRecognizer *)gr;

@end

@implementation LSFLightsCell

@synthesize lampID = _lampID;
@synthesize powerImage = _powerImage;
@synthesize nameLabel = _nameLabel;
@synthesize brightnessSlider = _brightnessSlider;
@synthesize brightnessSliderButton = _brightnessSliderButton;

-(IBAction)powerImagePressed: (UIButton *)sender
{
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = container.lampContainer;
    LSFLampModel *model = [lamps valueForKey: self.lampID];
    
    if (model != nil && model.state.onOff)
    {
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
            [lampManager transitionLampID: self.lampID onOffField: NO];
        });
    }
    else
    {
        LSFConstants *constants = [LSFConstants getConstants];
        
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
            [lampManager transitionLampID: self.lampID onOffField: YES];
            
            if (model.state.brightness == 0)
            {
                unsigned int scaledBrightness = [constants scaleLampStateValue: 25 withMax: 100];
                [lampManager transitionLampID: self.lampID brightnessField: scaledBrightness];
            }
        });
    }
}

-(IBAction)brightnessSliderChanged: (UISlider *)sender
{
    LSFConstants *constants = [LSFConstants getConstants];
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = container.lampContainer;
    LSFLampModel *model = [lamps valueForKey: self.lampID];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)sender.value withMax: 100];
        if ((model != nil) && (model.state.onOff == NO) && (scaledBrightness > 0))
        {
            [lampManager transitionLampID: self.lampID onOffField: YES];
        }
        [lampManager transitionLampID: self.lampID brightnessField: scaledBrightness];
    });
}

-(IBAction)brightnessSliderTouchedWhileDisabled: (UIButton *)sender
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                    message: @"This Lamp is not able to change its brightness."
                                                   delegate: nil
                                          cancelButtonTitle: @"OK"
                                          otherButtonTitles: nil];
    [alert show];
}

/*
 * Private functions
 */
-(void)sliderTapped: (UIGestureRecognizer *)gr
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
        LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
        NSMutableDictionary *lamps = container.lampContainer;
        LSFLampModel *model = [lamps valueForKey: self.lampID];

        LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
        unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)value withMax: 100];
        if ((model != nil) && (model.state.onOff == NO) && (scaledBrightness > 0))
        {
            [lampManager transitionLampID: self.lampID onOffField: YES];
        }
        [lampManager transitionLampID: self.lampID brightnessField: scaledBrightness];
    });
}

@end