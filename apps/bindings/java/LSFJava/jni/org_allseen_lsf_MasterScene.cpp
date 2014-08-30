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
#include <jni.h>

#include <qcc/Debug.h>

#include "JEnum.h"
#include "JStringArray.h"
#include "NUtil.h"
#include "XMasterScene.h"
#include "XObject.h"

#include "org_allseen_lsf_MasterScene.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;
using namespace lsf;

JNIEXPORT
void JNICALL Java_org_allseen_lsf_MasterScene_setScenes(JNIEnv *env, jobject thiz, jobjectArray jSceneIDs)
{
    XMasterScene *xDelegate = GetHandle<XMasterScene*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    JStringArray::CopyStringArray(jSceneIDs, xDelegate->scenes);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_MasterScene_getScenes(JNIEnv *env, jobject thiz)
{
    XMasterScene *xDelegate = GetHandle<XMasterScene*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jobjectArray jSceneIDs = JStringArray::NewStringArray(xDelegate->scenes);
    if (env->ExceptionCheck() || !jSceneIDs) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return jSceneIDs;

}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_MasterScene_isDependentOnScene(JNIEnv *env, jobject thiz, jstring jSceneID)
{
    XMasterScene* xDelegate = GetHandle<XMasterScene*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    JString xSceneID(jSceneID);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("JString failed"));
        return NULL;
    }

    if (!xSceneID.c_str()) {
        QCC_LogError(ER_FAIL, ("JString invalid"));
        return NULL;
    }

    LSFString cSceneID = xSceneID.c_str();

    jobject jResponseCode = JEnum::jResponseCodeEnum->getObject((int)(xDelegate->IsDependentOnScene(cSceneID)));
    if (env->ExceptionCheck() || !jResponseCode) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return jResponseCode;
}

JNIEXPORT
jstring JNICALL Java_org_allseen_lsf_MasterScene_toString(JNIEnv *env, jobject thiz)
{
    return XObject::ToString<XMasterScene>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_MasterScene_createNativeObject(JNIEnv *env, jobject thiz)
{
    CreateHandle<XMasterScene>(thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_MasterScene_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<XMasterScene>(thiz);
}

