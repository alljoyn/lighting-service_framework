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

// The template definitions below are intended to be #include'd from
// the corresponding .h file, hence the #ifdef guard
#ifdef LSF_JNI_JPRIMITIVEARRAY_H_

#define QCC_MODULE LSF_QCC_MODULE

namespace lsf {

template <typename JTYPE, typename CTYPE>
JPrimitiveArray<JTYPE, CTYPE>::JPrimitiveArray(JTYPE jarr)
{
    jarray = jarr;

    Reset();
}

template <typename JTYPE, typename CTYPE>
JPrimitiveArray<JTYPE, CTYPE>::~JPrimitiveArray()
{
    Destroy();
}

template <typename JTYPE, typename CTYPE>
CTYPE* JPrimitiveArray<JTYPE, CTYPE>::c_arr()
{
    Create();
    return carray;
}

template <typename JTYPE, typename CTYPE>
jsize JPrimitiveArray<JTYPE, CTYPE>::len()
{
    Create();
    return length;
}

template <typename JTYPE, typename CTYPE>
void JPrimitiveArray<JTYPE, CTYPE>::Reset()
{
    carray = NULL;
    length = 0;

    getMethod = NULL;
    releaseMethod = NULL;
}

template <typename JTYPE, typename CTYPE>
void JPrimitiveArray<JTYPE, CTYPE>::Create()
{
    if (jarray && !carray) {
        JNIEnv* env = GetEnv();

        getMethod = GetMethodToGetPrimitiveArrayElements();
        releaseMethod = GetMethodToReleasePrimitiveArrayElements();

        if (getMethod && releaseMethod) {
            carray = (env->*getMethod)(jarray, NULL);

            if (!env->ExceptionCheck() && carray) {
                length = env->GetArrayLength(jarray);
            } else {
                QCC_LogError(ER_FAIL, ("GetPrimitiveArrayElements() failed"));
                Reset();
            }
        }
    }
}

template <typename JTYPE, typename CTYPE>
void JPrimitiveArray<JTYPE, CTYPE>::Destroy()
{
    if (jarray && carray) {
        (GetEnv()->*releaseMethod)(jarray, carray, 0);
        Reset();
    }
}

} /* namespace lsf */

#undef QCC_MODULE

#endif /* LSF_JNI_JPRIMITIVEARRAY_H_ */
