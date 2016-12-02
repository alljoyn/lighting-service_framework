#ifndef _SCENE_ELEMENT_MANAGER_H_
#define _SCENE_ELEMENT_MANAGER_H_
/**
 * \ingroup ControllerClient
 */
/**
 * \file lighting_controller_client/inc/SceneElementManager.h
 * This file provides definitions for Scene Element Manager
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
#include <LSFResponseCodes.h>
#include <ControllerClientDefs.h>
#include <list>

namespace lsf {

class ControllerClient;

/**
 * SceneElement callback class
 */
class SceneElementManagerCallback {
  public:
    virtual ~SceneElementManagerCallback() { }

    /**
     * Response to SceneElementManager::GetAllSceneElementIds.
     *
     * @param responseCode    The response code
     * @param sceneElementIDs The list of sceneElement ID's
     */
    virtual void GetAllSceneElementIDsReplyCB(const LSFResponseCode& responseCode, const LSFStringList& sceneElementIDs) { }

    /**
     * Response to SceneElementManager::GetSceneElementName.
     *
     * @param responseCode    The response code: \n
     *   LSF_OK - operation succeeded \n
     *   LSF_ERR_NOT_FOUND  - the sceneElement not found \n
     *   LSF_ERR_INVALID_ARGS - Language not supported \n
     * @param sceneElementID    The id of the sceneElement
     * @param language          The language of the sceneElement name
     * @param sceneElementName  The name of the sceneElement
     */
    virtual void GetSceneElementNameReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID, const LSFString& language, const LSFString& sceneElementName) { }

    /**
     * Response to SceneElementManager::SetSceneElementName.
     *
     * @param responseCode    The response code: \n
     *   LSF_OK - operation succeeded \n
     *   LSF_ERR_INVALID_ARGS - Language not supported, length exceeds LSF_MAX_NAME_LENGTH \n
     *   LSF_ERR_EMPTY_NAME - sceneElement name is empty \n
     *   LSF_ERR_RESOURCES - blob length is longer than MAX_FILE_LEN \n
     * @param sceneElementID    The id of the sceneElement whose name was changed
     * @param language          language of the sceneElement
     */
    virtual void SetSceneElementNameReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID, const LSFString& language) { }

    /**
     * This signal is fired any time a sceneElement's name is changed.
     *
     * @param sceneElementIDs    The id of the sceneElement whose name changed
     */
    virtual void SceneElementsNameChangedCB(const LSFStringList& sceneElementIDs) { }

    /**
     * Response to SceneElementManager::CreateTransitionToStateSceneElement.
     *
     * @param responseCode    The response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, sceneElement name is empty, Invalid SceneElement components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new SceneElement \n
     * @param sceneElementID    The id of the new SceneElement
     */
    virtual void CreateTransitionToStateSceneElementReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     * Response to SceneElementManager::CreateTransitionToPresetSceneElement.
     *
     * @param responseCode    The response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, sceneElement name is empty, Invalid SceneElement components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new SceneElement \n
     * @param sceneElementID    The id of the new SceneElement
     */
    virtual void CreateTransitionToPresetSceneElementReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     * Response to SceneElementManager::CreatePulseWithStateSceneElement.
     *
     * @param responseCode    The response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, sceneElement name is empty, Invalid SceneElement components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new SceneElement \n
     * @param sceneElementID    The id of the new SceneElement
     */
    virtual void CreatePulseWithStateSceneElementReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     * Response to SceneElementManager::CreatePulseWithPresetSceneElement.
     *
     * @param responseCode    The response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, sceneElement name is empty, Invalid SceneElement components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new SceneElement \n
     * @param sceneElementID    The id of the new SceneElement
     */
    virtual void CreatePulseWithPresetSceneElementReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     * Response to SceneElementManager::CreateSceneElementWithEffectID.
     *
     * @param responseCode    The response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, sceneElement name is empty, Invalid SceneElement components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new SceneElement \n
     * @param sceneElementID    The id of the new SceneElement
     */
    virtual void CreateSceneElementWithEffectIDReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     *  This signal is fired any time a sceneElement is been created.
     *
     * @param sceneElementIDs    The id of the new sceneElement
     */
    virtual void SceneElementsCreatedCB(const LSFStringList& sceneElementIDs) { }

    /**
     * Response to SceneElementManager::UpdateTransitionToStateSceneElement.
     *
     * @param responseCode    The response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, sceneElement name is empty, Invalid SceneElement components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new SceneElement \n
     * @param sceneElementID    The id of the new SceneElement
     */
    virtual void UpdateTransitionToStateSceneElementReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     * Response to SceneElementManager::UpdateTransitionToPresetSceneElement.
     *
     * @param responseCode    The response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, sceneElement name is empty, Invalid SceneElement components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new SceneElement \n
     * @param sceneElementID    The id of the new SceneElement
     */
    virtual void UpdateTransitionToPresetSceneElementReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     * Response to SceneElementManager::UpdatePulseWithStateSceneElement.
     *
     * @param responseCode    The response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, sceneElement name is empty, Invalid SceneElement components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new SceneElement \n
     * @param sceneElementID    The id of the new SceneElement
     */
    virtual void UpdatePulseWithStateSceneElementReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     * Response to SceneElementManager::UpdatePulseWithPresetSceneElement.
     *
     * @param responseCode    The response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, sceneElement name is empty, Invalid SceneElement components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new SceneElement \n
     * @param sceneElementID    The id of the new SceneElement
     */
    virtual void UpdatePulseWithPresetSceneElementReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     * Response to SceneElementManager::UpdateSceneElementWithEffectID.
     *
     * @param responseCode    The response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_INVALID_ARGS - Language not supported, sceneElement name is empty, Invalid SceneElement components specified, ame length exceeds \n
     *  LSF_ERR_RESOURCES - Could not allocate memory \n
     *  LSF_ERR_NO_SLOT - No slot for new SceneElement \n
     * @param sceneElementID    The id of the new SceneElement
     */
    virtual void UpdateSceneElementWithEffectIDReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     * This signal is fired any time a sceneElement has been updated.
     *
     * @param sceneElementIDs    The id of the updated sceneElement
     */
    virtual void SceneElementsUpdatedCB(const LSFStringList& sceneElementIDs) { }

    /**
     * Response to SceneElementManager::DeleteSceneElement.
     *
     * @param responseCode    The response code: \n
     *  LSF_OK - operation succeeded \n
     *  LSF_ERR_NOT_FOUND - can't find sceneElement id \n
     * @param sceneElementID    The id of the deleted sceneElement
     */
    virtual void DeleteSceneElementReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     * This signal is fired any time a sceneElement has been deleted.
     *
     * @param sceneElementIDs    The id of the deleted sceneElement
     */
    virtual void SceneElementsDeletedCB(const LSFStringList& sceneElementIDs) { }

    /**
     * Response to SceneElementManager::GetSceneElement.
     *
     * @param responseCode    The response code: \n
     *  return LSF_OK \n
     *  return LSF_ERR_NOT_FOUND - sceneElement not found \n
     * @param sceneElementID    The id of the sceneElement
     * @param sceneElement      The sceneElement data
     */
    virtual void GetSceneElementReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID, const SceneElement& sceneElement) { }

    /**
     * Response to SceneElementManager::ApplySceneElement.
     *
     * @param responseCode    The response code \n
     *  LSF_OK - on success \n
     *  LSF_ERR_NOT_FOUND - sceneElement id not found in current list of sceneElements
     * @param sceneElementID    The id of the sceneElement
     */
    virtual void ApplySceneElementReplyCB(const LSFResponseCode& responseCode, const LSFString& sceneElementID) { }

    /**
     * This signal is fired any time a sceneElement has been applied.
     *
     * @param sceneElementIDs    The id of the sceneElement
     */
    virtual void SceneElementsAppliedCB(const LSFStringList& sceneElementIDs) { }
};


