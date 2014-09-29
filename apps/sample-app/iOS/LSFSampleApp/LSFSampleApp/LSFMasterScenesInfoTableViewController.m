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

#import "LSFMasterScenesInfoTableViewController.h"
#import "LSFMasterSceneMembersTableViewController.h"
#import "LSFMasterScenesChangeNameViewController.h"
#import "LSFMasterSceneDataModel.h"
#import "LSFMasterSceneModelContainer.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"

@interface LSFMasterScenesInfoTableViewController ()

@property (nonatomic, strong) LSFMasterSceneDataModel *masterSceneModel;

@end

@implementation LSFMasterScenesInfoTableViewController

@synthesize masterSceneID = _masterSceneID;
@synthesize masterSceneModel = _masterSceneModel;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.smsmc setReloadMasterScenesDelegate: self];
    });

    [self reloadMasterSceneWithID: self.masterSceneID];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.smsmc setReloadMasterScenesDelegate: nil];
    });
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LSFReloadMasterScenesCallbackDelegate Implementation
 */
-(void)reloadMasterSceneWithID: (NSString *)masterSceneID
{
    if ([self.masterSceneID isEqualToString: masterSceneID])
    {
        LSFMasterSceneModelContainer *container = [LSFMasterSceneModelContainer getMasterSceneModelContainer];
        NSMutableDictionary *masterSceneDictionary = container.masterScenesContainer;
        self.masterSceneModel = [masterSceneDictionary valueForKey: self.masterSceneID];

        [self.tableView reloadData];
    }
}

-(void)deleteMasterScenesWithIDs: (NSArray *)masterSceneIDs andNames: (NSArray *)masterSceneNames
{
    if ([masterSceneIDs containsObject: self.masterSceneID])
    {
        int index = [masterSceneIDs indexOfObject: self.masterSceneID];

        [self.navigationController popToRootViewControllerAnimated: YES];

        dispatch_async(dispatch_get_main_queue(), ^{
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Master Scene Not Found"
                                                            message: [NSString stringWithFormat: @"The master scene \"%@\" no longer exists.", [masterSceneNames objectAtIndex: index]]
                                                           delegate: nil
                                                  cancelButtonTitle: @"OK"
                                                  otherButtonTitles: nil];
            [alert show];
        });
    }
}

/*
 * UITableViewDataSource Implementation
 */
-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 1;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 0)
    {
        UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle: UITableViewCellStyleValue1 reuseIdentifier: nil];
        cell.textLabel.text = @"Master Scene Name";
        cell.detailTextLabel.text = self.masterSceneModel.name;
        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        return cell;
    }
    else
    {
        UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle: UITableViewCellStyleDefault reuseIdentifier: nil];
        cell.textLabel.text = @"Scenes in the master scene";
        cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        return cell;
    }
}

-(void)tableView: (UITableView *)tableView didSelectRowAtIndexPath: (NSIndexPath *)indexPath
{
    if (indexPath.section == 0)
    {
        [self performSegueWithIdentifier: @"ChangeMasterSceneName" sender: nil];
    }
    else if (indexPath.section == 1)
    {
        [self performSegueWithIdentifier: @"ModifySceneMembers" sender: nil];
    }
}

/*
 * Segue Function
 */
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString: @"ModifySceneMembers"])
    {
        UINavigationController *nc = (UINavigationController *)[segue destinationViewController];
        LSFMasterSceneMembersTableViewController *msmtvc = (LSFMasterSceneMembersTableViewController *)nc.topViewController;
        msmtvc.masterSceneModel = self.masterSceneModel;
        msmtvc.usesCancel = YES;
    }
    else if ([segue.identifier isEqualToString: @"ChangeMasterSceneName"])
    {
        LSFMasterScenesChangeNameViewController *mscnvc = [segue destinationViewController];
        mscnvc.masterSceneID = self.masterSceneID;
    }
}

@end
