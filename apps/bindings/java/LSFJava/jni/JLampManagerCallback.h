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

#ifndef LSF_JNI_JLAMPMANAGERCALLBACK_H_
#define LSF_JNI_JLAMPMANAGERCALLBACK_H_

#include <list>
#include <string>

#include <LampManager.h>    // lighting/service_framework

#include "NDefs.h"
#include "NUtil.h"
#include "NTypes.h"

namespace lsf {

class JLampManagerCallback : public LampManagerCallback {
  public:
    JLampManagerCallback(jobject jobj);
    virtual ~JLampManagerCallback();

    virtual void GetAllLampIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& lampIDs) LSF_OVERRIDE;
    virtual void GetLampNameReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LSFString& language, const LSFString& lampName) LSF_OVERRIDE;
    virtual void GetLampManufacturerReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LSFString& language, const LSFString& manufacturer) LSF_OVERRIDE;
    virtual void SetLampNameReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LSFString& language) LSF_OVERRIDE;
    virtual void LampNameChangedCB(const LSFString& lampID, const LSFString& lampName) LSF_OVERRIDE;
    virtual void LampsFoundCB(const LSFStringList& lampIDs) LSF_OVERRIDE;
    virtual void LampsLostCB(const LSFStringList& lampIDs) LSF_OVERRIDE;
    virtual void GetLampDetailsReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LampDetails& lampDetails) LSF_OVERRIDE;
    virtual void GetLampParametersReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LampParameters& lampParameters) LSF_OVERRIDE;
    virtual void GetLampParametersEnergyUsageMilliwattsFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& energyUsageMilliwatts) LSF_OVERRIDE;
    virtual void GetLampParametersLumensFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& brightnessLumens) LSF_OVERRIDE;
    virtual void GetLampStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LampState& lampState) LSF_OVERRIDE;
    virtual void GetLampStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const bool& onOff) LSF_OVERRIDE;
    virtual void GetLampStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& hue) LSF_OVERRIDE;
    virtual void GetLampStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& saturation) LSF_OVERRIDE;
    virtual void GetLampStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& brightness) LSF_OVERRIDE;
    virtual void GetLampStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& colorTemp) LSF_OVERRIDE;
    virtual void ResetLampStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void LampStateChangedCB(const LSFString& lampID, const LampState& lampState) LSF_OVERRIDE;
    virtual void TransitionLampStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void PulseLampWithStateReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void PulseLampWithPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void TransitionLampStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void TransitionLampStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void TransitionLampStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void TransitionLampStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void TransitionLampStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void GetLampFaultsReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LampFaultCodeList& faultCodes) LSF_OVERRIDE;
    virtual void GetLampServiceVersionReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const uint32_t& lampServiceVersion) LSF_OVERRIDE;
    virtual void ClearLampFaultReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LampFaultCode& faultCode) LSF_OVERRIDE;
    virtual void ResetLampStateOnOffFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void ResetLampStateHueFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void ResetLampStateSaturationFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void ResetLampStateBrightnessFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void ResetLampStateColorTempFieldReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void TransitionLampStateToPresetReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID) LSF_OVERRIDE;
    virtual void GetLampSupportedLanguagesReplyCB(const LSFResponseCode& responseCode, const LSFString& lampID, const LSFStringList& supportedLanguages) LSF_OVERRIDE;;

  protected:
    jweak jdelegate;
};

} /* namespace lsf */
#endif /* LSF_JNI_JLAMPMANAGERCALLBACK_H_ */
