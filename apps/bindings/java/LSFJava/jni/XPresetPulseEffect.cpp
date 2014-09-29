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

#include "NUtil.h"
#include "XObject.h"
#include "XPresetPulseEffect.h"

namespace lsf {

//TODO-FIX Refactor to common location?
static LSFStringList emptyStringList;
static LSFString emptyString;
static uint32_t emptyValue = 0;

XPresetPulseEffect::XPresetPulseEffect(jobject jobj) : PulseLampsLampGroupsWithPreset(emptyStringList, emptyStringList, emptyString, emptyString, emptyValue, emptyValue, emptyValue)
{
    // Currently nothing to do
}

XPresetPulseEffect::~XPresetPulseEffect()
{
    // Currently nothing to do
}

XPresetPulseEffect&
XPresetPulseEffect::operator=(const PulseLampsLampGroupsWithPreset& other)
{
    PulseLampsLampGroupsWithPreset::operator=(other);
    return *this;
}

jobjectArray
XPresetPulseEffect::NewArray(std::list<PulseLampsLampGroupsWithPreset>& cEffectList)
{
    return XObject::NewArray<PulseLampsLampGroupsWithPreset, XPresetPulseEffect>(XClass::xPresetPulseEffect, cEffectList);
}

void
XPresetPulseEffect::CopyArray(jobjectArray jObjectArray, std::list<PulseLampsLampGroupsWithPreset>& cEffectList)
{
    XObject::CopyArray<PulseLampsLampGroupsWithPreset, XPresetPulseEffect>(XClass::xPresetPulseEffect, jObjectArray, cEffectList);
}

} /* namespace lsf */
