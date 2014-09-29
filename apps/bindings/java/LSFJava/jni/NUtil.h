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
#ifndef _LSF_ALLJOYN_JAVA_UTIL_H
#define _LSF_ALLJOYN_JAVA_UTIL_H

#include <jni.h>
#include <assert.h>
#include <stddef.h>

#include <string>

#include <qcc/Debug.h>
#include <alljoyn/Status.h>

#define QCC_MODULE "AJN-LSF-JNI"

std::string GetJavaNativeMethodNameFromFunc(char const *func);
std::string GetJavaMethodNameFromCPPMethodName(char const *func);

JNIEnv* GetEnv();
JNIEnv* GetEnv(jint*);
void DeleteEnv(jint);

void Throw(const char*, const char*);
void ThrowErrorReplyBusException(const char*, const char*);

template <typename T> T* CreateHandle(jobject);
template <typename T> void DestroyHandle(jobject);

template <typename T> T GetHandle(jobject, const char* = "nativeHandle");
void SetHandle(jobject, void*, const char* = "nativeHandle");

template <typename T> T GetNativeListener(JNIEnv *, jobject);

class XClass {
  public:
    // Classes here require a call to do_find() in JNI_OnLoad()
    static XClass *xLampState;
    static XClass *xLampDetails;
    static XClass *xLampParameters;
    static XClass *xLampGroup;
    static XClass *xScene;
    static XClass *xMasterScene;
    static XClass *xStateTransitionEffect;
    static XClass *xPresetTransitionEffect;
    static XClass *xStatePulseEffect;
    static XClass *xPresetPulseEffect;

    XClass(const char *name) { className = name; classRef = NULL; }
    ~XClass();

    void doFind(JNIEnv* env) {
        jclass classObj = env->FindClass(className);

        if (classObj) {
            classRef = (jclass)env->NewGlobalRef(classObj);
        }
    }

    const char *className;
    jclass classRef;
};

/**
 * Helper function to wrap StringUTFChars to ensure proper release of resource.
 *
 * @warning NULL is a valid value, so exceptions must be checked for explicitly
 * by the caller after constructing the JString.
 */
class JString {
  public:
    JString(jstring s);
    ~JString();
    const char* c_str() { return str; }
  private:
    JString(const JString& other);
    JString& operator =(const JString& other);

    jstring jstr;
    const char* str;
};

/**
 * A helper class to wrap local references ensuring proper release.
 */
template <class T>
class JLocalRef {
  public:
    JLocalRef() : jobj(NULL) { }
    JLocalRef(const T& obj) : jobj(obj) { }
    ~JLocalRef() { if (jobj) { GetEnv()->DeleteLocalRef(jobj); } }
    JLocalRef& operator=(T obj)
    {
        if (jobj) {
            GetEnv()->DeleteLocalRef(jobj);
        }
        jobj = obj;
        return *this;
    }
    operator T() { return jobj; }
    T move()
    {
        T ret = jobj;
        jobj = NULL;
        return ret;
    }
  private:
    T jobj;
};

/**
 * A scoped JNIEnv pointer to ensure proper release.
 */
class JScopedEnv {
  public:
    JScopedEnv();
    ~JScopedEnv();
    JNIEnv* operator->() { return env; }
  private:
    JScopedEnv(const JScopedEnv& other);
    JScopedEnv& operator =(const JScopedEnv& other);

    JNIEnv* env;
    jint detached;
};


jobject JStatus(QStatus);

template <typename T>
T* CreateHandle(jobject jobj)
{
    T* cobj = new T(jobj);
    if (cobj == NULL) {
        Throw("java/lang/OutOfMemoryError", NULL);
        return NULL;
    }

    return CreateHandle(jobj, cobj);
}

template <typename T>
T* CreateHandle(jobject jobj, T* cobj)
{
    JNIEnv* env = GetEnv();
    if (!jobj) {
        Throw("java/lang/NullPointerException", "failed to create native handle for null object");
        return NULL;
    }

    assert(GetHandle<T*>(jobj) == NULL);

    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("GetHandle() threw exception"));
        return NULL;
    }

    SetHandle(jobj, cobj);

    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("SetHandle() threw exception"));
        delete cobj;
        cobj = NULL;
    }

    return cobj;
}

template <typename T>
void DestroyHandle(jobject jobj)
{
    JNIEnv* env = GetEnv();
    if (!jobj) {
        Throw("java/lang/NullPointerException", "failed to delete native handle for null object");
        return;
    }

    T* obj = GetHandle<T*>(jobj);

    if (obj) {
        delete obj;
    } else {
        QCC_DbgPrintf(("Invalid attempt to delete a null handle"));
    }
}


/**
 * Get the native C++ handle of a given Java object.
 *
 * If we have an object that has a native counterpart, we need a way to get at
 * the native object from the Java object.  We do this by storing the native
 * pointer as an opaque handle in a Java field named "handle".  We use Java
 * reflection to pull the field out and return the handle value.
 *
 * Think of this handle as the counterpart to the object reference found in
 * the C++ objects that need to call into Java.  Java objects use the handle to
 * get at the C++ objects, and C++ objects use an object reference to get at
 * the Java objects.
 *
 * @return The handle value as a pointer.  NULL is a valid value.
 *
 * @warning This method makes native calls which may throw exceptions.  In the
 *          usual idiom, exceptions must be checked for explicitly by the caller
 *          after *every* call to GetHandle.  Since NULL is a valid value to
 *          return, validity of the returned pointer must be checked as well.
 */
template <typename T>
T GetHandle(jobject jobj, const char* member)
{
    JNIEnv* env = GetEnv();
    if (!jobj) {
        Throw("java/lang/NullPointerException", "failed to get native handle on null object");
        return NULL;
    }
    JLocalRef<jclass> clazz = env->GetObjectClass(jobj);
    jfieldID fid = env->GetFieldID(clazz, member, "J");
    void* handle = NULL;
    if (fid) {
        handle = (void*)env->GetLongField(jobj, fid);
    }

    return reinterpret_cast<T>(handle);
}

#undef QCC_MODULE

#endif  // _LSF_ALLJOYN_JAVA_UTIL_H

