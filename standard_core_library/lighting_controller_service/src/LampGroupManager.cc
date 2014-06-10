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

#include <LampGroupManager.h>
#include <ControllerService.h>
#include <qcc/atomic.h>
#include <qcc/Debug.h>
#include <SceneManager.h>
#include <OEMConfig.h>
#include <FileParser.h>

#include <algorithm>

using namespace lsf;
using namespace ajn;

#define QCC_MODULE "LAMP_GROUP_MANAGER"

LampGroupManager::LampGroupManager(ControllerService& controllerSvc, LampManager& lampMgr, SceneManager* sceneMgrPtr, const std::string& lampGroupFile) :
    Manager(controllerSvc, lampGroupFile), lampManager(lampMgr), sceneManagerPtr(sceneMgrPtr)
{
}

LSFResponseCode LampGroupManager::GetAllLampGroups(LampGroupMap& lampGroupMap)
{
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = lampGroupsLock.Lock();
    if (ER_OK == status) {
        lampGroupMap = lampGroups;
        status = lampGroupsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __FUNCTION__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __FUNCTION__));
    }

    return responseCode;
}

LSFResponseCode LampGroupManager::Reset(void)
{
    QCC_DbgPrintf(("%s", __FUNCTION__));
    LSFResponseCode responseCode = LSF_OK;
    QStatus tempStatus = lampGroupsLock.Lock();
    if (ER_OK == tempStatus) {
        /*
         * Record the IDs of all the LampGroups that are being deleted
         */
        LSFStringList lampGroupsList;
        for (LampGroupMap::iterator it = lampGroups.begin(); it != lampGroups.end(); ++it) {
            lampGroupsList.push_back(it->first);
        }

        /*
         * Clear the LampGroups
         */
        lampGroups.clear();
        tempStatus = lampGroupsLock.Unlock();
        if (ER_OK != tempStatus) {
            QCC_LogError(tempStatus, ("%s: lampGroupsLock.Unlock() failed", __FUNCTION__));
        }

        /*
         * Send the LampGroups deleted signal
         */
        if (lampGroupsList.size()) {
            tempStatus = controllerService.SendSignal(ControllerServiceLampGroupInterfaceName, "LampGroupsDeleted", lampGroupsList);
            if (ER_OK != tempStatus) {
                QCC_LogError(tempStatus, ("%s: Unable to send LampGroupsDeleted signal", __FUNCTION__));
            }
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(tempStatus, ("%s: lampGroupsLock.Lock() failed", __FUNCTION__));
    }

    return responseCode;
}

LSFResponseCode LampGroupManager::IsDependentOnLampGroup(LSFString& lampGroupID)
{
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = lampGroupsLock.Lock();
    if (ER_OK == status) {
        for (LampGroupMap::iterator it = lampGroups.begin(); it != lampGroups.end(); ++it) {
            responseCode = it->second.second.IsDependentLampGroup(lampGroupID);
            if (LSF_OK != responseCode) {
                break;
            }
        }
        status = lampGroupsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __FUNCTION__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __FUNCTION__));
    }

    if (LSF_OK == responseCode) {
        responseCode = sceneManagerPtr->IsDependentOnLampGroup(lampGroupID);
    }

    return responseCode;
}

void LampGroupManager::GetAllLampGroupIDs(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));

    LSFStringList idList;
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = lampGroupsLock.Lock();
    if (ER_OK == status) {
        for (LampGroupMap::iterator it = lampGroups.begin(); it != lampGroups.end(); ++it) {
            idList.push_back(it->first.c_str());
        }
        status = lampGroupsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __FUNCTION__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __FUNCTION__));
    }

    controllerService.SendMethodReplyWithResponseCodeAndListOfIDs(message, responseCode, idList);
}

