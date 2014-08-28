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
#include "JLampGroup.h"
#include "XJavaDelegator.h"

#include "JLampGroupManagerCallback.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;

namespace lsf {

JLampGroupManagerCallback::JLampGroupManagerCallback(jobject jobj)
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

JLampGroupManagerCallback::~JLampGroupManagerCallback()
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    env->DeleteWeakGlobalRef(jdelegate);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("DeleteWeakGlobalRef() failed"));
        return;
    }
}

void JLampGroupManagerCallback::GetAllLampGroupIDsReplyCB(const LSFResponseCode& responseCode, const LampGroupIDList& lampGroupIDs)
{
    XJavaDelegator::Call_Void_ResponseCode_StringList(jdelegate, __func__, responseCode, lampGroupIDs);
}

void JLampGroupManagerCallback::GetLampGroupNameReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID, const LSFString& language, const LampGroupName& lampGroupName)
{
    XJavaDelegator::Call_Void_ResponseCode_String_String_String(jdelegate, __func__, responseCode, lampGroupID, language, lampGroupName);
}

void JLampGroupManagerCallback::SetLampGroupNameReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID, const LSFString& language)
{
    XJavaDelegator::Call_Void_ResponseCode_String_String(jdelegate, __func__, responseCode, lampGroupID, language);
}

void JLampGroupManagerCallback::LampGroupsNameChangedCB(const LampGroupIDList& lampGroupIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, lampGroupIDs);
}

void JLampGroupManagerCallback::CreateLampGroupReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::LampGroupsCreatedCB(const LampGroupIDList& lampGroupIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, lampGroupIDs);
}

void JLampGroupManagerCallback::GetLampGroupReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID, const LampGroup& lampGroup)
{
    XJavaDelegator::Call_Void_ResponseCode_String_Object<LampGroup, JLampGroup>(jdelegate, __func__, XClass::xLampGroup, responseCode, lampGroupID, lampGroup);
}

void JLampGroupManagerCallback::DeleteLampGroupReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::LampGroupsDeletedCB(const LampGroupIDList& lampGroupIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, lampGroupIDs);
}

void JLampGroupManagerCallback::TransitionLampGroupStateReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::PulseLampGroupWithStateReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::PulseLampGroupWithPresetReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::TransitionLampGroupStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::TransitionLampGroupStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::TransitionLampGroupStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::TransitionLampGroupStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::TransitionLampGroupStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::ResetLampGroupStateReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::ResetLampGroupStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::ResetLampGroupStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::ResetLampGroupStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::ResetLampGroupStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::ResetLampGroupStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::UpdateLampGroupReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

void JLampGroupManagerCallback::LampGroupsUpdatedCB(const LampGroupIDList& lampGroupIDs)
{
    XJavaDelegator::Call_Void_StringList(jdelegate, __func__, lampGroupIDs);
}

void JLampGroupManagerCallback::TransitionLampGroupStateToPresetReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID)
{
    XJavaDelegator::Call_Void_ResponseCode_String(jdelegate, __func__, responseCode, lampGroupID);
}

} /* namespace lsf */
