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
#include <qcc/Debug.h>

#include "JControllerClient.h"
#include "XCppDelegator.h"
#include "XControllerServiceManager.h"
#include "XControllerServiceManagerCallback.h"

#include "org_allseen_lsf_ControllerServiceManager.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace lsf;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_ControllerServiceManager_getControllerServiceVersion(JNIEnv *env, jobject thiz)
{
    return XCppDelegator::Call_ControllerClientStatus<XControllerServiceManager>(env, thiz, &XControllerServiceManager::GetControllerServiceVersion);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_ControllerServiceManager_lightingResetControllerService(JNIEnv *env, jobject thiz)
{
    return XCppDelegator::Call_ControllerClientStatus<XControllerServiceManager>(env, thiz, &XControllerServiceManager::LightingResetControllerService);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_ControllerServiceManager_createNativeObject(JNIEnv *env, jobject thiz, jobject jController, jobject jCallback)
{
    JControllerClient *jcc = GetHandle<JControllerClient*>(jController);
    if (!jcc) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XControllerServiceManagerCallback *xcsmcb = GetHandle<XControllerServiceManagerCallback*>(jCallback);
    if (!xcsmcb) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XControllerServiceManager* xcsm = new XControllerServiceManager(thiz, *jcc, *xcsmcb);
    if (!xcsm) {
        QCC_LogError(ER_FAIL, ("JLampGroupManager() failed"));
        return;
    }

    CreateHandle<XControllerServiceManager>(thiz, xcsm);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_ControllerServiceManager_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<XControllerServiceManager>(thiz);
}

#ifdef __cplusplus
}
#endif

