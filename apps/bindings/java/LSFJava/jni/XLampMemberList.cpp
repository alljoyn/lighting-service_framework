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

// This .cpp file is #include'd in the .h file because some templated
// methods must be defined there. The following #ifdef allows the
// templated code to be visible there.
#ifdef LSF_JNI_XLAMPMEMBERLIST_H_INCLUDE_TEMPLATE_METHODS

#include <qcc/Debug.h>

#include "NDefs.h"
#include "NUtil.h"
#include "JStringArray.h"
#include "XLampMemberList.h"

#define QCC_MODULE LSF_QCC_MODULE

namespace lsf {

template <typename T>
void XLampMemberList::SetLamps(JNIEnv *env, jobject thiz, jobjectArray jLampIDs)
{
    T *xDelegate = GetHandle<T*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    JStringArray::CopyStringArray(jLampIDs, xDelegate->lamps);
}

template <typename T>
jobjectArray XLampMemberList::GetLamps(JNIEnv *env, jobject thiz)
{
    T *xDelegate = GetHandle<T*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jobjectArray jLamps = JStringArray::NewStringArray(xDelegate->lamps);
    if (env->ExceptionCheck() || !jLamps) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return jLamps;
}

template <typename T>
void XLampMemberList::SetLampGroups(JNIEnv *env, jobject thiz, jobjectArray jLampGroupIDs)
{
    T *xDelegate = GetHandle<T*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    JStringArray::CopyStringArray(jLampGroupIDs, xDelegate->lampGroups);
}

template <typename T>
jobjectArray XLampMemberList::GetLampGroups(JNIEnv *env, jobject thiz)
{
    T *xDelegate = GetHandle<T*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jobjectArray jLampGroups = JStringArray::NewStringArray(xDelegate->lampGroups);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    return jLampGroups;
}

} /* namespace lsf */

#undef QCC_MODULE

#endif /* LSF_JNI_XLAMPMEMBERLIST_H_INCLUDE_TEMPLATE_METHODS */
