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

#include <qcc/Debug.h>
#include <Manager.h>
#include <ControllerClient.h>

#define QCC_MODULE "MANAGER"

using namespace lsf;

Manager::Manager(ControllerClient& controllerClient) :
    controllerClient(controllerClient)
{
    QCC_DbgTrace(("%s", __func__));
}