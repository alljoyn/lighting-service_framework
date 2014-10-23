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

#import "LSFAboutManager.h"
#import "alljoyn/about/AJNAnnouncementReceiver.h"
#import "LSFConstants.h"
#import "AJNMessageArgument.h"
#import "alljoyn/about/AJNConvertUtil.h"
#import "LSFAboutData.h"
#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"
#import "LSFAllJoynManager.h"
#import "LSFDispatchQueue.h"
#import "AJNSessionOptions.h"
#import "alljoyn/about/AJNAboutClient.h"
#import "LSFConfigManager.h"
#import "LSFLampAnnouncementData.h"

@interface LSFAboutManager()

@property (nonatomic, strong) AJNBusAttachment *bus;
@property (strong, nonatomic) AJNAnnouncementReceiver *announcementReceiver;

-(void)getAboutDataFrom: (NSString *)peerName onPort: (unsigned int)port usingAboutData: (NSMutableDictionary *)aboutData;
-(void)extractAboutData: (NSMutableDictionary *)aboutData;
-(NSString *)extractNSStringFromAJNMessageArgument: (AJNMessageArgument *)msgArg;
-(NSString *)extractAppIDFromArrayOfBytes: (AJNMessageArgument *)msgArg;
-(NSString *)buildNSStringFromAJNMessageArgument: (AJNMessageArgument *)msgArg;

@end

@implementation LSFAboutManager

@synthesize bus = _bus;
@synthesize announcementReceiver = _announcementReceiver;

-(id)initWithBusAttachment: (AJNBusAttachment *)bus
{
    self = [super init];
    
    if (self)
    {
        self.bus = bus;
        [self.bus enableConcurrentCallbacks];
        [self.bus addMatchRule: @"sessionless='t',type='error'"];
    }
    
    return self;
}

-(void)registerAnnouncementHandler
{
    NSLog(@"LSFAboutManager - registerAnnouncementHandler() executing");

    LSFConstants *constants = [LSFConstants getConstants];

    const char* lampInterfaces[] = { [constants.lampStateInterfaceName UTF8String], [constants.lampDetailsInterfaceName UTF8String], [constants.lampParametersInterfaceName UTF8String],
        [constants.lampServiceInterfaceName UTF8String] };

    const char* controllerInterfaces[] =
    {
        [constants.controllerServiceInterfaceName UTF8String],
        [constants.controllerServiceLampInterfaceName UTF8String],
        [constants.controllerServiceLampGroupInterfaceName UTF8String],
        [constants.controllerServicePresetInterfaceName UTF8String],
        [constants.controllerServiceSceneInterfaceName UTF8String],
        [constants.controllerServiceMasterSceneInterfaceName UTF8String],
        [constants.configServiceInterfaceName UTF8String],
        [constants.aboutInterfaceName UTF8String]
    };

    self.announcementReceiver = [[AJNAnnouncementReceiver alloc] initWithAnnouncementListener: self andBus: self.bus];
    QStatus status = [self.announcementReceiver registerAnnouncementReceiverForInterfaces: lampInterfaces withNumberOfInterfaces: 4];
    if (status != ER_OK)
    {
        NSLog(@"Error registering announcement interfaces for lamps");
    }

    status = [self.announcementReceiver registerAnnouncementReceiverForInterfaces: controllerInterfaces withNumberOfInterfaces: 8];
    if (status != ER_OK)
    {
        NSLog(@"Error registering announcement interfaces for controllers");
    }
}

-(void)unregisterAnnouncementHandler
{
    NSLog(@"LSFAboutManager - unregisterAnnouncementHandler() executing");

    LSFConstants *constants = [LSFConstants getConstants];

    const char* lampInterfaces[] = { [constants.lampStateInterfaceName UTF8String], [constants.lampDetailsInterfaceName UTF8String], [constants.lampParametersInterfaceName UTF8String],
        [constants.lampServiceInterfaceName UTF8String] };

    const char* controllerInterfaces[] =
    {
        [constants.controllerServiceInterfaceName UTF8String],
        [constants.controllerServiceLampInterfaceName UTF8String],
        [constants.controllerServiceLampGroupInterfaceName UTF8String],
        [constants.controllerServicePresetInterfaceName UTF8String],
        [constants.controllerServiceSceneInterfaceName UTF8String],
        [constants.controllerServiceMasterSceneInterfaceName UTF8String],
        [constants.configServiceInterfaceName UTF8String],
        [constants.aboutInterfaceName UTF8String]
    };

    QStatus status = [self.announcementReceiver unRegisterAnnouncementReceiverForInterfaces: lampInterfaces withNumberOfInterfaces: 4];
    if (status != ER_OK)
    {
        NSLog(@"Error unregistering announcement interfaces for lamps");
    }

    status = [self.announcementReceiver unRegisterAnnouncementReceiverForInterfaces: controllerInterfaces withNumberOfInterfaces: 8];
    if (status != ER_OK)
    {
        NSLog(@"Error unregistering announcement interfaces for lamps");
    }
}