/**
 * sceneElement management class
 */
class SceneElementManager : public Manager {

    friend class ControllerClient;

  public:
    /**
     * SceneElementManager CTOR.
     * @param controller - a reference to ControllerClient instance
     * @param callback - a reference to SceneElementManagerCallback instance, to get the callback messages
     */
    SceneElementManager(ControllerClient& controller, SceneElementManagerCallback& callback);

    /**
     * Get the IDs of all available sceneElements. \n
     * Response comes in SceneElementManagerCallback::GetAllSceneElementIDsReplyCB
     */
    ControllerClientStatus GetAllSceneElementIDs(void);

    /**
     * Get the name of the given SceneElement. \n
     * Response in SceneElementManagerCallback::GetSceneElementNameReplyCB
     *
     * @param sceneElementID    The id of the sceneElement
     * @param language   The requested language
     */
    ControllerClientStatus GetSceneElementName(const LSFString& sceneElementID, const LSFString& language = LSFString("en"));

    /**
     * Set the name of a SceneElement. \n
     * Response in SceneElementManagerCallback::SetSceneElementNameReplyCB
     *
     * @param sceneElementID    The id of the sceneElement to modify
     * @param sceneElementName  The new sceneElement name
     * @param language   The requested language
     */
    ControllerClientStatus SetSceneElementName(const LSFString& sceneElementID, const LSFString& sceneElementName, const LSFString& language = LSFString("en"));

