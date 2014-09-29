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
#include "XPresetTransitionEffect.h"

namespace lsf {

//TODO-FIX Refactor to common location?
static LSFStringList emptyStringList;
static LSFString emptyString;
static uint32_t emptyValue = 0;

XPresetTransitionEffect::XPresetTransitionEffect(jobject jobj) : TransitionLampsLampGroupsToPreset(emptyStringList, emptyStringList, emptyString, emptyValue)
{
    // Currently nothing to do
}

XPresetTransitionEffect::~XPresetTransitionEffect()
{
    // Currently nothing to do
}

XPresetTransitionEffect&
XPresetTransitionEffect::operator=(const TransitionLampsLampGroupsToPreset& other)
{
    TransitionLampsLampGroupsToPreset::operator=(other);
    return *this;
}

jobjectArray
XPresetTransitionEffect::NewArray(std::list<TransitionLampsLampGroupsToPreset>& cEffectList)
{
    return XObject::NewArray<TransitionLampsLampGroupsToPreset, XPresetTransitionEffect>(XClass::xPresetTransitionEffect, cEffectList);
}

void
XPresetTransitionEffect::CopyArray(jobjectArray jObjectArray, std::list<TransitionLampsLampGroupsToPreset>& cEffectList)
{
    XObject::CopyArray<TransitionLampsLampGroupsToPreset, XPresetTransitionEffect>(XClass::xPresetTransitionEffect, jObjectArray, cEffectList);
}

} /* namespace lsf */
