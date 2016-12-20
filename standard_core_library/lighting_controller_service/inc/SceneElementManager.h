#ifndef _SCENE_ELEMENT_MANAGER_H_
#define _SCENE_ELEMENT_MANAGER_H_
/**
 * \ingroup ControllerService
 */
/**
 * \file lighting_controller_service/inc/SceneElementManager.h
 * This file provides definitions for sceneElement manager
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
#else
#include <Manager.h>
#endif

#include <Mutex.h>
#include <LSFTypes.h>

#include <string>
#include <map>
#include "LSFNamespaceSpecifier.h"

namespace lsf {

OPTIONAL_NAMESPACE_CONTROLLER_SERVICE

class SceneManager;
class LampGroupManager;
class TransitionEffectManager;
class PulseEffectManager;

/**
 * sceneElement management class
 */
class SceneElementManager : public Manager {

    friend class SceneManager;

  public:
    /**
     * SceneElementManager CTOR
     */
    SceneElementManager(ControllerService& controllerSvc, LampGroupManager* lampGroupManager, TransitionEffectManager* transistionEffectMgr, PulseEffectManager* pulseEffectMgr, SceneManager* sceneMgr, const std::string& sceneElementFile);
    /**
     * SceneElementManager DTOR
     */
    ~SceneElementManager() { }
    /**
     * Clear all scene elements from the SceneElementManager. \n
     * sends signal to the controller client 'org.allseen.LSF.ControllerService.SceneElement' 'SceneElementsDeleted' indicating all the deleted scene elements. \n
     * @return LSF_OK on success
     */
    LSFResponseCode Reset(void);
    /**
     * Is Dependent On Lamp Group - is there a scene element that depends on the specified group. \n
     * @param lampGroupID - lamp group unique identifier
     * @return LSF_OK if not depend
     */
    LSFResponseCode IsDependentOnLampGroup(LSFString& lampGroupID);
    /**
     * Check if there is a scene element that depends on specific effect
     * @param effectId - the effect id
     * @return LSF_OK if there is not dependency \n
     *         LSF_ERR_DEPENDENCY if there is dependency
     */
    LSFResponseCode IsDependentOnEffect(LSFString& effectId);
    /**
     * Get All SceneElement IDs. \n
     * Return asynchronous reply with response code: \n
     *  LSF_OK - operation succeeded
     */
    void GetAllSceneElementIDs(ajn::Message& message);
    /**
     * Get SceneElement name. \n
     * @param message type Message contains: sceneElement unique id (type 's') and requested language (type 's') \n
     * Return asynchronous reply with response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_NOT_FOUND  - the sceneElement not found \n
     *  LSF_ERR_INVALID_ARGS - Language not supported
     */
    void GetSceneElementName(ajn::Message& message);
    /**
     * Set SceneElement name. \n
     * @param message with  MsgArgs -  unique id (signature 's'), name (signature 's'), language (signature 's'). \n
     * Return asynchronous reply with response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, length exceeds LSF_MAX_NAME_LENGTH \n
     *  LSF_ERR_EMPTY_NAME - sceneElement name is empty \n
     *  LSF_ERR_RESOURCES - blob length is longer than MAX_FILE_LEN
     */
    void SetSceneElementName(ajn::Message& message);
    /**
     * Delete SceneElement \n
     * @param message type Message. Contains one MsgArg with sceneElement id. \n
     * Return asynchronous reply with response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_NOT_FOUND - can't find sceneElement id
     */
    void DeleteSceneElement(ajn::Message& message);
    /**
     * Create SceneElement and sending signal 'SceneElementsCreated' \n
     * @param message (type Message) with 4 message arguments as parameters (type ajn::MsgArg). \n
     * The arguments should have the following types: \n
     *      TransitionLampsLampGroupsToState \n
     *      TransitionLampsLampGroupsToPreset \n
     *      PulseLampsLampGroupsWithState \n
     *      PulseLampsLampGroupsWithPreset
     *
     * Return asynchronous reply with response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, sceneElement name is empty, Invalid SceneElement components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new SceneElement
     */
    void CreateSceneElement(ajn::Message& message);
    /**
     * Modify an existing sceneElement and then sending signal 'SceneElementsUpdated' \n
     * @param message (type Message) with 4 message arguments as parameters (type ajn::MsgArg). \n
     * The arguments should have the following types: \n
     *      TransitionLampsLampGroupsToState \n
     *      TransitionLampsLampGroupsToPreset \n
     *      PulseLampsLampGroupsWithState \n
     *      PulseLampsLampGroupsWithPreset
     */
    void UpdateSceneElement(ajn::Message& message);
    /**
     * Get SceneElement. - reply asynchronously with sceneElement content: \n
     *  TransitionLampsLampGroupsToState \n
     *  TransitionLampsLampGroupsToPreset \n
     *  PulseLampsLampGroupsWithState \n
     *  PulseLampsLampGroupsWithPreset \n
     * @param message type Message contains MsgArg with parameter unique id (type 's') \n
     *  return LSF_OK \n
     *  return LSF_ERR_NOT_FOUND - sceneElement not found
     */
    void GetSceneElement(ajn::Message& message);
    /**
     * Apply SceneElement. \n
     * @param message type Message with MsgArg parameter - sceneElement id (type 's') \n
     * reply asynchronously with response code: \n
     *  LSF_OK - on success \n
     *  LSF_ERR_NOT_FOUND - sceneElement id not found in current list of sceneElements
     */
    void ApplySceneElement(ajn::Message& message);
    /**
     * Apply multiple SceneElements. \n
     * @param sceneElementIDs list of scene element IDs to apply \n
     * @return LSF_OK on success
     *  LSF_ERR_NOT_FOUND - sceneElement id not found in current list of sceneElements
     */
    LSFResponseCode ApplySceneElementInternal(ajn::Message& message, LSFStringList& sceneElementIDs, LSFString sceneOrMasterSceneId);
    /**
     * Get All SceneElements. \n
     * Return asynchronous answer - the all sceneElements by its reference parameter \n
     * @param sceneElementMap of type SceneElementMap - reference of sceneElementMap to get all sceneElements \n
     * @return LSF_OK on success.
     */
    LSFResponseCode GetAllSceneElements(SceneElementMap& sceneElementMap);
    /**
     * Update persistent data
     */
    void ReadWriteFile();
    /**
     * Read Saved Data from persistent data
     */
    void ReadSavedData();
    /**
     * Get Blob Info about checksum and timestamp
     */
    void GetBlobInfo(uint32_t& checksum, uint64_t& timestamp) {
        sceneElementsLock.Lock();
        GetBlobInfoInternal(checksum, timestamp);
        sceneElementsLock.Unlock();
    }
    /**
     * Handle Received Blob
     */
    void HandleReceivedBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp);
    /**
     * Get the version of the sceneElement interface. \n
     * Return asynchronously. \n
     * @return version of the sceneElement itnerface
     */
    uint32_t GetControllerServiceSceneElementInterfaceVersion(void);

    /**
     * Send SceneElementApplied signal
     */
    void SendSceneElementAppliedSignal(LSFString& sceneElementId);

  protected:

    SceneElementMap sceneElements;
    Mutex sceneElementsLock;
    LampGroupManager* lampGroupManagerPtr;
    TransitionEffectManager* transitionEffectManagerPtr;
    PulseEffectManager* pulseEffectManagerPtr;
    SceneManager* sceneManagerPtr;
    size_t blobLength;

    LSFResponseCode CreateSceneElementInternal(SceneElement& sceneElement, LSFString& name, LSFString& language, LSFString& sceneElementID);

    LSFResponseCode DeleteSceneElementInternal(LSFString& sceneElementID);

    LSFResponseCode GetSceneElementInternal(LSFString& sceneElementID, SceneElement& sceneElement);

    void SendSceneElementsCreatedSignal(LSFStringList& sceneElementIds);

    /**
     * Replace Map
     */
    void ReplaceMap(std::istringstream& stream);

    /**
     * Get String
     */
    bool GetString(std::string& output, uint32_t& checksum, uint64_t& timestamp);

    /**
     * get scene element string
     */
    std::string GetString(const SceneElementMap& items);

    /**
     * get scene element string
     */
    std::string GetString(const std::string& name, const std::string& id, const SceneElement& sceneElement);
};

OPTIONAL_NAMESPACE_CLOSE

} //lsf

#endif