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

#import <Foundation/Foundation.h>
#import "LSFControllerClient.h"
#import "LSFLampManager.h"
#import "LSFPresetManager.h"
#import "AJNBusAttachment.h"
#import "LSFReloadUIDelegate.h"
#import "LSFControllerServiceConnectedDelegate.h"
#import "LSFLampGroupManager.h"
#import "LSFControllerMaintenance.h"
#import "LSFSceneManager.h"

@interface LSFAllJoynManager : NSObject

@property (nonatomic, strong) AJNBusAttachment *bus;
@property (nonatomic, strong) LSFControllerClient *lsfControllerClient;
@property (nonatomic, strong) LSFLampManager *lsfLampManager;
@property (nonatomic, strong) LSFLampGroupManager *lsfLampGroupManager;
@property (nonatomic, strong) LSFPresetManager *lsfPresetManager;
@property (nonatomic, strong) LSFSceneManager *lsfSceneManager;
@property (nonatomic, strong) LSFControllerMaintenance *controllerMaintenance;
@property (nonatomic, strong) NSString *controllerName;
@property (nonatomic) BOOL isConnectedToController;
@property (nonatomic) BOOL foundLamps;

+(LSFAllJoynManager *)getAllJoynManager;
-(void)setReloadUIDelegate: (id<LSFReloadUIDelegate>)reloadUIDelegate;
-(void)setControllerServiceConnectedDelegate: (id<LSFControllerServiceConnectedDelegate>)delegate;

@end