void LampGroupManager::GetLampGroupName(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));
    LSFString name;
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    LSFString lampGroupID(uniqueId);

    LSFString language = static_cast<LSFString>(args[1].v_string.str);
    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __FUNCTION__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = lampGroupsLock.Lock();
        if (ER_OK == status) {
            LampGroupMap::iterator it = lampGroups.find(uniqueId);
            if (it != lampGroups.end()) {
                name = it->second.first;
                responseCode = LSF_OK;
            }
            status = lampGroupsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __FUNCTION__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __FUNCTION__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDLanguageAndName(message, responseCode, lampGroupID, language, name);
}

void LampGroupManager::SetLampGroupName(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    bool nameChanged = false;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    LSFString lampGroupID(uniqueId);

    const char* name;
    args[1].Get("s", &name);

    LSFString language = static_cast<LSFString>(args[2].v_string.str);
    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __FUNCTION__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        if (strlen(name) > LSF_MAX_NAME_LENGTH) {
            responseCode = LSF_ERR_INVALID_ARGS;
            QCC_LogError(ER_FAIL, ("%s: strlen(name) > LSF_MAX_NAME_LENGTH", __FUNCTION__));
        } else {
            QStatus status = lampGroupsLock.Lock();
            if (ER_OK == status) {
                LampGroupMap::iterator it = lampGroups.find(uniqueId);
                if (it != lampGroups.end()) {
                    it->second.first = LSFString(name);
                    responseCode = LSF_OK;
                    nameChanged = true;
                }
                status = lampGroupsLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __FUNCTION__));
                }
            } else {
                responseCode = LSF_ERR_BUSY;
                QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __FUNCTION__));
            }
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDAndName(message, responseCode, lampGroupID, language);

    if (nameChanged) {
        ScheduleFileUpdate();
        LSFStringList idList;
        idList.push_back(lampGroupID);
        controllerService.SendSignal(ControllerServiceLampGroupInterfaceName, "LampGroupsNameChanged", idList);
    }
}

void LampGroupManager::CreateLampGroup(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_OK;

    bool created = false;
    LSFString lampGroupID;

    const ajn::MsgArg* inputArgs;
    size_t numInputArgs;
    message->GetArgs(numInputArgs, inputArgs);

    QStatus status = lampGroupsLock.Lock();
    if (ER_OK == status) {
        if (lampGroups.size() < MAX_SUPPORTED_NUM_LSF_ENTITY) {
            lampGroupID = GenerateUniqueID("LAMP_GROUP");
            LampGroup lampGroup(inputArgs[0], inputArgs[1]);
            lampGroups[lampGroupID].first = lampGroupID;
            lampGroups[lampGroupID].second = lampGroup;
            created = true;
        } else {
            QCC_LogError(ER_FAIL, ("%s: No slot for new LampGroup", __FUNCTION__));
            responseCode = LSF_ERR_NO_SLOT;
        }
        status = lampGroupsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __FUNCTION__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __FUNCTION__));
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);

    if (created) {
        ScheduleFileUpdate();
        LSFStringList idList;
        idList.push_back(lampGroupID);
        controllerService.SendSignal(ControllerServiceLampGroupInterfaceName, "LampGroupsCreated", idList);
    }
}

void LampGroupManager::UpdateLampGroup(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    bool updated = false;

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    LSFString lampGroupID(uniqueId);
    LampGroup lampGroup(args[1], args[2]);

    QStatus status = lampGroupsLock.Lock();
    if (ER_OK == status) {
        LampGroupMap::iterator it = lampGroups.find(uniqueId);
        if (it != lampGroups.end()) {
            lampGroups[lampGroupID].second = lampGroup;
            responseCode = LSF_OK;
            updated = true;
        }
        status = lampGroupsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __FUNCTION__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __FUNCTION__));
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);

    if (updated) {
        ScheduleFileUpdate();
        LSFStringList idList;
        idList.push_back(lampGroupID);
        controllerService.SendSignal(ControllerServiceLampGroupInterfaceName, "LampGroupsUpdated", idList);
    }
}

