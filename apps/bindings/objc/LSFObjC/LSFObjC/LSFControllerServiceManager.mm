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

#import "LSFControllerServiceManager.h"
#import "LSFControllerServiceManagerCallback.h"

@interface LSFControllerServiceManager()

@property (nonatomic, readonly) lsf::ControllerServiceManager *controllerServiceManager;
@property (nonatomic, assign) LSFControllerServiceManagerCallback *controllerServiceManagerCallback;

@end

@implementation LSFControllerServiceManager

@synthesize controllerServiceManager = _controllerServiceManager;
@synthesize controllerServiceManagerCallback = _controllerServiceManagerCallback;

-(id)initWithControllerClient: (LSFControllerClient *)controllerClient andControllerServiceManagerCallbackDelegate: (id<LSFControllerServiceManagerCallbackDelegate>)csmDelegate
{
    self = [super init];
    
    if (self)
    {
        self.controllerServiceManagerCallback = new LSFControllerServiceManagerCallback(csmDelegate);
        self.handle = new lsf::ControllerServiceManager(*(static_cast<lsf::ControllerClient*>(controllerClient.handle)), *(self.controllerServiceManagerCallback));
    }
    
    return self;
}

-(ControllerClientStatus)getControllerServiceVersion
{
    return self.controllerServiceManager->GetControllerServiceVersion();
}

-(ControllerClientStatus)lightingResetControllerService
{
    return self.controllerServiceManager->LightingResetControllerService();
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
- (lsf::ControllerServiceManager *)controllerServiceManager
{
    return static_cast<lsf::ControllerServiceManager*>(self.handle);
}

@end
