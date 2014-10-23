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

#include "JEnum.h"
#include "NUtil.h"

//using namespace std;
//using namespace qcc;
//using namespace ajn;

/** The cached JVM pointer, valid across all contexts. */
static JavaVM* jvm = NULL;

/** java/lang cached items - these are guaranteed to be loaded at all times. */
static jclass CLS_Integer = NULL;
static jclass CLS_Object = NULL;
static jclass CLS_String = NULL;

/** org/alljoyn/bus */
static jclass CLS_BusException = NULL;
static jclass CLS_ErrorReplyBusException = NULL;
//static jclass CLS_IntrospectionListener = NULL;
//static jclass CLS_BusObjectListener = NULL;
//static jclass CLS_MessageContext = NULL;
//static jclass CLS_MsgArg = NULL;
//static jclass CLS_Signature = NULL;
static jclass CLS_Status = NULL;
//static jclass CLS_Variant = NULL;
//static jclass CLS_BusAttachment = NULL;
//static jclass CLS_SessionOpts = NULL;

static jmethodID MID_Integer_intValue = NULL;
static jmethodID MID_Object_equals = NULL;
static jmethodID MID_BusException_log = NULL;

XClass *XClass::xLampState = new XClass("org/allseen/lsf/LampState");
XClass *XClass::xLampDetails = new XClass("org/allseen/lsf/LampDetails");
XClass *XClass::xLampParameters = new XClass("org/allseen/lsf/LampParameters");
XClass *XClass::xLampGroup = new XClass("org/allseen/lsf/LampGroup");
XClass *XClass::xScene = new XClass("org/allseen/lsf/Scene");
XClass *XClass::xMasterScene = new XClass("org/allseen/lsf/MasterScene");
XClass *XClass::xStateTransitionEffect = new XClass("org/allseen/lsf/StateTransitionEffect");
XClass *XClass::xPresetTransitionEffect = new XClass("org/allseen/lsf/PresetTransitionEffect");
XClass *XClass::xStatePulseEffect = new XClass("org/allseen/lsf/StatePulseEffect");
XClass *XClass::xPresetPulseEffect = new XClass("org/allseen/lsf/PresetPulseEffect");