void LampGroupManager::DeleteLampGroup(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;

    bool deleted = false;

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    const char* lampGroupId;
    args[0].Get("s", &lampGroupId);

    LSFString lampGroupID(lampGroupId);

    responseCode = IsDependentOnLampGroup(lampGroupID);

    if (LSF_OK == responseCode) {
        QStatus status = lampGroupsLock.Lock();
        if (ER_OK == status) {
            LampGroupMap::iterator it = lampGroups.find(lampGroupId);
            if (it != lampGroups.end()) {
                lampGroups.erase(it);
                deleted = true;
            } else {
                responseCode = LSF_ERR_NOT_FOUND;
            }
            status = lampGroupsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __FUNCTION__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __FUNCTION__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);

    if (deleted) {
        ScheduleFileUpdate();
        LSFStringList idList;
        idList.push_back(lampGroupID);
        controllerService.SendSignal(ControllerServiceLampGroupInterfaceName, "LampGroupsDeleted", idList);
    }
}

void LampGroupManager::GetLampGroup(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    MsgArg outArgs[4];

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    QStatus status = lampGroupsLock.Lock();
    if (ER_OK == status) {
        LampGroupMap::iterator it = lampGroups.find(uniqueId);
        if (it != lampGroups.end()) {
            it->second.second.Get(&outArgs[2], &outArgs[3]);
            responseCode = LSF_OK;
        } else {
            outArgs[2].Set("as", 0, NULL);
            outArgs[3].Set("as", 0, NULL);
        }
        status = lampGroupsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __FUNCTION__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __FUNCTION__));
    }

    outArgs[0].Set("u", responseCode);
    outArgs[1].Set("s", uniqueId);

    controllerService.SendMethodReply(message, outArgs, 4);
}

void LampGroupManager::ResetLampGroupState(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);
    LSFString lampGroupId = static_cast<LSFString>(args[0].v_string.str);
    QCC_DbgPrintf(("lampGroupId=%s", lampGroupId.c_str()));

    LSFStringList lamps;
    lamps.clear();

    LSFStringList lampGroupList;
    lampGroupList.push_back(lampGroupId);

    responseCode = GetAllGroupLamps(lampGroupList, lamps);
    QCC_DbgPrintf(("%s: Got a list of %d lamps", __FUNCTION__, lamps.size()));

    if (lamps.empty()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupId);
    } else {
        lampManager.ResetLampStateInternal(message, lamps, true);
    }
}

void LampGroupManager::TransitionLampGroupState(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);
    LSFString lampGroupId = static_cast<LSFString>(args[0].v_string.str);
    LampState state(args[1]);
    uint32_t transitionPeriod = static_cast<uint32_t>(args[2].v_uint32);
    QCC_DbgPrintf(("%s: lampGroupId=%s state=%s transitionPeriod=%d", __FUNCTION__, lampGroupId.c_str(), state.c_str(), transitionPeriod));

    LSFStringList lamps;
    lamps.clear();

    LSFStringList lampGroupList;
    lampGroupList.push_back(lampGroupId);

    TransitionLampsLampGroupsToStateList transitionToStateComponent;
    TransitionLampsLampGroupsToPresetList transitionToPresetComponent;
    PulseLampsLampGroupsWithStateList pulseWithStateComponent;
    PulseLampsLampGroupsWithPresetList pulseWithPresetComponent;

    TransitionLampsLampGroupsToState component(lamps, lampGroupList, state, transitionPeriod);
    transitionToStateComponent.push_back(component);

    responseCode = ChangeLampGroupStateAndField(message, transitionToStateComponent, transitionToPresetComponent, pulseWithStateComponent, pulseWithPresetComponent, true);

    if (LSF_ERR_NOT_FOUND == responseCode) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupId);
    }
}

