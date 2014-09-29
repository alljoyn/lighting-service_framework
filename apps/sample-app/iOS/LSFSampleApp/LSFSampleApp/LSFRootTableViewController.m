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

#import "LSFRootTableViewController.h"
#import "LSFEnums.h"
#import "LSFLightsTableViewController.h"

@interface LSFRootTableViewController ()

-(void)controllerNotificationReceived: (NSNotification *)notification;

@end

@implementation LSFRootTableViewController

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(controllerNotificationReceived:) name: @"ControllerNotification" object: nil];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    [[NSNotificationCenter defaultCenter] removeObserver: self];
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

-(void)controllerNotificationReceived: (NSNotification *)notification
{
    //NSLog(@"LSFRootTableViewController - notficicationReceived() executing");
    //NSLog(@"Notification Name = %@", notification.name);

    NSDictionary *userInfo = notification.userInfo;
    NSNumber *controllerStatus = [userInfo valueForKey: @"status"];

    if (controllerStatus.intValue == Connected)
    {
        NSLog(@"Controller is now connected");
        [self.tableView reloadData];
    }
    else if (controllerStatus.intValue == Disconnected)
    {
        NSLog(@"Controller is now disconnected");
        [self.tableView reloadData];
    }
}

@end
