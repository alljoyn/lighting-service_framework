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

#ifdef LSF_BINDINGS
#include <lsf/controllerservice/Condition.h>
#else
#include <Condition.h>
#endif

#include <Mutex.h>

using namespace lsf;

#ifdef LSF_BINDINGS
using namespace controllerservice;
#endif

Condition::Condition()
{
    pthread_cond_init(&condition, NULL);
}

Condition::~Condition()
{
    pthread_cond_destroy(&condition);
}

void Condition::Signal()
{
    pthread_cond_signal(&condition);
}

void Condition::Broadcast()
{
    pthread_cond_broadcast(&condition);
}

void Condition::Wait(Mutex& mutex)
{
    pthread_cond_wait(&condition, mutex.GetMutex());
}