void LampGroupManager::PulseLampGroupWithState(ajn::Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);
    LSFString lampGroupID = static_cast<LSFString>(args[0].v_string.str);
    LampState fromLampGroupState(args[1]);
    LampState toLampGroupState(args[2]);
    uint32_t period = static_cast<uint32_t>(args[3].v_uint32);
    uint32_t duration = static_cast<uint32_t>(args[4].v_uint32);
    uint32_t numPulses = static_cast<uint32_t>(args[5].v_uint32);
    QCC_DbgPrintf(("%s: lampGroupID=%s, fromLampGroupState=%s, period=%d, duration=%d, numPulses=%d",
                   __FUNCTION__, lampGroupID.c_str(), fromLampGroupState.c_str(), period, duration, numPulses));
    QCC_DbgPrintf(("%s: toLampGroupState=%s", __FUNCTION__, toLampGroupState.c_str()));

    LSFStringList lamps;
    lamps.clear();

    LSFStringList lampGroupList;
    lampGroupList.push_back(lampGroupID);

    responseCode = GetAllGroupLamps(lampGroupList, lamps);
    QCC_DbgPrintf(("%s: Got a list of %d lamps", __FUNCTION__, lamps.size()));

    TransitionLampsLampGroupsToStateList transitionToStateComponent;
    TransitionLampsLampGroupsToPresetList transitionToPresetComponent;
    PulseLampsLampGroupsWithStateList pulseWithStateComponent;
    PulseLampsLampGroupsWithPresetList pulseWithPresetComponent;

    PulseLampsLampGroupsWithState component(lamps, lampGroupList, fromLampGroupState, toLampGroupState, period, duration, numPulses);
    pulseWithStateComponent.push_back(component);

    responseCode = ChangeLampGroupStateAndField(message, transitionToStateComponent, transitionToPresetComponent, pulseWithStateComponent, pulseWithPresetComponent, true);

    if (LSF_ERR_NOT_FOUND == responseCode) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);
    }
}

void LampGroupManager::PulseLampGroupWithPreset(ajn::Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);
    LSFString lampGroupID = static_cast<LSFString>(args[0].v_string.str);
    LSFString fromPresetID = static_cast<LSFString>(args[1].v_string.str);
    LSFString toPresetID = static_cast<LSFString>(args[2].v_string.str);
    uint32_t period = static_cast<uint32_t>(args[3].v_uint32);
    uint32_t duration = static_cast<uint32_t>(args[4].v_uint32);
    uint32_t numPulses = static_cast<uint32_t>(args[5].v_uint32);
    QCC_DbgPrintf(("%s: lampGroupID=%s, fromPresetID=%s, toPresetID=%s, period=%d, duration=%d, numPulses=%d",
                   __FUNCTION__, lampGroupID.c_str(), fromPresetID.c_str(), toPresetID.c_str(), period, duration, numPulses));

    LSFStringList lamps;
    lamps.clear();

    LSFStringList lampGroupList;
    lampGroupList.push_back(lampGroupID);

    TransitionLampsLampGroupsToStateList transitionToStateComponent;
    TransitionLampsLampGroupsToPresetList transitionToPresetComponent;
    PulseLampsLampGroupsWithStateList pulseWithStateComponent;
    PulseLampsLampGroupsWithPresetList pulseWithPresetComponent;

    PulseLampsLampGroupsWithPreset component(lamps, lampGroupList, fromPresetID, toPresetID, period, duration, numPulses);
    pulseWithPresetComponent.push_back(component);

    responseCode = ChangeLampGroupStateAndField(message, transitionToStateComponent, transitionToPresetComponent, pulseWithStateComponent, pulseWithPresetComponent, true);

    if (LSF_ERR_NOT_FOUND == responseCode) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);
    }
}

void LampGroupManager::TransitionLampGroupStateToPreset(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);
    LSFString lampGroupId = static_cast<LSFString>(args[0].v_string.str);
    LSFString preset = static_cast<LSFString>(args[1].v_string.str);
    uint32_t transitionPeriod = static_cast<uint32_t>(args[2].v_uint32);
    QCC_DbgPrintf(("%s: lampGroupId=%s preset=%s transitionPeriod=%d", __FUNCTION__, lampGroupId.c_str(), preset.c_str(), transitionPeriod));

    LSFStringList lamps;
    lamps.clear();

    LSFStringList lampGroupList;
    lampGroupList.push_back(lampGroupId);

    TransitionLampsLampGroupsToStateList transitionToStateComponent;
    TransitionLampsLampGroupsToPresetList transitionToPresetComponent;
    PulseLampsLampGroupsWithStateList pulseWithStateComponent;
    PulseLampsLampGroupsWithPresetList pulseWithPresetComponent;

    TransitionLampsLampGroupsToPreset component(lamps, lampGroupList, preset, transitionPeriod);
    transitionToPresetComponent.push_back(component);

    responseCode = ChangeLampGroupStateAndField(message, transitionToStateComponent, transitionToPresetComponent, pulseWithStateComponent, pulseWithPresetComponent, true);

    if (LSF_ERR_NOT_FOUND == responseCode) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupId);
    }
}

