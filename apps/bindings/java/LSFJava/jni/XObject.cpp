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
#ifdef LSF_JNI_XOBJECT_H_INCLUDE_TEMPLATE_METHODS

#include <android/log.h>

#include <qcc/Debug.h>

#include "NDefs.h"
#include "NUtil.h"
#include "XObject.h"

#define QCC_MODULE LSF_QCC_MODULE

namespace lsf {

template <typename XTYPE>
jstring XObject::ToString(JNIEnv *env, jobject thiz)
{
    XTYPE *xDelegate = GetHandle<XTYPE*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    jstring jstrValue = env->NewStringUTF(xDelegate->c_str());
    if (env->ExceptionCheck() || !jstrValue) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return NULL;
    }

    return jstrValue;
}

template <typename CTYPE, typename XTYPE>
jobjectArray XObject::NewArray(XClass* xClass, const std::list<CTYPE>& cObjectList)
{
    JScopedEnv env;
    typename std::list<CTYPE>::size_type cCount = cObjectList.size();

    jobjectArray jObjectArray = env->NewObjectArray((jsize)cCount, xClass->classRef, NULL);
    if (env->ExceptionCheck() || !jObjectArray) {
        QCC_LogError(ER_FAIL, ("NewObjectArray() failed"));
        return NULL;
    }

    typename std::list<CTYPE>::const_iterator it = cObjectList.begin();
    for (int i = 0; i < cCount; i++, it++) {
        jmethodID jConstructor = env->GetMethodID(xClass->classRef, "<init>", "()V");
        if (env->ExceptionCheck() || !jConstructor) {
            QCC_LogError(ER_FAIL, ("GetMethodID() failed"));
            return NULL;
        }

        jobject jObject = env->NewObject(xClass->classRef, jConstructor);
        if (env->ExceptionCheck() || !jObject) {
            QCC_LogError(ER_FAIL, ("NewObject() failed"));
            return NULL;
        }

        XTYPE *xObject = GetHandle<XTYPE *>(jObject);
        if (env->ExceptionCheck() || !xObject) {
            QCC_LogError(ER_FAIL, ("GetHandle() failed"));
            return NULL;
        }

        *xObject = *it;

        env->SetObjectArrayElement(jObjectArray, i, jObject);
        if (env->ExceptionCheck()) {
            QCC_LogError(ER_FAIL, ("SetObjectArrayElement() failed"));
            return NULL;
        }

        env->DeleteLocalRef(jObject);
    }

    return jObjectArray;
}

template <typename CTYPE, typename XTYPE>
void XObject::CopyArray(XClass* xClass, jobjectArray jObjectArray, std::list<CTYPE>& cObjectList)
{
    JScopedEnv env;

    jsize jCount = env->GetArrayLength(jObjectArray);
    if (env->ExceptionCheck() || (jCount < 0)) {
        QCC_LogError(ER_FAIL, ("GetArrayLength() invalid"));
        return;
    }

    cObjectList.clear();

    for (jsize i = 0; i < jCount; i++) {
        jstring jObject = (jstring)env->GetObjectArrayElement(jObjectArray, i);
        if (env->ExceptionCheck() || !jObject) {
            QCC_LogError(ER_FAIL, ("GetObjectArrayElement() failed"));
            return;
        }

        XTYPE *xObject = GetHandle<XTYPE *>(jObject);
        if (env->ExceptionCheck() || !xObject) {
            QCC_LogError(ER_FAIL, ("GetHandle() failed"));
            return;
        }

        CTYPE cObject(*xObject);

        cObjectList.push_back(cObject);

        env->DeleteLocalRef(jObject);
   }
}

} /* namespace lsf */

#undef QCC_MODULE

#endif /* LSF_JNI_XOBJECT_H_INCLUDE_TEMPLATE_METHODS */
