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

#include "JLampState.h"
#include "NUtil.h"
#include "XLampMemberList.h"
#include "XObject.h"
#include "XStateTransitionEffect.h"
#include "XTransitionEffect.h"

#include "org_allseen_lsf_StateTransitionEffect.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;
using namespace lsf;

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StateTransitionEffect_setLamps(JNIEnv *env, jobject thiz, jobjectArray jLampIDs)
{
    XLampMemberList::SetLamps<XStateTransitionEffect>(env, thiz, jLampIDs);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_StateTransitionEffect_getLamps(JNIEnv *env, jobject thiz)
{
    return XLampMemberList::GetLamps<XStateTransitionEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StateTransitionEffect_setLampGroups(JNIEnv *env, jobject thiz, jobjectArray jLampGroupIDs)
{
    XLampMemberList::SetLampGroups<XStateTransitionEffect>(env, thiz, jLampGroupIDs);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_StateTransitionEffect_getLampGroups(JNIEnv *env, jobject thiz)
{
    return XLampMemberList::GetLampGroups<XStateTransitionEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StateTransitionEffect_setTransitionPeriod(JNIEnv *env, jobject thiz, jlong jTransitionPeriod)
{
    XTransitionEffect::SetTransitionPeriod<XStateTransitionEffect>(env, thiz, jTransitionPeriod);
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_StateTransitionEffect_getTransitionPeriod(JNIEnv *env, jobject thiz)
{
    return XTransitionEffect::GetTransitionPeriod<XStateTransitionEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StateTransitionEffect_setLampState(JNIEnv *env, jobject thiz, jobject jLampState)
{
    XStateTransitionEffect *xStateTransitionEffect = GetHandle<XStateTransitionEffect*>(thiz);
    if (env->ExceptionCheck() || !xStateTransitionEffect) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    JLampState::CopyLampState(jLampState, xStateTransitionEffect->state);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_StateTransitionEffect_getLampState(JNIEnv *env, jobject thiz)
{
    XStateTransitionEffect *xStateTransitionEffect = GetHandle<XStateTransitionEffect*>(thiz);
    if (env->ExceptionCheck() || !xStateTransitionEffect) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return JLampState::NewLampState(xStateTransitionEffect->state);
}

JNIEXPORT
jstring JNICALL Java_org_allseen_lsf_StateTransitionEffect_toString(JNIEnv *env, jobject thiz)
{
    return XObject::ToString<XStateTransitionEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StateTransitionEffect_createNativeObject(JNIEnv *env, jobject thiz)
{
    CreateHandle<XStateTransitionEffect>(thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StateTransitionEffect_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<XStateTransitionEffect>(thiz);
}
