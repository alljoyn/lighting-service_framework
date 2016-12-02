#ifndef _LSF_SEMAPHORE_H_
#define _LSF_SEMAPHORE_H_
/**
 * \ingroup Common
 */
/**
 * \file  common/inc/LSFSemaphore.h
 * This file provides definitions for LSF semaphore
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
#include <semaphore.h>
#include <stdint.h>
#include <Mutex.h>

namespace lsf {

/**
 * Class that implements a Semaphore \n
 * Use a semaphore when some thread wants to sleep until some other thread tells it to wake up
 */
class LSFSemaphore {
  public:

    /**
     * Constructor
     */
    LSFSemaphore();

    /**
     * Destrctor
     */
    ~LSFSemaphore();

    /**
     * Wait on a Semaphore
     */
    void Wait(void);

    /**
     * Post to a Semaphore
     */
    void Post(void);

  private:

    /**
     * Mutex associated with the Semaphore
     */
    sem_t mutex;
};


}

#endif