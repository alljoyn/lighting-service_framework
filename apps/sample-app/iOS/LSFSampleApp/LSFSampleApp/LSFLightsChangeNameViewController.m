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

#import "LSFLightsChangeNameViewController.h"
#import "LSFDispatchQueue.h"
#import "LSFLampManager.h"
#import "LSFAllJoynManager.h"
#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"
#import "LSFUtilityFunctions.h"
#import "LSFGarbageCollector.h"
#import "LSFLightsTableViewController.h"
#import "LSFLightInfoTableViewController.h"

@interface LSFLightsChangeNameViewController ()

@property (nonatomic, strong) LSFLampModel *lampModel;
@property (nonatomic) BOOL doneButtonPressed;

-(BOOL)checkForDuplicateName: (NSString *)name;

@end

@implementation LSFLightsChangeNameViewController

@synthesize lampID = _lampID;
@synthesize lampNameTextField = _lampNameTextField;
@synthesize lampModel = _lampModel;
@synthesize doneButtonPressed = _doneButtonPressed;

-(void)viewDidLoad
{
    [super viewDidLoad];
}

-(void)viewWillAppear: (BOOL)animated
{
    [super viewWillAppear: animated];

    //Set lamps callback delegate
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.slmc setReloadLightsDelegate: self];

        LSFGarbageCollector *garbageCollector = [LSFGarbageCollector getGarbageCollector];
        [garbageCollector setReloadLightsDelegate: self];
    });

    [self.lampNameTextField becomeFirstResponder];
    self.doneButtonPressed = NO;

    [self reloadLampWithID: self.lampID];
}

-(void)viewWillDisappear: (BOOL)animated
{
    [super viewWillDisappear: animated];

    //Clear lamps callback delegate
    dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
        LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
        [ajManager.slmc setReloadLightsDelegate: nil];

        LSFGarbageCollector *garbageCollector = [LSFGarbageCollector getGarbageCollector];
        [garbageCollector setReloadLightsDelegate: nil];
    });
}

-(void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

/*
 * LSFReloadLightsCallbackDelegate Implementation
 */
-(void)reloadLampWithID: (NSString *)lampID
{
    if ([self.lampID isEqualToString: lampID])
    {
        //Refresh lamp model from dictionary using lamp ID
        LSFLampModelContainer *lampsContainer = [LSFLampModelContainer getLampModelContainer];
        NSMutableDictionary *lamps = lampsContainer.lampContainer;
        self.lampModel = [lamps valueForKey: self.lampID];

        self.lampNameTextField.text = self.lampModel.name;
    }
}

-(void)deleteLampWithID: (NSString *)lampID andName: (NSString *)lampName
{
//    if ([self.lampID isEqualToString: lampID])
//    {
//        [self.navigationController popToRootViewControllerAnimated: YES];
//
//        dispatch_async(dispatch_get_main_queue(), ^{
//            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Connection Lost"
//                                                            message: [NSString stringWithFormat: @"Unable to connect to \"%@\".", lampName]
//                                                           delegate: nil
//                                                  cancelButtonTitle: @"OK"
//                                                  otherButtonTitles: nil];
//            [alert show];
//        });
//    }
}

/*
 * UITextFieldDelegate Implementation
 */
-(BOOL)textFieldShouldReturn: (UITextField *)textField
{
    if (![LSFUtilityFunctions checkNameLength: self.lampNameTextField.text entity: @"Lamp Name"])
    {
        return NO;
    }

    BOOL nameMatchFound = [self checkForDuplicateName: self.lampNameTextField.text];
    
    if (!nameMatchFound)
    {
        self.doneButtonPressed = YES;
        [textField resignFirstResponder];
        
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
            [lampManager setLampNameWithID: self.lampID andName: self.lampNameTextField.text];
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
        [self.lampNameTextField resignFirstResponder];
        
        dispatch_async(([LSFDispatchQueue getDispatchQueue]).queue, ^{
            LSFLampManager *lampManager = ([LSFAllJoynManager getAllJoynManager]).lsfLampManager;
            [lampManager setLampNameWithID: self.lampID andName: self.lampNameTextField.text];
        });
    }
}

/*
 * Private functions
 */
-(BOOL)checkForDuplicateName: (NSString *)name
{
    LSFLampModelContainer *container = [LSFLampModelContainer getLampModelContainer];
    NSDictionary *lamps = container.lampContainer;
    
    for (LSFLampModel *model in [lamps allValues])
    {
        if ([name isEqualToString: model.name])
        {
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle: @"Duplicate Name"
                                                            message: [NSString stringWithFormat: @"Warning: there is already a lamp named \"%@.\" Although it's possible to use the same name for more than one lamp, it's better to give each lamp a unique name.\n\nKeep duplicate lamp name \"%@\"?", name, name]
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