    /**
     *  Create a new TransitionToState SceneElement. \n
     *  Response in SceneElementManagerCallback::CreateTransitionToStateSceneElementReplyCB
     *
     * @param sceneElement      The sceneElement of type TransitionLampsLampGroupsToState
     * @param sceneElementName  The sceneElement name
     * @param language          The sceneElement language
     */
    ControllerClientStatus CreateTransitionToStateSceneElement(const TransitionLampsLampGroupsToState& sceneElement, const LSFString& sceneElementName, const LSFString& language = LSFString("en"));

    /**
     *  Create a new TransitionToPreset SceneElement. \n
     *  Response in SceneElementManagerCallback::CreateTransitionToPresetSceneElementReplyCB
     *
     * @param sceneElement      The sceneElement of type TransitionLampsLampGroupsToPreset
     * @param sceneElementName  The sceneElement name
     * @param language          The sceneElement language
     */
    ControllerClientStatus CreateTransitionToPresetSceneElement(const TransitionLampsLampGroupsToPreset& sceneElement, const LSFString& sceneElementName, const LSFString& language = LSFString("en"));

    /**
     *  Create a new PulseWithState SceneElement. \n
     *  Response in SceneElementManagerCallback::CreatePulseWithStateSceneElementReplyCB
     *
     * @param sceneElement      The sceneElement of type PulseLampsLampGroupsWithState
     * @param sceneElementName  The sceneElement name
     * @param language          The sceneElement language
     */
    ControllerClientStatus CreatePulseWithStateSceneElement(const PulseLampsLampGroupsWithState& sceneElement, const LSFString& sceneElementName, const LSFString& language = LSFString("en"));

    /**
     *  Create a new PulseWithPreset SceneElement. \n
     *  Response in SceneElementManagerCallback::CreatePulseWithPresetSceneElementReplyCB
     *
     * @param sceneElement      The sceneElement of type PulseLampsLampGroupsWithPreset
     * @param sceneElementName  The sceneElement name
     * @param language          The sceneElement language
     */
    ControllerClientStatus CreatePulseWithPresetSceneElement(const PulseLampsLampGroupsWithPreset& sceneElement, const LSFString& sceneElementName, const LSFString& language = LSFString("en"));

    /**
     *  Create a new SceneElement with EffectID. \n
     *  Response in SceneElementManagerCallback::CreateSceneElementWithEffectIDReplyCB
     *
     * @param sceneElement      The sceneElement with EffectID
     * @param sceneElementName  The sceneElement name
     * @param language          The sceneElement language
     */
    ControllerClientStatus CreateSceneElementWithEffectID(const SceneElementWithEffectID& sceneElement, const LSFString& sceneElementName, const LSFString& language = LSFString("en"));

    /**
     * Modify an existing sceneElement. \n
     * Response in SceneElementManagerCallback::UpdateTransitionToStateSceneElementReplyCB \n
     *
     * @param sceneElementID    The id of the sceneElement to modify
     * @param sceneElement      The new value of the sceneElement
     */
    ControllerClientStatus UpdateTransitionToStateSceneElement(const LSFString& sceneElementID, const TransitionLampsLampGroupsToState& sceneElement);

    /**
     * Modify an existing sceneElement. \n
     * Response in SceneElementManagerCallback::UpdateTransitionToPresetSceneElementReplyCB \n
     *
     * @param sceneElementID    The id of the sceneElement to modify
     * @param sceneElement      The new value of the sceneElement
     */
    ControllerClientStatus UpdateTransitionToPresetSceneElement(const LSFString& sceneElementID, const TransitionLampsLampGroupsToPreset& sceneElement);

    /**
     * Modify an existing sceneElement. \n
     * Response in SceneElementManagerCallback::UpdatePulseWithStateSceneElementReplyCB \n
     *
     * @param sceneElementID    The id of the sceneElement to modify
     * @param sceneElement      The new value of the sceneElement
     */
    ControllerClientStatus UpdatePulseWithStateSceneElement(const LSFString& sceneElementID, const PulseLampsLampGroupsWithState& sceneElement);

    /**
     * Modify an existing sceneElement. \n
     * Response in SceneElementManagerCallback::UpdatePulseWithPresetSceneElementReplyCB \n
     *
     * @param sceneElementID    The id of the sceneElement to modify
     * @param sceneElement      The new value of the sceneElement
     */
    ControllerClientStatus UpdatePulseWithPresetSceneElement(const LSFString& sceneElementID, const PulseLampsLampGroupsWithPreset& sceneElement);

