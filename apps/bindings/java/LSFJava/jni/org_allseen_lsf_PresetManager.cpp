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

#include "JControllerClient.h"
#include "JLampState.h"
#include "NUtil.h"
#include "XCppDelegator.h"
#include "XPresetManager.h"
#include "XPresetManagerCallback.h"

#include "org_allseen_lsf_PresetManager.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;
using namespace lsf;

JNIEXPORT jobject JNICALL Java_org_allseen_lsf_PresetManager_getAllPresetIDs(JNIEnv *env, jobject thiz)
{
    return XCppDelegator::Call_ControllerClientStatus<XPresetManager>(env, thiz, &XPresetManager::GetAllPresetIDs);
}

JNIEXPORT jobject JNICALL Java_org_allseen_lsf_PresetManager_getPreset(JNIEnv *env, jobject thiz, jstring jPresetID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<XPresetManager>(env, thiz, jPresetID, &XPresetManager::GetPreset);
}

JNIEXPORT jobject JNICALL Java_org_allseen_lsf_PresetManager_getPresetName(JNIEnv *env, jobject thiz, jstring jPresetID, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_String_String<XPresetManager>(env, thiz, jPresetID, jLanguage, &XPresetManager::GetPresetName);
}

JNIEXPORT jobject JNICALL Java_org_allseen_lsf_PresetManager_setPresetName(JNIEnv *env, jobject thiz, jstring jPresetID, jstring jPresetName, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_String_String_String<XPresetManager>(env, thiz, jPresetID, jPresetName, jLanguage, &XPresetManager::SetPresetName);
}

JNIEXPORT jobject JNICALL Java_org_allseen_lsf_PresetManager_createPreset(JNIEnv *env, jobject thiz, jobject jPreset, jstring jPresetName, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_Object_String_String<XPresetManager, JLampState, LampState>(env, thiz, jPreset, jPresetName, jLanguage, &XPresetManager::CreatePreset);
}

JNIEXPORT jobject JNICALL Java_org_allseen_lsf_PresetManager_updatePreset(JNIEnv *env, jobject thiz, jstring jPresetID, jobject jPreset)
{
    return XCppDelegator::Call_ControllerClientStatus_String_Object<XPresetManager, JLampState, LampState>(env, thiz, jPresetID, jPreset, &XPresetManager::UpdatePreset);
}

JNIEXPORT jobject JNICALL Java_org_allseen_lsf_PresetManager_deletePreset(JNIEnv *env, jobject thiz, jstring jPresetID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<XPresetManager>(env, thiz, jPresetID, &XPresetManager::DeletePreset);
}

JNIEXPORT jobject JNICALL Java_org_allseen_lsf_PresetManager_getDefaultLampState(JNIEnv *env, jobject thiz)
{
    return XCppDelegator::Call_ControllerClientStatus<XPresetManager>(env, thiz, &XPresetManager::GetDefaultLampState);
}

JNIEXPORT jobject JNICALL Java_org_allseen_lsf_PresetManager_setDefaultLampState(JNIEnv *env, jobject thiz, jobject jDefaultLampState)
{
    return XCppDelegator::Call_ControllerClientStatus_Object<XPresetManager, JLampState, LampState>(env, thiz, jDefaultLampState, &XPresetManager::SetDefaultLampState);
}

JNIEXPORT void JNICALL Java_org_allseen_lsf_PresetManager_createNativeObject(JNIEnv *env, jobject thiz, jobject jControllerClient, jobject jPresetManagerCallback)
{
    JControllerClient *xControllerClient = GetHandle<JControllerClient*>(jControllerClient);
    if (env->ExceptionCheck() || !xControllerClient) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XPresetManagerCallback *xPresetManagerCallback = GetHandle<XPresetManagerCallback*>(jPresetManagerCallback);
    if (env->ExceptionCheck() || !xPresetManagerCallback) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XPresetManager* xPresetManager = new XPresetManager(thiz, *xControllerClient, *xPresetManagerCallback);
    if (env->ExceptionCheck() || !xPresetManager) {
        QCC_LogError(ER_FAIL, ("JSavedStateManager() failed"));
        return;
    }

    CreateHandle<XPresetManager>(thiz, xPresetManager);
}

JNIEXPORT void JNICALL Java_org_allseen_lsf_PresetManager_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<XPresetManager>(thiz);
}
