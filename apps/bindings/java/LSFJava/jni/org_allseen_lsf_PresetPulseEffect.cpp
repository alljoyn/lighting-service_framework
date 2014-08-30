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
#include "XPresetPulseEffect.h"
#include "XPulseEffect.h"

#include "org_allseen_lsf_PresetPulseEffect.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;
using namespace lsf;

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetPulseEffect_setLamps(JNIEnv *env, jobject thiz, jobjectArray jLampIDs)
{
    XLampMemberList::SetLamps<XPresetPulseEffect>(env, thiz, jLampIDs);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_PresetPulseEffect_getLamps(JNIEnv *env, jobject thiz)
{
    return XLampMemberList::GetLamps<XPresetPulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetPulseEffect_setLampGroups(JNIEnv *env, jobject thiz, jobjectArray jLampGroupIDs)
{
    XLampMemberList::SetLampGroups<XPresetPulseEffect>(env, thiz, jLampGroupIDs);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_PresetPulseEffect_getLampGroups(JNIEnv *env, jobject thiz)
{
    return XLampMemberList::GetLampGroups<XPresetPulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetPulseEffect_setPulsePeriod(JNIEnv *env, jobject thiz, jlong jPulsePeriod)
{
    XPulseEffect::SetPulsePeriod<XPresetPulseEffect>(env, thiz, jPulsePeriod);
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_PresetPulseEffect_getPulsePeriod(JNIEnv *env, jobject thiz)
{
    return XPulseEffect::GetPulsePeriod<XPresetPulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetPulseEffect_setPulseDuration(JNIEnv *env, jobject thiz, jlong jPulseDuration)
{
    XPulseEffect::SetPulseDuration<XPresetPulseEffect>(env, thiz, jPulseDuration);
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_PresetPulseEffect_getPulseDuration(JNIEnv *env, jobject thiz)
{
    return XPulseEffect::GetPulseDuration<XPresetPulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetPulseEffect_setPulseCount(JNIEnv *env, jobject thiz, jlong jPulseCount)
{
    XPulseEffect::SetPulseCount<XPresetPulseEffect>(env, thiz, jPulseCount);
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_PresetPulseEffect_getPulseCount(JNIEnv *env, jobject thiz)
{
    return XPulseEffect::GetPulseCount<XPresetPulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetPulseEffect_setFromPreset(JNIEnv *env, jobject thiz, jstring jPresetID)
{
    XPresetPulseEffect *xPresetPulseEffect = GetHandle<XPresetPulseEffect*>(thiz);
    if (env->ExceptionCheck() || !xPresetPulseEffect) {
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

    xPresetPulseEffect->fromPreset = xPresetID.c_str();
}

JNIEXPORT
jstring JNICALL Java_org_allseen_lsf_PresetPulseEffect_getFromPreset(JNIEnv *env, jobject thiz)
{
    XPresetPulseEffect *xPresetPulseEffect = GetHandle<XPresetPulseEffect*>(thiz);
    if (env->ExceptionCheck() || !xPresetPulseEffect) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jstring jPresetID = env->NewStringUTF(xPresetPulseEffect->fromPreset.c_str());
    if (env->ExceptionCheck() || !jPresetID) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return NULL;
    }

    return jPresetID;
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetPulseEffect_setToPreset(JNIEnv *env, jobject thiz, jstring jPresetID)
{
    XPresetPulseEffect *xPresetPulseEffect = GetHandle<XPresetPulseEffect*>(thiz);
    if (env->ExceptionCheck() || !xPresetPulseEffect) {
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

    xPresetPulseEffect->toPreset = xPresetID.c_str();
}

JNIEXPORT
jstring JNICALL Java_org_allseen_lsf_PresetPulseEffect_getToPreset(JNIEnv *env, jobject thiz)
{
    XPresetPulseEffect *xPresetPulseEffect = GetHandle<XPresetPulseEffect*>(thiz);
    if (env->ExceptionCheck() || !xPresetPulseEffect) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jstring jPresetID = env->NewStringUTF(xPresetPulseEffect->toPreset.c_str());
    if (env->ExceptionCheck() || !jPresetID) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return NULL;
    }

    return jPresetID;
}

JNIEXPORT
jstring JNICALL Java_org_allseen_lsf_PresetPulseEffect_toString(JNIEnv *env, jobject thiz)
{
    return XObject::ToString<XPresetPulseEffect>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetPulseEffect_createNativeObject(JNIEnv *env, jobject thiz)
{
    CreateHandle<XPresetPulseEffect>(thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_PresetPulseEffect_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<XPresetPulseEffect>(thiz);
}
