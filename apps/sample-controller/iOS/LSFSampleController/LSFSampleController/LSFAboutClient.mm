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

#import "LSFAboutClient.h"
#import "alljoyn/about/AJNAnnouncementReceiver.h"
#import "LSFConstants.h"
#import "AJNMessageArgument.h"
#import "alljoyn/about/AJNConvertUtil.h"
#import "LSFAboutData.h"
#import "LSFLampModelContainer.h"
#import "LSFLampModel.h"

@interface LSFAboutClient()

@property (nonatomic, strong) AJNBusAttachment *bus;
@property (strong, nonatomic) AJNAnnouncementReceiver *announcementReceiver;

@end

@implementation LSFAboutClient

@synthesize bus = _bus;
@synthesize announcementReceiver = _announcementReceiver;

-(id)initWithBusAttachment: (AJNBusAttachment *)bus
{
    self = [super init];
    
    if (self)
    {
        LSFConstants *constants = [LSFConstants getConstants];
        
        self.bus = bus;
        [self.bus enableConcurrentCallbacks];
        [self.bus addMatchRule: @"sessionless='t',type='error'"];
        
        const char* interfaces[] = { [constants.lampStateInterfaceName UTF8String], [constants.lampDetailsInterfaceName UTF8String], [constants.lampParametersInterfaceName UTF8String],
            [constants.lampServiceInterfaceName UTF8String]};
        
        self.announcementReceiver = [[AJNAnnouncementReceiver alloc] initWithAnnouncementListener: self andBus: self.bus];
        [self.announcementReceiver registerAnnouncementReceiverForInterfaces: interfaces withNumberOfInterfaces: 4];
    }
    
    return self;
}

- (void)announceWithVersion: (uint16_t)version port: (uint16_t)port busName: (NSString *)busName objectDescriptions: (NSMutableDictionary *)objectDescs aboutData: (NSMutableDictionary **)aboutData;
{
    if (port == 42)
    {
        NSLog(@"Announcement received on port 42.");
        
        LSFAboutData *myAboutData = [[LSFAboutData alloc] init];
        NSMutableDictionary *about = *aboutData;
        
        //Extract AppId
        AJNMessageArgument *msgArg = [about valueForKey: @"AppId"];
        myAboutData.appID = [self extractAppIDFromArrayOfBytes: msgArg];
        
        //Extract DefaultLanguage
        msgArg = [about valueForKey: @"DefaultLanguage"];
        myAboutData.defaultLanguage = [self extractNSStringFromAJNMessageArgument: msgArg];
        
        //Extract DeviceName
        msgArg = [about valueForKey: @"DeviceName"];
        myAboutData.deviceName = [self extractNSStringFromAJNMessageArgument: msgArg];
        
        //Extract DeviceID
        msgArg = [about valueForKey: @"DeviceId"];
        myAboutData.deviceID = [self extractNSStringFromAJNMessageArgument: msgArg];
        
        //Extract AppName
        msgArg = [about valueForKey: @"AppName"];
        myAboutData.appName = [self extractNSStringFromAJNMessageArgument: msgArg];
        
        //Extract Manufacturer
        msgArg = [about valueForKey: @"Manufacturer"];
        myAboutData.manufacturer = [self extractNSStringFromAJNMessageArgument: msgArg];
        
        //Extract ModelNumber
        msgArg = [about valueForKey: @"ModelNumber"];
        myAboutData.modelNumber = [self extractNSStringFromAJNMessageArgument: msgArg];
        
        NSMutableDictionary *lamps = ((LSFLampModelContainer *)[LSFLampModelContainer getLampModelContainer]).lampContainer;
        LSFLampModel *lampModel = [lamps valueForKey: myAboutData.deviceID];
        
        if (lampModel == nil)
        {
            NSLog(@"Did not find a lamp model for lamp ID, creating new lamp model");
            lampModel = [[LSFLampModel alloc] initWithLampID: myAboutData.deviceID];
            lampModel.aboutData = myAboutData;
            [lamps setValue: lampModel forKey: myAboutData.deviceID];
        }
        else
        {
            lampModel.aboutData = myAboutData;
            NSLog(@"updateLampID - Found LampModel for lamp ID");
        }
    }
    else
    {
        NSLog(@"Received about announcement on port %i.", port);
        
    }
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

@end
