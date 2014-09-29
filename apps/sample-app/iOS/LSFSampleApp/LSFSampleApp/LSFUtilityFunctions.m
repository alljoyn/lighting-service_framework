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

#import "LSFUtilityFunctions.h"
#import "LampValues.h"
#import "LSFLampModelContainer.h"
#import "LSFGroupModelContainer.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"

@implementation LSFUtilityFunctions


+(BOOL)checkNameLength: (NSString *)name entity: (NSString *)entity
{
    if ([name length] > LSF_MAX_NAME_LENGTH)
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: [NSString stringWithFormat: @"%@ Error", entity]
                                                        message: [NSString stringWithFormat: @"Name has exceeded %d characters limit", LSF_MAX_NAME_LENGTH ]
                                                       delegate: self
                                              cancelButtonTitle: nil
                                              otherButtonTitles: @"OK", nil];
        [alert show];
        return NO;
    }
    return YES;
}

+(BOOL)checkWhiteSpaces: (NSString *)name entity: (NSString *)entity
{
    //only whitespaces
    if (([name stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]].length == 0) || [name hasPrefix:@" "])
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: [NSString stringWithFormat: @"%@ Error", entity]
                                                        message: @"Name can't start or be only spaces"
                                                       delegate: self
                                              cancelButtonTitle: nil
                                              otherButtonTitles: @"OK", nil];
        [alert show];
        return NO;
    }
    return YES;
}

+(NSString *)buildSectionTitleString: (LSFSceneElementDataModel *)sceneElement
{
    BOOL firstElementFound = NO;
    NSMutableString *titleString = [[NSMutableString alloc] initWithString: @""];

    LSFGroupModelContainer *groupContainer = [LSFGroupModelContainer getGroupModelContainer];
    NSMutableDictionary *groups = groupContainer.groupContainer;

    for (int i = 0; !firstElementFound && i < sceneElement.members.lampGroups.count; i++)
    {
        NSString *lampGroupID = [sceneElement.members.lampGroups objectAtIndex: i];
        LSFGroupModel *model = [groups valueForKey: lampGroupID];

        if (model != nil)
        {
            [titleString appendFormat: @"\"%@\"", model.name];
            firstElementFound = YES;
        }
    }

    LSFLampModelContainer *lampsContainer = [LSFLampModelContainer getLampModelContainer];
    NSMutableDictionary *lamps = lampsContainer.lampContainer;

    for (int i = 0; !firstElementFound && i < sceneElement.members.lamps.count; i++)
    {
        NSString *lampID = [sceneElement.members.lamps objectAtIndex: i];
        LSFLampModel *model = [lamps valueForKey: lampID];

        if (model != nil)
        {
            [titleString appendFormat: @"\"%@\"", model.name];
            firstElementFound = YES;
        }
    }

    unsigned int remainingSceneMembers = (sceneElement.members.lamps.count + sceneElement.members.lampGroups.count - 1);

    if (remainingSceneMembers > 0)
    {
        [titleString appendFormat: @" (and %u more)", remainingSceneMembers];
    }

    return [NSString stringWithString: titleString];
}


+(void)colorIndicatorSetup: (UIImageView*)colorIndicatorImage dataState: (LSFLampState*) dataState
{
    CAShapeLayer *circleShape = [CAShapeLayer layer];
    [circleShape setPosition:CGPointMake([colorIndicatorImage bounds].size.width/2.0f, [colorIndicatorImage bounds].size.height/2.0f)];
    [circleShape setBounds:CGRectMake(0.0f, 0.0f, [colorIndicatorImage bounds].size.width, [colorIndicatorImage bounds].size.height)];
    [circleShape setPath:[[UIBezierPath bezierPathWithRoundedRect:CGRectMake(0.0f, 0.0f, 10.0f, 10.0f) cornerRadius:10.0f]  CGPath]];
    [circleShape setLineWidth:0.3f];
    [circleShape setStrokeColor:[[UIColor colorWithRed:0.49 green:0.49 blue:0.49 alpha:1] CGColor]]; // #7d7d7d
    UIColor* fillColor = [self calcFillColor: dataState];
    [circleShape setFillColor: [fillColor CGColor]];
    [[colorIndicatorImage layer] addSublayer:circleShape];
}

