/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

#include <ControllerServiceManager.h>
#include <ControllerClient.h>
#include <qcc/Debug.h>

using namespace lsf;
using namespace ajn;

#define QCC_MODULE "CONTROLLER_SERVICE_MANAGER"

namespace lsf {

ControllerServiceManager::ControllerServiceManager(ControllerClient& controllerClient, ControllerServiceManagerCallback& callback) :
    Manager(controllerClient),
    callback(callback)
{
    controllerClient.controllerServiceManagerPtr = this;
}

ControllerClientStatus ControllerServiceManager::GetControllerServiceVersion(void)
{
    QCC_DbgPrintf(("%s", __func__));
    return controllerClient.MethodCallAsyncForReplyWithUint32Value(
               ControllerServiceInterfaceName,
               "GetControllerServiceVersion");
}

ControllerClientStatus ControllerServiceManager::LightingResetControllerService(void)
{
    QCC_DbgPrintf(("%s", __func__));
    return controllerClient.MethodCallAsyncForReplyWithUint32Value(
               ControllerServiceInterfaceName,
               "LightingResetControllerService");
}

}