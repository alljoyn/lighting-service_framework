#ifndef _MASTER_SCENE_MANAGER_H_
#define _MASTER_SCENE_MANAGER_H_
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

#include <Manager.h>
#include <SceneManager.h>

#include <Mutex.h>
#include <LSFTypes.h>

#include <string>
#include <map>

namespace lsf {

class MasterSceneManager : public Manager {

  public:
    MasterSceneManager(ControllerService& controllerSvc, SceneManager& sceneMgr, const std::string& masterSceneFile);

    LSFResponseCode Reset(void);
    LSFResponseCode IsDependentOnScene(LSFString& sceneID);

    void GetAllMasterSceneIDs(ajn::Message& message);
    void GetMasterSceneName(ajn::Message& message);
    void SetMasterSceneName(ajn::Message& message);
    void DeleteMasterScene(ajn::Message& message);
    void CreateMasterScene(ajn::Message& message);
    void UpdateMasterScene(ajn::Message& message);
    void GetMasterScene(ajn::Message& message);
    void ApplyMasterScene(ajn::Message& message);

    void SendMasterSceneAppliedSignal(LSFString& sceneorMasterSceneId);

    LSFResponseCode GetAllMasterScenes(MasterSceneMap& masterSceneMap);

    void ReadSavedData();
    void ReadWriteFile();

    uint32_t GetControllerServiceMasterSceneInterfaceVersion(void);

    virtual bool GetString(std::string& output, uint32_t& checksum, uint64_t& timestamp);

    void GetBlobInfo(uint32_t& checksum, uint64_t& timestamp) {
        masterScenesLock.Lock();
        GetBlobInfoInternal(checksum, timestamp);
        masterScenesLock.Unlock();
    }

    void HandleReceivedBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp);

  private:

    void ReplaceMap(std::istringstream& stream);

    MasterSceneMap masterScenes;
    Mutex masterScenesLock;
    SceneManager& sceneManager;
    size_t blobLength;

    std::string GetString(const MasterSceneMap& items);
    std::string GetString(const std::string& name, const std::string& id, const MasterScene& msc);
};

}

#endif
