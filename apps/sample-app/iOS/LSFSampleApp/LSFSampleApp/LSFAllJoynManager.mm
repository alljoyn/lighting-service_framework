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

#import "LSFAllJoynManager.h"
#import "AJNPasswordManager.h"
#import "LSFSampleLampGroupManager.h"
#import "LSFDispatchQueue.h"

@interface LSFAllJoynManager()

@property (nonatomic, strong) dispatch_queue_t backgroundQueue;
@property (nonatomic, strong) NSMutableDictionary *lampsAnnouncementData;

@end

@implementation LSFAllJoynManager

@synthesize bus = _bus;
@synthesize lsfControllerClient = _lsfControllerClient;
@synthesize lsfControllerServiceManager = _lsfControllerServiceManager;
@synthesize lsfLampManager = _lsfLampManager;
@synthesize lsfPresetManager = _lsfPresetManager;
@synthesize lsfLampGroupManager = _lsfLampGroupManager;
@synthesize lsfSceneManager = _lsfSceneManager;
@synthesize lsfMasterSceneManager = _lsfMasterSceneManager;
@synthesize isConnectedToController = _isConnectedToController;
@synthesize sccc = _sccc;
@synthesize scsmc = _scsmc;
@synthesize slmc = _slmc;
@synthesize spmc = _spmc;
@synthesize slgmc = _slgmc;
@synthesize ssmc = _ssmc;
@synthesize smsmc = _smsmc;
@synthesize backgroundQueue = _backgroundQueue;
@synthesize aboutManager = _aboutManager;
@synthesize lampsAnnouncementData = _lampsAnnouncementData;

+(LSFAllJoynManager *)getAllJoynManager
{
    static LSFAllJoynManager *ajManager = nil;
    static dispatch_once_t onceToken;
    
    dispatch_once(&onceToken, ^{
        NSLog(@"Creating LSFAllJoynManager instance");
        ajManager = [[self alloc] init];
    });
    
    return ajManager;
}

-(id)init
{
    self = [super init];
    
    if (self)
    {
        //NSLog(@"LSFAllJoynManager - init() function executing");
        
        self.isConnectedToController = NO;
        
        //Create dispatch queue and callback objects
        self.backgroundQueue = ([LSFDispatchQueue getDispatchQueue]).queue;
        self.sccc = [[LSFSampleControllerClientCallback alloc] init];
        self.scsmc = [[LSFSampleControllerServiceManagerCallback alloc] init];
        self.slmc = [[LSFSampleLampManagerCallback alloc] init];
        self.slgmc = [[LSFSampleLampGroupManagerCallback alloc] init];
        self.spmc = [[LSFSamplePresetManagerCallback alloc] init];
        self.ssmc = [[LSFSampleSceneManagerCallback alloc] init];
        self.smsmc = [[LSFSampleMasterSceneManagerCallback alloc] init];
        
        //Create Bus
        QStatus status = ER_OK;
        self.bus = [[AJNBusAttachment alloc] initWithApplicationName: @"LSFSampleController" allowRemoteMessages: YES];
        
        //Create password for Bundled Router
        status = [AJNPasswordManager setCredentialsForAuthMechanism: @"ALLJOYN_PIN_KEYX" usingPassword: @"000000"];
        if (status != ER_OK)
        {
            NSLog(@"ERROR: Failed to set Password Manager Credentials");
        }
        
        //Start the bus
        status = [self.bus start];
        if (status != ER_OK)
        {
            NSLog(@"ERROR: Failed to start bus. %@", [AJNStatus descriptionForStatusCode: status]);
        }
        
        //Connect the bus
        status = [self.bus connectWithArguments: @"null:"];
        if (status != ER_OK)
        {
            NSLog(@"ERROR: Failed to connect bus. %@", [AJNStatus descriptionForStatusCode: status]);
        }
        
//        status = [self.bus requestWellKnownName: @"org.alljoyn.BusNode" withFlags: kAJNBusNameFlagDoNotQueue];
//        if (status != ER_OK)
//        {
//            NSLog(@"ERROR: Failed to get well-known name for bundled router");
//        }
//        
//        status = [self.bus advertiseName: @"quiet@org.alljoyn.BusNode" withTransportMask: kAJNTransportMaskAny];
//        if (status != ER_OK)
//        {
//            NSLog(@"ERROR: Failed to advertise well-known name for bundled router");
//        }
        
        self.lsfControllerClient = [[LSFControllerClient alloc] initWithBusAttachment: self.bus andControllerClientCallbackDelegate: self.sccc];
        self.lsfControllerServiceManager = [[LSFControllerServiceManager alloc] initWithControllerClient: self.lsfControllerClient andControllerServiceManagerCallbackDelegate: self.scsmc];
        self.lsfLampManager = [[LSFLampManager alloc] initWithControllerClient: self.lsfControllerClient andLampManagerCallbackDelegate: self.slmc];
        self.lsfLampGroupManager = [[LSFSampleLampGroupManager alloc] initWithControllerClient: self.lsfControllerClient andLampManagerCallbackDelegate: self.slgmc];
        self.lsfPresetManager = [[LSFPresetManager alloc] initWithControllerClient: self.lsfControllerClient andPresetManagerCallbackDelegate: self.spmc];
        self.lsfSceneManager = [[LSFSceneManager alloc] initWithControllerClient: self.lsfControllerClient andSceneManagerCallbackDelegate: self.ssmc];
        self.lsfMasterSceneManager = [[LSFMasterSceneManager alloc] initWithControllerClient: self.lsfControllerClient andMasterSceneManagerCallbackDelegate: self.smsmc];
        self.aboutManager = [[LSFAboutManager alloc] initWithBusAttachment: self.bus];
        self.lampsAnnouncementData = [[NSMutableDictionary alloc] init];
    }
    
    return self;
}

-(void)addNewLamp: (NSString*)lampID lampAnnouncementData: (LSFLampAnnouncementData*)lampAnnData
{
    if (!self.lampsAnnouncementData[lampID])
    {
        [self.lampsAnnouncementData setObject: lampAnnData forKey: lampID];
    }
    else
    {
        [self.lampsAnnouncementData setValue: lampAnnData forKey: lampID];
    }
}

-(void)getAboutDataForLampID: (NSString*)lampID
{
    LSFLampAnnouncementData* lampAnnData = [self.lampsAnnouncementData  objectForKey: lampID];
    [self.aboutManager getAboutDataFromBusName: [lampAnnData busName] onPort: [lampAnnData port]];
}
@end
