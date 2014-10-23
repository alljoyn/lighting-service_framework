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

#ifndef LSF_JNI_XCONTROLLERSERVICEMANAGERCALLBACK_H_
#define LSF_JNI_XCONTROLLERSERVICEMANAGERCALLBACK_H_

#include <jni.h>

#include <ControllerServiceManager.h>   // lighting/service_framework

#include "NDefs.h"

namespace lsf {

class XControllerServiceManagerCallback : public ControllerServiceManagerCallback {
public:
    XControllerServiceManagerCallback(jobject jobj);
    virtual ~XControllerServiceManagerCallback();

    virtual void GetControllerServiceVersionReplyCB(const uint32_t& version) LSF_OVERRIDE;
    virtual void LightingResetControllerServiceReplyCB(const LSFResponseCode& responseCode) LSF_OVERRIDE;
    virtual void ControllerServiceLightingResetCB(void) LSF_OVERRIDE;
    virtual void ControllerServiceNameChangedCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName) LSF_OVERRIDE;

protected:
    jweak jdelegate;
};

} /* namespace lsf */
#endif /* LSF_JNI_XCONTROLLERSERVICEMANAGERCALLBACK_H_ */

