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

#import "LSFGroupsAddNameViewController.h"
#import "LSFGroupsAddLampsTableViewController.h"
#import "LSFGroupModelContainer.h"
#import "LSFGroupModel.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"

@interface LSFGroupsAddNameViewController ()

@property (nonatomic) BOOL doneButtonPressed;

-(BOOL)checkForDuplicateName: (NSString *)name;

@end

@implementation LSFGroupsAddNameViewController

@synthesize groupNameTextField = _groupNameTextField;
@synthesize doneButtonPressed = _doneButtonPressed;

-(void)viewDidLoad
{
    [super viewDidLoad];
    
    [self.groupNameTextField becomeFirstResponder];
    self.doneButtonPressed = NO;
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];
    
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager setReloadUIDelegate: self];
    });
    
    //Hide toolbar because it is not needed
    [self.navigationController.toolbar setHidden: YES];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];
    
    //Unhide toolbar because it is not needed
    [self.navigationController.toolbar setHidden: NO];
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LSFReloadUIDelegate Implementation
 */
-(void)reloadUI
{
    //Empty callback, do not need to reload any data in this view
}

/*
 * UITextFieldDelegate implementation
 */
-(BOOL)textFieldShouldReturn: (UITextField *)textField
{
    BOOL nameMatchFound = [self checkForDuplicateName: self.groupNameTextField.text];
    
    if (!nameMatchFound)
    {
        self.doneButtonPressed = YES;
        [textField resignFirstResponder];
        
        return YES;
    }
    
    return NO;
}

/*
 * UIAlertViewDelegate implementation
 */
-(void)alertView: (UIAlertView *)alertView clickedButtonAtIndex: (NSInteger)buttonIndex
{
    if (buttonIndex == 0)
    {
        [alertView dismissWithClickedButtonIndex: 0 animated: YES];
    }
    
    if (buttonIndex == 1)
    {
        [alertView dismissWithClickedButtonIndex: 1 animated: YES];
        
        self.doneButtonPressed = YES;
        [self.groupNameTextField resignFirstResponder];
    }
}

/*
 * Cancel Button Handler
 */
-(IBAction)cancelGroupCreate: (id)sender
{
    [self dismissViewControllerAnimated: YES completion: nil];
}

/*
 * Private functions
 */
-(BOOL)checkForDuplicateName: (NSString *)name
{
    LSFGroupModelContainer *container = [LSFGroupModelContainer getGroupModelContainer];
    NSDictionary *groups = container.groupContainer;
    
    for (LSFGroupModel *model in [groups allValues])
    {
        if ([name isEqualToString: model.name])
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Duplicate Name"
                                                            message: [NSString stringWithFormat: @"Warning: there is already a group named \"%@.\" Although it's possible to use the same name for more than one group, it's better to give each group a unique name.\n\nKeep duplicate group name \"%@\"?", name, name]
                                                           delegate: self
                                                  cancelButtonTitle: @"NO"
                                                  otherButtonTitles: @"YES", nil];
            [alert show];
            
            return YES;
        }
    }
    
    return NO;
}

/*
 * Segue Methods
 */
-(BOOL)shouldPerformSegueWithIdentifier: (NSString *)identifier sender: (id)sender
{
    if ([identifier isEqualToString: @"ChooseGroupLamps"])
    {
        if ([self.groupNameTextField.text isEqualToString: @""])
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"No Group Name"
                                                            message: @"You need to provide a group name in order to proceed."
                                                           delegate: self
                                                  cancelButtonTitle: @"OK"
                                                  otherButtonTitles: nil];
            [alert show];
            
            return NO;
        }
    }
    
    return YES;
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    if ([segue.identifier isEqualToString: @"ChooseGroupLamps"])
    {
        LSFGroupsAddLampsTableViewController *galtvc = [segue destinationViewController];
        galtvc.groupName = [NSString stringWithFormat: @"%@", self.groupNameTextField.text];
    }
}

@end
