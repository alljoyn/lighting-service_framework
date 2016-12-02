#ifndef PERSISTENCE_THREAD_H
#define PERSISTENCE_THREAD_H
/**
 * \ingroup ControllerService
 */
/**
 * @file
 * This file provides definitions for persistent thread
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

#include <Thread.h>
#include <LSFSemaphore.h>
#include "LSFNamespaceSpecifier.h"

namespace lsf {

OPTIONAL_NAMESPACE_CONTROLLER_SERVICE

class ControllerService;
/**
 * Thread dedicated for persisted data
 */
class PersistenceThread : public Thread {
  public:
    /**
     * class constructor
     */
    PersistenceThread(ControllerService& service);
    /**
     * class destructor
     */
    virtual ~PersistenceThread();
    /**
     * Signal to the thread to be active
     */
    void SignalReadWrite();
    /**
     * Thread run method
     */
    virtual void Run();
    /**
     * Stop thread
     */
    virtual void Stop();
    /**
     * Join thread after stopped
     */
    virtual void Join();

  private:
    ControllerService& service;
    bool running;
    LSFSemaphore semaphore;
};

OPTIONAL_NAMESPACE_CLOSE

} //lsf


#endif