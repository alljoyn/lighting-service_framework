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

#import "LSFScenesEnterNameViewController.h"
#import "LSFScenesMembersTableViewController.h"
#import "LSFSceneModelContainer.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFUtilityFunctions.h"

@interface LSFScenesEnterNameViewController ()

@end

@implementation LSFScenesEnterNameViewController

@synthesize sceneModel = _sceneModel;
@synthesize nameTextField = _nameTextField;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];
    [self.nameTextField becomeFirstResponder];

    //Hide toolbar because it is not needed
    [self.navigationController.toolbar setHidden: YES];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    //Unhide toolbar because it is not needed
    [self.navigationController.toolbar setHidden: NO];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * UITextFieldDelegate implementation
 */
-(BOOL)textFieldShouldReturn: (UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
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
        [self.nameTextField resignFirstResponder];
        [self performSegueWithIdentifier: @"ChooseSceneLamps" sender: self];
    }
}

/*
 * Cancel Button Handler
 */
-(IBAction)cancelButtonPressed: (id)sender
{
    [self dismissViewControllerAnimated: YES completion: nil];
}

-(IBAction)nextButtonPressed: (id)sender
{
    if ([self.nameTextField.text isEqualToString: @""])
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"No Scene Name"
                                                        message: @"You need to provide a scene name in order to proceed."
                                                       delegate: self
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];

        return;
    }
    else if ([self checkForDuplicateName: self.nameTextField.text])
    {
        return;
    }
    else if (![LSFUtilityFunctions checkNameLength: self.nameTextField.text entity: @"Scene Name"])
    {
        return;
    }
    else if (![LSFUtilityFunctions checkWhiteSpaces:self.nameTextField.text entity: @"Scene Name"])
    {
        return;
    }
    else
    {
        [self performSegueWithIdentifier: @"ChooseSceneLamps" sender: self];
    }
}

/*
 * Override checkForDuplicateName function
 */
-(BOOL)checkForDuplicateName: (NSString *)name
{
    LSFSceneModelContainer *container = [LSFSceneModelContainer getSceneModelContainer];
    NSDictionary *scenes = container.sceneContainer;

    for (LSFSceneDataModel *model in [scenes allValues])
    {
        if ([name isEqualToString: model.name])
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Duplicate Name"
                                                            message: [NSString stringWithFormat: @"Warning: there is already a scene named \"%@.\" Although it's possible to use the same name for more than one scene, it's better to give each scene a unique name.\n\nKeep duplicate scene name \"%@\"?", name, name]
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
-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    self.sceneModel.name = self.nameTextField.text;

    if ([segue.identifier isEqualToString: @"ChooseSceneLamps"])
    {
        LSFScenesMembersTableViewController *smtvc = [segue destinationViewController];
        smtvc.sceneModel = self.sceneModel;
        smtvc.usesCancel = NO;
    }
}

@end
