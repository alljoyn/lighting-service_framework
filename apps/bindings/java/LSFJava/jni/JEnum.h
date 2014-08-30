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

#ifndef LSF_JNI_JENUM_H_
#define LSF_JNI_JENUM_H_

#include <jni.h>

namespace lsf {

class JEnum {
  public:
    static JEnum * jControllerClientStatusEnum;
    static JEnum * jErrorCodeEnum;
    static JEnum * jResponseCodeEnum;
    static JEnum * jStatusCodeEnum;

    static JEnum * jLampMakeEnum;
    static JEnum * jLampModelEnum;
    static JEnum * jDeviceTypeEnum;
    static JEnum * jLampTypeEnum;
    static JEnum * jBaseTypeEnum;

    JEnum(char const *className);
    JEnum(char const *className, char const *objMethodName, char const *intMethodName);
    virtual ~JEnum();

    char const *getClassName();
    jclass getClass();

    jobject getObject(int value);
    jint getValue(jobject obj);

  protected:
    char const *clsName;
    char const *objMetName;
    char const *intMetName;
    jclass clsEnum;
    jmethodID midFromValue;
    jmethodID midGetValue;

    void init(char const *className, char const *objMethodName, char const *intMethodName);
    void create();
};

} /* namespace lsf */

#endif /* LSF_JNI_JENUM_H_ */
