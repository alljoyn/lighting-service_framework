#ifndef _PULSE_EFFECT_MANAGER_H_
#define _PULSE_EFFECT_MANAGER_H_
/**
 * \ingroup ControllerClient
 */
/**
 * \file lighting_controller_client/inc/PulseEffectManager.h
 * This file provides definitions for pulseEffect manager
 */
/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

#include <LSFTypes.h>
#include <Manager.h>
#include <ControllerClientDefs.h>

#include <list>
#include <LSFResponseCodes.h>

namespace lsf {

class ControllerClient;

/**
 * A callback class which object delivered to the PulseEffectManager and its object and methods delivered as a handle to get method replies and signals related to presence of the lamps.
 */
class PulseEffectManagerCallback {
  public:
    /**
     * Destructor
     */
    virtual ~PulseEffectManagerCallback() { }

    /**
     * Response to PulseEffectManager::GetPulseEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - pulse effect with requested id is not found. \n
     * @param responseCode          The return code
     * @param pulseEffectID    The id of the PulseEffect
     * @param pulseEffect      The pulse effect information
     *
     */
    virtual void GetPulseEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& pulseEffectID, const PulseEffect& pulseEffect) { }

    /**
     * Response to PulseEffectManager::ApplyPulseEffectOnLamps. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - pulse effect with requested id is not found. \n
     * @param responseCode          The return code
     * @param pulseEffectID    The id of the PulseEffect
     *
     */
    virtual void ApplyPulseEffectOnLampsReplyCB(const LSFResponseCode& responseCode, const LSFString& pulseEffectID, const LSFStringList& lampIDs) { }

    /**
     * Response to PulseEffectManager::ApplyPulseEffectOnLampGroups. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - pulse effect with requested id is not found. \n
     * @param responseCode          The return code
     * @param pulseEffectID    The id of the PulseEffect
     *
     */
    virtual void ApplyPulseEffectOnLampGroupsReplyCB(const LSFResponseCode& responseCode, const LSFString& pulseEffectID, const LSFStringList& lampGroupIDs) { }

    /**
     * Response to PulseEffectManager::GetAllPulseEffectIDs. \n
     * response code LSF_OK on success. \n
     * @param responseCode          The return code
     * @param pulseEffectIDs   A list of LSFString's
     */
    virtual void GetAllPulseEffectIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& pulseEffectIDs) { }

    /**
     * Response to PulseEffectManager::GetPulseEffectName. \n
     * Response code LSF_OK on success. \n
     * @param responseCode    The return code
     * @param pulseEffectID    The id of the PulseEffect
     * @param language
     * @param pulseEffectName  The name of the PulseEffect
     */
    virtual void GetPulseEffectNameReplyCB(const LSFResponseCode& responseCode, const LSFString& pulseEffectID, const LSFString& language, const LSFString& pulseEffectName) { }

    /**
     * Response to PulseEffectManager::SetPulseEffectName. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_EMPTY_NAME - pulseEffect name is empty. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     * @param responseCode    The return code
     * @param pulseEffectID    The id of the PulseEffect
     * @param language
     */
    virtual void SetPulseEffectNameReplyCB(const LSFResponseCode& responseCode, const LSFString& pulseEffectID, const LSFString& language) { }

    /**
     * A PulseEffect has been renamed
     *
     * @param pulseEffectIDs    The id of the PulseEffect
     */
    virtual void PulseEffectsNameChangedCB(const LSFStringList& pulseEffectIDs) { }

    /**
     * Response to PulseEffectManager::CreatePulseEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_EMPTY_NAME - pulseEffect name is empty. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     * @param responseCode    The return code
     * @param pulseEffectID    The id of the new PulseEffect
     * @param trackingID     The tracking ID that the application can use to associate the method call with the reply
     */
    virtual void CreatePulseEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& pulseEffectID, const uint32_t& trackingID) { }

    /**
     * A PulseEffect has been created
     *
     * @param pulseEffectIDs    The id of the PulseEffect
     */
    virtual void PulseEffectsCreatedCB(const LSFStringList& pulseEffectIDs) { }

    /**
     * Response to PulseEffectManager::UpdatePulseEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_INVALID_ARGS - language not supported, name is too long. \n
     *      LSF_ERR_RESOURCES - blob is too big. \n
     * @param responseCode    The return code
     * @param pulseEffectID    The id of the new PulseEffect
     */
    virtual void UpdatePulseEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& pulseEffectID) { }

    /**
     * A PulseEffect has been updated
     *
     * @param pulseEffectIDs    The id of the PulseEffect
     */
    virtual void PulseEffectsUpdatedCB(const LSFStringList& pulseEffectIDs) { }

    /**
     * Response to PulseEffectManager::DeletePulseEffect. \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - pulse effect with requested id is not found. \n
     * @param responseCode    The return code
     * @param pulseEffectID    The id of the PulseEffect
     */
    virtual void DeletePulseEffectReplyCB(const LSFResponseCode& responseCode, const LSFString& pulseEffectID) { }

    /**
     * A PulseEffect has been deleted
     *
     * @param pulseEffectIDs    The id of the PulseEffect
     */
    virtual void PulseEffectsDeletedCB(const LSFStringList& pulseEffectIDs) { }
};

