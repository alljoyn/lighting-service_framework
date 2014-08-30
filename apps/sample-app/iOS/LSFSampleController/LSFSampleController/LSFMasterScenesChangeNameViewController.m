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

#import "LSFMasterScenesChangeNameViewController.h"
#import "LSFMasterSceneModelContainer.h"
#import "LSFMasterSceneDataModel.h"
#import "LSFDispatchQueue.h"
#import "LSFAllJoynManager.h"
#import "LSFUtilityFunctions.h"

@interface LSFMasterScenesChangeNameViewController ()

@property (nonatomic) BOOL doneButtonPressed;
@property (nonatomic, strong) LSFMasterSceneDataModel *masterSceneModel;

-(BOOL)checkForDuplicateName: (NSString *)name;

@end

@implementation LSFMasterScenesChangeNameViewController

@synthesize masterSceneID = _masterSceneID;
@synthesize masterSceneNameTextField = _masterSceneNameTextField;
@synthesize doneButtonPressed = _doneButtonPressed;
@synthesize masterSceneModel = _masterSceneModel;

- (void)viewDidLoad
{
    [super viewDidLoad];

    LSFMasterSceneModelContainer *container = [LSFMasterSceneModelContainer getMasterSceneModelContainer];
    NSMutableDictionary *masterScenes = container.masterScenesContainer;
    self.masterSceneModel = [masterScenes objectForKey: self.masterSceneID];

    [self.masterSceneNameTextField becomeFirstResponder];
    self.masterSceneNameTextField.text = self.masterSceneModel.name;
    self.doneButtonPressed = NO;
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
    if ([self.masterSceneNameTextField.text isEqualToString: @""])
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"No Master Scene Name"
                                                        message: @"You need to provide a master scene name in order to proceed."
                                                       delegate: nil
                                              cancelButtonTitle: @"OK"
                                              otherButtonTitles: nil];
        [alert show];

        return NO;
    }

    if (![LSFUtilityFunctions checkNameLength: self.masterSceneNameTextField.text entity: @"Master Scene Name"])
    {
        return NO;
    }

    if (![LSFUtilityFunctions checkWhiteSpaces: self.masterSceneNameTextField.text entity:@"Master Scene Name"])
    {
        return NO;
    }

    BOOL nameMatchFound = [self checkForDuplicateName: self.masterSceneNameTextField.text];

    if (!nameMatchFound)
    {
        self.doneButtonPressed = YES;
        [textField resignFirstResponder];

        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFMasterSceneManager *masterSceneManager = ([LSFAllJoynManager getAllJoynManager]).lsfMasterSceneManager;
            [masterSceneManager setMasterSceneNameWithID: self.masterSceneID andMasterSceneName: self.masterSceneNameTextField.text];
        });

        return YES;
    }

    return NO;
}

-(void)textFieldDidEndEditing: (UITextField *)textField
{
    if (self.doneButtonPressed)
    {
        [self.navigationController popViewControllerAnimated: YES];
    }
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
        [self.masterSceneNameTextField resignFirstResponder];

        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFMasterSceneManager *masterSceneManager = ([LSFAllJoynManager getAllJoynManager]).lsfMasterSceneManager;
            [masterSceneManager setMasterSceneNameWithID: self.masterSceneID andMasterSceneName: self.masterSceneNameTextField.text];
        });
    }
}

/*
 * Private functions
 */
-(BOOL)checkForDuplicateName: (NSString *)name
{
    LSFMasterSceneModelContainer *container = [LSFMasterSceneModelContainer getMasterSceneModelContainer];
    NSDictionary *masterScenes = container.masterScenesContainer;

    for (LSFMasterSceneDataModel *model in [masterScenes allValues])
    {
        if ([name isEqualToString: model.name])
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Duplicate Name"
                                                            message: [NSString stringWithFormat: @"Warning: there is already a master scene named \"%@.\" Although it's possible to use the same name for more than one master scene, it's better to give each master scene a unique name.\n\nKeep duplicate master scene name \"%@\"?", name, name]
                                                           delegate: self
                                                  cancelButtonTitle: @"NO"
                                                  otherButtonTitles: @"YES", nil];
            [alert show];

            return YES;
        }
    }

    return NO;
}

@end
