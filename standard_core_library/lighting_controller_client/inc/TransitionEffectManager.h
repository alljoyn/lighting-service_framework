#ifndef _TRANSITION_EFFECT_MANAGER_H_
#define _TRANSITION_EFFECT_MANAGER_H_
/**
 * \ingroup ControllerClient
 */
/**
 * \file lighting_controller_client/inc/TransitionEffectManager.h
 * This file provides definitions for transitionEffect manager
 */
/******************************************************************************
 * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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
 * A callback class which object delivered to the TransitionEffectManager and its object and methods delivered as a handle to get method replies and signals related to presence of the lamps.
 */
class TransitionEffectManagerCallback {
  public:
    /**
     * Destructor
     */
    virtual ~TransitionEffectManagerCallback() { }

    /**
     * Response to TransitionEffectManager::GetTransitionEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - transition effect with requested id is not found. \n
     * @param responseCode          The return code
     * @param transitionEffectID    The id of the TransitionEffect
     * @param transitionEffect      The transition effect information
     *
     */
    virtual void GetTransitionEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& transitionEffectID, const TransitionEffect& transitionEffect) { }

    /**
     * Response to TransitionEffectManager::ApplyTransitionEffectOnLamps. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - transition effect with requested id is not found. \n
     * @param responseCode          The return code
     * @param transitionEffectID    The id of the TransitionEffect
     *
     */
    virtual void ApplyTransitionEffectOnLampsReplyCB(const LSFResponseCode& responseCode, const LSFString& transitionEffectID, const LSFStringList& lampIDs) { }

    /**
     * Response to TransitionEffectManager::ApplyTransitionEffectOnLampGroups. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - transition effect with requested id is not found. \n
     * @param responseCode          The return code
     * @param transitionEffectID    The id of the TransitionEffect
     *
     */
    virtual void ApplyTransitionEffectOnLampGroupsReplyCB(const LSFResponseCode& responseCode, const LSFString& transitionEffectID, const LSFStringList& lampGroupIDs) { }

    /**
     * Response to TransitionEffectManager::GetAllTransitionEffectIDs. \n
     * response code LSF_OK on success. \n
     * @param responseCode          The return code
     * @param transitionEffectIDs   A list of LSFString's
     */
    virtual void GetAllTransitionEffectIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& transitionEffectIDs) { }

    /**
     * Response to TransitionEffectManager::GetTransitionEffectName. \n
     * Response code LSF_OK on success. \n
     * @param responseCode    The return code
     * @param transitionEffectID    The id of the TransitionEffect
     * @param language
     * @param transitionEffectName  The name of the TransitionEffect
     */
    virtual void GetTransitionEffectNameReplyCB(const LSFResponseCode& responseCode, const LSFString& transitionEffectID, const LSFString& language, const LSFString& transitionEffectName) { }

    /**
     * Response to TransitionEffectManager::SetTransitionEffectName. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_EMPTY_NAME - transitionEffect name is empty. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     * @param responseCode    The return code
     * @param transitionEffectID    The id of the TransitionEffect
     * @param language
     */
    virtual void SetTransitionEffectNameReplyCB(const LSFResponseCode& responseCode, const LSFString& transitionEffectID, const LSFString& language) { }

    /**
     * A TransitionEffect has been renamed
     *
     * @param transitionEffectIDs    The id of the TransitionEffect
     */
    virtual void TransitionEffectsNameChangedCB(const LSFStringList& transitionEffectIDs) { }

    /**
     * Response to TransitionEffectManager::CreateTransitionEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_EMPTY_NAME - transitionEffect name is empty. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     * @param responseCode    The return code
     * @param transitionEffectID    The id of the new TransitionEffect
     * @param trackingID     The tracking ID that the application can use to associate the method call with the reply
     */
    virtual void CreateTransitionEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& transitionEffectID, const uint32_t& trackingID) { }

    /**
     * A TransitionEffect has been created
     *
     * @param transitionEffectIDs    The id of the TransitionEffect
     */
    virtual void TransitionEffectsCreatedCB(const LSFStringList& transitionEffectIDs) { }

    /**
     * Response to TransitionEffectManager::UpdateTransitionEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     * @param responseCode    The return code
     * @param transitionEffectID    The id of the new TransitionEffect
     */
    virtual void UpdateTransitionEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& transitionEffectID) { }

    /**
     * A TransitionEffect has been updated
     *
     * @param transitionEffectIDs    The id of the TransitionEffect
     */
    virtual void TransitionEffectsUpdatedCB(const LSFStringList& transitionEffectIDs) { }

    /**
     * Response to TransitionEffectManager::DeleteTransitionEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - transition effect with requested id is not found. \n
     * @param responseCode    The return code
     * @param transitionEffectID    The id of the TransitionEffect
     */
    virtual void DeleteTransitionEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& transitionEffectID) { }

    /**
     * A TransitionEffect has been deleted
     *
     * @param transitionEffectIDs    The id of the TransitionEffect
     */
    virtual void TransitionEffectsDeletedCB(const LSFStringList& transitionEffectIDs) { }
};

