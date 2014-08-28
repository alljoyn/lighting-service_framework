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

#ifndef LSF_JNI_JLAMPGROUPMANAGERCALLBACK_H_
#define LSF_JNI_JLAMPGROUPMANAGERCALLBACK_H_

#include <list>
#include <string>

#include <LampGroupManager.h>   // lighting/service_framework

#include "NDefs.h"
#include "NUtil.h"
#include "NTypes.h"

namespace lsf {

class JLampGroupManagerCallback : public LampGroupManagerCallback {
  public:
    JLampGroupManagerCallback(jobject jobj);
    virtual ~JLampGroupManagerCallback();

    virtual void GetAllLampGroupIDsReplyCB(const LSFResponseCode& responseCode, const LampGroupIDList& lampGroupIDs) LSF_OVERRIDE;
    virtual void GetLampGroupNameReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID, const LSFString& language, const LampGroupName& lampGroupName) LSF_OVERRIDE;
    virtual void SetLampGroupNameReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID, const LSFString& language) LSF_OVERRIDE;
    virtual void LampGroupsNameChangedCB(const LampGroupIDList& lampGroupIDs) LSF_OVERRIDE;
    virtual void CreateLampGroupReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void LampGroupsCreatedCB(const LampGroupIDList& lampGroupIDs) LSF_OVERRIDE;
    virtual void GetLampGroupReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID, const LampGroup& lampGroup) LSF_OVERRIDE;
    virtual void DeleteLampGroupReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void LampGroupsDeletedCB(const LampGroupIDList& lampGroupIDs) LSF_OVERRIDE;
    virtual void TransitionLampGroupStateReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void PulseLampGroupWithStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID) LSF_OVERRIDE;
    virtual void PulseLampGroupWithPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID) LSF_OVERRIDE;
    virtual void TransitionLampGroupStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void TransitionLampGroupStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void TransitionLampGroupStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void TransitionLampGroupStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void TransitionLampGroupStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void ResetLampGroupStateReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void ResetLampGroupStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void ResetLampGroupStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void ResetLampGroupStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void ResetLampGroupStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void ResetLampGroupStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void UpdateLampGroupReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;
    virtual void LampGroupsUpdatedCB(const LampGroupIDList& lampGroupIDs) LSF_OVERRIDE;
    virtual void TransitionLampGroupStateToPresetReplyCB(const LSFResponseCode& responseCode, const LampGroupID& lampGroupID) LSF_OVERRIDE;

  protected:
    jweak jdelegate;
};

} /* namespace lsf */
#endif /* LSF_JNI_JLAMPGROUPMANAGERCALLBACK_H_ */
