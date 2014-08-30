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

#include <stddef.h>

#include <qcc/Debug.h>

#include "XLongArray.h"
#include "NUtil.h"

#define QCC_MODULE "AJN-LSF-JNI"

namespace lsf {

jlongArray XLongArray::NewLongArray(const std::list<uint32_t>& vals)
{
    JScopedEnv env;
    std::list<uint32_t>::size_type count = vals.size();

    jlongArray jarr = env->NewLongArray(count);
    if (env->ExceptionCheck() || !jarr) {
        QCC_LogError(ER_FAIL, ("NewLongArray() failed"));
        return NULL;
    }

    std::list<uint32_t>::const_iterator it = vals.begin();
    for (int i = 0; i < count; i++, it++) {
        jlong jval = (jlong)(*it);

        env->SetLongArrayRegion(jarr, i, 1, &jval);
        if (env->ExceptionCheck()) {
            QCC_LogError(ER_FAIL, ("SetLongArrayRegion() failed"));
            return NULL;
        }
    }

    return jarr;
}

void XLongArray::CopyLongArray(jlongArray jarr, std::list<uint32_t>& vals)
{
    JScopedEnv env;

    jsize count = env->GetArrayLength(jarr);
    if (env->ExceptionCheck() || (count < 0)) {
        QCC_LogError(ER_FAIL, ("GetArrayLength() invalid"));
        return;
    }

    jlong* jvals = env->GetLongArrayElements(jarr, NULL);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("GetLongArrayElements() invalid"));
        return;
    }

    vals.clear();

    for (jsize i = 0; i < count; i++) {
        vals.push_back((uint32_t)jvals[i]);
    }

    env->ReleaseLongArrayElements(jarr, jvals, JNI_ABORT);
    if (env->ExceptionCheck()) {
        QCC_LogError(ER_FAIL, ("ReleaseLongArrayElements() invalid"));
        return;
    }
}

} /* namespace lsf */

#undef QCC_MODULE
