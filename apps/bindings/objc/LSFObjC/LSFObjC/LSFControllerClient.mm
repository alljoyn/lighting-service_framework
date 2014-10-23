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

#import "LSFControllerClient.h"
#import "LSFControllerClientCallback.h"

@interface LSFControllerClient ()

@property (nonatomic, assign) AJNBusAttachment *bus;
@property (nonatomic, assign) LSFControllerClientCallback *controllerClientCallback;
@property (nonatomic, readonly) ControllerClient *controllerClient;

@end

@implementation LSFControllerClient

@synthesize bus = _bus;
@synthesize controllerClientCallback = _controllerClientCallback;
@synthesize controllerClient = _controllerClient;

-(id)initWithBusAttachment: (AJNBusAttachment *)bus
                            andControllerClientCallbackDelegate: (id<LSFControllerClientCallbackDelegate>)cccDelegate;
{
    self = [super init];
    
    if (self)
    {
        self.bus = bus;
        self.controllerClientCallback = new LSFControllerClientCallback(cccDelegate);
        self.handle = new ControllerClient(*static_cast<ajn::BusAttachment*>(bus.handle),
                                           *(self.controllerClientCallback));
    }
    
    return self;
}

-(void)dealloc
{
    lsf::ControllerClient *ptr = [self controllerClient];

    if (ptr)
    {
        delete ptr;
    }

    self.handle = NULL;
}

-(unsigned int)getVersion
{
    return self.controllerClient->GetVersion();
}

-(ControllerClientStatus)start
//-(void)start
{
    return self.controllerClient->Start();
    //self.controllerClient->Start();
}

-(ControllerClientStatus)stop
//-(void)stop
{
    return self.controllerClient->Stop();
    //self.controllerClient->Stop();
}

/*
 * Accessor for the internal C++ API object this objective-c class encapsulates
 */
- (lsf::ControllerClient *)controllerClient
{
    return static_cast<lsf::ControllerClient*>(self.handle);
}

@end

