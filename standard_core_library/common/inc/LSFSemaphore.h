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
 *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
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