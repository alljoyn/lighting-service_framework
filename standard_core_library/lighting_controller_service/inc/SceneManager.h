#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_
/**
 * \ingroup ControllerService
 */
/**
 * \file lighting_controller_service/inc/SceneManager.h
 * This file provides definitions for scene manager
 */
/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#ifdef LSF_BINDINGS
#include <lsf/controllerservice/Manager.h>
#include <lsf/controllerservice/LampGroupManager.h>
#include <lsf/controllerservice/SceneElementManager.h>
#else
#include <Manager.h>
#include <LampGroupManager.h>
#include <SceneElementManager.h>
#endif

#include <Mutex.h>
#include <LSFTypes.h>

#include <string>
#include <map>
#include "LSFNamespaceSpecifier.h"

namespace lsf {

OPTIONAL_NAMESPACE_CONTROLLER_SERVICE

class MasterSceneManager;
class SceneObject;

/**
 * scene file management class
 */
class SceneFileManager : public Manager {
  public:
    /**
     * SceneFileManager constructor
     */
    SceneFileManager(ControllerService& controllerSvc, const std::string& filePathToManager = "", const std::string& scene2filePath = "");

    /**
     * Schedule the New Scene File Read
     */
    void ScheduleScene2FileRead(ajn::Message& message);

    /**
     * Read from the New Scene File
     */
    bool ValidateScene2FileAndRead(std::istringstream& filestream);

    /**
     * Read from the New Scene File
     */
    bool ValidateScene2FileAndReadInternal(uint32_t& checksum, uint64_t& timestamp, std::istringstream& filestream);

    /**
     * Get New Scene File information \n
     * Answer returns synchronously by the reference parameters
     * @param checksum
     * @param time
     */
    void GetScene2BlobInfoInternal(uint32_t& checksum, uint64_t& time);

    /**
     * Write New Scene File With Checksum And Timestamp
     */
    void WriteScene2FileWithChecksumAndTimestamp(const std::string& str, uint32_t checksum, uint64_t timestamp);

