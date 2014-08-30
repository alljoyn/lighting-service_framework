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

#include "XLampMemberList.h"
#include "XObject.h"
#include "XPresetTransitionEffect.h"
#include "XTransitionEffect.h"

#include "org_allseen_lsf_PresetTransitionEffect.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;
using namespace lsf;

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetTransitionEffect_setLamps(JNIEnv *env, jobject thiz, jobjectArray jLampIDs)
{
    XLampMemberList::SetLamps<XPresetTransitionEffect>(env, thiz, jLampIDs);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_PresetTransitionEffect_getLamps(JNIEnv *env, jobject thiz)
{
    return XLampMemberList::GetLamps<XPresetTransitionEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetTransitionEffect_setLampGroups(JNIEnv *env, jobject thiz, jobjectArray jLampGroupIDs)
{
    XLampMemberList::SetLampGroups<XPresetTransitionEffect>(env, thiz, jLampGroupIDs);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_PresetTransitionEffect_getLampGroups(JNIEnv *env, jobject thiz)
{
    return XLampMemberList::GetLampGroups<XPresetTransitionEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetTransitionEffect_setTransitionPeriod(JNIEnv *env, jobject thiz, jlong jTransitionPeriod)
{
    XTransitionEffect::SetTransitionPeriod<XPresetTransitionEffect>(env, thiz, jTransitionPeriod);
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_PresetTransitionEffect_getTransitionPeriod(JNIEnv *env, jobject thiz)
{
    return XTransitionEffect::GetTransitionPeriod<XPresetTransitionEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetTransitionEffect_setPresetID(JNIEnv *env, jobject thiz, jstring jPresetID)
{
    XPresetTransitionEffect *xPresetTransitionEffect = GetHandle<XPresetTransitionEffect*>(thiz);
    if (env->ExceptionCheck() || !xPresetTransitionEffect) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    JString xPresetID(jPresetID);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("JString failed"));
        return;
    }

    if (!xPresetID.c_str()) {
        QCC_LogError(ER_FAIL, ("JString invalid"));
        return;
    }

    xPresetTransitionEffect->presetID = xPresetID.c_str();
}

JNIEXPORT
jstring JNICALL Java_org_allseen_lsf_PresetTransitionEffect_getPresetID(JNIEnv *env, jobject thiz)
{
    XPresetTransitionEffect *xPresetTransitionEffect = GetHandle<XPresetTransitionEffect*>(thiz);
    if (env->ExceptionCheck() || !xPresetTransitionEffect) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jstring jPresetID = env->NewStringUTF(xPresetTransitionEffect->presetID.c_str());
    if (env->ExceptionCheck() || !jPresetID) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return NULL;
    }

    return jPresetID;
}

JNIEXPORT
jstring JNICALL Java_org_allseen_lsf_PresetTransitionEffect_toString(JNIEnv *env, jobject thiz)
{
    return XObject::ToString<XPresetTransitionEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetTransitionEffect_createNativeObject(JNIEnv *env, jobject thiz)
{
    CreateHandle<XPresetTransitionEffect>(thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetTransitionEffect_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<XPresetTransitionEffect>(thiz);
}

