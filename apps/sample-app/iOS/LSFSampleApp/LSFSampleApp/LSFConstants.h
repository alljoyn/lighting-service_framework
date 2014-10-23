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
#import <SystemConfiguration/CaptiveNetwork.h>

@interface LSFConstants : NSObject

@property (nonatomic, strong) NSDictionary *lampDetailsAboutData;
@property (nonatomic, strong) NSArray *lampDetailsFields;
@property (nonatomic, strong) NSArray *aboutFields;
@property (nonatomic, strong) NSString *lampServiceObjectDescription;
@property (nonatomic, strong) NSString *lampServiceInterfaceName;
@property (nonatomic, strong) NSString *lampStateInterfaceName;
@property (nonatomic, strong) NSString *lampParametersInterfaceName;
@property (nonatomic, strong) NSString *lampDetailsInterfaceName;
@property (nonatomic, strong) NSString *controllerServiceObjectDescription;
@property (nonatomic, strong) NSString *controllerServiceInterfaceName;
@property (nonatomic, strong) NSString *controllerServiceLampInterfaceName;
@property (nonatomic, strong) NSString *controllerServiceLampGroupInterfaceName;
@property (nonatomic, strong) NSString *controllerServicePresetInterfaceName;
@property (nonatomic, strong) NSString *controllerServiceSceneInterfaceName;
@property (nonatomic, strong) NSString *controllerServiceMasterSceneInterfaceName;
@property (nonatomic, strong) NSString *aboutInterfaceName;
@property (nonatomic, strong) NSString *configServiceInterfaceName;
@property (nonatomic, strong) NSString *defaultLanguage;
@property (nonatomic, strong) NSString *ALL_LAMPS_GROUP_ID;
@property (nonatomic, strong) NSArray *supportedEffects;
@property (nonatomic, strong) NSArray *effectImages;
@property (nonatomic) unsigned int pollingDelaySeconds;
@property (nonatomic) unsigned int lampExperationMilliseconds;
@property (nonatomic) double RETRY_INTERVAL;
@property (nonatomic, strong) UIColor *uniformPowerGreen;
@property (nonatomic, strong) UIColor *midstatePowerOrange;
@property (nonatomic) unsigned int UI_DELAY;

+(LSFConstants *)getConstants;
-(unsigned int)scaleLampStateValue: (unsigned int)value withMax: (unsigned int)max;
-(unsigned int)unscaleLampStateValue: (unsigned int)value withMax: (unsigned int)max;
-(unsigned int)scaleColorTemp: (unsigned int)value;
-(unsigned int)unscaleColorTemp: (unsigned int)value;
-(NSString *)currentWifiSSID;

@end
