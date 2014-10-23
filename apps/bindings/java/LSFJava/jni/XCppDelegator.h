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

#ifndef LSF_JNI_XCPPDELEGATOR_H_
#define LSF_JNI_XCPPDELEGATOR_H_

#include <jni.h>

#include <ControllerClientDefs.h>

#include "JEnum.h"
#include "NDefs.h"
#include "NUtil.h"

namespace lsf {

class XCppDelegator {
private:
    XCppDelegator();

public:
    template <typename T> static jobject Call_ControllerClientStatus(JNIEnv *env, jobject thiz, ControllerClientStatus (T::*cMethod)(void));
    template <typename T> static jobject Call_ControllerClientStatus_String(JNIEnv *env, jobject thiz, jstring jString, ControllerClientStatus (T::*cMethod)(const std::string&));
    template <typename T> static jobject Call_ControllerClientStatus_String_String(JNIEnv *env, jobject thiz, jstring jString1, jstring jString2, ControllerClientStatus (T::*cMethod)(const std::string&, const std::string&));
    template <typename T> static jobject Call_ControllerClientStatus_String_String_String(JNIEnv *env, jobject thiz, jstring jString1, jstring jString2, jstring jString3, ControllerClientStatus (T::*cMethod)(const std::string&, const std::string&, const std::string&));

    template <typename T> static jobject Call_ControllerClientStatus_String_Boolean(JNIEnv *env, jobject thiz, jstring jString, jboolean jBoolean, ControllerClientStatus (T::*cMethod)(const std::string&, const bool&));
    template <typename T> static jobject Call_ControllerClientStatus_String_UInt32(JNIEnv *env, jobject thiz, jstring jString, jlong jLong, ControllerClientStatus (T::*cMethod)(const std::string&, const uint32_t&));
    template <typename T> static jobject Call_ControllerClientStatus_String_UInt32_UInt32(JNIEnv *env, jobject thiz, jstring jString, jlong jLong1, jlong jLong2, ControllerClientStatus (T::*cMethod)(const std::string&, const uint32_t&, const uint32_t&));
    template <typename T> static jobject Call_ControllerClientStatus_String_String_UInt32(JNIEnv *env, jobject thiz, jstring jString1, jstring jString2, jlong jLong, ControllerClientStatus (T::*cMethod)(const std::string&, const std::string&, const uint32_t&));
    template <typename T> static jobject Call_ControllerClientStatus_String_String_UInt32_UInt32_UInt32_String(JNIEnv *env, jobject thiz, jstring jString1, jstring jString2, jlong jLong1, jlong jLong2, jlong jLong3, jstring jString3, ControllerClientStatus (T::*cMethod)(const std::string&, const std::string&, const uint32_t&, const uint32_t&, const uint32_t&, const std::string&));

    template <typename T1, typename T2, typename T3> static jobject Call_ControllerClientStatus_String_Object(JNIEnv *env, jobject thiz, jstring jString, jobject jObject, ControllerClientStatus (T1::*cMethod)(const std::string&, const T3&));
    template <typename T1, typename T2, typename T3> static jobject Call_ControllerClientStatus_String_Object_UInt32(JNIEnv *env, jobject thiz, jstring jString, jobject jObject, jlong jLong, ControllerClientStatus (T1::*cMethod)(const std::string&, const T3&, const uint32_t&));
    template <typename T1, typename T2, typename T3> static jobject Call_ControllerClientStatus_String_Object_UInt32_UInt32_UInt32_Object(JNIEnv *env, jobject thiz, jstring jString, jobject jObject1, jlong jLong1, jlong jLong2, jlong jLong3, jobject jObject2, ControllerClientStatus (T1::*cMethod)(const std::string&, const T3&, const uint32_t&, const uint32_t&, const uint32_t&, const T3&));

    template <typename T1, typename T2, typename T3> static jobject Call_ControllerClientStatus_Object(JNIEnv *env, jobject thiz, jobject jObject, ControllerClientStatus (T1::*cMethod)(const T3&));
    template <typename T1, typename T2, typename T3> static jobject Call_ControllerClientStatus_Object_String_String(JNIEnv *env, jobject thiz, jobject jObject, jstring jString1, jstring jString2, ControllerClientStatus (T1::*cMethod)(const T3&, const std::string&, const std::string&));
};

} /* namespace lsf */

// The .cpp file is #include'd in this .h file because some templated
// methods must be defined here. The following #define prevents the
// non-templated code from being visible here.
#define LSF_JNI_XCPPDELEGATOR_H_INCLUDE_TEMPLATE_METHODS
#include "XCppDelegator.cpp"
#undef LSF_JNI_XCPPDELEGATOR_H_INCLUDE_TEMPLATE_METHODS

#endif /* LSF_JNI_XCPPDELEGATOR_H_ */
