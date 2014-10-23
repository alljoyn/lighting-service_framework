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
#include <android/log.h>

#include <qcc/Debug.h>

#include "JEnum.h"
#include "JStringArray.h"
#include "XLongArray.h"
#include "XJavaDelegator.h"

#define QCC_MODULE "AJN-LSF-JNI"


namespace lsf {

// This .cpp file is #include'd in the .h file because some templated
// methods must be defined there. The following #ifndef prevents the
// non-templated code from being visible there.
#ifndef LSF_JNI_XJAVADELEGATOR_H_INCLUDE_TEMPLATE_METHODS

void XJavaDelegator::Call_Void(const jweak jdelegate, char const *func)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    Call_Void_Variadic(env, jdelegate, func, "()V");
}

void XJavaDelegator::Call_Void_UInt32(const jweak jdelegate, char const *func, const uint32_t &uint32Value)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    Call_Void_Variadic(env, jdelegate, func, "(J)V", (jlong)uint32Value);
}

void XJavaDelegator::Call_Void_ResponseCode(const jweak jdelegate, char const *func, const LSFResponseCode &responseCode)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    Call_Void_Variadic(env, jdelegate, func, "(Lorg/allseen/lsf/ResponseCode;)V", jresponseCode);
}

void XJavaDelegator::Call_Void_EnumList(const jweak jdelegate, char const *func, const jobjectArray &jEnumList, char const *enumClass)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    char sig[512];
    snprintf(sig, sizeof(sig), "([L%s;)V", enumClass);

    Call_Void_Variadic(env, jdelegate, func, sig, jEnumList);
}

void XJavaDelegator::Call_Void_StringList(const jweak jdelegate, char const *func, const LSFStringList &strList)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobjectArray jstrList = JStringArray::NewStringArray(strList);
    if (env->ExceptionCheck() || !jstrList) {
        QCC_LogError(ER_FAIL, ("NewStringArray() failed"));
        return;
    }

    Call_Void_Variadic(env, jdelegate, func, "([Ljava/lang/String;)V", jstrList);
}

void XJavaDelegator::Call_Void_ResponseCode_String(const jweak jdelegate, char const *func, const LSFResponseCode &responseCode, const LSFString &strValue)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    jstring jstrValue = env->NewStringUTF(strValue.c_str());
    if (env->ExceptionCheck() || !jstrValue) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    Call_Void_Variadic(env, jdelegate, func, "(Lorg/allseen/lsf/ResponseCode;Ljava/lang/String;)V", jresponseCode, jstrValue);
}

void XJavaDelegator::Call_Void_ResponseCode_StringList(const jweak jdelegate, char const *func, const LSFResponseCode &responseCode, const LSFStringList &strList)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    jobjectArray jstrList = JStringArray::NewStringArray(strList);
    if (env->ExceptionCheck() || !jstrList) {
        QCC_LogError(ER_FAIL, ("NewStringArray() failed"));
        return;
    }

    Call_Void_Variadic(env, jdelegate, func, "(Lorg/allseen/lsf/ResponseCode;[Ljava/lang/String;)V", jresponseCode, jstrList);
}
#endif //LSF_JNI_XJAVADELEGATOR_H_INCLUDE_TEMPLATE_METHODS

// This .cpp file is #include'd in the .h file because some templated
// methods must be defined there. The following #ifdef allows the
// templated code to be visible there.
#ifdef LSF_JNI_XJAVADELEGATOR_H_INCLUDE_TEMPLATE_METHODS
template <typename CTYPE, typename XTYPE>
void XJavaDelegator::Call_Void_String_Object(const jweak jdelegate, char const *func, XClass *xClass, const LSFString &strValue, const CTYPE &objValue)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jstring jstrValue = env->NewStringUTF(strValue.c_str());
    if (env->ExceptionCheck() || !jstrValue) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    jmethodID jconstructor = env->GetMethodID(xClass->classRef, "<init>", "()V");
    if (env->ExceptionCheck() || !jconstructor) {
        QCC_LogError(ER_FAIL, ("GetMethodID() failed"));
        return;
    }

    jobject jobjValue = env->NewObject(xClass->classRef, jconstructor);
    if (env->ExceptionCheck() || !jobjValue) {
        QCC_LogError(ER_FAIL, ("NewObject() failed"));
        return;
    }

    XTYPE *xobjValue = GetHandle<XTYPE *>(jobjValue);
    if (env->ExceptionCheck() || !xobjValue) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    *xobjValue = objValue;

