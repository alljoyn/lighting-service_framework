#ifndef _MUTEX_H_
#define _MUTEX_H_
/**
 * \ingroup Common
 */
/**
 * \file  common/inc/Mutex.h
 * This file provides definitions for mutex
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
/**
 * \ingroup Common
 */
#include <pthread.h>
#include <errno.h>
#include <alljoyn/Status.h>

namespace lsf {
/**
 * a wrapper class to mutex. \n
 * Use a mutex when thread wants to execute code that should not be executed by any other thread at the same time. \n
 * Mutex 'down' happens in one thread and mutex 'up' must happen in the same thread later on. \n
 */
class Mutex {
  public:
    /**
     * Mutex constructor
     */
    Mutex();
    /**
     * Mutex destructor
     */
    ~Mutex();
    /**
     * Lock mutex
     */
    QStatus Lock();
    /**
     * Unlock mutex
     */
    QStatus Unlock();
    /**
     * Get mutex object
     */
    pthread_mutex_t* GetMutex() { return &mutex; }

  private:

    pthread_mutex_t mutex;

};

}

#endif