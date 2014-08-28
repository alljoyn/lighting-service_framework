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

#import "LSFScenesTableViewController.h"
#import "LSFScenesEnterNameViewController.h"
#import "LSFLampModelContainer.h"
#import "LSFGroupModelContainer.h"
#import "LSFSceneModelContainer.h"
#import "LSFLampModel.h"
#import "LSFGroupModel.h"
#import "LSFSceneDataModel.h"
#import "LSFSceneCell.h"
#import "LSFNoEffectDataModel.h"
#import "LSFTransitionEffectDataModel.h"
#import "LSFPulseEffectDataModel.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"

@interface LSFScenesTableViewController ()

@property (nonatomic, strong) NSArray *sceneData;
@property (nonatomic, strong) UIBarButtonItem *myEditButton;
@property (nonatomic, strong) UIBarButtonItem *settingsButton;
@property (nonatomic, strong) UIBarButtonItem *addButton;

-(void)plusButtonPressed;
-(void)settingsButtonPressed;
-(NSString *)buildDetailsString: (LSFSceneDataModel *)sceneModel;
-(void)appendLampNames: (NSArray *)lampIDs toString: (NSMutableString *)detailsString;
-(void)appendGroupNames: (NSArray *)groupIDs toString: (NSMutableString *)detailsString;

@end

@implementation LSFScenesTableViewController

@synthesize myEditButton = _myEditButton;
@synthesize settingsButton = _settingsButton;
@synthesize addButton = _addButton;

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self.navigationItem setHidesBackButton:YES];
    
    self.addButton = [[UIBarButtonItem alloc]
                      initWithBarButtonSystemItem:UIBarButtonSystemItemAdd
                      target: self
                      action: @selector(plusButtonPressed)];
    
    self.settingsButton = [[UIBarButtonItem alloc]
                           initWithImage: [UIImage imageNamed: @"nav_settings_regular_icon.png"]
                           style:UIBarButtonItemStylePlain
                           target: self
                           action: @selector(settingsButtonPressed)];
    
    NSArray *actionButtonItems = @[self.settingsButton, self.addButton];
    self.navigationItem.rightBarButtonItems = actionButtonItems;
    self.navigationItem.leftBarButtonItem = self.editButtonItem;

    NSMutableArray *vc = [NSMutableArray arrayWithArray: self.navigationController.viewControllers];
    [vc removeObjectAtIndex: 0];
    [self.navigationController setViewControllers: vc];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];
}

-(void)viewDidAppear: (BOOL)animated
{
    [super viewDidAppear: animated];
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager setReloadUIDelegate: self];
    });
    
    [self reloadUI];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LSFReloadUIDelegate Implementation
 */
-(void)reloadUI
{
    LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
    self.sceneData = [container.sceneContainer allValues];

    [self.tableView reloadData];
}

/*
 * UITableViewDataSource Implementation
 */
-(UITableViewCell *)tableView: (UITableView *)tableView cellForRowAtIndexPath: (NSIndexPath *)indexPath
{
    LSFSceneDataModel *data = [self.sceneData objectAtIndex: [indexPath row]];

    LSFSceneCell *cell = [tableView dequeueReusableCellWithIdentifier: @"SceneCell" forIndexPath: indexPath];
    cell.sceneType = Basic;
    cell.sceneModel = data;
    cell.nameLabel.text = data.name;
    cell.detailsLabel.text = [self buildDetailsString: data];
    
    return cell;
}

-(NSInteger)tableView: (UITableView *)tableView numberOfRowsInSection: (NSInteger)section
{
    LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
    return [container.sceneContainer count];
}

-(CGFloat)tableView: (UITableView *)tableView heightForRowAtIndexPath: (NSIndexPath *)indexPath
{
    return 70;
}

-(BOOL)tableView: (UITableView *)tableView canEditRowAtIndexPath: (NSIndexPath *)indexPath
{
    return YES;
}

/*
 * Segue Method
 */
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString: @"CreateScene"])
    {
        UINavigationController *nc = (UINavigationController *)[segue destinationViewController];
        LSFScenesEnterNameViewController *senvc = (LSFScenesEnterNameViewController *)nc.topViewController;
        senvc.sceneModel = [[LSFSceneDataModel alloc] init];
    }
}

/*
 * UIActionSheetDelegate implementation
 */
-(void)actionSheet: (UIActionSheet *)actionSheet clickedButtonAtIndex: (NSInteger)buttonIndex
{
    switch (buttonIndex)
    {
        case 0:
            [self performSegueWithIdentifier: @"CreateScene" sender: self];
            break;
        case 1:
            NSLog(@"Add Master Scene button pressed");
            break;
        case 2:
            NSLog(@"Cancel button pressed");
            break;
     }
}

/*
 * Private functions
 */
-(void)plusButtonPressed
{
    UIActionSheet *actionSheet = [[UIActionSheet alloc] initWithTitle: nil delegate: self cancelButtonTitle: @"Cancel" destructiveButtonTitle: nil otherButtonTitles: @"Add Scene", @"Add Master Scene", nil];
    [actionSheet showInView: self.view];
};

-(void)settingsButtonPressed
{
    [self performSegueWithIdentifier: @"ScenesSettings" sender: self];
}

-(NSString *)buildDetailsString: (LSFSceneDataModel *)sceneModel
{
    NSMutableString *detailsString = [[NSMutableString alloc] init];

    for (LSFNoEffectDataModel *nedm in sceneModel.noEffects)
    {
        [self appendLampNames: nedm.members.lamps toString: detailsString];
        [self appendGroupNames: nedm.members.lampGroups toString: detailsString];
    }

    for (LSFTransitionEffectDataModel *tedm in sceneModel.transitionEffects)
    {
        [self appendLampNames: tedm.members.lamps toString: detailsString];
        [self appendGroupNames: tedm.members.lampGroups toString: detailsString];
    }

    for (LSFPulseEffectDataModel *pedm in sceneModel.pulseEffects)
    {
        [self appendLampNames: pedm.members.lamps toString: detailsString];
        [self appendGroupNames: pedm.members.lampGroups toString: detailsString];
    }

    return detailsString;
}

-(void)appendLampNames: (NSArray *)lampIDs toString: (NSMutableString *)detailsString
{
    LSFLampModelContainer *lampContainer = [LSFLampModelContainer getLampModelContainer];

    for (NSString *lampID in lampIDs)
    {
        LSFLampModel *lampModel = [lampContainer.lampContainer valueForKey: lampID];

        if (lampModel != nil)
        {
            [detailsString appendString: lampModel.name];
        }
    }
}

-(void)appendGroupNames: (NSArray *)groupIDs toString: (NSMutableString *)detailsString
{
    LSFGroupModelContainer *groupContainer = [LSFGroupModelContainer getGroupModelContainer];

    for (NSString *groupID in groupIDs)
    {
        LSFGroupModel *groupModel = [groupContainer.groupContainer valueForKey: groupID];

        if (groupModel != nil)
        {
            [detailsString appendString: groupModel.name];
        }
    }
}

@end