void LampGroupManager::TransitionLampGroupStateField(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);
    LSFString lampGroupId = static_cast<LSFString>(args[0].v_string.str);
    LSFString fieldName = static_cast<LSFString>(args[1].v_string.str);
    MsgArg* varArg;
    args[2].Get("v", &varArg);
    uint32_t transitionPeriod = static_cast<uint32_t>(args[3].v_uint32);
    QCC_DbgPrintf(("lampGroupId=%s fieldName=%s transitionPeriod=%d", lampGroupId.c_str(), fieldName.c_str(), transitionPeriod));

    LSFStringList lamps;
    lamps.clear();

    LSFStringList lampGroupList;
    lampGroupList.push_back(lampGroupId);

    responseCode = GetAllGroupLamps(lampGroupList, lamps);
    QCC_DbgPrintf(("%s: Got a list of %d lamps", __FUNCTION__, lamps.size()));

    LampsAndStateList transitionToState;
    LampsAndPresetList transitionToPreset;
    LampsAndStateFieldList stateField;
    PulseLampsWithStateList pulseWithState;
    PulseLampsWithPresetList pulseWithPreset;

    if (lamps.empty()) {
        controllerService.SendMethodReplyWithResponseCodeIDAndName(message, responseCode, lampGroupId, fieldName);
    } else {
        LampsAndStateField stateFieldComponent(lamps, fieldName, *varArg, transitionPeriod);
        stateField.push_back(stateFieldComponent);
        lampManager.ChangeLampStateAndField(message, transitionToState, transitionToPreset, stateField, pulseWithState, pulseWithPreset, true);
    }
}

