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

#import "LSFLoadingViewController.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFConstants.h"

@interface LSFLoadingViewController ()

@property (nonatomic, strong) NSString *ssid;

@end

@implementation LSFLoadingViewController

@synthesize activityIndicator = _activityIndicator;
@synthesize networkInfo = _networkInfo;
@synthesize activityLabel = _activityLabel;
@synthesize ssid = _ssid;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];
    // Do any additional setup after loading the view.
    
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
    LSFConstants *constants = [LSFConstants getConstants];
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        [ajManager setControllerServiceConnectedDelegate: self];
    });
    
    self.ssid = [constants currentWifiSSID];
    
    if (!ajManager.isConnectedToController && !ajManager.foundLamps)
    {
        self.networkInfo.text = [NSString stringWithFormat: @"No controller service is available\non the network %@", self.ssid];
        [self.activityIndicator startAnimating];
    }
    else if (ajManager.isConnectedToController && !ajManager.foundLamps)
    {
        self.networkInfo.text = [NSString stringWithFormat: @"No lamps have been detected on\non the network %@", self.ssid];
        self.activityLabel.text = @"Searching for lamps...";
        
        if (![self.activityIndicator isAnimating])
        {
            [self.activityIndicator startAnimating];
        }
    }
    else if (ajManager.isConnectedToController && ajManager.foundLamps)
    {
        [ajManager setControllerServiceConnectedDelegate: nil];
        [self.activityIndicator stopAnimating];
        
        [self performSegueToNextViewController];
    }
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

-(void)performSegueToNextViewController
{
    //TODO - perform desired
}

/*
 * LSFControllerServiceConnectedDelegate implementation
 */
-(void)connectedToControllerService
{
    self.networkInfo.text = [NSString stringWithFormat: @"No lamps have been detected on\non the network %@", self.ssid];
    self.activityLabel.text = @"Searching for lamps...";
}

-(void)lampsFound
{
    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
    [ajManager setControllerServiceConnectedDelegate: nil];
    
    [self.activityIndicator stopAnimating];
    [self performSegueWithIdentifier: @"ShowLampsTable" sender: self];
}

@end
