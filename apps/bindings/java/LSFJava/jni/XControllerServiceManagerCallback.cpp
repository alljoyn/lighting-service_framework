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

#include "XJavaDelegator.h"
#include "XControllerServiceManagerCallback.h"

#define QCC_MODULE "AJN-LSF-JNI"

namespace lsf {

XControllerServiceManagerCallback::XControllerServiceManagerCallback(jobject jobj)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    /*
     * Be careful when using a weak global reference.  They can only be
     * passed to NewLocalRef, NewGlobalRef and DeleteWeakGlobalRef.
     */
    QCC_DbgPrintf(("Taking weak global reference to Java object %p", jobj));
    jdelegate = env->NewWeakGlobalRef(jobj);
    if (env->ExceptionCheck() || !jdelegate) {
        QCC_LogError(ER_FAIL, ("NewWeakGlobalRef() failed"));
        return;
    }
}

XControllerServiceManagerCallback::~XControllerServiceManagerCallback()
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    env->DeleteWeakGlobalRef(jdelegate);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("DeleteWeakGlobalRef() failed"));
        return;
    }
}

void XControllerServiceManagerCallback::GetControllerServiceVersionReplyCB(const uint32_t& version)
{
    XJavaDelegator::Call_Void_UInt32(jdelegate, __func__, version);
}

void XControllerServiceManagerCallback::LightingResetControllerServiceReplyCB(const LSFResponseCode& responseCode)
{
    XJavaDelegator::Call_Void_ResponseCode(jdelegate, __func__, responseCode);
}

void XControllerServiceManagerCallback::ControllerServiceLightingResetCB(void)
{
    XJavaDelegator::Call_Void(jdelegate, __func__);
}

void XControllerServiceManagerCallback::ControllerServiceNameChangedCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName)
{
    XJavaDelegator::Call_Void_String_String(jdelegate, __func__, controllerServiceDeviceID, controllerServiceName);
}

} /* namespace lsf */
