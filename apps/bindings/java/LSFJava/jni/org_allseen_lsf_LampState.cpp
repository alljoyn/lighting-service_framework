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

#include <vector>

#include <qcc/Debug.h>
#include <qcc/Log.h>

#include <alljoyn/MsgArg.h>

#include "NUtil.h"
#include "JLampState.h"

#include "org_allseen_lsf_LampState.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace lsf;

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampState_setOnOff(JNIEnv *env, jobject thiz, jboolean jOnOff)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    xLampState->onOff = (bool)jOnOff;
    xLampState->nullState = false;
}

JNIEXPORT
jboolean JNICALL Java_org_allseen_lsf_LampState_getOnOff(JNIEnv *env, jobject thiz)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jboolean)xLampState->onOff;
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampState_setHue(JNIEnv *env, jobject thiz, jlong jHue)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    xLampState->hue = (uint32_t)jHue;
    xLampState->nullState = false;
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_LampState_getHue(JNIEnv *env, jobject thiz)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jlong)xLampState->hue;
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampState_setSaturation(JNIEnv *env, jobject thiz, jlong jSaturation)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    xLampState->saturation = (uint32_t)jSaturation;
    xLampState->nullState = false;
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_LampState_getSaturation(JNIEnv *env, jobject thiz)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jlong)xLampState->saturation;
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampState_setColorTemp(JNIEnv *env, jobject thiz, jlong jColorTemp)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    xLampState->colorTemp = (uint32_t)jColorTemp;
    xLampState->nullState = false;
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_LampState_getColorTemp(JNIEnv *env, jobject thiz)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jlong)xLampState->colorTemp;
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampState_setBrightness(JNIEnv *env, jobject thiz, jlong jBrightness)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    xLampState->brightness = (uint32_t)jBrightness;
    xLampState->nullState = false;
}

JNIEXPORT
jlong JNICALL Java_org_allseen_lsf_LampState_getBrightness(JNIEnv *env, jobject thiz)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jlong)xLampState->brightness;
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampState_setNull(JNIEnv *env, jobject thiz, jboolean jIsNull)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    xLampState->nullState = (bool)jIsNull;
}

JNIEXPORT
jboolean JNICALL Java_org_allseen_lsf_LampState_isNull(JNIEnv *env, jobject thiz)
{
    JLampState* xLampState = GetHandle<JLampState*>(thiz);
    if (env->ExceptionCheck() || !xLampState) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return 0;
    }

    return (jboolean)xLampState->nullState;
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampState_createNativeObject(JNIEnv *env, jobject thiz)
{
    CreateHandle<JLampState>(thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampState_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<JLampState>(thiz);
}
