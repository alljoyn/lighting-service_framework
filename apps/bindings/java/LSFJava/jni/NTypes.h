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
#ifndef _LSF_ALLJOYN_JAVA_TYPES_H
#define _LSF_ALLJOYN_JAVA_TYPES_H

#include <LSFTypes.h>   // lighting/service_framework

namespace lsf {
//typedef LSFString LampID;
typedef LSFString LampGroupID;
//typedef LSFString SavedStateID;
//typedef LSFString SceneID;

//typedef LSFStringList LampIDList;
typedef LSFStringList LampGroupIDList;
//typedef LSFStringList SavedStateIDList;
//typedef LSFStringList SceneIDList;
//typedef LSFStringList LanguageCodeList;

//typedef LSFString LampName;
typedef LSFString LampGroupName;
//typedef LSFString ManufacturerName;
//typedef LSFString StateFieldName;

//typedef LSFString DeviceID;
//typedef LSFString ServiceName;
//typedef LSFString LanguageCode;

typedef std::list<uint32_t> UInt32List;
typedef std::list<uint32_t> EnumList;
}

#endif /* _LSF_ALLJOYN_JAVA_TYPES_H */
