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
#ifdef LSF_JNI_XTRANSITIONEFFECT_H_INCLUDE_TEMPLATE_METHODS

#include <qcc/Debug.h>

#include "NDefs.h"
#include "XTransitionEffect.h"

#define QCC_MODULE LSF_QCC_MODULE

namespace lsf {

template <typename T>
void XTransitionEffect::SetTransitionPeriod(JNIEnv *env, jobject thiz, jlong jTransitionPeriod)
{
    T *xDelegate = GetHandle<T*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return;
    }

    xDelegate->transitionPeriod = (uint32_t)jTransitionPeriod;
}

template <typename T>
jlong XTransitionEffect::GetTransitionPeriod(JNIEnv *env, jobject thiz)
{
    T *xDelegate = GetHandle<T*>(thiz);
    if (env->ExceptionCheck() || !xDelegate) {
        QCC_LogError(ER_FAIL, ("GetHandle() failed"));
        return (jint)0;
    }

    return (jlong)xDelegate->transitionPeriod;
}

} /* namespace lsf */

#undef QCC_MODULE

#endif /* LSF_JNI_XPULSEEFFECT_H_INCLUDE_TEMPLATE_METHODS */