/**
 * a class manage the status of the lamps
 */
class TransitionEffectManager : public Manager {

    friend class ControllerClient;

  public:
    /**
     * class constructor
     */
    TransitionEffectManager(ControllerClient& controller, TransitionEffectManagerCallback& callback);

    /**
     * Get all transition effect ids. \n
     * Return asynchronous all transition effect ids which are not the default lamp state. \n
     * Response in TransitionEffectManagerCallback::GetAllTransitionEffectIDsReplyCB. \n
     * @return CONTROLLER_CLIENT_OK on success to send the request. \n
     */
    ControllerClientStatus GetAllTransitionEffectIDs(void);

    /**
     * Get existing transition effect. \n
     * Response in TransitionEffectManagerCallback::GetTransitionEffectReplyCB. \n
     * @param transitionEffectID type LSFString which is transition effect id. \n
     * Return asynchronously the transition effect response code, unique id and requested lamp state \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - transition effect with requested id is not found. \n
     */
    ControllerClientStatus GetTransitionEffect(const LSFString& transitionEffectID);

    /**
     * Apply transition effect on lamps. \n
     * Response in TransitionEffectManagerCallback::ApplyTransitionEffectOnLampsReplyCB. \n
     * @param transitionEffectID type LSFString which is transition effect id. \n
     * @param lampIDs type LSFStringList which is the list of lamp ids. \n
     * Return asynchronously the apply transition effect on lamps response code and transition effect id\n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - transition effect with requested id is not found. \n
     */
    ControllerClientStatus ApplyTransitionEffectOnLamps(const LSFString& transitionEffectID, const LSFStringList& lampIDs);

    /**
     * Apply transition effect on lamp groups. \n
     * Response in TransitionEffectManagerCallback::ApplyTransitionEffectOnLampGroupsReplyCB. \n
     * @param transitionEffectID type LSFString which is transition effect id. \n
     * @param lampGroupIDs type LSFStringList which is the list of lamp group ids. \n
     * Return asynchronously the apply transition effect on lamp groups response code and transition effect id\n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - transition effect with requested id is not found. \n
     */
    ControllerClientStatus ApplyTransitionEffectOnLampGroups(const LSFString& transitionEffectID, const LSFStringList& lampGroupIDs);

    /**
     * Get the name of a TransitionEffect. \n
     * Response in TransitionEffectManagerCallback::GetTransitionEffectNameReplyCB. \n
     * Return asynchronously the transition effect name, id, language and response code. \n
     *
     * @param transitionEffectID    The id of the TransitionEffect
     * @param language
     */
    ControllerClientStatus GetTransitionEffectName(const LSFString& transitionEffectID, const LSFString& language = LSFString("en"));

    /**
     * Set the name of a TransitionEffect. \n
     * Return asynchronously the transition effect new name, id, language and response code. \n
     * Response in TransitionEffectManagerCallback::SetTransitionEffectNameReplyCB
     *
     * @param transitionEffectID    The id of the TransitionEffect
     * @param transitionEffectName  The new name of the TransitionEffect
     * @param language
     */
    ControllerClientStatus SetTransitionEffectName(const LSFString& transitionEffectID, const LSFString& transitionEffectName, const LSFString& language = LSFString("en"));