    const std::string scene2FilePath;               /**< the file location of the new SceneWithSceneElements file */
    uint32_t scene2CheckSum;                        /**< checkSum of the new SceneWithSceneElements file */
    uint64_t scene2TimeStamp;                       /**< timestamp of the new SceneWithSceneElements file */
    std::list<ajn::Message> readScene2BlobMessages; /**< Read new Scene blob messages */
};
/**
 * scene management class
 */
class SceneManager : public SceneFileManager {
    friend class MasterSceneManager;
    friend class SceneObject;
    friend class SceneElementManager;
  public:
    /**
     * SceneManager CTOR
     */
    SceneManager(ControllerService& controllerSvc, SceneElementManager* sceneElementMgr, MasterSceneManager* masterSceneMgr, const std::string& sceneFile, const std::string& sceneWithSceneElementsFile);
    /**
     * UnregisterSceneEventActionObjects. \n
     * The method is called during controller service stop(). \n
     * Deletes all SceneObject instances. \n
     * No event and action is possible after this method.
     */
    void UnregisterSceneEventActionObjects(void);
    /**
     * Clearing all scenes.
     * @return
     *      LSF_OK - in case operation succeeded
     */
    LSFResponseCode Reset(void);
    /**
     * Check if there a scene that depends on specific scene element. \n
     * @param sceneElementID - the scene element id
     * @return LSF_OK if there is not dependency
     *         LSF_ERR_DEPENDENCY if there is dependency
     */
    LSFResponseCode IsDependentOnSceneElement(LSFString& sceneElementID);
    /**
     * Get All Scene IDs. \n
     * Return asynchronous reply with response code: \n
     *  LSF_OK - operation succeeded
     */
    void GetAllSceneIDs(ajn::Message& message);
    /**
     * Get Scene name. \n
     * @param message type Message contains: scene unique id (type 's') and requested language (type 's') \n
     * Return asynchronous reply with response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_NOT_FOUND  - the scene not found \n
     *  LSF_ERR_INVALID_ARGS - Language not supported
     */
    void GetSceneName(ajn::Message& message);
    /**
     * Set Scene name. \n
     * @param message with  MsgArgs -  unique id (signature 's'), name (signature 's'), language (signature 's'). \n
     * Return asynchronous reply with response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, length exceeds LSF_MAX_NAME_LENGTH \n
     *  LSF_ERR_EMPTY_NAME - scene name is empty \n
     *  LSF_ERR_RESOURCES - blob length is longer than MAX_FILE_LEN
     */
    void SetSceneName(ajn::Message& message);
    /**
     * Delete Scene \n
     * @param message type Message. Contains one MsgArg with scene id. \n
     * Return asynchronous reply with response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_NOT_FOUND - can't find scene id
     */
    void DeleteScene(ajn::Message& message);
    /**
     * Create Scene and sending signal 'ScenesCreated' \n
     * @param message (type Message) with 4 message arguments as parameters (type ajn::MsgArg). \n
     * The arguments should have the following types: \n
     *      TransitionLampsLampGroupsToState \n
     *      TransitionLampsLampGroupsToPreset \n
     *      PulseLampsLampGroupsWithState \n
     *      PulseLampsLampGroupsWithPreset
     *
     * Return asynchronous reply with response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, scene name is empty, Invalid Scene components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new Scene
     */
    void CreateScene(ajn::Message& message);
    /**
     * Create Scene with SceneElements and sending signal 'ScenesCreated' \n
     * @param message (type Message) with 3 message arguments as parameters (type ajn::MsgArg). \n
     * The arguments should have the following types: \n
     *      LSFStringList (list of SceneElement IDs)\n
     *      LSFString (name of the Scene)\n
     *      LSFString (language code for the name of the Scene)\n
     *
     * Return asynchronous reply with response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, scene name is empty, Invalid Scene components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new Scene
     */
    void CreateSceneWithSceneElements(ajn::Message& message);
    /**
     * Modify an existing scene and then sending signal 'ScenesUpdated' \n
     * @param message (type Message) with 4 message arguments as parameters (type ajn::MsgArg). \n
     * The arguments should have the following types: \n
     *      TransitionLampsLampGroupsToState \n
     *      TransitionLampsLampGroupsToPreset \n
     *      PulseLampsLampGroupsWithState \n
     *      PulseLampsLampGroupsWithPreset
     */
    void UpdateScene(ajn::Message& message);
    /**
     * Modify an existing scene with SceneElements and then sending signal 'ScenesUpdated' \n
     * @param message (type Message) with 2 message arguments as parameters (type ajn::MsgArg). \n
     * The arguments should have the following types: \n
     *      LSFString (ID of the Scene to modify)\n
     *      LSFStringList (updated list of SceneElement IDs)\n
     */
    void UpdateSceneWithSceneElements(ajn::Message& message);
    /**
     * Get Scene. - reply asynchronously with scene content: \n
     *  TransitionLampsLampGroupsToState \n
     *  TransitionLampsLampGroupsToPreset \n
     *  PulseLampsLampGroupsWithState \n
     *  PulseLampsLampGroupsWithPreset \n
     * @param message type Message contains MsgArg with parameter unique id (type 's') \n
     *  return LSF_OK \n
     *  return LSF_ERR_NOT_FOUND - scene not found
     */
    void GetScene(ajn::Message& message);
    /**
     * Get Scene with SceneElements - reply asynchronously with scene content: \n
     *      LSFStringList (list of SceneElement IDs)\n
     * @param message type Message contains MsgArg with parameter unique id (type 's') \n
     *  return LSF_OK \n
     *  return LSF_ERR_NOT_FOUND - scene not found
     */
    void GetSceneWithSceneElements(ajn::Message& message);
    /**
     * Apply Scene. \n
     * @param message type Message with MsgArg parameter - scene id (type 's') \n
     * reply asynchronously with response code: \n
     *  LSF_OK - on success \n
     *  LSF_ERR_NOT_FOUND - scene id not found in current list of scenes
     */
    void ApplyScene(ajn::Message& message);
    /**
     * Send Scene Or Master Scene Applied Signal. \n
     * Sending signals in case a scene applied: \n
     *       Sessionless signal 'SceneApplied' for event and action mechanism. \n
     *      'MasterScenesApplied' signal - in case that master scene applied. \n
     *      'ScenesApplied' signal - in case that scene applied. \n
     *
     * @param sceneorMasterSceneId - the applied scene id or master scene id
     */
    void SendSceneOrMasterSceneAppliedSignal(LSFString& sceneorMasterSceneId);
    /**
     * Get All Scenes. \n
     * Return asynchronous answer - the all scenes by its reference parameter \n
     * @param sceneMap of type SceneMap - reference of sceneMap to get all scenes \n
     * @return LSF_OK on success.
     */
    LSFResponseCode GetAllScenes(SceneMap& sceneMap);
    /**
     * Read Write File \n
     * Reading scenes information from the persistent data and might update other interested controller services by sending blob messages.
     */
    void ReadWriteFile();
    /**
     * Read Saved Data. \n
     * Reads saved info from persistent data
     */
    void ReadSavedData();
    /**
     * Get the version of the scene inerface. \n
     * Return asynchronously. \n
     * @return version of the scene inerface
     */
    uint32_t GetControllerServiceSceneInterfaceVersion(void);
    /**
     * Get string representation of scene objects. \n
     * @param output - string representation of scene objects
     * @param checksum - of the output
     * @param timestamp - current time
     */
    bool GetString(std::string& output, std::string& updates, std::string& scene2, uint32_t& checksum, uint64_t& timestamp, uint32_t& updatesChksum, uint64_t& updatesTs, uint32_t& scene2Chksum, uint64_t& scene2Ts);
    /**
     * Get file information. \n
     * Derived from Manager class. \n
     * Answer returns synchronously by the reference parameters.
     * @param checksum
     * @param timestamp
     */
    void GetBlobInfo(uint32_t& checksum, uint64_t& timestamp) {
        scenesLock.Lock();
        GetBlobInfoInternal(checksum, timestamp);
        scenesLock.Unlock();
    }
    /**
     * Get file information. \n
     * Derived from Manager class. \n
     * Answer returns synchronously by the reference parameters.
     * @param checksum
     * @param timestamp
     */
    void GetUpdateBlobInfo(uint32_t& checksum, uint64_t& timestamp) {
        scenesLock.Lock();
        GetUpdateBlobInfoInternal(checksum, timestamp);
        scenesLock.Unlock();
    }
    /**
     * Get file information. \n
     * Derived from Manager class. \n
     * Answer returns synchronously by the reference parameters.
     * @param checksum
     * @param timestamp
     */
    void GetScene2BlobInfo(uint32_t& checksum, uint64_t& timestamp) {
        scenesLock.Lock();
        GetScene2BlobInfoInternal(checksum, timestamp);
        scenesLock.Unlock();
    }
    /**
     * Write the blob containing scene information to persistent data. \n
     * @param blob - string containing scenes information.
     * @param checksum
     * @param timestamp
     */
    void HandleReceivedBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp);
    /**
     * Handle Received Update Blob
     */
    void HandleReceivedUpdateBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp);
    /**
     * Handle Received Scene2 Blob
     */
    void HandleReceivedScene2Blob(const std::string& blob, uint32_t checksum, uint64_t timestamp);
    /*
     * Done with blob sync
     */
    void RefreshSceneData(bool isLeader = false) {
        scenesLock.Lock();
        SyncSceneData(isLeader);
        ScheduleFileWrite();
        scenesLock.Unlock();
    }

  private:

    void ReplaceMap(std::istringstream& stream);

    void ReplaceUpdatesList(std::istringstream& stream);

    void ReplaceScene2List(std::istringstream& stream);

    LSFResponseCode ApplySceneNestedInternal(ajn::Message message, LSFStringList& sceneList, LSFString sceneOrMasterSceneId);

    LSFResponseCode CreateSceneWithSceneElements(Scene& scene, SceneWithSceneElements& sceneWithSceneElements);

    LSFResponseCode CreateScene(Scene& scene, SceneWithSceneElements& sceneWithSceneElements);

    LSFResponseCode GetSceneInternal(Scene& scene, SceneWithSceneElements& sceneWithSceneElements, LSFString& sceneID);

    LSFResponseCode CreateSceneInternal(Scene& scene, SceneWithSceneElements& sceneWithSceneElements, LSFString& name, LSFString& language, LSFString& sceneID);

    LSFResponseCode UpdateSceneInternal(Scene& scene, SceneWithSceneElements& sceneWithSceneElements, LSFString& sceneID);

    typedef std::map<LSFString, SceneObject*> SceneObjectMap;

    SceneObjectMap scenes;
    SceneWithSceneElementsMap sceneWithSceneElementsMap;
    std::list<LSFString> sceneUpdates;    /**< List of SceneIDs that were updated */
    SceneMap oldSceneMap;
    Mutex scenesLock;
    SceneElementManager* sceneElementManager;
    MasterSceneManager* masterSceneManager;
    size_t blobLength;

    /**
     * The function should only be called with scenesLock locked and is
     * solely meant to be used during blob synchronization process
     */
    void SyncSceneData(bool isLeader = false);

    void GetString(std::string& scenes, std::string& scene2, const SceneObjectMap& items);
    std::string GetUpdatesString(const std::list<LSFString>& updates);
    std::string GetString(const std::string& name, const std::string& id, const Scene& scene);
    std::string GetString(const std::string& name, const std::string& id, const SceneWithSceneElements& sceneWithSceneElements);
};

