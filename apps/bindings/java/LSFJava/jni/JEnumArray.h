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

#ifndef LSF_JNI_JENUMARRAY_H_
#define LSF_JNI_JENUMARRAY_H_

#include <list>
#include <stddef.h>

#include <jni.h>

#include <qcc/Debug.h>

#include "JEnum.h"

#include "NDefs.h"
#include "NUtil.h"

namespace lsf {

class JEnumArray {
public:
    template <typename T> static jobjectArray NewEnumArray(JEnum &jenum, const std::list<T> &cenums);
    template <typename T> static void CopyEnumArray(jobjectArray jarr, JEnum &jenum, std::list<T> &cenums);
};

} /* namespace lsf */

// The templated method definitions must be included in this header
#include "JEnumArray.cpp"

#endif /* LSF_JNI_JENUMARRAY_H_ */