/**
 * Implement the load hook for the alljoyn_java native library.
 *
 * The Java VM (JVM) calls JNI_OnLoad when a native library is loaded (as a
 * result, for example, of a System.loadLibrary).  We take this opportunity to
 * Store a pointer to the JavaVM and do as much of the fairly expensive calls
 * into Java reflection as we can.  This is also useful since we may not have
 * access to all of the bits and pieces in all contexts, so it is useful
 * to get at them all where/when we can.
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm,
                                  void* reserved)
{
    QCC_UseOSLogging(true);
    QCC_SetDebugLevel("CONTROLLER_CLIENT", 7);

    jvm = vm;
    JNIEnv* env;
    if (jvm->GetEnv((void**)&env, JNI_VERSION_1_2)) {
        return JNI_ERR;
    } else {
        jclass clazz;

        clazz = env->FindClass("java/lang/Integer");
        if (!clazz) {
            return JNI_ERR;
        }
        CLS_Integer = (jclass)env->NewGlobalRef(clazz);

        MID_Integer_intValue = env->GetMethodID(CLS_Integer, "intValue", "()I");
        if (!MID_Integer_intValue) {
            return JNI_ERR;
        }

        clazz = env->FindClass("java/lang/Object");
        if (!clazz) {
            return JNI_ERR;
        }
        CLS_Object = (jclass)env->NewGlobalRef(clazz);

        MID_Object_equals = env->GetMethodID(CLS_Object, "equals", "(Ljava/lang/Object;)Z");
        if (!MID_Object_equals) {
            return JNI_ERR;
        }

        clazz = env->FindClass("java/lang/String");
        if (!clazz) {
            return JNI_ERR;
        }
        CLS_String = (jclass)env->NewGlobalRef(clazz);

        clazz = env->FindClass("org/alljoyn/bus/BusException");
        if (!clazz) {
            return JNI_ERR;
        }
        CLS_BusException = (jclass)env->NewGlobalRef(clazz);
        MID_BusException_log = env->GetStaticMethodID(CLS_BusException, "log", "(Ljava/lang/Throwable;)V");
        if (!MID_BusException_log) {
            return JNI_ERR;
        }

        clazz = env->FindClass("org/alljoyn/bus/ErrorReplyBusException");
        if (!clazz) {
            return JNI_ERR;
        }
        CLS_ErrorReplyBusException = (jclass)env->NewGlobalRef(clazz);
//
//        clazz = env->FindClass("org/alljoyn/bus/IntrospectionListener");
//        if (!clazz) {
//            return JNI_ERR;
//        }
//        CLS_IntrospectionListener = (jclass)env->NewGlobalRef(clazz);
//
//        clazz = env->FindClass("org/alljoyn/bus/BusObjectListener");
//        if (!clazz) {
//            return JNI_ERR;
//        }
//        CLS_BusObjectListener = (jclass)env->NewGlobalRef(clazz);
//
//        clazz = env->FindClass("org/alljoyn/bus/MsgArg");
//        if (!clazz) {
//            return JNI_ERR;
//        }
//        CLS_MsgArg = (jclass)env->NewGlobalRef(clazz);
//        MID_MsgArg_marshal = env->GetStaticMethodID(CLS_MsgArg, "marshal", "(JLjava/lang/String;Ljava/lang/Object;)V");
//        if (!MID_MsgArg_marshal) {
//            return JNI_ERR;
//        }
//        MID_MsgArg_marshal_array = env->GetStaticMethodID(CLS_MsgArg, "marshal", "(JLjava/lang/String;[Ljava/lang/Object;)V");
//        if (!MID_MsgArg_marshal_array) {
//            return JNI_ERR;
//        }
//        MID_MsgArg_unmarshal = env->GetStaticMethodID(CLS_MsgArg, "unmarshal", "(JLjava/lang/reflect/Type;)Ljava/lang/Object;");
//        if (!MID_MsgArg_unmarshal) {
//            return JNI_ERR;
//        }
//        MID_MsgArg_unmarshal_array = env->GetStaticMethodID(CLS_MsgArg, "unmarshal", "(Ljava/lang/reflect/Method;J)[Ljava/lang/Object;");
//        if (!MID_MsgArg_unmarshal_array) {
//            return JNI_ERR;
//        }
//
//        clazz = env->FindClass("org/alljoyn/bus/MessageContext");
//        if (!clazz) {
//            return JNI_ERR;
//        }
//        CLS_MessageContext = (jclass)env->NewGlobalRef(clazz);
//
//        clazz = env->FindClass("org/alljoyn/bus/Signature");
//        if (!clazz) {
//            return JNI_ERR;
//        }
//        CLS_Signature = (jclass)env->NewGlobalRef(clazz);
//
        clazz = env->FindClass("org/alljoyn/bus/Status");
        if (!clazz) {
            return JNI_ERR;
        }
        CLS_Status = (jclass)env->NewGlobalRef(clazz);
//
//        clazz = env->FindClass("org/alljoyn/bus/Variant");
//        if (!clazz) {
//            return JNI_ERR;
//        }
//        CLS_Variant = (jclass)env->NewGlobalRef(clazz);
//
//        clazz = env->FindClass("org/alljoyn/bus/BusAttachment");
//        if (!clazz) {
//            return JNI_ERR;
//        }
//        CLS_BusAttachment = (jclass)env->NewGlobalRef(clazz);
//
//        clazz = env->FindClass("org/alljoyn/bus/SessionOpts");
//        if (!clazz) {
//            return JNI_ERR;
//        }
//        CLS_SessionOpts = (jclass)env->NewGlobalRef(clazz);

        XClass::xLampState->doFind(env);
        XClass::xLampDetails->doFind(env);
        XClass::xLampParameters->doFind(env);

        XClass::xLampGroup->doFind(env);
        XClass::xScene->doFind(env);
        XClass::xMasterScene->doFind(env);
        XClass::xStateTransitionEffect->doFind(env);
        XClass::xPresetTransitionEffect->doFind(env);
        XClass::xStatePulseEffect->doFind(env);
        XClass::xPresetPulseEffect->doFind(env);

        // DCD: Workaround to make sure the Enum classes are loaded
        //      here with this class loader.
        lsf::JEnum::jControllerClientStatusEnum->getObject(0);
        lsf::JEnum::jErrorCodeEnum->getObject(0);
        lsf::JEnum::jResponseCodeEnum->getObject(0);
        lsf::JEnum::jStatusCodeEnum->getObject(0);

        lsf::JEnum::jLampMakeEnum->getObject(0);
        lsf::JEnum::jLampModelEnum->getObject(0);
        lsf::JEnum::jDeviceTypeEnum->getObject(0);
        lsf::JEnum::jLampTypeEnum->getObject(0);
        lsf::JEnum::jBaseTypeEnum->getObject(0);

        return JNI_VERSION_1_2;
    }
}

std::string GetJavaNativeMethodNameFromFunc(char const *func)
{
    std::string cstr = func;
    std::string::size_type pos1 = cstr.rfind('_') + 1;
    std::string::size_type pos2 = cstr.rfind('(');

    return cstr.substr(pos1, pos2 - pos1);
}

std::string GetJavaMethodNameFromCPPMethodName(char const *func)
{
    std::string cstr = func;
    std::string::size_type pos2 = cstr.rfind('(');
    std::string::size_type pos1 = cstr.rfind(':', pos2) + 1;
    std::string name = cstr.substr(pos1, pos2 - pos1);

    name[0] = tolower(name[0]);

    return name;
}

/**
 * Get a valid JNIEnv pointer.
 *
 * A JNIEnv pointer is only valid in an associated JVM thread.  In a callback
 * function (from C++), there is no associated JVM thread, so we need to obtain
 * a valid JNIEnv.  This is a helper function to make that happen.
 *
 * @return The JNIEnv pointer valid in the calling context.
 */
