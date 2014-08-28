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
#include "JLampState.h"
#include "XJavaDelegator.h"
#include "XPresetManagerCallback.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;

namespace lsf {

XPresetManagerCallback::XPresetManagerCallback(jobject jobj)
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

XPresetManagerCallback::~XPresetManagerCallback()
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    env->DeleteWeakGlobalRef(jdelegate);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("DeleteWeakGlobalRef() failed"));
        return;
    }
}

void XPresetManagerCallback::GetPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID, const LampState& preset)
{
    XJavaDelegator::Call_Void_ResponseCode_String_Object<LampState, JLampState>(jdelegate, __func__, XClass::xLampState, responseCode, presetID, preset);
}

void XPresetManagerCallback::GetAllPresetIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& presetIDs)
{
    XJavaDelegator::Call_Void_ResponseCode_StringList(jdelegate, __func__, responseCode, presetIDs);
}

void XPresetManagerCallback::GetPresetNameReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID, const LSFString& language, const LSFString& presetName)
{
    XJavaDelegator::Call_Void_ResponseCode_String_String_String(jdelegate, __func__, responseCode, presetID, language, presetName);
}

void XPresetManagerCallback::SetPresetNameReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID, const LSFString& language)
{
    XJavaDelegator::Call_Void_ResponseCode_String_String(jdelegate, __func__, responseCode, presetID, language);
}

void XPresetManagerCallback::PresetsNameChangedCB(const LSFStringList& presetIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, presetIDs);
}

void XPresetManagerCallback::CreatePresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, presetID);
}

void XPresetManagerCallback::PresetsCreatedCB(const LSFStringList& presetIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, presetIDs);
}

void XPresetManagerCallback::UpdatePresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, presetID);
}

void XPresetManagerCallback::PresetsUpdatedCB(const LSFStringList& presetIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, presetIDs);
}

void XPresetManagerCallback::DeletePresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, presetID);
}

void XPresetManagerCallback::PresetsDeletedCB(const LSFStringList& presetIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, presetIDs);
}

} /* namespace lsf */
