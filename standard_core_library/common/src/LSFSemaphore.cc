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

#include <LSFSemaphore.h>
#include <qcc/Debug.h>

#include <time.h>

using namespace lsf;

#define QCC_MODULE "LSF_SEMAPHORE"

LSFSemaphore::LSFSemaphore()
{
    QCC_DbgPrintf(("%s", __func__));
    sem_init(&mutex, 0, 0);
}

LSFSemaphore::~LSFSemaphore()
{
    QCC_DbgPrintf(("%s", __func__));
    sem_destroy(&mutex);
}

void LSFSemaphore::Wait(void)
{
    QCC_DbgPrintf(("%s", __func__));
    sem_wait(&mutex);
}

void LSFSemaphore::Post(void)
{
    QCC_DbgPrintf(("%s", __func__));
    sem_post(&mutex);
}