JNIEnv* GetEnv()
{
    return GetEnv(0);
}

JNIEnv* GetEnv(jint* result)
{
    JNIEnv* env;
    jint ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_2);
    if (result) {
        *result = ret;
    }
    if (JNI_EDETACHED == ret) {
#if defined(QCC_OS_ANDROID)
        ret = jvm->AttachCurrentThread(&env, NULL);
#else
        ret = jvm->AttachCurrentThread((void**)&env, NULL);
#endif
    }
    assert(JNI_OK == ret);
    return env;
}

/**
 * Inverse of GetEnv.
 */
void DeleteEnv(jint result)
{
    if (JNI_EDETACHED == result) {
        jvm->DetachCurrentThread();
    }
}


/**
 * Construct a scoped JNIEnv pointer.
 */
JScopedEnv::JScopedEnv()
    : env(GetEnv(&detached))
{
}

/**
 * Destroy a scoped JNIEnv pointer.
 */
JScopedEnv::~JScopedEnv()
{
    /* Clear any pending exceptions before detaching. */
    {
        JLocalRef<jthrowable> ex = env->ExceptionOccurred();
        if (ex) {
            env->ExceptionClear();
            env->CallStaticVoidMethod(CLS_BusException, MID_BusException_log, (jthrowable)ex);
        }
    }
    DeleteEnv(detached);
}

/**
 * Construct a representation of a string with wrapped StringUTFChars.
 *
 * @param s the string to wrap.
 */
JString::JString(jstring s)
    : jstr(s), str(jstr ? GetEnv()->GetStringUTFChars(jstr, NULL) : NULL)
{
}

/**
 * Destroy a string with wrapped StringUTFChars.
 */
JString::~JString()
{
    if (str) {
        GetEnv()->ReleaseStringUTFChars(jstr, str);
    }
}

/**
 * Helper function to throw an exception
 */
void Throw(const char* name, const char* msg)
{
    JNIEnv* env = GetEnv();
    JLocalRef<jclass> clazz = env->FindClass(name);
    if (clazz) {
        env->ThrowNew(clazz, msg);
    }
}

/**
 * Helper function to throw a bus exception
 */
void ThrowErrorReplyBusException(const char* name, const char* message)
{
    JNIEnv* env = GetEnv();
    JLocalRef<jstring> jname = env->NewStringUTF(name);
    if (!jname) {
        return;
    }
    JLocalRef<jstring> jmessage = env->NewStringUTF(message);
    if (!jmessage) {
        return;
    }
    jmethodID mid = env->GetMethodID(CLS_ErrorReplyBusException, "<init>",
                                     "(Ljava/lang/String;Ljava/lang/String;)V");
    JLocalRef<jthrowable> jexc = (jthrowable)env->NewObject(CLS_ErrorReplyBusException, mid,
                                                            (jstring)jname, (jstring)jmessage);
    if (jexc) {
        env->Throw(jexc);
    }
}

/**
 * Set the native C++ handle of a given Java object.
 *
 * If we have an object that has a native counterpart, we need a way to get at
 * the native object from the Java object.  We do this by storing the native
 * pointer as an opaque handle in a Java field named "handle".  We use Java
 * reflection to determine the field out and set the handle value.
 *
 * @param jobj The Java object which needs to have its handle set.
 * @param handle The pointer to the C++ object which is the handle value.
 *
 * @warning May throw an exception.
 */
void SetHandle(jobject jobj, void* handle, const char* member)
{
    JNIEnv* env = GetEnv();
    if (!jobj) {
        Throw("java/lang/NullPointerException", "failed to set native handle on null object");
        return;
    }
    JLocalRef<jclass> clazz = env->GetObjectClass(jobj);
    jfieldID fid = env->GetFieldID(clazz, member, "J");
    if (fid) {
        env->SetLongField(jobj, fid, (jlong)handle);
    }
}

/**
 * Given a Java listener object, return its corresponding C++ object.
 */
template <typename T>
T GetNativeListener(JNIEnv* env, jobject jlistener)
{
    return GetHandle<T>(jlistener);
}

/**
 * Translate a C++ return status code (QStatus) into a Java return status code
 * (JStatus).
 *
 * We have things called QStatus which are integers returned by the C++ side of
 * the bindings.  We need to translate those into a Java version (JStatus) that
 * serves the same purpose.
 *
 * @return A org.alljoyn.bus.Status enum value from the QStatus.
 */
jobject JStatus(QStatus status)
{
    JNIEnv* env = GetEnv();
    jmethodID mid = env->GetStaticMethodID(CLS_Status, "create", "(I)Lorg/alljoyn/bus/Status;");
    if (!mid) {
        return NULL;
    }
    return env->CallStaticObjectMethod(CLS_Status, mid, status);
}

