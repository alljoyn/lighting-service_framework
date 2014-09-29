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

#include "NUtil.h"
#include "JLampState.h"

#define QCC_MODULE "AJN-LSF-JNI"

using namespace ajn;
using namespace lsf;

JLampState::JLampState(jobject jobj)
{
    // Currently nothing to do
}

JLampState::~JLampState()
{
    // Currently nothing to do
}

JLampState& JLampState::operator=(const LampState& other)
{
    LampState::operator=(other);
    return *this;
}

jobject JLampState::NewLampState(LampState& cLampState)
{
    JScopedEnv env;

    jmethodID jconstructor = env->GetMethodID(XClass::xLampState->classRef, "<init>", "()V");
    if (env->ExceptionCheck() || !jconstructor) {
        QCC_LogError(ER_FAIL, ("GetMethodID() failed"));
        return NULL;
    }

    jobject jobjValue = env->NewObject(XClass::xLampState->classRef, jconstructor);
    if (env->ExceptionCheck() || !jobjValue) {
        QCC_LogError(ER_FAIL, ("NewObject() failed"));
        return NULL;
    }

    JLampState *xobjValue = GetHandle<JLampState *>(jobjValue);
    if (env->ExceptionCheck() || !xobjValue) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return NULL;
    }

    *xobjValue = cLampState;

    return jobjValue;
}

void JLampState::CopyLampState(jobject jSrcLampState, LampState& cDstLampState)
{
    JScopedEnv env;

    JLampState *xSrcLampState = GetHandle<JLampState*>(jSrcLampState);
    if (env->ExceptionCheck() || !xSrcLampState) {
        QCC_LogError(ER_FAIL, ("JString failed"));
        return;
    }

    cDstLampState = (LampState&)*xSrcLampState;
}

#undef QCC_MODULE