+(UIColor*)calcFillColor: (LSFLampState*) dataState
{
    //Create original color using hue, saturation, brightness
    UIColor* colorToFill = [UIColor colorWithHue: ((CGFloat)(dataState.hue) / 360) saturation: ((CGFloat)(dataState.saturation) / 100) brightness: ((CGFloat)(dataState.brightness) / 100) alpha: 1.000];

    //Convert the original color to RGB format
    CGFloat colorToFillRed, colorToFillGreen, colorToFillBlue, colorToFillAlpha;
    [colorToFill getRed: &colorToFillRed green: &colorToFillGreen blue: &colorToFillBlue alpha: &colorToFillAlpha];

    //Scale original color RGB values
    colorToFillRed *= 255;
    colorToFillGreen *= 255;
    colorToFillBlue *= 255;

    //Convert colorTemp to RGB format
    UIColor* colorTempRGB = [self convertColorTempToRGB: dataState.colorTemp];
    CGFloat colorTempRed, colorTempGreen, colorTempBlue, colorTempAlpha;
    [colorTempRGB getRed:&colorTempRed green:&colorTempGreen blue:&colorTempBlue alpha:&colorTempAlpha];

    //Scale colorTemp RGB values
    colorTempRed *= 255;
    colorTempGreen *= 255;
    colorTempBlue *= 255;

    //Create a colorTemp factor
    int sum = (int) (colorTempRed + colorTempGreen + colorTempBlue);

    //Compute factors for RGB channels
    double ctR = (colorTempRed / sum * 3);
    double ctG = (colorTempGreen / sum * 3);
    double ctB = (colorTempBlue / sum * 3);

    //Multiply each channel in its factor
    int newR = (int)round(ctR * colorToFillRed);
    int newG = (int)round(ctG * colorToFillGreen);
    int newB = (int)round(ctB * colorToFillBlue);

    //Fix values if needed
    if(newR > 255) newR = 255;
    if(newG > 255) newG = 255;
    if(newB > 255) newB = 255;

    return [UIColor colorWithRed:(newR/255.0f) green:(newG/255.0f) blue:(newB/255.0f) alpha:1];
}

+(UIColor*)convertColorTempToRGB:(unsigned int) colorTemp
{
    double r = 0;
    double g = 0;
    double b = 0;

    if (colorTemp < 1000)
    colorTemp = 1000;

    if (colorTemp > 40000)
    colorTemp = 40000;

    double tmpKelvin = colorTemp/100;

    //Calculate red
    if(tmpKelvin <= 66)
    {
        r = 255;
    }
    else
    {
        //Note: the R-squared value for this approximation is .988
        double tmpCalc = tmpKelvin - 60;
        tmpCalc = 329.698727446 * (pow(tmpCalc, -0.1332047592));
        r = tmpCalc;
        if(r < 0)
        r = 0;
        if(r > 255)
        r = 255;
    }

    //Calculate green
    if(tmpKelvin <= 66)
    {
        //Note: the R-squared value for this approximation is .996
        double tmpCalc = tmpKelvin;
        tmpCalc = 99.4708025861 * log(tmpCalc) - 161.1195681661;
        g = tmpCalc;
        if(g < 0)
        g = 0;
        if(g > 255)
        g = 255;
    }
    else
    {
        //Note: the R-squared value for this approximation is .987
        double tmpCalc = tmpKelvin - 60;
        tmpCalc = 288.1221695283 * (pow(tmpCalc,-0.0755148492));
        g = tmpCalc;
        if(g < 0)
        g = 0;
        if(g > 255)
        g = 255;
    }

    //Calculate blue
    if(tmpKelvin >= 66)
    {
        b = 255;
    }
    else if(tmpKelvin <= 19)
    {
        b = 0;
    }
    else
    {
        //Note: the R-squared value for this approximation is .998
        double tmpCalc = tmpKelvin - 10;
        tmpCalc = 138.5177312231 * log(tmpCalc) - 305.0447927307;
        b = tmpCalc;
        if(b < 0)
        b = 0;
        if(b > 255)
        b = 255;
    }

    UIColor *colorTempRGB = [UIColor colorWithRed:(r/255.0f) green:(g/255.0f) blue:(b/255.0f) alpha:1];
    return colorTempRGB;
}

@end