/**
 * a class manage the status of the lamps
 */
class PulseEffectManager : public Manager {

    friend class ControllerClient;

  public:
    /**
     * class constructor
     */
    PulseEffectManager(ControllerClient& controller, PulseEffectManagerCallback& callback);

    /**
     * Get all pulse effect ids. \n
     * Return asynchronous all pulse effect ids which are not the default lamp state. \n
     * Response in PulseEffectManagerCallback::GetAllPulseEffectIDsReplyCB. \n
     * @return CONTROLLER_CLIENT_OK on success to send the request. \n
     */
    ControllerClientStatus GetAllPulseEffectIDs(void);

    /**
     * Get existing pulse effect. \n
     * Response in PulseEffectManagerCallback::GetPulseEffectReplyCB. \n
     * @param pulseEffectID type LSFString which is pulse effect id. \n
     * Return asynchronously the pulse effect response code, unique id and requested lamp state \n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - pulse effect with requested id is not found. \n
     */
    ControllerClientStatus GetPulseEffect(const LSFString& pulseEffectID);

    /**
     * Apply pulse effect on lamps. \n
     * Response in PulseEffectManagerCallback::ApplyPulseEffectOnLampsReplyCB. \n
     * @param pulseEffectID type LSFString which is pulse effect id. \n
     * @param lampIDs type LSFStringList which is the list of lamp ids. \n
     * Return asynchronously the apply pulse effect on lamps response code and pulse effect id\n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - pulse effect with requested id is not found. \n
     */
    ControllerClientStatus ApplyPulseEffectOnLamps(const LSFString& pulseEffectID, const LSFStringList& lampIDs);

    /**
     * Apply pulse effect on lamp groups. \n
     * Response in PulseEffectManagerCallback::ApplyPulseEffectOnLampGroupsReplyCB. \n
     * @param pulseEffectID type LSFString which is pulse effect id. \n
     * @param lampGroupIDs type LSFStringList which is the list of lamp group ids. \n
     * Return asynchronously the apply pulse effect on lamp groups response code and pulse effect id\n
     * response code LSF_OK on success. \n
     *      LSF_ERR_NOT_FOUND - pulse effect with requested id is not found. \n
     */
    ControllerClientStatus ApplyPulseEffectOnLampGroups(const LSFString& pulseEffectID, const LSFStringList& lampGroupIDs);

    /**
     * Get the name of a PulseEffect. \n
     * Response in PulseEffectManagerCallback::GetPulseEffectNameReplyCB. \n
     * Return asynchronously the pulse effect name, id, language and response code. \n
     *
     * @param pulseEffectID    The id of the PulseEffect
     * @param language
     */
    ControllerClientStatus GetPulseEffectName(const LSFString& pulseEffectID, const LSFString& language = LSFString("en"));

