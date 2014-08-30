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
#include "JEnum.h"
#include "NUtil.h"
#include "XCppDelegator.h"
#include "XMasterScene.h"
#include "XMasterSceneManager.h"
#include "XMasterSceneManagerCallback.h"

#include "org_allseen_lsf_MasterSceneManager.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;
using namespace lsf;

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_MasterSceneManager_getAllMasterSceneIDs(JNIEnv *env, jobject thiz)
{
    return XCppDelegator::Call_ControllerClientStatus<XMasterSceneManager>(env, thiz, &XMasterSceneManager::GetAllMasterSceneIDs);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_MasterSceneManager_getMasterSceneName(JNIEnv *env, jobject thiz, jstring jMasterSceneID, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_String_String<XMasterSceneManager>(env, thiz, jMasterSceneID, jLanguage, &XMasterSceneManager::GetMasterSceneName);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_MasterSceneManager_setMasterSceneName(JNIEnv *env, jobject thiz, jstring jMasterSceneID, jstring jMasterSceneName, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_String_String_String<XMasterSceneManager>(env, thiz, jMasterSceneID, jMasterSceneName, jLanguage, &XMasterSceneManager::SetMasterSceneName);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_MasterSceneManager_createMasterScene(JNIEnv *env, jobject thiz, jobject jMasterScene, jstring jMasterSceneName, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_Object_String_String<XMasterSceneManager, XMasterScene, MasterScene>(env, thiz, jMasterScene, jMasterSceneName, jLanguage, &XMasterSceneManager::CreateMasterScene);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_MasterSceneManager_updateMasterScene(JNIEnv *env, jobject thiz, jstring jMasterSceneID, jobject jMasterScene)
{
    return XCppDelegator::Call_ControllerClientStatus_String_Object<XMasterSceneManager, XMasterScene, MasterScene>(env, thiz, jMasterSceneID, jMasterScene, &XMasterSceneManager::UpdateMasterScene);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_MasterSceneManager_getMasterScene(JNIEnv *env, jobject thiz, jstring jMasterSceneID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<XMasterSceneManager>(env, thiz, jMasterSceneID, &XMasterSceneManager::GetMasterScene);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_MasterSceneManager_deleteMasterScene(JNIEnv *env, jobject thiz, jstring jMasterSceneID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<XMasterSceneManager>(env, thiz, jMasterSceneID, &XMasterSceneManager::DeleteMasterScene);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_MasterSceneManager_applyMasterScene(JNIEnv *env, jobject thiz, jstring jMasterSceneID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<XMasterSceneManager>(env, thiz, jMasterSceneID, &XMasterSceneManager::ApplyMasterScene);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_MasterSceneManager_createNativeObject(JNIEnv *env, jobject thiz, jobject jController, jobject jCallback)
{
    JControllerClient *xController = GetHandle<JControllerClient*>(jController);
    if (!xController) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XMasterSceneManagerCallback *xCallback = GetHandle<XMasterSceneManagerCallback*>(jCallback);
    if (!xCallback) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XMasterSceneManager* xmsm = new XMasterSceneManager(thiz, *xController, *xCallback);
    if (!xmsm) {
        QCC_LogError(ER_FAIL, ("JSceneManager() failed"));
        return;
    }

    CreateHandle<XMasterSceneManager>(thiz, xmsm);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_MasterSceneManager_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<XMasterSceneManager>(thiz);
}
