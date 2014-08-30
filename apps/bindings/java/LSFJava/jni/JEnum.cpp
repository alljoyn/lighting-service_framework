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

#include <stdio.h>

#include <qcc/Debug.h>

#include "NUtil.h"
#include "JEnum.h"

#define QCC_MODULE "AJN-LSF-JNI"

namespace lsf {

JEnum* JEnum::jControllerClientStatusEnum = new JEnum("org/allseen/lsf/ControllerClientStatus");
JEnum* JEnum::jErrorCodeEnum = new JEnum("org/allseen/lsf/ErrorCode");
JEnum* JEnum::jResponseCodeEnum = new JEnum("org/allseen/lsf/ResponseCode");
JEnum* JEnum::jStatusCodeEnum = new JEnum("org/alljoyn/bus/Status", "create", "getErrorCode");

JEnum* JEnum::jLampMakeEnum = new JEnum("org/allseen/lsf/LampMake");
JEnum* JEnum::jLampModelEnum = new JEnum("org/allseen/lsf/LampModel");
JEnum* JEnum::jDeviceTypeEnum = new JEnum("org/allseen/lsf/DeviceType");
JEnum* JEnum::jLampTypeEnum = new JEnum("org/allseen/lsf/LampType");
JEnum* JEnum::jBaseTypeEnum = new JEnum("org/allseen/lsf/BaseType");

JEnum::JEnum(char const *className)
{
    init(className, "fromValue", "getValue");
}

JEnum::JEnum(char const *className, char const *objMethodName, char const *intMethodName)
{
    init(className, objMethodName, intMethodName);
}

JEnum::~JEnum()
{
    JScopedEnv env;

    if (clsEnum) {
        env->DeleteGlobalRef(clsEnum);
    }
}

char const *JEnum::getClassName()
{
    return clsName;
}

jclass JEnum::getClass()
{
    return clsEnum;
}

jobject JEnum::getObject(int value)
{
    JScopedEnv env;

    if (!clsEnum) {
        create();
    }

    return (clsEnum && midFromValue) ? env->CallStaticObjectMethod(clsEnum, midFromValue, value) : NULL;
}

jint JEnum::getValue(jobject jobj)
{
    JScopedEnv env;

    if (!clsEnum) {
        create();
    }

    return (clsEnum && midGetValue) ? env->CallIntMethod(jobj, midGetValue) : (jint)-1;
}

void JEnum::init(char const *className, char const *objMethodName, char const *intMethodName)
{
    clsName = className;
    objMetName = objMethodName;
    intMetName = intMethodName;
    clsEnum = NULL;
    midFromValue = NULL;
    midGetValue = NULL;
}

void JEnum::create()
{
    JScopedEnv env;

    jclass clazz = env->FindClass(clsName);
    if (env->ExceptionCheck() || !clazz) {
        QCC_LogError(ER_FAIL, ("FindClass() failed"));
        return;
    }

    clsEnum = (jclass)env->NewGlobalRef(clazz);
    if (env->ExceptionCheck() || !clsEnum) {
        QCC_LogError(ER_FAIL, ("NewGlobalRef() failed"));
        return;
    }

    char sig[128];
    snprintf(sig, sizeof(sig), "(I)L%s;", clsName);

    midFromValue = env->GetStaticMethodID(clsEnum, objMetName, sig);
    if (env->ExceptionCheck() || !midFromValue) {
        QCC_LogError(ER_FAIL, ("GetStaticMethodID() failed"));
        return;
    }

    midGetValue = env->GetMethodID(clsEnum, intMetName, "()I");
    if (env->ExceptionCheck() || !midGetValue) {
        QCC_LogError(ER_FAIL, ("GetMethodID() failed"));
        return;
    }
}

} /* namespace lsf */
