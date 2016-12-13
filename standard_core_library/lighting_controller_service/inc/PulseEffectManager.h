#ifndef _PULSE_EFFECT_MANAGER_H_
#define _PULSE_EFFECT_MANAGER_H_
/**
 * \ingroup ControllerService
 */
/**
 * @file
 * This file provides definitions for pulse effect manager
 */
/******************************************************************************
 * Copyright (c) 2016 Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
 *
 *    SPDX-License-Identifier: Apache-2.0
 *
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Copyright 2016 Open Connectivity Foundation and Contributors to
 *    AllSeen Alliance. All rights reserved.
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
 * class manages the pulse effect of the lamps. \n
 * pulse effect is the ability to save lamp states and to use them when required later on.
 */
class PulseEffectManager : public Manager {
    friend class LampManager;
  public:
    /**
     * class constructor. \n
     * @param controllerSvc - reference to controller service instance
     * @param sceneMgrPtr - pointer to scene manager
     * @param pulseEffectFile - The full path of pulse effect file to be the persistent data
     */
    PulseEffectManager(ControllerService& controllerSvc, SceneManager* sceneMgrPtr, const std::string& pulseEffectFile);
    /**
     * Clears the pulseEffects data. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.PulseEffect' 'PulseEffectsDeleted'. \n
     * @return LSF_OK on success. \n
     */
    LSFResponseCode Reset(void);
    /**
     * Get all pulse effect ids. \n
     * Return asynchronous all pulse effect ids which are not the default lamp state. \n
     * response code LSF_OK on success. \n
     */
    void GetAllPulseEffectIDs(ajn::Message& msg);
    /**
     * Get pulse effect name. \n
     * @param msg type Message with MsgArg unique id. \n
     * Return asynchronously the pulse effect name, id, language and response code. \n
     * response code LSF_OK on success. \n
     */
    void GetPulseEffectName(ajn::Message& msg);
    /**
     * Set pulse effect name. \n
     * @param msg type Message with MsgArg pulse effect unique id. \n
     * Return asynchronously the pulse effect new name, id, language and response code. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.PulseEffect' 'PulseEffectsNameChanged'. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_EMPTY_NAME - pulse effect name is empty. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     */
    void SetPulseEffectName(ajn::Message& msg);
    /**
     * Create new pulseEffect. \n
     * @param msg type Message with MsgArgs: LampState, name, language. \n
     * Return asynchronously the pulse effect response code and auto generated unique id. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.PulseEffect' 'PulseEffectsCreated'. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_EMPTY_NAME - pulse effect name is empty. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     *
     */
    void CreatePulseEffect(ajn::Message& msg);
    /**
     * Update existing pulseEffect. \n
     * @param msg type Message with MsgArgs: pulse effect id. \n
     * Return asynchronously the pulse effect response code and unique id. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.PulseEffect' 'PulseEffectsUpdated'. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     */
    void UpdatePulseEffect(ajn::Message& msg);
    /**
     * Delete existing pulseEffect. \n
     * @param msg type Message with MsgArgs: pulse effect id. \n
     * Return asynchronously the pulse effect response code and unique id. \n
     * Send signal to the controller clients 'org.allseen.LSF.ControllerService.PulseEffect' 'PulseEffectsDeleted'. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - pulse effect with requested id is not found. \n
     */
    void DeletePulseEffect(ajn::Message& msg);
    /**
     * Get existing pulseEffect. \n
     * @param msg type Message with MsgArgs: pulse effect id. \n
     * Return asynchronously the pulse effect response code, unique id and requested lamp state \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - pulse effect with requested id is not found. \n
     */
    void GetPulseEffect(ajn::Message& msg);
    /**
     * Get existing pulseEffect. \n
     * @param pulseEffectID - The pulse effect unique id. \n
     * @param pulseEffect -   The pulse effect. \n
     * @return \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - pulse effect with requested id is not found. \n
     */
    LSFResponseCode GetPulseEffectInternal(const LSFString& pulseEffectID, PulseEffect& pulseEffect);
    /**
     * Get all pulseEffects. \n
     * @param pulseEffectMap - the requested pulseEffects filled synchronously. \n
     * response code LSF_OK on success. \n
     */
    LSFResponseCode GetAllPulseEffects(PulseEffectMap& pulseEffectMap);
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
     * Get Controller Service PulseEffect Interface Version. \n
     * @return 32 unsigned integer version. \n
     */
    uint32_t GetControllerServicePulseEffectInterfaceVersion(void);
    /**
     * Get the pulseEffects information as a string. \n
     * @return true if data is written to file
     */
    bool GetString(std::string& output, std::string& updates, uint32_t& checksum, uint64_t& timestamp, uint32_t& updatesChksum, uint64_t& updatesTs);
    /**
     * Get blob information about checksum and time stamp.
     */
    void GetBlobInfo(uint32_t& checksum, uint64_t& timestamp) {
        pulseEffectsLock.Lock();
        GetBlobInfoInternal(checksum, timestamp);
        pulseEffectsLock.Unlock();
    }
    /**
     * Get blob information about checksum and time stamp.
     */
    void GetUpdateBlobInfo(uint32_t& checksum, uint64_t& timestamp) {
        pulseEffectsLock.Lock();
        GetUpdateBlobInfoInternal(checksum, timestamp);
        pulseEffectsLock.Unlock();
    }

  private:

    void ReplaceMap(std::istringstream& stream);

    void ReplaceUpdatesList(std::istringstream& stream);

    PulseEffectMap pulseEffects;
    std::set<LSFString> pulseEffectUpdates;    /**< List of PulseEffectIDs that were updated */
    Mutex pulseEffectsLock;
    SceneManager* sceneManagerPtr;
    size_t blobLength;

    std::string GetString(const PulseEffectMap& items);
    std::string GetUpdatesString(const std::set<LSFString>& updates);
    std::string GetString(const std::string& name, const std::string& id, const PulseEffect& pulseEffect);
};

}


#endif