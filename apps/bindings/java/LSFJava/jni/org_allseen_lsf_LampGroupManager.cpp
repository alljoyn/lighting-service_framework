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

#include "JLampGroup.h"
#include "JLampGroupManager.h"
#include "JLampState.h"
#include "XCppDelegator.h"

#include "org_allseen_lsf_LampGroupManager.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;
using namespace lsf;

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_getAllLampGroupIDs(JNIEnv *env, jobject thiz)
{
    return XCppDelegator::Call_ControllerClientStatus<JLampGroupManager>(env, thiz, &JLampGroupManager::GetAllLampGroupIDs);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_getLampGroupName(JNIEnv *env, jobject thiz, jstring jGroupID, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_String_String<JLampGroupManager>(env, thiz, jGroupID, jLanguage, &JLampGroupManager::GetLampGroupName);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_setLampGroupName(JNIEnv *env, jobject thiz, jstring jGroupID, jstring jGroupName, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_String_String_String<JLampGroupManager>(env, thiz, jGroupID, jGroupName, jLanguage, &JLampGroupManager::SetLampGroupName);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_createLampGroup(JNIEnv *env, jobject thiz, jobject jGroup, jstring jGroupName, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_Object_String_String<JLampGroupManager, JLampGroup, LampGroup>(env, thiz, jGroup, jGroupName, jLanguage, &JLampGroupManager::CreateLampGroup);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_updateLampGroup(JNIEnv *env, jobject thiz, jstring jGroupID, jobject jGroup)
{
    return XCppDelegator::Call_ControllerClientStatus_String_Object<JLampGroupManager, JLampGroup, LampGroup>(env, thiz, jGroupID, jGroup, &JLampGroupManager::UpdateLampGroup);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_getLampGroup(JNIEnv *env, jobject thiz, jstring jGroupID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<JLampGroupManager>(env, thiz, jGroupID, &JLampGroupManager::GetLampGroup);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_deleteLampGroup(JNIEnv *env, jobject thiz, jstring jGroupID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<JLampGroupManager>(env, thiz, jGroupID, &JLampGroupManager::DeleteLampGroup);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_transitionLampGroupState(JNIEnv *env, jobject thiz, jstring jGroupID, jobject jLampState, jlong jDuration)
{
    return XCppDelegator::Call_ControllerClientStatus_String_Object_UInt32<JLampGroupManager, JLampState, LampState>(env, thiz, jGroupID, jLampState, jDuration, &JLampGroupManager::TransitionLampGroupState);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_pulseLampGroupWithState(JNIEnv *env, jobject thiz, jstring jGroupID, jobject jToState, jlong jPeriod, jlong jDuration, jlong jCount, jobject jFromState)
{
    return XCppDelegator::Call_ControllerClientStatus_String_Object_UInt32_UInt32_UInt32_Object<JLampGroupManager, JLampState, LampState>(env, thiz, jGroupID, jToState, jPeriod, jDuration, jCount, jFromState, &JLampGroupManager::PulseLampGroupWithState);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_pulseLampGroupWithPreset(JNIEnv *env, jobject thiz, jstring jGroupID, jstring jToPresetID, jlong jPeriod, jlong jDuration, jlong jCount, jstring jFromPresetID)
{
    return XCppDelegator::Call_ControllerClientStatus_String_String_UInt32_UInt32_UInt32_String<JLampGroupManager>(env, thiz, jGroupID, jToPresetID, jPeriod, jDuration, jCount, jFromPresetID, &JLampGroupManager::PulseLampGroupWithPreset);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_transitionLampGroupStateOnOffField(JNIEnv *env, jobject thiz, jstring jGroupID, jboolean jOnOff)
{
    return XCppDelegator::Call_ControllerClientStatus_String_Boolean<JLampGroupManager>(env, thiz, jGroupID, jOnOff, &JLampGroupManager::TransitionLampGroupStateOnOffField);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_transitionLampGroupStateHueField(JNIEnv *env, jobject thiz, jstring jGroupID, jlong jHue, jlong jDuration)
{
    return XCppDelegator::Call_ControllerClientStatus_String_UInt32_UInt32<JLampGroupManager>(env, thiz, jGroupID, jHue, jDuration, &JLampGroupManager::TransitionLampGroupStateHueField);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_transitionLampGroupStateSaturationField(JNIEnv *env, jobject thiz, jstring jGroupID, jlong jSaturation, jlong jDuration)
{
    return XCppDelegator::Call_ControllerClientStatus_String_UInt32_UInt32<JLampGroupManager>(env, thiz, jGroupID, jSaturation, jDuration, &JLampGroupManager::TransitionLampGroupStateSaturationField);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_transitionLampGroupStateBrightnessField(JNIEnv *env, jobject thiz, jstring jGroupID, jlong jBrightness, jlong jDuration)
{
    return XCppDelegator::Call_ControllerClientStatus_String_UInt32_UInt32<JLampGroupManager>(env, thiz, jGroupID, jBrightness, jDuration, &JLampGroupManager::TransitionLampGroupStateBrightnessField);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_transitionLampGroupStateColorTempField(JNIEnv *env, jobject thiz, jstring jGroupID, jlong jColorTemp, jlong jDuration)
{
    return XCppDelegator::Call_ControllerClientStatus_String_UInt32_UInt32<JLampGroupManager>(env, thiz, jGroupID, jColorTemp, jDuration, &JLampGroupManager::TransitionLampGroupStateColorTempField);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampLampGroupManager_transitionLampGroupStateToPreset(JNIEnv *env, jobject thiz, jstring jGroupID, jstring jPresetID, jlong jDuration)
{
    return XCppDelegator::Call_ControllerClientStatus_String_String_UInt32<JLampGroupManager>(env, thiz, jGroupID, jPresetID, jDuration, &JLampGroupManager::TransitionLampGroupStateToPreset);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_resetLampGroupState(JNIEnv *env, jobject thiz, jstring jGroupID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<JLampGroupManager>(env, thiz, jGroupID, &JLampGroupManager::ResetLampGroupState);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_resetLampGroupStateOnOffField(JNIEnv *env, jobject thiz, jstring jGroupID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<JLampGroupManager>(env, thiz, jGroupID, &JLampGroupManager::ResetLampGroupStateOnOffField);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_resetLampGroupStateHueField(JNIEnv *env, jobject thiz, jstring jGroupID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<JLampGroupManager>(env, thiz, jGroupID, &JLampGroupManager::ResetLampGroupStateHueField);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_resetLampGroupStateSaturationField(JNIEnv *env, jobject thiz, jstring jGroupID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<JLampGroupManager>(env, thiz, jGroupID, &JLampGroupManager::ResetLampGroupStateSaturationField);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_resetLampGroupStateBrightnessField(JNIEnv *env, jobject thiz, jstring jGroupID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<JLampGroupManager>(env, thiz, jGroupID, &JLampGroupManager::ResetLampGroupStateBrightnessField);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroupManager_resetLampGroupStateColorTempField(JNIEnv *env, jobject thiz, jstring jGroupID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<JLampGroupManager>(env, thiz, jGroupID, &JLampGroupManager::ResetLampGroupStateColorTempField);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampGroupManager_createNativeObject(JNIEnv *env, jobject thiz, jobject controller, jobject callback)
{
    JControllerClient *jcc = GetHandle<JControllerClient*>(controller);
    if (!jcc) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    JLampGroupManagerCallback *jgmcb = GetHandle<JLampGroupManagerCallback*>(callback);
    if (!jgmcb) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    JLampGroupManager* jgm = new JLampGroupManager(thiz, *jcc, *jgmcb);
    if (!jgm) {
        QCC_LogError(ER_FAIL, ("JLampGroupManager() failed"));
        return;
    }

    CreateHandle<JLampGroupManager>(thiz, jgm);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampGroupManager_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<JLampGroupManager>(thiz);
}

