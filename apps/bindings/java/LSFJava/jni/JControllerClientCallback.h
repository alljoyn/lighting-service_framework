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

#ifndef LSF_JNI_JCONTROLLERCLIENTCALLBACK_H_
#define LSF_JNI_JCONTROLLERCLIENTCALLBACK_H_

#include <string.h>

#include <jni.h>

#include <LampResponseCodes.h>  // lighting/service_framework
#include <ControllerClient.h>   // lighting/service_framework

#include "NDefs.h"
#include "NTypes.h"
#include "NUtil.h"

namespace lsf {

class JControllerClientCallback : public ControllerClientCallback {
  public:
    JControllerClientCallback(jobject jobj);
    virtual ~JControllerClientCallback();

    virtual void ConnectedToControllerServiceCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName) LSF_OVERRIDE;
    virtual void ConnectToControllerServiceFailedCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName) LSF_OVERRIDE;
    virtual void DisconnectedFromControllerServiceCB(const LSFString& controllerServiceDeviceID, const LSFString& controllerServiceName) LSF_OVERRIDE;
    virtual void ControllerClientErrorCB(const ErrorCodeList& errorCodeList) LSF_OVERRIDE;

  protected:
    jweak jdelegate;
};

} /* namespace lsf */
#endif /* LSF_JNI_JCONTROLLERCLIENTCALLBACK_H_ */
