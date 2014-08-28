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

#ifndef _LSF_SCENEMANAGER_CALLBACK_H
#define _LSF_SCENEMANAGER_CALLBACK_H

#import "LSFSceneManagerCallbackDelegate.h"
#import "SceneManager.h"

using namespace lsf;

class LSFSceneManagerCallback : public SceneManagerCallback {
public:
    LSFSceneManagerCallback(id<LSFSceneManagerCallbackDelegate> delegate);
    ~LSFSceneManagerCallback();
    void GetAllSceneIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& sceneIDs);
    void GetSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const LSFString& language, const LSFString& sceneName);
    void SetSceneNameReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const LSFString& language);
    void ScenesNameChangedCB(const LSFStringList& sceneIDs);
    void CreateSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID);
    void ScenesCreatedCB(const LSFStringList& sceneIDs);
    void UpdateSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID);
    void ScenesUpdatedCB(const LSFStringList& sceneIDs);
    void DeleteSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID);
    void ScenesDeletedCB(const LSFStringList& sceneIDs);
    void GetSceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID, const Scene& data);
    void ApplySceneReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneID);
    void ScenesAppliedCB(const LSFStringList& sceneIDs);
    
private:
    id<LSFSceneManagerCallbackDelegate> _smDelegate;
};

#endif /* defined(_LSF_SCENEMANAGER_CALLBACK_H) */
