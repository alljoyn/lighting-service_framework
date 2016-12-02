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

#include <qcc/Debug.h>
#include <PersistenceThread.h>
#include <ControllerService.h>

using namespace lsf;

#define QCC_MODULE "PERSISTED_THREAD"

PersistenceThread::PersistenceThread(ControllerService& service)
    : service(service),
    running(true)
{
    QCC_DbgTrace(("%s", __func__));
}

PersistenceThread::~PersistenceThread()
{
    QCC_DbgTrace(("%s", __func__));
}


void PersistenceThread::Run()
{
    QCC_DbgTrace(("%s", __func__));
    while (running) {
        // wait!
        semaphore.Wait();

        if (running) {
            service.GetLampGroupManager().ReadWriteFile();
            service.GetMasterSceneManager().ReadWriteFile();
            service.GetPresetManager().ReadWriteFile();
            service.GetSceneManager().ReadWriteFile();
            service.GetTransitionEffectManager().ReadWriteFile();
            service.GetPulseEffectManager().ReadWriteFile();
            service.GetUnknownBlobGroupManager().ReadWriteFile();
        }
    }
    QCC_DbgPrintf(("%s: Exited", __func__));
}

void PersistenceThread::SignalReadWrite()
{
    QCC_DbgTrace(("%s", __func__));
    // signal
    semaphore.Post();
}

void PersistenceThread::Stop()
{
    QCC_DbgTrace(("%s", __func__));
    running = false;
    SignalReadWrite();
}

void PersistenceThread::Join()
{
    QCC_DbgTrace(("%s", __func__));
    Thread::Join();
}