    /**
     * Create a new transitionEffect. \n
     * Response in TransitionEffectManagerCallback::CreateTransitionEffectReplyCB. \n
     * Return asynchronously the transition effect response code and auto generated unique id. \n
     *
     * @param trackingID  Controller Client returns a tracking ID in this variable which the application
     *                    can use to associate the reply for this call with the request
     * @param transitionEffect The new transition effect information
     * @param transitionEffectName
     * @param language
     */
    ControllerClientStatus CreateTransitionEffect(uint32_t& trackingID, const TransitionEffect& transitionEffect, const LSFString& transitionEffectName, const LSFString& language = LSFString("en"));

    /**
     * Update an existing TransitionEffect. \n
     * Response in TransitionEffectManagerCallback::UpdateTransitionEffectReplyCB. \n
     * Return asynchronously the transition effect response code and unique id. \n
     *
     * @param transitionEffectID    The id of the TransitionEffect
     * @param transitionEffect      The new transition effect information
     */
    ControllerClientStatus UpdateTransitionEffect(const LSFString& transitionEffectID, const TransitionEffect& transitionEffect);

    /**
     * Delete a transitionEffect. \n
     * Return asynchronously the transition effect response code and unique id. \n
     * Response in TransitionEffectManagerCallback::DeleteTransitionEffectReplyCB. \n
     *
     * @param transitionEffectID    The id of the TransitionEffect to delete
     */
    ControllerClientStatus DeleteTransitionEffect(const LSFString& transitionEffectID);

    /**
     * Get the TransitionEffect Info and Name. \n
     * Combination of GetTransitionEffect and GetTransitionEffectName. \n
     *
     * @param transitionEffectID    The ID of the master transitionEffect
     * @param language
     */
    ControllerClientStatus GetTransitionEffectDataSet(const LSFString& transitionEffectID, const LSFString& language = LSFString("en"));

  private:

    // signal handlers
    void TransitionEffectsNameChanged(LSFStringList& idList) {
        callback.TransitionEffectsNameChangedCB(idList);
    }

    void TransitionEffectsCreated(LSFStringList& idList) {
        callback.TransitionEffectsCreatedCB(idList);
    }

    void TransitionEffectsUpdated(LSFStringList& idList) {
        callback.TransitionEffectsUpdatedCB(idList);
    }

    void TransitionEffectsDeleted(LSFStringList& idList) {
        callback.TransitionEffectsDeletedCB(idList);
    }

    // method reply handlers
    void GetAllTransitionEffectIDsReply(LSFResponseCode& responseCode, LSFStringList& idList) {
        callback.GetAllTransitionEffectIDsReplyCB(responseCode, idList);
    }

    void GetTransitionEffectReply(ajn::Message& message);
    void ApplyTransitionEffectOnLampsReply(Message& message);
    void ApplyTransitionEffectOnLampGroupsReply(Message& message);

    void GetTransitionEffectNameReply(LSFResponseCode& responseCode, LSFString& lsfId, LSFString& language, LSFString& lsfName) {
        callback.GetTransitionEffectNameReplyCB(responseCode, lsfId, language, lsfName);
    }

    void SetTransitionEffectNameReply(LSFResponseCode& responseCode, LSFString& lsfId, LSFString& language) {
        callback.SetTransitionEffectNameReplyCB(responseCode, lsfId, language);
    }

    void CreateTransitionEffectReply(LSFResponseCode& responseCode, LSFString& lsfId, uint32_t& trackingID) {
        callback.CreateTransitionEffectReplyCB(responseCode, lsfId, trackingID);
    }

    void UpdateTransitionEffectReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.UpdateTransitionEffectReplyCB(responseCode, lsfId);
    }

    void DeleteTransitionEffectReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.DeleteTransitionEffectReplyCB(responseCode, lsfId);
    }

    TransitionEffectManagerCallback& callback;
};

}

#endif