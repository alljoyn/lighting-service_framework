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

#include "NUtil.h"
#include "XObject.h"
#include "XScene.h"
#include "XStateTransitionEffect.h"
#include "XPresetTransitionEffect.h"
#include "XStatePulseEffect.h"
#include "XPresetPulseEffect.h"

#include "org_allseen_lsf_Scene.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;
using namespace lsf;

JNIEXPORT
void JNICALL Java_org_allseen_lsf_Scene_setStateTransitionEffects(JNIEnv *env, jobject thiz, jobjectArray jEffects)
{
    XScene *xDelegate = GetHandle<XScene*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XObject::CopyArray<TransitionLampsLampGroupsToState, XStateTransitionEffect>(XClass::xStateTransitionEffect, jEffects, xDelegate->transitionToStateComponent);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_Scene_getStateTransitionEffects(JNIEnv *env, jobject thiz)
{
    XScene *xDelegate = GetHandle<XScene*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jobjectArray jEffects = XObject::NewArray<TransitionLampsLampGroupsToState, XStateTransitionEffect>(XClass::xStateTransitionEffect, xDelegate->transitionToStateComponent);
    if (env->ExceptionCheck() || !jEffects) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return jEffects;
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_Scene_setPresetTransitionEffects(JNIEnv *env, jobject thiz, jobjectArray jEffects)
{
    XScene *xDelegate = GetHandle<XScene*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XObject::CopyArray<TransitionLampsLampGroupsToPreset, XPresetTransitionEffect>(XClass::xPresetTransitionEffect, jEffects, xDelegate->transitionToPresetComponent);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_Scene_getPresetTransitionEffects(JNIEnv *env, jobject thiz)
{
    XScene *xDelegate = GetHandle<XScene*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jobjectArray jEffects = XObject::NewArray<TransitionLampsLampGroupsToPreset, XPresetTransitionEffect>(XClass::xPresetTransitionEffect, xDelegate->transitionToPresetComponent);
    if (env->ExceptionCheck() || !jEffects) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return jEffects;
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_Scene_setStatePulseEffects(JNIEnv *env, jobject thiz, jobjectArray jEffects)
{
    XScene *xDelegate = GetHandle<XScene*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XObject::CopyArray<PulseLampsLampGroupsWithState, XStatePulseEffect>(XClass::xStatePulseEffect, jEffects, xDelegate->pulseWithStateComponent);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_Scene_getStatePulseEffects(JNIEnv *env, jobject thiz)
{
    XScene *xDelegate = GetHandle<XScene*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jobjectArray jEffects = XObject::NewArray<PulseLampsLampGroupsWithState, XStatePulseEffect>(XClass::xStatePulseEffect, xDelegate->pulseWithStateComponent);
    if (env->ExceptionCheck() || !jEffects) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return jEffects;
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_Scene_setPresetPulseEffects(JNIEnv *env, jobject thiz, jobjectArray jEffects)
{
    XScene *xDelegate = GetHandle<XScene*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XObject::CopyArray<PulseLampsLampGroupsWithPreset, XPresetPulseEffect>(XClass::xPresetPulseEffect, jEffects, xDelegate->pulseWithPresetComponent);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_Scene_getPresetPulseEffects(JNIEnv *env, jobject thiz)
{
    XScene *xDelegate = GetHandle<XScene*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jobjectArray jEffects = XObject::NewArray<PulseLampsLampGroupsWithPreset, XPresetPulseEffect>(XClass::xPresetPulseEffect, xDelegate->pulseWithPresetComponent);
    if (env->ExceptionCheck() || !jEffects) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return jEffects;
}

JNIEXPORT
jstring JNICALL Java_org_allseen_lsf_Scene_toString(JNIEnv *env, jobject thiz)
{
    return XObject::ToString<XScene>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_Scene_createNativeObject(JNIEnv *env, jobject thiz)
{
    CreateHandle<XScene>(thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_Scene_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<XScene>(thiz);
}

