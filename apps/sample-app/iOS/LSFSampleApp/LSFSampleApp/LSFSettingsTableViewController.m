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

#import "LSFSettingsTableViewController.h"
#import "LSFSettingsInfoViewController.h"
#import "LSFControllerModel.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"

@interface LSFSettingsTableViewController ()

@end

@implementation LSFSettingsTableViewController

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.scsmc setReloadControllerDelegate: self];
    });

    [self reloadControllerData];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.scsmc setReloadControllerDelegate: nil];
    });
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LSFReloadControllerCallbackDelegate Implementation
 */
-(void)reloadControllerData
{
    [self.controllerNameLabel setText:([LSFControllerModel getControllerModel]).name];
}

/*
 * UITableViewDelegate Implementation
 */
-(void)tableView: (UITableView *)tableView didSelectRowAtIndexPath: (NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];

    switch (indexPath.section)
    {
        case 1:
            [self performSegueWithIdentifier: @"ScenesSettingInfo" sender: cell]; //reuseIdentifier = eulaInfoCell
            break;
        case 2:
            [self performSegueWithIdentifier: @"ScenesSettingInfo" sender: cell]; //reuseIdentifier = sourceCodeInfoCell
            break;
        case 3:
            [self performSegueWithIdentifier: @"ScenesSettingInfo" sender: cell]; //reuseIdentifier = teamInfoCell
            break;
        default:
            break;
    }
}

/*
 * Segue Method Implementation
 */
-(void)prepareForSegue: (UIStoryboardSegue *)segue sender: (id)sender
{
    if ([segue.identifier isEqualToString: @"ScenesSettingInfo"])
    {
        NSString *cellIdentifier = [(UITableViewCell*)sender reuseIdentifier];
        LSFSettingsInfoViewController *ssivc = [segue destinationViewController];

        if ([cellIdentifier isEqualToString:@"eulaInfoCell"])
        {
            ssivc.title = @"EULA";
            ssivc.inputText = cellIdentifier; //TODO - forward the proper text per cellIdentifier
        }
        else if ([cellIdentifier isEqualToString:@"sourceCodeInfoCell"])
        {
            ssivc.title = @"Source Code";
            ssivc.inputText = cellIdentifier; //TODO - forward the proper text per cellIdentifier
        }
        else if ([cellIdentifier isEqualToString:@"teamInfoCell"])
        {
            ssivc.title = @"Team";
            ssivc.inputText = cellIdentifier; //TODO - forward the proper text per cellIdentifier
        }

    }
}

@end
