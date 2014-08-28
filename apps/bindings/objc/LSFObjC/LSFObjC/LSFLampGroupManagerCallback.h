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
 ******************************************************************************/

#ifndef _LSF_GROUPMANAGER_CALLBACK_H
#define _LSF_GROUPMANAGER_CALLBACK_H

#import "LSFLampGroupManagerCallbackDelegate.h"
#import "LampGroupManager.h"

using namespace lsf;

class LSFLampGroupManagerCallback : public LampGroupManagerCallback {
  public:
    LSFLampGroupManagerCallback(id<LSFLampGroupManagerCallbackDelegate> delegate);
    ~LSFLampGroupManagerCallback();    
    void GetAllLampGroupIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& lampGroupIDs);
    void GetLampGroupNameReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID, const LSFString& language, const LSFString& lampGroupName);
    void SetLampGroupNameReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID, const LSFString& language);
    void LampGroupsNameChangedCB(const LSFStringList& lampGroupIDs);
    void CreateLampGroupReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void LampGroupsCreatedCB(const LSFStringList& lampGroupIDs);
    void GetLampGroupReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID, const LampGroup& lampGroup);
    void DeleteLampGroupReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void LampGroupsDeletedCB(const LSFStringList& lampGroupIDs);
    void TransitionLampGroupStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void PulseLampGroupWithStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID);
    void PulseLampGroupWithPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID);
    void TransitionLampGroupStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void TransitionLampGroupStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void TransitionLampGroupStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void TransitionLampGroupStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void TransitionLampGroupStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void ResetLampGroupStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void ResetLampGroupStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void ResetLampGroupStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void ResetLampGroupStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void ResetLampGroupStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void ResetLampGroupStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void UpdateLampGroupReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);
    void LampGroupsUpdatedCB(const LSFStringList& lampGroupIDs);
    void TransitionLampGroupStateToPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& lampGroupID);

  private:
    id<LSFLampGroupManagerCallbackDelegate> _lgmDelegate;
};

#endif /* defined(_LSF_GROUPMANAGER_CALLBACK_H) */