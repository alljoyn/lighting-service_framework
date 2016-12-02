#ifndef LSF_CONDITION_H
#define LSF_CONDITION_H
/**
 * \defgroup ControllerService
 * ControllerService code
 */
/**
 * \ingroup ControllerService
 */
/**
 * @file
 * This file provides definitions for the Condition class
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

#include <pthread.h>
#include <Mutex.h>
#include "LSFNamespaceSpecifier.h"

namespace lsf {

OPTIONAL_NAMESPACE_CONTROLLER_SERVICE

/**
 * abstraction of posix pthread_cond_t
 */
class Condition {
  public:
    /**
     * class constructor
     */
    Condition();
    /**
     * class destructor
     */
    ~Condition();
    /**
     * unblock at least one of the threads that are blocked on the specified condition variable cond (if any threads are blocked on cond).
     */
    void Signal();
    /**
     * unblock all threads currently blocked on the specified condition variable condition
     */
    void Broadcast();
    /**
     * wait on condition
     */
    void Wait(Mutex& mutex);

  private:

    pthread_cond_t condition;
};

OPTIONAL_NAMESPACE_CLOSE

} //lsf

#endif