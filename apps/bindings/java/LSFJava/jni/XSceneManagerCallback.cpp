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

#include "JEnum.h"
#include "XJavaDelegator.h"
#include "XScene.h"
#include "XSceneManagerCallback.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;

namespace lsf {

XSceneManagerCallback::XSceneManagerCallback(jobject jobj)
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

XSceneManagerCallback::~XSceneManagerCallback()
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    env->DeleteWeakGlobalRef(jdelegate);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("DeleteWeakGlobalRef() failed"));
        return;
    }
}

void XSceneManagerCallback::GetAllSceneIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& sceneIDs)
{
    XJavaDelegator::Call_Void_ResponseCode_StringList(jdelegate, __func__, responseCode, sceneIDs);
}

void XSceneManagerCallback::GetSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const LSFString& language, const LSFString& sceneName)
{
    XJavaDelegator::Call_Void_ResponseCode_String_String_String(jdelegate, __func__, responseCode, sceneID, language, sceneName);
}

void XSceneManagerCallback::SetSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const LSFString& language)
{
    XJavaDelegator::Call_Void_ResponseCode_String_String(jdelegate, __func__, responseCode, sceneID, language);
}

void XSceneManagerCallback::ScenesNameChangedCB(const LSFStringList& sceneIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, sceneIDs);
}

void XSceneManagerCallback::CreateSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, sceneID);
}

void XSceneManagerCallback::ScenesCreatedCB(const LSFStringList& sceneIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, sceneIDs);
}

void XSceneManagerCallback::UpdateSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, sceneID);
}

void XSceneManagerCallback::ScenesUpdatedCB(const LSFStringList& sceneIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, sceneIDs);
}

void XSceneManagerCallback::DeleteSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, sceneID);
}

void XSceneManagerCallback::ScenesDeletedCB(const LSFStringList& sceneIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, sceneIDs);
}

void XSceneManagerCallback::GetSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const Scene& scene)
{
    XJavaDelegator::Call_Void_ResponseCode_String_Object<Scene, XScene>(jdelegate, __func__, XClass::xScene, responseCode, sceneID, scene);
}

void XSceneManagerCallback::ApplySceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, sceneID);
}

void XSceneManagerCallback::ScenesAppliedCB(const LSFStringList& sceneIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, sceneIDs);
}

} /* namespace lsf */
