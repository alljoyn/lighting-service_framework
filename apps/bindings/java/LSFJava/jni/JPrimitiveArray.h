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

#ifndef LSF_JNI_JPRIMITIVEARRAY_H_
#define LSF_JNI_JPRIMITIVEARRAY_H_

#include <jni.h>
#include <stddef.h>

#include "NUtil.h"

namespace lsf {

template <typename JTYPE, typename CTYPE>
class JPrimitiveArray {
  public:
    JPrimitiveArray(JTYPE jarr);
    virtual ~JPrimitiveArray();

    CTYPE* c_arr();
    jsize len();

  protected:
    typedef CTYPE* (JNIEnv::*GetMethod)(JTYPE, jboolean *);
    typedef void (JNIEnv::*ReleaseMethod)(JTYPE, CTYPE*, jint);

    JTYPE jarray;
    CTYPE* carray;
    jsize length;

    GetMethod getMethod;
    ReleaseMethod releaseMethod;

    void Reset();
    void Create();
    void Destroy();

    virtual GetMethod GetMethodToGetPrimitiveArrayElements() = 0;
    virtual ReleaseMethod GetMethodToReleasePrimitiveArrayElements() = 0;
};

} /* namespace lsf */

// The templated method definitions must be included in this header
#include "JPrimitiveArray.cpp"

#endif /* LSF_JNI_JPRIMITIVEARRAY_H_ */
