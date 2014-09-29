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

#import "LSFControllerServiceManagerCallback.h"

LSFControllerServiceManagerCallback::LSFControllerServiceManagerCallback(id<LSFControllerServiceManagerCallbackDelegate> delegate) : _csmDelegate(delegate)
{
    //Empty Constructor
}

LSFControllerServiceManagerCallback::~LSFControllerServiceManagerCallback()
{
    _csmDelegate = nil;
}

void LSFControllerServiceManagerCallback::GetControllerServiceVersionReplyCB(uint32_t version)
{
    [_csmDelegate getControllerServiceVersionReply: version];
}

void LSFControllerServiceManagerCallback::LightingResetControllerServiceReplyCB(LSFResponseCode responseCode)
{
    [_csmDelegate lightingResetControllerServiceReplyWithCode: responseCode];
}

void LSFControllerServiceManagerCallback::ControllerServiceLightingResetCB(void)
{
    [_csmDelegate controllerServiceLightingReset];
}

void LSFControllerServiceManagerCallback::ControllerServiceNameChangedCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName)
{
    [_csmDelegate controllerServiceNameChangedForControllerID: [NSString stringWithUTF8String: controllerServiceDeviceID.c_str()] andName: [NSString stringWithUTF8String: controllerServiceName.c_str()]];
}