    /**
     * Modify an existing sceneElement. \n
     * Response in SceneElementManagerCallback::UpdateSceneElementWithEffectIDReplyCB \n
     *
     * @param sceneElementID    The id of the sceneElement to modify
     * @param sceneElement      The new value of the sceneElement
     */
    ControllerClientStatus UpdateSceneElementWithEffectID(const LSFString& sceneElementID, const SceneElementWithEffectID& sceneElement);

    /**
     * Delete an existing sceneElement. \n
     * Response in SceneElementManagerCallback::DeleteSceneElementReplyCB
     *
     * @param sceneElementID    The id of the sceneElement to delete
     */
    ControllerClientStatus DeleteSceneElement(const LSFString& sceneElementID);

    /**
     * Get the information about the specified sceneElement. \n
     * Response in SceneElementManagerCallback::GetSceneElementReplyCB
     *
     * @param sceneElementID    The id of the sceneElement to find
     */
    ControllerClientStatus GetSceneElement(const LSFString& sceneElementID);

    /**
     * Apply a sceneElement. \n
     * Activate an already created sceneElement. Make it happen. \n
     * Response in SceneElementManagerCallback::ApplySceneElementReplyCB
     *
     * @param sceneElementID    The ID of the sceneElement to apply
     */
    ControllerClientStatus ApplySceneElement(const LSFString& sceneElementID);

    /**
     * Get the SceneElement Info and Name. \n
     * Combination of GetSceneElement and GetScneneName. Responses are accordingly. \n
     *
     * @param sceneElementID    The ID of the master sceneElement
     * @param language   The requested language
     */
    ControllerClientStatus GetSceneElementDataSet(const LSFString& sceneElementID, const LSFString& language = LSFString("en"));

  private:

    // Signal handlers:
    void SceneElementsNameChanged(LSFStringList& idList) {
        callback.SceneElementsNameChangedCB(idList);
    }

    void SceneElementsCreated(LSFStringList& idList) {
        callback.SceneElementsCreatedCB(idList);
    }

    void SceneElementsUpdated(LSFStringList& idList) {
        callback.SceneElementsUpdatedCB(idList);
    }

    void SceneElementsDeleted(LSFStringList& idList) {
        callback.SceneElementsDeletedCB(idList);
    }

    void SceneElementsApplied(LSFStringList& idList) {
        callback.SceneElementsAppliedCB(idList);
    }

    // asynch method response handlers
    void GetAllSceneElementIDsReply(LSFResponseCode& responseCode, LSFStringList& idList) {
        callback.GetAllSceneElementIDsReplyCB(responseCode, idList);
    }

    void GetSceneElementReply(ajn::Message& message);

    void ApplySceneElementReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.ApplySceneElementReplyCB(responseCode, lsfId);
    }

    void DeleteSceneElementReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.DeleteSceneElementReplyCB(responseCode, lsfId);
    }

    void SetSceneElementNameReply(LSFResponseCode& responseCode, LSFString& lsfId, LSFString& language) {
        callback.SetSceneElementNameReplyCB(responseCode, lsfId, language);
    }

    void GetSceneElementNameReply(LSFResponseCode& responseCode, LSFString& lsfId, LSFString& language, LSFString& lsfName) {
        callback.GetSceneElementNameReplyCB(responseCode, lsfId, language, lsfName);
    }

    void CreateTransitionToStateSceneElementReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.CreateTransitionToStateSceneElementReplyCB(responseCode, lsfId);
    }

    void CreateTransitionToPresetSceneElementReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.CreateTransitionToPresetSceneElementReplyCB(responseCode, lsfId);
    }

    void CreatePulseWithStateSceneElementReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.CreatePulseWithStateSceneElementReplyCB(responseCode, lsfId);
    }

    void CreatePulseWithPresetSceneElementReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.CreatePulseWithPresetSceneElementReplyCB(responseCode, lsfId);
    }

    void CreateSceneElementWithEffectIDReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.CreateSceneElementWithEffectIDReplyCB(responseCode, lsfId);
    }

    void UpdateTransitionToStateSceneElementReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.UpdateTransitionToStateSceneElementReplyCB(responseCode, lsfId);
    }

    void UpdateTransitionToPresetSceneElementReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.UpdateTransitionToPresetSceneElementReplyCB(responseCode, lsfId);
    }

    void UpdatePulseWithStateSceneElementReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.UpdatePulseWithStateSceneElementReplyCB(responseCode, lsfId);
    }

    void UpdatePulseWithPresetSceneElementReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.UpdatePulseWithPresetSceneElementReplyCB(responseCode, lsfId);
    }

    void UpdateSceneElementWithEffectIDReply(LSFResponseCode& responseCode, LSFString& lsfId) {
        callback.UpdateSceneElementWithEffectIDReplyCB(responseCode, lsfId);
    }

    SceneElementManagerCallback& callback;
};


}

#endif