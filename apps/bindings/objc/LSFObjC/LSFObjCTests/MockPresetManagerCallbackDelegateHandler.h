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
#import "LSFObjC/LSFPresetManagerCallbackDelegate.h"

@interface MockPresetManagerCallbackDelegateHandler : NSObject <LSFPresetManagerCallbackDelegate>

-(id)init;
-(NSArray *)getCallbackData;

//Delegate methods
-(void)getPresetReplyWithCode: (LSFResponseCode)rc presetID: (NSString *)presetID andPreset: (LSFLampState *)preset;
-(void)getAllPresetIDsReplyWithCode: (LSFResponseCode)rc andPresetIDs: (NSArray *)presetIDs;
-(void)getPresetNameReplyWithCode: (LSFResponseCode)rc presetID: (NSString *)presetID language: (NSString *)language andPresetName: (NSString *)presetName;
-(void)setPresetNameReplyWithCode: (LSFResponseCode)rc presetID: (NSString *)presetID andLanguage: (NSString *)language;
-(void)presetsNameChanged: (NSArray *)presetIDs;
-(void)createPresetReplyWithCode: (LSFResponseCode)rc andPresetID: (NSString *)presetID;
-(void)presetsCreated: (NSArray *)presetIDs;
-(void)updatePresetReplyWithCode: (LSFResponseCode)rc andPresetID: (NSString *)presetID;
-(void)presetsUpdated: (NSArray *)presetIDs;
-(void)deletePresetReplyWithCode: (LSFResponseCode)rc andPresetID: (NSString *)presetID;
-(void)presetsDeleted: (NSArray *)presetIDs;
-(void)getDefaultLampStateReplyWithCode: (LSFResponseCode)rc andLampState: (LSFLampState *)defaultLampState;
-(void)setDefaultLampStateReplyWithCode: (LSFResponseCode)rc;
-(void)defaultLampStateChanged;

@end
