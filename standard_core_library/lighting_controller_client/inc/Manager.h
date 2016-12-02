#ifndef _MANAGER_H_
#define _MANAGER_H_
/**
 * \ingroup ControllerClient
 */
/**
 * \file  lighting_controller_client/inc/Manager.h
 * This file provides definitions for manager type of classes
 */

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

#include <alljoyn/InterfaceDescription.h>
#include <alljoyn/MessageReceiver.h>

namespace lsf {

class ControllerClient;
/**
 * Manager base class for managers
 */
class Manager : public ajn::MessageReceiver {

  protected:
    /**
     * manager constructor
     */
    Manager(ControllerClient& controllerClient);
    /**
     * controller client reference
     */
    ControllerClient& controllerClient;

  private:

    Manager();
    Manager(const Manager& other);
    Manager& operator=(const Manager& other);
};

}

#endif