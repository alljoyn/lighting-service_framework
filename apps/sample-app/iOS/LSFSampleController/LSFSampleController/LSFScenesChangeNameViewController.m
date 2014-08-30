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

#import "LSFScenesChangeNameViewController.h"
#import "LSFDispatchQueue.h"
#import "LSFLampGroupManager.h"
#import "LSFAllJoynManager.h"
#import "LSFSceneModelContainer.h"
#import "LSFUtilityFunctions.h"

@interface LSFScenesChangeNameViewController ()

@property (nonatomic) BOOL doneButtonPressed;

-(BOOL)checkForDuplicateName: (NSString *)name;

@end

@implementation LSFScenesChangeNameViewController

@synthesize sceneModel = _sceneModel;
@synthesize sceneNameTextField = _sceneNameTextField;
@synthesize doneButtonPressed = _doneButtonPressed;

- (void)viewDidLoad
{
    [super viewDidLoad];

    [self.sceneNameTextField becomeFirstResponder];
    self.sceneNameTextField.text = self.sceneModel.name;
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
    if (![LSFUtilityFunctions checkNameLength: self.sceneNameTextField.text entity: @"Scene Name"])
    {
        return NO;
    }

    if (![LSFUtilityFunctions checkWhiteSpaces:self.sceneNameTextField.text entity: @"Scene Name"])
    {
        return NO;
    }

    BOOL nameMatchFound = [self checkForDuplicateName: self.sceneNameTextField.text];

    if (!nameMatchFound)
    {
        self.doneButtonPressed = YES;
        [textField resignFirstResponder];

        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFSceneManager *sceneManager = ([LSFAllJoynManager getAllJoynManager]).lsfSceneManager;
            [sceneManager setSceneNameWithID: self.sceneModel.theID andSceneName: self.sceneNameTextField.text];
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
        [self.sceneNameTextField resignFirstResponder];

        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFSceneManager *sceneManager = ([LSFAllJoynManager getAllJoynManager]).lsfSceneManager;
            [sceneManager setSceneNameWithID: self.sceneModel.theID andSceneName: self.sceneNameTextField.text];
        });
    }
}

/*
 * Private functions
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

@end