LSFResponseCode LampGroupManager::ChangeLampGroupStateAndField(Message& message,
                                                               TransitionLampsLampGroupsToStateList& transitionToStateComponent,
                                                               TransitionLampsLampGroupsToPresetList& transitionToPresetComponent,
                                                               PulseLampsLampGroupsWithStateList& pulseWithStateComponent,
                                                               PulseLampsLampGroupsWithPresetList& pulseWithPresetComponent,
                                                               bool groupOperation)
{
    LSFResponseCode responseCode = LSF_OK;

    LampsAndStateList transitionToStateList;
    LampsAndPresetList transitionToPresetList;
    PulseLampsWithStateList pulseWithStateList;
    PulseLampsWithPresetList pulseWithPresetList;
    LampsAndStateFieldList stateFieldList;

    uint8_t numLamps = 0;

    while (transitionToStateComponent.size()) {
        TransitionLampsLampGroupsToState transitionToStateComp = transitionToStateComponent.front();
        LampsAndState lampsAndState(transitionToStateComp.lamps, transitionToStateComp.state, transitionToStateComp.transitionPeriod);

        GetAllGroupLamps(transitionToStateComp.lampGroups, lampsAndState.lamps);

        if (lampsAndState.lamps.size()) {
            transitionToStateList.push_back(lampsAndState);
            numLamps += lampsAndState.lamps.size();
        }

        transitionToStateComponent.pop_front();
    }

    while (transitionToPresetComponent.size()) {
        TransitionLampsLampGroupsToPreset transitionToPresetComp = transitionToPresetComponent.front();
        LampsAndPreset lampsAndPreset(transitionToPresetComp.lamps, transitionToPresetComp.presetID, transitionToPresetComp.transitionPeriod);

        GetAllGroupLamps(transitionToPresetComp.lampGroups, lampsAndPreset.lamps);

        if (lampsAndPreset.lamps.size()) {
            transitionToPresetList.push_back(lampsAndPreset);
            numLamps += lampsAndPreset.lamps.size();
        }

        transitionToPresetComponent.pop_front();
    }

    while (pulseWithStateComponent.size()) {
        PulseLampsLampGroupsWithState pulseWithStateComp = pulseWithStateComponent.front();
        PulseLampsWithState pulseWithState(pulseWithStateComp.lamps, pulseWithStateComp.fromState, pulseWithStateComp.toState,
                                           pulseWithStateComp.pulsePeriod, pulseWithStateComp.pulseDuration, pulseWithStateComp.numPulses);

        GetAllGroupLamps(pulseWithStateComp.lampGroups, pulseWithState.lamps);

        if (pulseWithState.lamps.size()) {
            pulseWithStateList.push_back(pulseWithState);
            numLamps += pulseWithState.lamps.size();
        }

        pulseWithStateComponent.pop_front();
    }

    while (pulseWithPresetComponent.size()) {
        PulseLampsLampGroupsWithPreset pulseWithPresetComp = pulseWithPresetComponent.front();
        PulseLampsWithPreset pulseWithPreset(pulseWithPresetComp.lamps, pulseWithPresetComp.fromPreset, pulseWithPresetComp.toPreset,
                                             pulseWithPresetComp.pulsePeriod, pulseWithPresetComp.pulseDuration, pulseWithPresetComp.numPulses);

        GetAllGroupLamps(pulseWithPresetComp.lampGroups, pulseWithPreset.lamps);

        if (pulseWithPreset.lamps.size()) {
            pulseWithPresetList.push_back(pulseWithPreset);
            numLamps += pulseWithPreset.lamps.size();
        }

        pulseWithPresetComponent.pop_front();
    }

    if (numLamps == 0) {
        responseCode = LSF_ERR_NOT_FOUND;
    } else {
        lampManager.ChangeLampStateAndField(message, transitionToStateList, transitionToPresetList, stateFieldList, pulseWithStateList, pulseWithPresetList, groupOperation, !groupOperation);
    }

    return responseCode;
}

void LampGroupManager::ResetLampGroupStateField(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __FUNCTION__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);
    LSFString lampGroupId = static_cast<LSFString>(args[0].v_string.str);
    LSFString fieldName = static_cast<LSFString>(args[1].v_string.str);
    QCC_DbgPrintf(("%s: lampGroupId=%s fieldName=%s", __FUNCTION__, lampGroupId.c_str(), fieldName.c_str()));

    LSFStringList lamps;
    lamps.clear();

    LSFStringList lampGroupList;
    lampGroupList.push_back(lampGroupId);

    responseCode = GetAllGroupLamps(lampGroupList, lamps);
    QCC_DbgPrintf(("%s: Got a list of %d lamps", __FUNCTION__, lamps.size()));

    if (lamps.empty()) {
        controllerService.SendMethodReplyWithResponseCodeIDAndName(message, responseCode, lampGroupId, fieldName);
    } else {
        lampManager.ResetLampStateFieldInternal(message, lamps, fieldName, true);
    }
}

