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
#include <qcc/Log.h>

#include "JEnum.h"
#include "JLampDetails.h"

#include "NUtil.h"

#include "org_allseen_lsf_LampDetails.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace lsf;

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampDetails_getMake(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    LampMake cLampMake = xLampDetails->make;

    return JEnum::jLampMakeEnum->getObject((int)cLampMake);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampDetails_getModel(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    LampModel cLampModel = xLampDetails->model;

    return JEnum::jLampModelEnum->getObject((int)cLampModel);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampDetails_getType(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    DeviceType cDeviceType = xLampDetails->type;

    return JEnum::jDeviceTypeEnum->getObject((int)cDeviceType);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampDetails_getLampType(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    LampType cLampType = xLampDetails->lampType;

    return JEnum::jLampTypeEnum->getObject((int)cLampType);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampDetails_getLampBaseType(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    BaseType cBaseType = xLampDetails->lampBaseType;

    return JEnum::jBaseTypeEnum->getObject((int)cBaseType);
}

JNIEXPORT
jint JNICALL Java_org_allseen_lsf_LampDetails_getLampBeamAngle(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jint)xLampDetails->lampBeamAngle;
}

JNIEXPORT
jboolean JNICALL Java_org_allseen_lsf_LampDetails_isDimmable(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jboolean)xLampDetails->dimmable;
}

JNIEXPORT
jboolean JNICALL Java_org_allseen_lsf_LampDetails_hasColor(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jboolean)xLampDetails->color;
}

JNIEXPORT
jboolean JNICALL Java_org_allseen_lsf_LampDetails_hasVariableColorTemp(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jboolean)xLampDetails->variableColorTemp;
}

JNIEXPORT
jboolean JNICALL Java_org_allseen_lsf_LampDetails_hasEffects(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jboolean)xLampDetails->hasEffects;
}

JNIEXPORT
jint JNICALL Java_org_allseen_lsf_LampDetails_getMinVoltage(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jint)xLampDetails->minVoltage;
}

JNIEXPORT
jint JNICALL Java_org_allseen_lsf_LampDetails_getMaxVoltage(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jint)xLampDetails->maxVoltage;
}

JNIEXPORT
jint JNICALL Java_org_allseen_lsf_LampDetails_getWattage(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jint)xLampDetails->wattage;
}

JNIEXPORT
jint JNICALL Java_org_allseen_lsf_LampDetails_getIncandescentEquivalent(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jint)xLampDetails->incandescentEquivalent;
}

JNIEXPORT
jint JNICALL Java_org_allseen_lsf_LampDetails_getMaxLumens(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jint)xLampDetails->maxLumens;
}

JNIEXPORT
jint JNICALL Java_org_allseen_lsf_LampDetails_getMinTemperature(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jint)xLampDetails->minTemperature;
}

JNIEXPORT
jint JNICALL Java_org_allseen_lsf_LampDetails_getMaxTemperature(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jint)xLampDetails->maxTemperature;
}

JNIEXPORT
jint JNICALL Java_org_allseen_lsf_LampDetails_getColorRenderingIndex(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jint)xLampDetails->colorRenderingIndex;
}

JNIEXPORT
jstring JNICALL Java_org_allseen_lsf_LampDetails_getLampID(JNIEnv *env, jobject thiz)
{
    JLampDetails* xLampDetails = GetHandle<JLampDetails*>(thiz);
    if (env->ExceptionCheck() || !xLampDetails) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jstring jLampID = env->NewStringUTF(xLampDetails->lampID.c_str());
    if (env->ExceptionCheck() || !jLampID) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return NULL;
    }

    return jLampID;
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampDetails_createNativeObject(JNIEnv*env, jobject thiz)
{
    CreateHandle<JLampDetails>(thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampDetails_destroyNativeObject(JNIEnv*env, jobject thiz)
{
    DestroyHandle<JLampDetails>(thiz);
}