//    __android_log_print(ANDROID_LOG_DEBUG, QCC_MODULE, "cObj = %s", objValue.c_str());
//    __android_log_print(ANDROID_LOG_DEBUG, QCC_MODULE, "xObj = %s", xobjValue->c_str());

    char sig[512];
    snprintf(sig, sizeof(sig), "(Ljava/lang/String;L%s;)V", xClass->className);

    Call_Void_Variadic(env, jdelegate, func, sig, jstrValue, jobjValue);
}
#endif //LSF_JNI_XJAVADELEGATOR_H_INCLUDE_TEMPLATE_METHODS


// This .cpp file is #include'd in the .h file because some templated
// methods must be defined there. The following #ifndef prevents the
// non-templated code from being visible there.
#ifndef LSF_JNI_XJAVADELEGATOR_H_INCLUDE_TEMPLATE_METHODS
void XJavaDelegator::Call_Void_String_String(const jweak jdelegate, char const *func, const LSFString &strValue1, const LSFString &strValue2)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jstring jstrValue1 = env->NewStringUTF(strValue1.c_str());
    if (env->ExceptionCheck() || !jstrValue1) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    jstring jstrValue2 = env->NewStringUTF(strValue2.c_str());
    if (env->ExceptionCheck() || !jstrValue2) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    Call_Void_Variadic(env, jdelegate, func, "(Ljava/lang/String;Ljava/lang/String;)V", jstrValue1, jstrValue2);
}

void XJavaDelegator::Call_Void_ResponseCode_String_Boolean(const jweak jdelegate, char const *func, const LSFResponseCode &responseCode, const LSFString &strValue, const bool &boolValue)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    jstring jstrValue = env->NewStringUTF(strValue.c_str());
    if (env->ExceptionCheck() || !jstrValue) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    jboolean jboolValue = (jboolean)boolValue;

    Call_Void_Variadic(env, jdelegate, func, "(Lorg/allseen/lsf/ResponseCode;Ljava/lang/String;Z)V", jresponseCode, jstrValue, jboolValue);
}

void XJavaDelegator::Call_Void_ResponseCode_String_SInt32(const jweak jdelegate, char const *func, const LSFResponseCode &responseCode, const LSFString &strValue, const uint32_t &uint32Value)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    jstring jstrValue = env->NewStringUTF(strValue.c_str());
    if (env->ExceptionCheck() || !jstrValue) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    jint jintValue = (jint)uint32Value;

    Call_Void_Variadic(env, jdelegate, func, "(Lorg/allseen/lsf/ResponseCode;Ljava/lang/String;I)V", jresponseCode, jstrValue, jintValue);
}

void XJavaDelegator::Call_Void_ResponseCode_String_UInt32(const jweak jdelegate, char const *func, const LSFResponseCode &responseCode, const LSFString &strValue, const uint32_t &uint32Value)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    jstring jstrValue = env->NewStringUTF(strValue.c_str());
    if (env->ExceptionCheck() || !jstrValue) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    jlong jlongValue = (jlong)uint32Value;

    Call_Void_Variadic(env, jdelegate, func, "(Lorg/allseen/lsf/ResponseCode;Ljava/lang/String;J)V", jresponseCode, jstrValue, jlongValue);
}

