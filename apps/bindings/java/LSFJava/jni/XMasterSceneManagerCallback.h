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

#ifndef LSF_JNI_XMASTERSCENEMANAGERCALLBACK_H_
#define LSF_JNI_XMASTERSCENEMANAGERCALLBACK_H_

#include <jni.h>

#include <MasterSceneManager.h>   // lighting/service_framework

#include "NDefs.h"
#include "NTypes.h"

namespace lsf {

class XMasterSceneManagerCallback : public MasterSceneManagerCallback {
public:
    XMasterSceneManagerCallback(jobject jobj);
    virtual ~XMasterSceneManagerCallback();

    virtual void GetAllMasterSceneIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& masterSceneIDs) LSF_OVERRIDE;
    virtual void GetMasterSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID, const LSFString& language, const LSFString& masterSceneName) LSF_OVERRIDE;
    virtual void SetMasterSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID, const LSFString& language) LSF_OVERRIDE;
    virtual void MasterScenesNameChangedCB(const LSFStringList& masterSceneIDs) LSF_OVERRIDE;
    virtual void CreateMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID) LSF_OVERRIDE;
    virtual void MasterScenesCreatedCB(const LSFStringList& masterSceneIDs) LSF_OVERRIDE;
    virtual void GetMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID, const MasterScene& masterScene) LSF_OVERRIDE;
    virtual void DeleteMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID) LSF_OVERRIDE;
    virtual void MasterScenesDeletedCB(const LSFStringList& masterSceneIDs) LSF_OVERRIDE;
    virtual void UpdateMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID) LSF_OVERRIDE;
    virtual void MasterScenesUpdatedCB(const LSFStringList& masterSceneIDs) LSF_OVERRIDE;
    virtual void ApplyMasterSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& masterSceneID) LSF_OVERRIDE;
    virtual void MasterScenesAppliedCB(const LSFStringList& masterSceneIDs) LSF_OVERRIDE;

protected:
    jweak jdelegate;
};

} /* namespace lsf */
#endif /* LSF_JNI_XMASTERSCENEMANAGERCALLBACK_H_ */
