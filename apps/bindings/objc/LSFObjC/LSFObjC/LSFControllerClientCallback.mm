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

#import "LSFControllerClientCallback.h"

LSFControllerClientCallback::LSFControllerClientCallback(id<LSFControllerClientCallbackDelegate> delegate) : _ccDelegate(delegate)
{
    //Empty constructor
}

LSFControllerClientCallback::~LSFControllerClientCallback()
{
    _ccDelegate = nil;
}

void LSFControllerClientCallback::ConnectedToControllerServiceCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName)
{
    [_ccDelegate connectedToControllerServiceWithID: [NSString stringWithUTF8String: controllerServiceDeviceID.c_str()] andName:[NSString stringWithUTF8String: controllerServiceName.c_str()]];
}

void LSFControllerClientCallback::ConnectToControllerServiceFailedCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName)
{
    [_ccDelegate connectToControllerServiceFailedForID: [NSString stringWithUTF8String: controllerServiceDeviceID.c_str()] andName: [NSString stringWithUTF8String: controllerServiceName.c_str()]];
}

void LSFControllerClientCallback::DisconnectedFromControllerServiceCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName)
{
    [_ccDelegate disconnectedFromControllerServiceWithID: [NSString stringWithUTF8String: controllerServiceDeviceID.c_str()] andName: [NSString stringWithUTF8String: controllerServiceName.c_str()]];
}

void LSFControllerClientCallback::ControllerClientErrorCB(const ErrorCodeList& errorCodeList)
{
    NSMutableArray *errorCodesArray = [[NSMutableArray alloc] init];
    
    for (std::list<ErrorCode>::const_iterator iter = errorCodeList.begin(); iter != errorCodeList.end(); ++iter)
    {
        [errorCodesArray addObject: [NSNumber numberWithInt: (*iter)]];
    }
    
    [_ccDelegate controllerClientError: errorCodesArray];
}