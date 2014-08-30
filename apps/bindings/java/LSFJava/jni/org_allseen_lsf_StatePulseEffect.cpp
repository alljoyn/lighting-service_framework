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
#include "XPulseEffect.h"
#include "XStatePulseEffect.h"

#include "org_allseen_lsf_StatePulseEffect.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace lsf;

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StatePulseEffect_setLamps(JNIEnv *env, jobject thiz, jobjectArray jLampIDs)
{
    XLampMemberList::SetLamps<XStatePulseEffect>(env, thiz, jLampIDs);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_StatePulseEffect_getLamps(JNIEnv *env, jobject thiz)
{
    return XLampMemberList::GetLamps<XStatePulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StatePulseEffect_setLampGroups(JNIEnv *env, jobject thiz, jobjectArray jLampGroupIDs)
{
    XLampMemberList::SetLampGroups<XStatePulseEffect>(env, thiz, jLampGroupIDs);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_StatePulseEffect_getLampGroups(JNIEnv *env, jobject thiz)
{
    return XLampMemberList::GetLampGroups<XStatePulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StatePulseEffect_setPulsePeriod(JNIEnv *env, jobject thiz, jlong jPulsePeriod)
{
    XPulseEffect::SetPulsePeriod<XStatePulseEffect>(env, thiz, jPulsePeriod);
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_StatePulseEffect_getPulsePeriod(JNIEnv *env, jobject thiz)
{
    return XPulseEffect::GetPulsePeriod<XStatePulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StatePulseEffect_setPulseDuration(JNIEnv *env, jobject thiz, jlong jPulseDuration)
{
    XPulseEffect::SetPulseDuration<XStatePulseEffect>(env, thiz, jPulseDuration);
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_StatePulseEffect_getPulseDuration(JNIEnv *env, jobject thiz)
{
    return XPulseEffect::GetPulseDuration<XStatePulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StatePulseEffect_setPulseCount(JNIEnv *env, jobject thiz, jlong jPulseCount)
{
    XPulseEffect::SetPulseCount<XStatePulseEffect>(env, thiz, jPulseCount);
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_StatePulseEffect_getPulseCount(JNIEnv *env, jobject thiz)
{
    return XPulseEffect::GetPulseCount<XStatePulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StatePulseEffect_setFromLampState(JNIEnv *env, jobject thiz, jobject jLampState)
{
    XStatePulseEffect *xStatePulseEffect = GetHandle<XStatePulseEffect*>(thiz);
    if (env->ExceptionCheck() || !xStatePulseEffect) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    JLampState::CopyLampState(jLampState, xStatePulseEffect->fromState);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_StatePulseEffect_getFromLampState(JNIEnv *env, jobject thiz)
{
    XStatePulseEffect *xStatePulseEffect = GetHandle<XStatePulseEffect*>(thiz);
    if (env->ExceptionCheck() || !xStatePulseEffect) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return JLampState::NewLampState(xStatePulseEffect->fromState);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StatePulseEffect_setToLampState(JNIEnv *env, jobject thiz, jobject jLampState)
{
    XStatePulseEffect *xStatePulseEffect = GetHandle<XStatePulseEffect*>(thiz);
    if (env->ExceptionCheck() || !xStatePulseEffect) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    JLampState::CopyLampState(jLampState, xStatePulseEffect->toState);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_StatePulseEffect_getToLampState(JNIEnv *env, jobject thiz)
{
    XStatePulseEffect *xStatePulseEffect = GetHandle<XStatePulseEffect*>(thiz);
    if (env->ExceptionCheck() || !xStatePulseEffect) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return JLampState::NewLampState(xStatePulseEffect->toState);
}

JNIEXPORT
jstring JNICALL Java_org_allseen_lsf_StatePulseEffect_toString(JNIEnv *env, jobject thiz)
{
    return XObject::ToString<XStatePulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StatePulseEffect_createNativeObject(JNIEnv *env, jobject thiz)
{
    CreateHandle<XStatePulseEffect>(thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_StatePulseEffect_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<XStatePulseEffect>(thiz);
}