LSFResponseCode LampGroupManager::GetAllGroupLamps(LSFStringList& lampGroupList, LSFStringList& lamps)
{
    QCC_DbgPrintf(("%s: lampGroupList.size()(%d)", __FUNCTION__, lampGroupList.size()));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = lampGroupsLock.Lock();
    if (ER_OK == status) {
        while (lampGroupList.size()) {
            LSFString lampGroupId = lampGroupList.front();
            LampGroupMap::iterator it = lampGroups.find(lampGroupId);
            if (it != lampGroups.end()) {
                QCC_DbgPrintf(("%s: Lamp list size = %d", __FUNCTION__, it->second.second.lamps.size()));
                for (LSFStringList::iterator lampIt = it->second.second.lamps.begin(); lampIt != it->second.second.lamps.end(); lampIt++) {
                    lamps.push_back(*lampIt);
                    QCC_DbgPrintf(("%s: lampId = %s", __FUNCTION__, (*lampIt).c_str()));
                }
                QCC_DbgPrintf(("%s: Lamp Groups list size = %d", __FUNCTION__, it->second.second.lampGroups.size()));
                if (it->second.second.lampGroups.size()) {
                    LSFStringList groupList = it->second.second.lampGroups;
                    LSFResponseCode tempResponseCode = GetAllGroupLamps(groupList, lamps);
                    if (LSF_ERR_NOT_FOUND == tempResponseCode) {
                        responseCode = LSF_ERR_PARTIAL;
                    } else {
                        responseCode = tempResponseCode;
                    }
                }
            } else {
                QCC_DbgPrintf(("%s: Lamp Group %s not found", __FUNCTION__, lampGroupId.c_str()));
                responseCode = LSF_ERR_NOT_FOUND;
            }
            lampGroupList.pop_front();
        }
        status = lampGroupsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __FUNCTION__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __FUNCTION__));
    }

    return responseCode;
}

void LampGroupManager::ReadSavedData()
{
    if (filePath.empty()) {
        return;
    }

    std::ifstream stream(filePath.c_str());

    if (!stream.is_open()) {
        QCC_DbgPrintf(("File not found: %s\n", filePath.c_str()));
        return;
    }

    while (!stream.eof()) {
        std::string token = ParseString(stream);

        if (token == "LampGroup") {
            std::string id = ParseString(stream);
            std::string name = ParseString(stream);
            LampGroup group;

            do {
                token = ParseString(stream);

                if (token == "Lamp") {
                    std::string member = ParseString(stream);
                    group.lamps.push_back(member);
                } else if (token == "LampGroup") {
                    std::string member = ParseString(stream);
                    group.lampGroups.push_back(member);
                } else {
                    break;
                }
            } while (token != "EndLampGroup");

            std::pair<LSFString, LampGroup> thePair(name, group);
            lampGroups[id] = thePair;
        }
    }
}

void LampGroupManager::WriteFile()
{
    QCC_DbgPrintf(("%s", __FUNCTION__));
    if (!updated) {
        printf("Not updated\n");
        return;
    }

    if (filePath.empty()) {
        printf("No path\n");
        return;
    }

    lampGroupsLock.Lock();
    // we can't hold this lock for the entire time!
    LampGroupMap mapCopy = lampGroups;
    updated = false;
    lampGroupsLock.Unlock();

    std::ofstream stream(filePath.c_str(), std::ios_base::out);
    if (!stream.is_open()) {
        printf("File not found: %s\n", filePath.c_str());
        QCC_DbgPrintf(("File not found: %s\n", filePath.c_str()));
        return;
    }

    // (LampGroup id "name" (Lamp id)* (SubGroup id)* EndLampGroup)*

    for (LampGroupMap::const_iterator it = mapCopy.begin(); it != mapCopy.end(); ++it) {
        const LSFString& id = it->first;
        const LSFString& name = it->second.first;
        const LampGroup& group = it->second.second;

        stream << "LampGroup " << id << " \"" << name << "\"";

        for (LSFStringList::const_iterator lit = group.lamps.begin(); lit != group.lamps.end(); ++lit) {
            stream << " Lamp " << *lit;
        }
        for (LSFStringList::const_iterator lit = group.lampGroups.begin(); lit != group.lampGroups.end(); ++lit) {
            stream << " LampGroup " << *lit;
        }

        stream << " EndLampGroup" << std::endl;
    }

    stream.close();
}

uint32_t LampGroupManager::GetControllerServiceLampGroupInterfaceVersion(void)
{
    QCC_DbgPrintf(("%s: controllerLampGroupInterfaceVersion=%d", __FUNCTION__, ControllerServiceLampGroupInterfaceVersion));
    return ControllerServiceLampGroupInterfaceVersion;
}