- (void)announceWithVersion: (uint16_t)version port: (uint16_t)port busName: (NSString *)busName objectDescriptions: (NSMutableDictionary *)objectDescs aboutData: (NSMutableDictionary **)aboutData;
{
    LSFConstants *constants = [LSFConstants getConstants];

    NSSet *cis = [NSSet setWithArray: [objectDescs valueForKey: constants.controllerServiceObjectDescription]];
    NSSet *controllerInterfacesSet = [NSSet setWithArray: [NSArray arrayWithObjects: constants.controllerServiceInterfaceName, constants.controllerServiceLampInterfaceName, constants.controllerServiceLampGroupInterfaceName, constants.controllerServicePresetInterfaceName, constants.controllerServiceSceneInterfaceName, constants.controllerServiceMasterSceneInterfaceName, nil]];

    NSSet *lis = [NSSet setWithArray: [objectDescs valueForKey: constants.lampServiceObjectDescription]];
    NSSet *lampInterfacesSet = [NSSet setWithArray: [NSArray arrayWithObjects: constants.lampServiceInterfaceName, constants.lampStateInterfaceName, constants.lampDetailsInterfaceName, constants.lampParametersInterfaceName, nil]];

    if ([lis isEqualToSet: lampInterfacesSet])
    {
        AJNMessageArgument *msgArg  = [*aboutData valueForKey: @"DeviceId"];
        NSString *lampID = [self extractNSStringFromAJNMessageArgument: msgArg];
        LSFLampAnnouncementData *lampAnnData = [[LSFLampAnnouncementData alloc] initPort: port busName: busName];

        dispatch_async(dispatch_queue_create("GetAboutData", NULL), ^{
            //[self getAboutDataFrom: busName onPort: port usingAboutData: *aboutData];
            [[LSFAllJoynManager getAllJoynManager] addNewLamp: lampID lampAnnouncementData: lampAnnData];
        });
    }
    else if ([cis isEqualToSet: controllerInterfacesSet])
    {
        LSFConfigManager *configManager = [LSFConfigManager getConfigManager];
        configManager.controllerBusName = busName;
        configManager.controllerPort = port;
    }
    else
    {
        NSLog(@"Supported interfaces do not match lamps or controller");
    }
}

-(void)getAboutDataFromBusName: (NSString *)busName onPort: (unsigned int)port
{
    [self getAboutDataFrom:busName onPort:port usingAboutData:nil];
}

/*
 * Private Functions
 */
-(void)getAboutDataFrom: (NSString *)peerName onPort: (unsigned int)port usingAboutData: (NSMutableDictionary *)aboutData
{
    AJNSessionOptions *opt = [[AJNSessionOptions alloc] initWithTrafficType: kAJNTrafficMessages supportsMultipoint: false proximity: kAJNProximityAny transportMask: kAJNTransportMaskAny];
    AJNSessionId sessionID = [self.bus joinSessionWithName: peerName onPort: port withDelegate: nil options: opt];

    if (sessionID == 0 || sessionID == -1)
    {
        NSLog(@"Failed to join session with lamp (%@). SID = %u", peerName, sessionID);
    }
    else
    {
        NSLog(@"Session ID = %u", sessionID);

        NSMutableDictionary *fullAboutData;
        AJNAboutClient *ajnAboutClient = [[AJNAboutClient alloc] initWithBus: self.bus];

        QStatus status;
        for (int i = 0; i < 5; i++)
        {
            status = [ajnAboutClient aboutDataWithBusName: peerName andLanguageTag: ([LSFConstants getConstants]).defaultLanguage andAboutData: &fullAboutData andSessionId: sessionID];

            if (status != ER_OK)
            {
                NSLog(@"Failed attempt (%i) to get about data from lamp (%@). Retrying.", i, peerName);
            }
            else
            {
                NSLog(@"Breaking on attempt %i", i);
                break;
            }
        }

        if (status != ER_OK)
        {
            NSLog(@"Failed 5 attempts to get about data from lamp (%@).", peerName);
        }
        else
        {
            [self extractAboutData: fullAboutData];
        }

        //Ensure AboutClient is nil before proceeding. This ensures the object gets cleaned up by the garbage collector
        ajnAboutClient = nil;
    }

    //Need to make sure I leave the session to ensure the sample app fully disconnects from the lamp
    QStatus status = [self.bus leaveSession: sessionID];
    if (status != ER_OK)
    {
        NSLog(@"Failed to leave session %u with %@", sessionID, peerName);
    }
    else
    {
        NSLog(@"Successfully left session %u with %@", sessionID, peerName);
    }

    //Clean up objects
    opt = nil;
    sessionID = nil;
}

