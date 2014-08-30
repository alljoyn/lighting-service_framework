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

#import "LSFGroupsCell.h"
#import "LSFDispatchQueue.h"
#import "LSFConstants.h"
#import "LSFAllJoynManager.h"

@interface LSFGroupsCell()

@property (nonatomic) unsigned int previousBrightness;

-(void)sliderTapped: (UIGestureRecognizer *)gr;

@end

@implementation LSFGroupsCell

@synthesize groupModel = _groupModel;
@synthesize powerImage = _powerImage;
@synthesize nameLabel = _nameLabel;
@synthesize brightnessSlider = _brightnessSlider;
@synthesize brightnessSliderButton = _brightnessSliderButton;

-(IBAction)powerImagePressed: (UIButton *)sender
{
    //NSLog(@"Power Image Pressed");
    
    if (self.groupModel.state.onOff)
    {
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFLampGroupManager *groupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
            [groupManager transitionLampGroupID: self.groupModel.theID onOffField: NO];
        });
    }
    else
    {
        LSFConstants *constants = [LSFConstants getConstants];
        
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFLampGroupManager *groupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
            [groupManager transitionLampGroupID: self.groupModel.theID onOffField: YES];
            
            if (self.groupModel.state.brightness == 0)
            {
                unsigned int scaledBrightness = [constants scaleLampStateValue: 25 withMax: 100];
                [groupManager transitionLampGroupID: self.groupModel.theID brightnessField: scaledBrightness];
            }
        });
    }
}

-(IBAction)brightnessSliderChanged: (UISlider *)sender
{
    NSLog(@"Brightness Slider Value Changed. Value = %i", (uint32_t)sender.value);

    LSFConstants *constants = [LSFConstants getConstants];
    self.previousBrightness = (uint32_t)sender.value;
        
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFLampGroupManager *groupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        unsigned int scaledBrightness = [constants scaleLampStateValue: (uint32_t)sender.value withMax: 100];
        if ((self.groupModel.state.onOff == NO) && (scaledBrightness > 0))
        {
            [groupManager transitionLampGroupID: self.groupModel.theID onOffField: YES];
        }
        [groupManager transitionLampGroupID: self.groupModel.theID brightnessField: scaledBrightness];
    });
}

-(IBAction)brightnessSliderTouchedWhileDisabled: (UIButton *)sender
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Error"
                                                    message: @"This group is not able to change its brightness since no lamps in the group are dimmable."
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
    //NSLog(@"Group Brightness Slider Tapped");
    
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
    
    unsigned int newBrightness = (uint32_t)value;
    self.brightnessSlider.value = newBrightness;
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFConstants *constants = [LSFConstants getConstants];
        LSFLampGroupManager *groupManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampGroupManager;
        unsigned int scaledBrightness = [constants scaleLampStateValue: newBrightness withMax: 100];
        if ((self.groupModel.state.onOff == NO) && (scaledBrightness > 0))
        {
            [groupManager transitionLampGroupID: self.groupModel.theID onOffField: YES];
        }
        [groupManager transitionLampGroupID: self.groupModel.theID brightnessField: scaledBrightness];
    });
}

@end
