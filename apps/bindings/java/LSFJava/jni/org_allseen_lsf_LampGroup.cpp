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

#include <alljoyn/MsgArg.h>

#include "JEnum.h"
#include "JLampGroup.h"
#include "XLampMemberList.h"
#include "XObject.h"

#include "org_allseen_lsf_LampGroup.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace lsf;

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampGroup_setLamps(JNIEnv *env, jobject thiz, jobjectArray jLampIDs)
{
    XLampMemberList::SetLamps<JLampGroup>(env, thiz, jLampIDs);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_LampGroup_getLamps(JNIEnv *env, jobject thiz)
{
    return XLampMemberList::GetLamps<JLampGroup>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampGroup_setLampGroups(JNIEnv *env, jobject thiz, jobjectArray jLampGroupIDs)
{
    XLampMemberList::SetLampGroups<JLampGroup>(env, thiz, jLampGroupIDs);
}

JNIEXPORT
jobjectArray JNICALL Java_org_allseen_lsf_LampGroup_getLampGroups(JNIEnv *env, jobject thiz)
{
    return XLampMemberList::GetLampGroups<JLampGroup>(env, thiz);
}

JNIEXPORT
jobject JNICALL Java_org_allseen_lsf_LampGroup_isDependentLampGroup(JNIEnv *env, jobject thiz, jstring jLampGroupID)
{
    JLampGroup* xLampGroup = GetHandle<JLampGroup*>(thiz);
    if (env->ExceptionCheck() || !xLampGroup) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    JString xLampGroupID(jLampGroupID);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("JString failed"));
        return NULL;
    }

    if (!xLampGroupID.c_str()) {
        QCC_LogError(ER_FAIL, ("JString invalid"));
        return NULL;
    }

    LSFString cLampGroupID = xLampGroupID.c_str();

    jobject jResponseCode = JEnum::jResponseCodeEnum->getObject((int)(xLampGroup->IsDependentLampGroup(cLampGroupID)));
    if (env->ExceptionCheck() || !jResponseCode) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return jResponseCode;
}

JNIEXPORT
jstring JNICALL Java_org_allseen_lsf_LampGroup_toString(JNIEnv *env, jobject thiz)
{
    return XObject::ToString<JLampGroup>(env, thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampGroup_createNativeObject(JNIEnv *env, jobject thiz)
{
    CreateHandle<JLampGroup>(thiz);
}

JNIEXPORT
void JNICALL Java_org_allseen_lsf_LampGroup_destroyNativeObject(JNIEnv *env, jobject thiz)
{
    DestroyHandle<JLampGroup>(thiz);
}