-(void)extractAboutData: (NSMutableDictionary *)aboutData
{
    LSFAboutData *myAboutData = [[LSFAboutData alloc] init];

    //Extract AppId
    AJNMessageArgument *msgArg = [aboutData valueForKey: @"AppId"];
    myAboutData.appID = [self extractAppIDFromArrayOfBytes: msgArg];

    //Extract DefaultLanguage
    msgArg = [aboutData valueForKey: @"DefaultLanguage"];
    myAboutData.defaultLanguage = [self extractNSStringFromAJNMessageArgument: msgArg];

    //Extract DeviceName
    msgArg = [aboutData valueForKey: @"DeviceName"];
    myAboutData.deviceName = [self extractNSStringFromAJNMessageArgument: msgArg];

    //Extract DeviceID
    msgArg = [aboutData valueForKey: @"DeviceId"];
    myAboutData.deviceID = [self extractNSStringFromAJNMessageArgument: msgArg];

    //Extract AppName
    msgArg = [aboutData valueForKey: @"AppName"];
    myAboutData.appName = [self extractNSStringFromAJNMessageArgument: msgArg];

    //Extract Manufacturer
    msgArg = [aboutData valueForKey: @"Manufacturer"];
    myAboutData.manufacturer = [self extractNSStringFromAJNMessageArgument: msgArg];

    //Extract ModelNumber
    msgArg = [aboutData valueForKey: @"ModelNumber"];
    myAboutData.modelNumber = [self extractNSStringFromAJNMessageArgument: msgArg];

    //Extract SupportedLanguages
    msgArg = [aboutData valueForKey: @"SupportedLanguages"];
    myAboutData.supportedLanguages = [self buildNSStringFromAJNMessageArgument: msgArg];

    //Extract Description
    msgArg = [aboutData valueForKey: @"Description"];
    myAboutData.description = [self extractNSStringFromAJNMessageArgument: msgArg];

    //Extract DateOfManufacture
    msgArg = [aboutData valueForKey: @"DateOfManufacture"];
    myAboutData.dateOfManufacture = [self extractNSStringFromAJNMessageArgument: msgArg];

    //Extract SoftwareVersion
    msgArg = [aboutData valueForKey: @"SoftwareVersion"];
    myAboutData.softwareVersion = [self extractNSStringFromAJNMessageArgument: msgArg];

    //Extract AJSoftwareVersion
    msgArg = [aboutData valueForKey: @"AJSoftwareVersion"];
    myAboutData.ajSoftwareVersion = [self extractNSStringFromAJNMessageArgument: msgArg];

    //Extract HardwareVersion
    msgArg = [aboutData valueForKey: @"HardwareVersion"];
    myAboutData.hardwareVersion = [self extractNSStringFromAJNMessageArgument: msgArg];

    //Extract SupportUrl
    msgArg = [aboutData valueForKey: @"SupportUrl"];
    myAboutData.supportURL = [self extractNSStringFromAJNMessageArgument: msgArg];

    LSFAllJoynManager *ajManager = [LSFAllJoynManager getAllJoynManager];
    [ajManager.slmc postUpdateLampID: myAboutData.deviceID withAboutData: myAboutData];
}

-(NSString *)extractNSStringFromAJNMessageArgument: (AJNMessageArgument *)msgArg
{
    QStatus status;
    const char* msgArgContent;
    status = [msgArg value: @"s", &msgArgContent];
    return [AJNConvertUtil convertConstCharToNSString: msgArgContent];
}

-(NSString *)extractAppIDFromArrayOfBytes: (AJNMessageArgument *)msgArg
{
    QStatus status;
    NSMutableData *ajnMsgArgData;
    uint8_t *AppIdBuffer;
    size_t numElements;
    status = [msgArg value:@"ay", &numElements, &AppIdBuffer];
    ajnMsgArgData = [NSMutableData dataWithBytes:AppIdBuffer length:(NSInteger)numElements];
    return [[ajnMsgArgData description] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"<>"]];
}

-(NSString *)buildNSStringFromAJNMessageArgument: (AJNMessageArgument *)msgArg
{
    NSMutableString *supportedLanguages = [[NSMutableString alloc] initWithString: @""];

    QStatus status;
    MsgArg *argsArray;
    size_t numElements;
    status = [msgArg value:@"as", &numElements, &argsArray];
    for (size_t i = 0; i < numElements; i++)
    {
        char* suppLang;
        argsArray[i].Get("s", &suppLang);

        NSString *supportedLanguage = [NSString stringWithUTF8String: suppLang];

        if (i == (numElements - 1))
        {
            [supportedLanguages appendFormat: @"%@", supportedLanguage];
        }
        else
        {
            [supportedLanguages appendFormat: @"%@, ", supportedLanguage];
        }
    }

    NSLog(@"Final Supported Languages = %@", supportedLanguages);

    return [NSString stringWithString: supportedLanguages];
}

@end