void XJavaDelegator::Call_Void_ResponseCode_String_Enum(const jweak jdelegate, char const *func, const LSFResponseCode &responseCode, const LSFString &strValue, JEnum *enumObject, const int &enumValue, char const *enumClass)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    jstring jstrValue = env->NewStringUTF(strValue.c_str());
    if (env->ExceptionCheck() || !jstrValue) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    jobject jenumValue = enumObject->getObject(enumValue);
    if (env->ExceptionCheck() || !jenumValue) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    char sig[512];
    snprintf(sig, sizeof(sig), "(Lorg/allseen/lsf/ResponseCode;Ljava/lang/String;L%s;)V", enumClass);

    Call_Void_Variadic(env, jdelegate, func, sig, jresponseCode, jstrValue, jenumValue);
}

void XJavaDelegator::Call_Void_ResponseCode_String_String(const jweak jdelegate, char const *func, const LSFResponseCode &responseCode, const LSFString &strValue1, const LSFString &strValue2)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    jstring jstrValue1 = env->NewStringUTF(strValue1.c_str());
    if (env->ExceptionCheck() || !jstrValue1) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    jstring jstrValue2 = env->NewStringUTF(strValue2.c_str());
    if (env->ExceptionCheck() || !jstrValue2) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    Call_Void_Variadic(env, jdelegate, func, "(Lorg/allseen/lsf/ResponseCode;Ljava/lang/String;Ljava/lang/String;)V", jresponseCode, jstrValue1, jstrValue2);
}
#endif //LSF_JNI_XJAVADELEGATOR_H_INCLUDE_TEMPLATE_METHODS

// This .cpp file is #include'd in the .h file because some templated
// methods must be defined there. The following #ifdef allows the
// templated code to be visible there.
#ifdef LSF_JNI_XJAVADELEGATOR_H_INCLUDE_TEMPLATE_METHODS
template <typename CTYPE, typename XTYPE>
void XJavaDelegator::Call_Void_ResponseCode_String_Object(const jweak jdelegate, char const *func, XClass *xClass, const LSFResponseCode &responseCode, const LSFString &strValue, const CTYPE &objValue)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    jstring jstrValue = env->NewStringUTF(strValue.c_str());
    if (env->ExceptionCheck() || !jstrValue) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    jmethodID jconstructor = env->GetMethodID(xClass->classRef, "<init>", "()V");
    if (env->ExceptionCheck() || !jconstructor) {
        QCC_LogError(ER_FAIL, ("GetMethodID() failed"));
        return;
    }

    jobject jobjValue = env->NewObject(xClass->classRef, jconstructor);
    if (env->ExceptionCheck() || !jobjValue) {
        QCC_LogError(ER_FAIL, ("NewObject() failed"));
        return;
    }

    XTYPE *xobjValue = GetHandle<XTYPE *>(jobjValue);
    if (env->ExceptionCheck() || !xobjValue) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    *xobjValue = objValue;

//    __android_log_print(ANDROID_LOG_DEBUG, QCC_MODULE, "cObj = %s", objValue.c_str());
//    __android_log_print(ANDROID_LOG_DEBUG, QCC_MODULE, "xObj = %s", xobjValue->c_str());

    char sig[512];
    snprintf(sig, sizeof(sig), "(Lorg/allseen/lsf/ResponseCode;Ljava/lang/String;L%s;)V", xClass->className);

    Call_Void_Variadic(env, jdelegate, func, sig, jresponseCode, jstrValue, jobjValue);
}
#endif //LSF_JNI_XJAVADELEGATOR_H_INCLUDE_TEMPLATE_METHODS

// This .cpp file is #include'd in the .h file because some templated
// methods must be defined there. The following #ifndef prevents the
// non-templated code from being visible there.
#ifndef LSF_JNI_XJAVADELEGATOR_H_INCLUDE_TEMPLATE_METHODS
void XJavaDelegator::Call_Void_ResponseCode_String_UInt32List(const jweak jdelegate, char const *func, const LSFResponseCode &responseCode, const LSFString &strValue, const UInt32List& uint32List)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    jstring jstrValue = env->NewStringUTF(strValue.c_str());
    if (env->ExceptionCheck() || !jstrValue) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    Call_Void_Variadic(env, jdelegate, func, "(Lorg/allseen/lsf/ResponseCode;Ljava/lang/String;[J;)V", jresponseCode, jstrValue, XLongArray::NewLongArray(uint32List));
}

