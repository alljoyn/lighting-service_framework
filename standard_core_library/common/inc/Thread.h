#ifndef _THREAD_H_
#define _THREAD_H_
/**
 * \ingroup Common
 */
/**
 * \file  common/inc/Thread.h
 * This file provides definitions for thread infrastructure
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

#include <alljoyn/Status.h>

namespace lsf {
/**
 * abstraction class to thread
 */
class Thread {
  public:
    /**
     * class constructor
     */
    Thread();
    /**
     * class destructor
     */
    virtual ~Thread();
    /**
     * This method is been called by the created thread after start()
     */
    virtual void Run() = 0;
    /**
     * Stop running thread.  \n
     * Can be implemented by changing a variable value that is shared with the run() method.  \n
     * This will release the thread.  \n
     */
    virtual void Stop() = 0;
    /**
     * Create and run new thread
     */
    QStatus Start();
    /**
     * Join running thread until it exits
     */
    void Join();

  private:

    static void* RunThread(void* data);

    pthread_t thread;
};

}


#endif