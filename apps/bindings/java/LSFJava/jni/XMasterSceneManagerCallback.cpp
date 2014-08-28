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
#include "XMasterScene.h"
#include "XMasterSceneManagerCallback.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;

namespace lsf {

XMasterSceneManagerCallback::XMasterSceneManagerCallback(jobject jobj)
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

XMasterSceneManagerCallback::~XMasterSceneManagerCallback()
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    env->DeleteWeakGlobalRef(jdelegate);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("DeleteWeakGlobalRef() failed"));
        return;
    }
}

void XMasterSceneManagerCallback::GetAllMasterSceneIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& masterSceneIDs)
{
    XJavaDelegator::Call_Void_ResponseCode_StringList(jdelegate, __func__, responseCode, masterSceneIDs);
}

void XMasterSceneManagerCallback::GetMasterSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID, const LSFString& language, const LSFString& masterSceneName)
{
    XJavaDelegator::Call_Void_ResponseCode_String_String_String(jdelegate, __func__, responseCode, masterSceneID, language, masterSceneName);
}

void XMasterSceneManagerCallback::SetMasterSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID, const LSFString& language)
{
    XJavaDelegator::Call_Void_ResponseCode_String_String(jdelegate, __func__, responseCode, masterSceneID, language);
}

void XMasterSceneManagerCallback::MasterScenesNameChangedCB(const LSFStringList& masterSceneIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, masterSceneIDs);
}

void XMasterSceneManagerCallback::CreateMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, masterSceneID);
}

void XMasterSceneManagerCallback::MasterScenesCreatedCB(const LSFStringList& masterSceneIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, masterSceneIDs);
}

void XMasterSceneManagerCallback::GetMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID, const MasterScene& masterScene)
{
    XJavaDelegator::Call_Void_ResponseCode_String_Object<MasterScene, XMasterScene>(jdelegate, __func__, XClass::xMasterScene, responseCode, masterSceneID, masterScene);
}

void XMasterSceneManagerCallback::DeleteMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, masterSceneID);
}

void XMasterSceneManagerCallback::MasterScenesDeletedCB(const LSFStringList& masterSceneIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, masterSceneIDs);
}

void XMasterSceneManagerCallback::UpdateMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, masterSceneID);
}

void XMasterSceneManagerCallback::MasterScenesUpdatedCB(const LSFStringList& masterSceneIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, masterSceneIDs);
}

void XMasterSceneManagerCallback::ApplyMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, masterSceneID);
}

void XMasterSceneManagerCallback::MasterScenesAppliedCB(const LSFStringList& masterSceneIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, masterSceneIDs);
}

} /* namespace lsf */