void XJavaDelegator::Call_Void_ResponseCode_String_StringList(const jweak jdelegate, char const *func, const LSFResponseCode &responseCode, const LSFString &strValue, const LSFStringList &strList)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    jstring jstrValue = env->NewStringUTF(strValue.c_str());
    if (env->ExceptionCheck() || !jstrValue) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    jobjectArray jstrList = JStringArray::NewStringArray(strList);
    if (env->ExceptionCheck() || !jstrList) {
        QCC_LogError(ER_FAIL, ("NewStringArray() failed"));
        return;
    }

    Call_Void_Variadic(env, jdelegate, func, "(Lorg/allseen/lsf/ResponseCode;Ljava/lang/String;[Ljava/lang/String;)V", jresponseCode, jstrValue, jstrList);
}

void XJavaDelegator::Call_Void_ResponseCode_String_String_String(const jweak jdelegate, char const *func, const LSFResponseCode &responseCode, const LSFString &strValue1, const LSFString &strValue2, const LSFString &strValue3)
{
    // Get the JNIEnv for the current native thread
    JScopedEnv env;

    jobject jresponseCode = JEnum::jResponseCodeEnum->getObject(responseCode);
    if (env->ExceptionCheck() || !jresponseCode) {
        QCC_LogError(ER_FAIL, ("getObject() failed"));
        return;
    }

    jstring jstrValue1 = env->NewStringUTF(strValue1.c_str());
    if (env->ExceptionCheck() || !jstrValue1) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    jstring jstrValue2 = env->NewStringUTF(strValue2.c_str());
    if (env->ExceptionCheck() || !jstrValue2) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    jstring jstrValue3 = env->NewStringUTF(strValue3.c_str());
    if (env->ExceptionCheck() || !jstrValue3) {
        QCC_LogError(ER_FAIL, ("NewStringUTF() failed"));
        return;
    }

    Call_Void_Variadic(env, jdelegate, func, "(Lorg/allseen/lsf/ResponseCode;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", jresponseCode, jstrValue1, jstrValue2, jstrValue3);
}

void XJavaDelegator::Call_Void_Variadic(JScopedEnv& env, const jweak jdelegate, const char *func, char const * const sig, ...)
{
    /*
     * The weak reference to the Java delegate cannot be directly used.  We have to get
     * a "hard" reference to it and then use that. If you try to use a weak reference
     * directly you will crash and burn.
     */
    jobject jobj = env->NewLocalRef(jdelegate);
    if (env->ExceptionCheck() || !jobj) {
        QCC_LogError(ER_FAIL, ("NewLocalRef() failed"));
        return;
    }

    jclass clazz = env->GetObjectClass(jobj);
    if (env->ExceptionCheck() || !clazz) {
        QCC_LogError(ER_FAIL, ("GetObjectClass() failed"));
        return;
    }

    std::string jfunc = GetJavaMethodNameFromCPPMethodName(func);
    jmethodID mid = env->GetMethodID(clazz, jfunc.c_str(), sig);
    if (env->ExceptionCheck() || !mid) {
        QCC_LogError(ER_FAIL, ("GetMethodID() failed"));
        return;
    }

    va_list args;
    va_start(args, sig);
    env->CallVoidMethodV(jobj, mid, args);
    va_end(args);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("CallVoidMethod() exception"));
        return;
    }
}

#endif //LSF_JNI_XJAVADELEGATOR_H_INCLUDE_TEMPLATE_METHODS

} //namespace lsf

#undef QCC_MODULE

