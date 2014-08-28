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

#ifndef LSF_JNI_XPRESETMANAGERCALLBACK_H_
#define LSF_JNI_XPRESETMANAGERCALLBACK_H_

#include <list>

#include <jni.h>

#include <PresetManager.h>

#include "NDefs.h"
#include "NUtil.h"
#include "NTypes.h"

namespace lsf {

class XPresetManagerCallback : public PresetManagerCallback {
  public:
    XPresetManagerCallback(jobject jobj);
    virtual ~XPresetManagerCallback();

    virtual void GetPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID, const LampState& preset) LSF_OVERRIDE;
    virtual void GetAllPresetIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& presetIDs) LSF_OVERRIDE;
    virtual void GetPresetNameReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID, const LSFString& language, const LSFString& presetName) LSF_OVERRIDE;
    virtual void SetPresetNameReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID, const LSFString& language) LSF_OVERRIDE;
    virtual void PresetsNameChangedCB(const LSFStringList& presetIDs) LSF_OVERRIDE;
    virtual void CreatePresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID) LSF_OVERRIDE;
    virtual void PresetsCreatedCB(const LSFStringList& presetIDs) LSF_OVERRIDE;
    virtual void UpdatePresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID) LSF_OVERRIDE;
    virtual void PresetsUpdatedCB(const LSFStringList& presetIDs) LSF_OVERRIDE;
    virtual void DeletePresetReplyCB(const LSFResponseCode& responseCode, const LSFString& presetID) LSF_OVERRIDE;
    virtual void PresetsDeletedCB(const LSFStringList& presetIDs) LSF_OVERRIDE;

  protected:
    jweak jdelegate;
};

} /* namespace lsf */
#endif /* LSF_JNI_XPRESETMANAGERCALLBACK_H_ */
