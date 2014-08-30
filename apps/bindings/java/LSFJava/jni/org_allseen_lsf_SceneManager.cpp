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
#include "XScene.h"
#include "XSceneManager.h"
#include "XSceneManagerCallback.h"

#include "org_allseen_lsf_SceneManager.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;
using namespace lsf;

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_SceneManager_getAllSceneIDs(JNIEnv *env, jobject thiz)
{
    return XCppDelegator::Call_ControllerClientStatus<XSceneManager>(env, thiz, &XSceneManager::GetAllSceneIDs);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_SceneManager_getSceneName(JNIEnv *env, jobject thiz, jstring jSceneID, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_String_String<XSceneManager>(env, thiz, jSceneID, jLanguage, &XSceneManager::GetSceneName);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_SceneManager_setSceneName(JNIEnv *env, jobject thiz, jstring jSceneID, jstring jSceneName, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_String_String_String<XSceneManager>(env, thiz, jSceneID, jSceneName, jLanguage, &XSceneManager::SetSceneName);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_SceneManager_createScene(JNIEnv *env, jobject thiz, jobject jScene, jstring jSceneName, jstring jLanguage)
{
    return XCppDelegator::Call_ControllerClientStatus_Object_String_String<XSceneManager, XScene, Scene>(env, thiz, jScene, jSceneName, jLanguage, &XSceneManager::CreateScene);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_SceneManager_updateScene(JNIEnv *env, jobject thiz, jstring jSceneID, jobject jScene)
{
    return XCppDelegator::Call_ControllerClientStatus_String_Object<XSceneManager, XScene, Scene>(env, thiz, jSceneID, jScene, &XSceneManager::UpdateScene);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_SceneManager_deleteScene(JNIEnv *env, jobject thiz, jstring jSceneID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<XSceneManager>(env, thiz, jSceneID, &XSceneManager::DeleteScene);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_SceneManager_getScene(JNIEnv *env, jobject thiz, jstring jSceneID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<XSceneManager>(env, thiz, jSceneID, &XSceneManager::GetScene);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_SceneManager_applyScene(JNIEnv *env, jobject thiz, jstring jSceneID)
{
    return XCppDelegator::Call_ControllerClientStatus_String<XSceneManager>(env, thiz, jSceneID, &XSceneManager::ApplyScene);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_SceneManager_createNativeObject(JNIEnv *env, jobject thiz, jobject jController, jobject jCallback)
{
    JControllerClient *xController = GetHandle<JControllerClient*>(jController);
    if (!xController) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XSceneManagerCallback *xCallback = GetHandle<XSceneManagerCallback*>(jCallback);
    if (!xCallback) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    XSceneManager* jsm = new XSceneManager(thiz, *xController, *xCallback);
    if (!jsm) {
        QCC_LogError(ER_FAIL, ("JSceneManager() failed"));
        return;
    }

    CreateHandle<XSceneManager>(thiz, jsm);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_SceneManager_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<XSceneManager>(thiz);
}
