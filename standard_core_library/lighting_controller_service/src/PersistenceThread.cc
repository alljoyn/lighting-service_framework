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
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include <qcc/Debug.h>

#ifdef LSF_BINDINGS
#include <lsf/controllerservice/PersistenceThread.h>
#include <lsf/controllerservice/ControllerService.h>
#else
#include <PersistenceThread.h>
#include <ControllerService.h>
#endif

using namespace lsf;

#ifdef LSF_BINDINGS
using namespace controllerservice;
#endif

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
            service.GetSceneElementManager().ReadWriteFile();
            service.GetSceneManager().ReadWriteFile();
            service.GetTransitionEffectManager().ReadWriteFile();
            service.GetPulseEffectManager().ReadWriteFile();
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
