/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 ******************************************************************************/

#include <string.h>
#include <jni.h>

#include <qcc/Debug.h>

#include "JControllerClientCallback.h"
#include "JControllerClient.h"
#include "JEnum.h"
#include "NUtil.h"

#include "org_allseen_lsf_ControllerClient.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;
using namespace lsf;

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_ControllerClient_start(JNIEnv*env, jobject thiz)
{
    JControllerClient *xController = GetHandle<JControllerClient*>(thiz);
    if (env->ExceptionCheck() || !xController) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return JEnum::jControllerClientStatusEnum->getObject((int)xController->Start());
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_ControllerClient_stop(JNIEnv*env, jobject thiz)
{
    JControllerClient *xController = GetHandle<JControllerClient*>(thiz);
    if (env->ExceptionCheck() || !xController) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return JEnum::jControllerClientStatusEnum->getObject((int)xController->Stop());
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_ControllerClient_createNativeObject(JNIEnv *env, jobject thiz, jobject jbus, jobject jcallback)
{
    BusAttachment *xbus = GetHandle<BusAttachment*>(jbus, "handle");
    if (!xbus) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    JControllerClientCallback *xcallback = GetHandle<JControllerClientCallback*>(jcallback);
    if (!xcallback) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    JControllerClient *xcontroller = new JControllerClient(thiz, *xbus, *xcallback);
    if (!xcontroller) {
        QCC_LogError(ER_FAIL, ("JLampManager() failed"));
        return;
    }

    CreateHandle<JControllerClient>(thiz, xcontroller);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_ControllerClient_destroyNativeObject(JNIEnv*env, jobject thiz)
{
    DestroyHandle<JControllerClient>(thiz);
}

#undef QCC_MODULE
