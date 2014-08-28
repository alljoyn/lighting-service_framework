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

#ifndef LSF_JNI_XSCENEMANAGERCALLBACK_H_
#define LSF_JNI_XSCENEMANAGERCALLBACK_H_

#include <list>

#include <jni.h>

#include <SceneManager.h>   // lighting/service_framework

#include "NDefs.h"
#include "NUtil.h"
#include "NTypes.h"

namespace lsf {

class XSceneManagerCallback : public SceneManagerCallback {
  public:
    XSceneManagerCallback(jobject jobj);
    virtual ~XSceneManagerCallback();

    virtual void GetAllSceneIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& sceneIDs) LSF_OVERRIDE;
    virtual void GetSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const LSFString& language, const LSFString& sceneName) LSF_OVERRIDE;
    virtual void SetSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const LSFString& language) LSF_OVERRIDE;
    virtual void ScenesNameChangedCB(const LSFStringList& sceneIDs) LSF_OVERRIDE;
    virtual void CreateSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID) LSF_OVERRIDE;
    virtual void ScenesCreatedCB(const LSFStringList& sceneIDs) LSF_OVERRIDE;
    virtual void UpdateSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID) LSF_OVERRIDE;
    virtual void ScenesUpdatedCB(const LSFStringList& sceneIDs) LSF_OVERRIDE;
    virtual void DeleteSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID) LSF_OVERRIDE;
    virtual void ScenesDeletedCB(const LSFStringList& sceneIDs) LSF_OVERRIDE;
    virtual void GetSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const Scene& scene) LSF_OVERRIDE;
    virtual void ApplySceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID) LSF_OVERRIDE;
    virtual void ScenesAppliedCB(const LSFStringList& sceneIDs) LSF_OVERRIDE;

  protected:
    jweak jdelegate;
};

} /* namespace lsf */
#endif /* LSF_JNI_XSCENEMANAGERCALLBACK_H_ */
