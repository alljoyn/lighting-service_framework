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

@end
