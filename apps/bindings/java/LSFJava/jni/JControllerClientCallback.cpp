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

#include "JEnumArray.h"
#include "JControllerClientCallback.h"
#include "JLampState.h"
#include "XJavaDelegator.h"

#define QCC_MODULE LSF_QCC_MODULE

using namespace ajn;

namespace lsf {

JControllerClientCallback::JControllerClientCallback(jobject jobj)
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

JControllerClientCallback::~JControllerClientCallback()
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    env->DeleteWeakGlobalRef(jdelegate);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("DeleteWeakGlobalRef() failed"));
        return;
    }
}

void JControllerClientCallback::ConnectedToControllerServiceCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName)
{
    XJavaDelegator::Call_Void_String_String(jdelegate, __func__, controllerServiceDeviceID, controllerServiceName);
}

void JControllerClientCallback::DisconnectedFromControllerServiceCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName)
{
    XJavaDelegator::Call_Void_String_String(jdelegate, __func__, controllerServiceDeviceID, controllerServiceName);
}

void JControllerClientCallback::ConnectToControllerServiceFailedCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName)
{
    XJavaDelegator::Call_Void_String_String(jdelegate, __func__, controllerServiceDeviceID, controllerServiceName);
}

void JControllerClientCallback::ControllerClientErrorCB(const ErrorCodeList& errorCodeList)
{
    //TODO-FIX XJavaDelegator::Call_Void_EnumList(jdelegate, __func__, JEnumArray::NewEnumArray(*JEnum::jErrorCodeEnum, errorCodeList), "org/allseen/lsf/ErrorCode");
}

} /* namespace lsf */

#undef QCC_MODULE
