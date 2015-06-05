#ifndef _STATIC_EFFECT_MANAGER_H_
#define _STATIC_EFFECT_MANAGER_H_
/**
 * \ingroup ControllerClient
 */
/**
 * \file lighting_controller_client/inc/StaticEffectManager.h
 * This file provides definitions for staticEffect manager
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

#include <LSFTypes.h>
#include <Manager.h>
#include <ControllerClientDefs.h>

#include <list>
#include <LSFResponseCodes.h>

namespace lsf {

class ControllerClient;

/**
 * A callback class which object delivered to the StaticEffectManager and its object and methods delivered as a handle to get method replies and signals related to presence of the lamps.
 */
class StaticEffectManagerCallback {
  public:
    /**
     * Destructor
     */
    virtual ~StaticEffectManagerCallback() { }

    /**
     * Response to StaticEffectManager::GetStaticEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - static effect with requested id is not found. \n
     * @param responseCode          The return code
     * @param staticEffectID    The id of the StaticEffect
     * @param staticEffect      The static effect information
     *
     */
    virtual void GetStaticEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& staticEffectID, const StaticEffect& staticEffect) { }

    /**
     * Response to StaticEffectManager::GetAllStaticEffectIDs. \n
     * response code LSF_OK on success. \n
     * @param responseCode          The return code
     * @param staticEffectIDs   A list of LSFString's
     */
    virtual void GetAllStaticEffectIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& staticEffectIDs) { }

    /**
     * Response to StaticEffectManager::GetStaticEffectName. \n
     * Response code LSF_OK on success. \n
     * @param responseCode    The return code
     * @param staticEffectID    The id of the StaticEffect
     * @param language
     * @param staticEffectName  The name of the StaticEffect
     */
    virtual void GetStaticEffectNameReplyCB(const LSFResponseCode& responseCode, const LSFString& staticEffectID, const LSFString& language, const LSFString& staticEffectName) { }

    /**
     * Response to StaticEffectManager::SetStaticEffectName. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_EMPTY_NAME - staticEffect name is empty. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     * @param responseCode    The return code
     * @param staticEffectID    The id of the StaticEffect
     * @param language
     */
    virtual void SetStaticEffectNameReplyCB(const LSFResponseCode& responseCode, const LSFString& staticEffectID, const LSFString& language) { }

    /**
     * A StaticEffect has been renamed
     *
     * @param staticEffectIDs    The id of the StaticEffect
     */
    virtual void StaticEffectsNameChangedCB(const LSFStringList& staticEffectIDs) { }

    /**
     * Response to StaticEffectManager::CreateStaticEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_EMPTY_NAME - staticEffect name is empty. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     * @param responseCode    The return code
     * @param staticEffectID    The id of the new StaticEffect
     */
    virtual void CreateStaticEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& staticEffectID) { }

    /**
     * A StaticEffect has been created
     *
     * @param staticEffectIDs    The id of the StaticEffect
     */
    virtual void StaticEffectsCreatedCB(const LSFStringList& staticEffectIDs) { }

    /**
     * Response to StaticEffectManager::UpdateStaticEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     * @param responseCode    The return code
     * @param staticEffectID    The id of the new StaticEffect
     */
    virtual void UpdateStaticEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& staticEffectID) { }

    /**
     * A StaticEffect has been updated
     *
     * @param staticEffectIDs    The id of the StaticEffect
     */
    virtual void StaticEffectsUpdatedCB(const LSFStringList& staticEffectIDs) { }

    /**
     * Response to StaticEffectManager::DeleteStaticEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - static effect with requested id is not found. \n
     * @param responseCode    The return code
     * @param staticEffectID    The id of the StaticEffect
     */
    virtual void DeleteStaticEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& staticEffectID) { }

    /**
     * A StaticEffect has been deleted
     *
     * @param staticEffectIDs    The id of the StaticEffect
     */
    virtual void StaticEffectsDeletedCB(const LSFStringList& staticEffectIDs) { }
};

/**
 * a class manage the status of the lamps
 */
class StaticEffectManager : public Manager {

    friend class ControllerClient;

  public:
    /**
     * class constructor
     */
    StaticEffectManager(ControllerClient& controller, StaticEffectManagerCallback& callback);

    /**
     * Get all static effect ids. \n
     * Return asynchronous all static effect ids which are not the default lamp state. \n
     * Response in StaticEffectManagerCallback::GetAllStaticEffectIDsReplyCB. \n
     * @return CONTROLLER_CLIENT_OK on success to send the request. \n
     */
    ControllerClientStatus GetAllStaticEffectIDs(void);

