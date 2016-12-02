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
#include <qcc/Debug.h>

using namespace lsf;

#define QCC_MODULE "LSF_THREAD"

void* Thread::RunThread(void* data)
{
    QCC_DbgPrintf(("%s", __func__));
    Thread* thread = static_cast<Thread*>(data);
    thread->Run();
    return NULL;
}

Thread::Thread()
{
    QCC_DbgPrintf(("%s", __func__));
}

Thread::~Thread()
{
    QCC_DbgPrintf(("%s", __func__));
}

QStatus Thread::Start()
{
    QCC_DbgPrintf(("%s", __func__));
    int ret = pthread_create(&thread, NULL, &Thread::RunThread, this);
    return ret == 0 ? ER_OK : ER_FAIL;
}

void Thread::Join()
{
    QCC_DbgPrintf(("%s", __func__));
    pthread_join(thread, NULL);
}
