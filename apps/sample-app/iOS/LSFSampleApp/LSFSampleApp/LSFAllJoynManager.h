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
#import "LSFControllerServiceManager.h"
#import "LSFLampManager.h"
#import "LSFPresetManager.h"
#import "AJNBusAttachment.h"
#import "LSFLampGroupManager.h"
#import "LSFSceneManager.h"
#import "LSFMasterSceneManager.h"
#import "LSFSampleControllerClientCallback.h"
#import "LSFSampleControllerServiceManagerCallback.h"
#import "LSFSampleLampManagerCallback.h"
#import "LSFSampleLampGroupManagerCallback.h"
#import "LSFSamplePresetManagerCallback.h"
#import "LSFSampleSceneManagerCallback.h"
#import "LSFSampleMasterSceneManagerCallback.h"
#import "LSFLampAnnouncementData.h"
#import "LSFAboutManager.h"

@interface LSFAllJoynManager : NSObject

@property (nonatomic, strong) AJNBusAttachment *bus;
@property (nonatomic, strong) LSFControllerClient *lsfControllerClient;
@property (nonatomic, strong) LSFControllerServiceManager *lsfControllerServiceManager;
@property (nonatomic, strong) LSFLampManager *lsfLampManager;
@property (nonatomic, strong) LSFLampGroupManager *lsfLampGroupManager;
@property (nonatomic, strong) LSFPresetManager *lsfPresetManager;
@property (nonatomic, strong) LSFSceneManager *lsfSceneManager;
@property (nonatomic, strong) LSFMasterSceneManager *lsfMasterSceneManager;
@property (nonatomic, strong) LSFSampleControllerClientCallback *sccc;
@property (nonatomic, strong) LSFSampleControllerServiceManagerCallback *scsmc;
@property (nonatomic, strong) LSFSampleLampManagerCallback *slmc;
@property (nonatomic, strong) LSFSampleLampGroupManagerCallback *slgmc;
@property (nonatomic, strong) LSFSamplePresetManagerCallback *spmc;
@property (nonatomic, strong) LSFSampleSceneManagerCallback *ssmc;
@property (nonatomic, strong) LSFSampleMasterSceneManagerCallback *smsmc;
@property (nonatomic, strong) LSFAboutManager *aboutManager;
@property (nonatomic) BOOL isConnectedToController;

+(LSFAllJoynManager *)getAllJoynManager;
-(void)addNewLamp: (NSString*)lampID lampAnnouncementData:(LSFLampAnnouncementData*)lampAnnData;
-(void)getAboutDataForLampID: (NSString*)lampID;

@end