    /**
     * Get existing static effect. \n
     * Response in StaticEffectManagerCallback::GetStaticEffectReplyCB. \n
     * @param staticEffectID type LSFString which is static effect id. \n
     * Return asynchronously the static effect response code, unique id and requested lamp state \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - static effect with requested id is not found. \n
     */
    ControllerClientStatus GetStaticEffect(const LSFString& staticEffectID);

    /**
     * Get the name of a StaticEffect. \n
     * Response in StaticEffectManagerCallback::GetStaticEffectNameReplyCB. \n
     * Return asynchronously the static effect name, id, language and response code. \n
     *
     * @param staticEffectID    The id of the StaticEffect
     * @param language
     */
    ControllerClientStatus GetStaticEffectName(const LSFString& staticEffectID, const LSFString& language = LSFString("en"));

    /**
     * Set the name of a StaticEffect. \n
     * Return asynchronously the static effect new name, id, language and response code. \n
     * Response in StaticEffectManagerCallback::SetStaticEffectNameReplyCB
     *
     * @param staticEffectID    The id of the StaticEffect
     * @param staticEffectName  The new name of the StaticEffect
     * @param language
     */
    ControllerClientStatus SetStaticEffectName(const LSFString& staticEffectID, const LSFString& staticEffectName, const LSFString& language = LSFString("en"));

    /**
     * Create a new staticEffect. \n
     * Response in StaticEffectManagerCallback::CreateStaticEffectReplyCB. \n
     * Return asynchronously the static effect response code and auto generated unique id. \n
     *
     * @param staticEffect The new static effect information
     * @param staticEffectName
     * @param language
     */
    ControllerClientStatus CreateStaticEffect(const StaticEffect& staticEffect, const LSFString& staticEffectName, const LSFString& language = LSFString("en"));

    /**
     * Update an existing StaticEffect. \n
     * Response in StaticEffectManagerCallback::UpdateStaticEffectReplyCB. \n
     * Return asynchronously the static effect response code and unique id. \n
     *
     * @param staticEffectID    The id of the StaticEffect
     * @param staticEffect      The new static effect information
     */
    ControllerClientStatus UpdateStaticEffect(const LSFString& staticEffectID, const StaticEffect& staticEffect);

    /**
     * Delete a staticEffect. \n
     * Return asynchronously the static effect response code and unique id. \n
     * Response in StaticEffectManagerCallback::DeleteStaticEffectReplyCB. \n
     *
     * @param staticEffectID    The id of the StaticEffect to delete
     */
    ControllerClientStatus DeleteStaticEffect(const LSFString& staticEffectID);

  private:

    // signal handlers
    void StaticEffectsNameChanged(LSFStringList& idList) {
        callback.StaticEffectsNameChangedCB(idList);
    }

    void StaticEffectsCreated(LSFStringList& idList) {
        callback.StaticEffectsCreatedCB(idList);
    }

    void StaticEffectsUpdated(LSFStringList& idList) {
        callback.StaticEffectsUpdatedCB(idList);
    }

    void StaticEffectsDeleted(LSFStringList& idList) {
        callback.StaticEffectsDeletedCB(idList);
    }

    // method reply handlers
    void GetAllStaticEffectIDsReply(LSFResponseCode& responseCode, LSFStringList& idList) {
        callback.GetAllStaticEffectIDsReplyCB(responseCode, idList);
    }

    void GetStaticEffectReply(ajn::Message& message);

    void GetStaticEffectNameReply(LSFResponseCode& responseCode, LSFString& lsfId, LSFString& language, LSFString& lsfName) {
        callback.GetStaticEffectNameReplyCB(responseCode, lsfId, language, lsfName);
    }

    void SetStaticEffectNameReply(LSFResponseCode& responseCode, LSFString& lsfId, LSFString& language) {
        callback.SetStaticEffectNameReplyCB(responseCode, lsfId, language);
    }

    void CreateStaticEffectReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.CreateStaticEffectReplyCB(responseCode, lsfId);
    }

    void UpdateStaticEffectReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.UpdateStaticEffectReplyCB(responseCode, lsfId);
    }

    void DeleteStaticEffectReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.DeleteStaticEffectReplyCB(responseCode, lsfId);
    }

    StaticEffectManagerCallback& callback;
};

}

#endif