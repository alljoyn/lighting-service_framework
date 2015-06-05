#ifndef _PRESET_MANAGER_H_
#define _PRESET_MANAGER_H_
/**
 * \ingroup ControllerService
 */
/**
 * @file
 * This file provides definitions for preset manager
 */
/******************************************************************************
 *  * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#include <Manager.h>

#include <Mutex.h>
#include <LSFTypes.h>

#include <string>
#include <map>

namespace lsf {

class SceneManager;
/**
 * class manages the preset of the lamps. \n
 * preset is the ability to save lamp states and to use them when required later on.
 */
class PresetManager : public Manager {
    friend class LampManager;
  public:
    /**
     * class constructor. \n
     * @param controllerSvc - reference to controller service instance
     * @param sceneMgrPtr - pointer to scene manager
     * @param presetFile - The full path of preset file to be the persistent data
     */
    PresetManager(ControllerService& controllerSvc, SceneManager* sceneMgrPtr, const std::string& presetFile);
    /**
     * Clears the presets data. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.Preset' 'PresetsDeleted'. \n
     * @return LSF_OK on success. \n
     */
    LSFResponseCode Reset(void);
    /**
     * Reset to default state. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.Preset' 'DefaultLampStateChanged'. \n
     * Removing preset with id 'DefaultLampState' \n
     * response code LSF_OK on success. \n
     */
    LSFResponseCode ResetDefaultState(void);
    /**
     * Get all preset ids. \n
     * Return asynchronous all preset ids which are not the default lamp state. \n
     * response code LSF_OK on success. \n
     */
    void GetAllPresetIDs(ajn::Message& msg);
    /**
     * Get preset name. \n
     * @param msg type Message with MsgArg unique id. \n
     * Return asynchronously the preset name, id, language and response code. \n
     * response code LSF_OK on success. \n
     */
    void GetPresetName(ajn::Message& msg);
    /**
     * Set preset name. \n
     * @param msg type Message with MsgArg preset unique id. \n
     * Return asynchronously the preset new name, id, language and response code. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.Preset' 'PresetsNameChanged'. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_EMPTY_NAME - preset name is empty. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     */
    void SetPresetName(ajn::Message& msg);
    /**
     * Create new preset. \n
     * @param msg type Message with MsgArgs: LampState, name, language. \n
     * Return asynchronously the preset response code and auto generated unique id. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.Preset' 'PresetsCreated'. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_EMPTY_NAME - preset name is empty. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     *
     */
    void CreatePreset(ajn::Message& msg);
    /**
     * Update existing preset. \n
     * @param msg type Message with MsgArgs: preset id. \n
     * Return asynchronously the preset response code and unique id. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.Preset' 'PresetsUpdated'. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     */
    void UpdatePreset(ajn::Message& msg);
    /**
     * Delete existing preset. \n
     * @param msg type Message with MsgArgs: preset id. \n
     * Return asynchronously the preset response code and unique id. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.Preset' 'PresetsDeleted'. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - preset with requested id is not found. \n
     */
    void DeletePreset(ajn::Message& msg);
    /**
     * Get existing preset. \n
     * @param msg type Message with MsgArgs: preset id. \n
     * Return asynchronously the preset response code, unique id and requested lamp state \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - preset with requested id is not found. \n
     */
    void GetPreset(ajn::Message& msg);
    /**
     * Get existing preset. \n
     * @param presetID - The preset unique id. \n
     * @param state - The requested lamp state filled synchronously as reference to object. \n
     * @return \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - preset with requested id is not found. \n
     */
    LSFResponseCode GetPresetInternal(const LSFString& presetID, LampState& state);
    /**
     * Get default preset. \n
     * Return asynchronously the preset response code and lamp state which id is 'DefaultLampState'. \n
     * response code LSF_OK on success. \n
     */
    void GetDefaultLampState(ajn::Message& msg);
    /**
     * Set default lamp state. \n
     * Fill the preset with id 'DefaultLampState' new lamp state value. \n
     * Creating it if it is not already exists. In this case the name and the id are the same. \n
     * @param msg type Message with MsgArg: lamp state. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.Preset' 'DefaultLampStateChanged'. \n
     * Return asynchronously the preset response code. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     */
    void SetDefaultLampState(ajn::Message& msg);
    /**
     * Get all presets. \n
     * @param presetMap - the requested presets filled synchronously as reference to LampState. \n
     * response code LSF_OK on success. \n
     */
    LSFResponseCode GetAllPresets(PresetMap& presetMap);
    /**
     * Get default lamp state who's preset name is 'DefaultLampState'. \n
     * @param state - Requested information  of type LampState. Filled synchronously.
     * @return ER_OK on success. \n
     *      LSF_ERR_NOT_FOUND in case it not exist. \n
     */
    LSFResponseCode GetDefaultLampStateInternal(LampState& state);
    /**
     * Read saved persistent data to cache
     */
    void ReadSavedData(void);
    /**
     * Write to cache persistent data
     */
    void ReadWriteFile(void);
    /**
     * Handle received blob. \n
     * Getting the blob string and wrting it to the file. \n
     */
    void HandleReceivedBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp);
    /**
     * Handle Received Update Blob
     */
    void HandleReceivedUpdateBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp);
    /**
     * Get Controller Service Preset Interface Version. \n
     * @return 32 unsigned integer version. \n
     */
    uint32_t GetControllerServicePresetInterfaceVersion(void);
    /**
     * Get the presets information as a string. \n
     * @return true if data is written to file
     */
    bool GetString(std::string& output, std::string& updates, uint32_t& checksum, uint64_t& timestamp, uint32_t& updatesChksum, uint64_t& updatesTs);
    /**
     * Get blob information about checksum and time stamp.
     */
    void GetBlobInfo(uint32_t& checksum, uint64_t& timestamp) {
        presetsLock.Lock();
        GetBlobInfoInternal(checksum, timestamp);
        presetsLock.Unlock();
    }
    /**
     * Get blob information about checksum and time stamp.
     */
    void GetUpdateBlobInfo(uint32_t& checksum, uint64_t& timestamp) {
        presetsLock.Lock();
        GetUpdateBlobInfoInternal(checksum, timestamp);
        presetsLock.Unlock();
    }

  private:

    void ReplaceMap(std::istringstream& stream);

    void ReplaceUpdatesList(std::istringstream& stream);

    LSFResponseCode SetDefaultLampStateInternal(LampState& state);

    PresetMap presets;
    std::set<LSFString> presetUpdates;    /**< List of PresetIDs that were updated */
    Mutex presetsLock;
    SceneManager* sceneManagerPtr;
    size_t blobLength;

    std::string GetString(const PresetMap& items);
    std::string GetUpdatesString(const std::set<LSFString>& updates);
    std::string GetString(const std::string& name, const std::string& id, const LampState& preset);
};

}


#endif