/**
 * SceneObject class - Implementing event and action mechanism \n
 * Class creates unique interface name for each instance 'org.allseen.LSF.ControllerService.ApplySceneEventAction + sceneId' \n
 * Object implementation located in path '/org/allseen/LSF/ControllerService/ApplySceneEventAction/' \n
 * All included in the controller service announcement \n
 * Implements one method 'ApplyScene' as an action \n
 * and one sessionless signal 'SceneApplied' as an event.
 */
class SceneObject : public BusObject, public Translator {
  public:
    /**
     * SceneObject CTOR
     * @param sceneMgr - SceneManager object reference
     * @param sceneid - the scene id related to the class object
     * @param tempScene
     * @param name  The scene name
     */
    SceneObject(SceneManager& sceneMgr, LSFString& sceneid, Scene& tempScene, SceneWithSceneElements& tempSceneWithSceneElements, LSFString& name);
    /**
     * SceneObject DTOR
     */
    ~SceneObject();

    /**
     * apply scene implementation \n
     * triggered in case 'ApplyScene' method called
     */
    void ApplySceneHandler(const InterfaceDescription::Member* member, Message& msg);
    /**
     * Send scene applied signal \n
     * Sessionless signal 'SceneApplied' for event and action mechanism
     */
    void SendSceneAppliedSignal(void);
    /**
     * Number of target languages \n
     * @return number
     */
    size_t NumTargetLanguages() {
        return 1;
    }
    /**
     * Get target language
     * @param index
     * @param ret - language returned
     */
    void GetTargetLanguage(size_t index, qcc::String& ret) {
        ret.assign("en");
    }
    /**
     * member override from Translator class. \n
     * Describes interface elements
     */
    const char* Translate(const char* sourceLanguage, const char* targetLanguage, const char* source, qcc::String& buffer);
    /**
     * BusObject override member
     */
    void ObjectRegistered(void);
    /**
     * Check if this scene object depends on specific scene element
     * @param sceneElementID - the scene element id
     * @return LSF_OK if there is not dependency \n
     *         LSF_ERR_DEPENDENCY if there is dependency
     */
    LSFResponseCode IsDependentOnSceneElement(LSFString& sceneElementID) {
        return sceneWithSceneElements.IsDependentOnSceneElement(sceneElementID);
    }

    SceneManager& sceneManager;         /**< Scene manager reference count */
    LSFString sceneId;                  /**< Scene id */
    Scene scene;                        /**< Scene object with embedded elements */
    SceneWithSceneElements sceneWithSceneElements; /**< Scene object with referenced elements */
    Mutex sceneNameMutex;               /**< Scene name mutex */
    LSFString sceneName;                /**< Scene name */
    const InterfaceDescription::Member* appliedSceneMember;          /**< applied scene signal */
};

OPTIONAL_NAMESPACE_CLOSE

} //lsf

#endif