    /**
     * Set the name of a PulseEffect. \n
     * Return asynchronously the pulse effect new name, id, language and response code. \n
     * Response in PulseEffectManagerCallback::SetPulseEffectNameReplyCB
     *
     * @param pulseEffectID    The id of the PulseEffect
     * @param pulseEffectName  The new name of the PulseEffect
     * @param language
     */
    ControllerClientStatus SetPulseEffectName(const LSFString& pulseEffectID, const LSFString& pulseEffectName, const LSFString& language = LSFString("en"));

    /**
     * Create a new pulseEffect. \n
     * Response in PulseEffectManagerCallback::CreatePulseEffectReplyCB. \n
     * Return asynchronously the pulse effect response code and auto generated unique id. \n
     *
     * @param trackingID  Controller Client returns a tracking ID in this variable which the application
     *                    can use to associate the reply for this call with the request
     * @param pulseEffect The new pulse effect information
     * @param pulseEffectName
     * @param language
     */
    ControllerClientStatus CreatePulseEffect(uint32_t& trackingID, const PulseEffect& pulseEffect, const LSFString& pulseEffectName, const LSFString& language = LSFString("en"));

    /**
     * Update an existing PulseEffect. \n
     * Response in PulseEffectManagerCallback::UpdatePulseEffectReplyCB. \n
     * Return asynchronously the pulse effect response code and unique id. \n
     *
     * @param pulseEffectID    The id of the PulseEffect
     * @param pulseEffect      The new pulse effect information
     */
    ControllerClientStatus UpdatePulseEffect(const LSFString& pulseEffectID, const PulseEffect& pulseEffect);

    /**
     * Delete a pulseEffect. \n
     * Return asynchronously the pulse effect response code and unique id. \n
     * Response in PulseEffectManagerCallback::DeletePulseEffectReplyCB. \n
     *
     * @param pulseEffectID    The id of the PulseEffect to delete
     */
    ControllerClientStatus DeletePulseEffect(const LSFString& pulseEffectID);

    /**
     * Get the PulseEffect Info and Name. \n
     * Combination of GetPulseEffect and GetPulseEffectName. \n
     *
     * @param pulseEffectID    The ID of the master pulseEffect
     * @param language
     */
    ControllerClientStatus GetPulseEffectDataSet(const LSFString& pulseEffectID, const LSFString& language = LSFString("en"));

  private:

    // signal handlers
    void PulseEffectsNameChanged(LSFStringList& idList) {
        callback.PulseEffectsNameChangedCB(idList);
    }

    void PulseEffectsCreated(LSFStringList& idList) {
        callback.PulseEffectsCreatedCB(idList);
    }

    void PulseEffectsUpdated(LSFStringList& idList) {
        callback.PulseEffectsUpdatedCB(idList);
    }

    void PulseEffectsDeleted(LSFStringList& idList) {
        callback.PulseEffectsDeletedCB(idList);
    }

    // method reply handlers
    void GetAllPulseEffectIDsReply(LSFResponseCode& responseCode, LSFStringList& idList) {
        callback.GetAllPulseEffectIDsReplyCB(responseCode, idList);
    }

    void GetPulseEffectReply(ajn::Message& message);

    void GetPulseEffectNameReply(LSFResponseCode& responseCode, LSFString& lsfId, LSFString& language, LSFString& lsfName) {
        callback.GetPulseEffectNameReplyCB(responseCode, lsfId, language, lsfName);
    }

    void SetPulseEffectNameReply(LSFResponseCode& responseCode, LSFString& lsfId, LSFString& language) {
        callback.SetPulseEffectNameReplyCB(responseCode, lsfId, language);
    }

    void CreatePulseEffectReply(LSFResponseCode& responseCode, LSFString& lsfId, uint32_t& trackingID) {
        callback.CreatePulseEffectReplyCB(responseCode, lsfId, trackingID);
    }

    void UpdatePulseEffectReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.UpdatePulseEffectReplyCB(responseCode, lsfId);
    }

    void ApplyPulseEffectOnLampsReply(ajn::Message& message);

    void ApplyPulseEffectOnLampGroupsReply(ajn::Message& message);

    void DeletePulseEffectReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.DeletePulseEffectReplyCB(responseCode, lsfId);
    }

    PulseEffectManagerCallback& callback;
};

}

#endif