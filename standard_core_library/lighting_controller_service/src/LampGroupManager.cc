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
#include <lsf/controllerservice/LampGroupManager.h>
#include <lsf/controllerservice/ControllerService.h>
#include <lsf/controllerservice/SceneManager.h>
#include <lsf/controllerservice/OEM_CS_Config.h>
#include <lsf/controllerservice/FileParser.h>
#else
#include <LampGroupManager.h>
#include <ControllerService.h>
#include <SceneManager.h>
#include <OEM_CS_Config.h>
#include <FileParser.h>
#endif

#include <qcc/atomic.h>
#include <qcc/Debug.h>
#include <sstream>
#include <streambuf>
#include <algorithm>

using namespace lsf;
using namespace ajn;

#ifdef LSF_BINDINGS
using namespace controllerservice;
#define QCC_MODULE "CONTROLLER_LAMP_GROUP_MANAGER"
#else
#define QCC_MODULE "LAMP_GROUP_MANAGER"
#endif

LampGroupManager::LampGroupManager(ControllerService& controllerSvc, LampManager& lampMgr, SceneElementManager* sceneElementMgrPtr, const std::string& lampGroupFile) :
    Manager(controllerSvc, lampGroupFile), lampManager(lampMgr), sceneElementManagerPtr(sceneElementMgrPtr), blobLength(0)
{
    QCC_DbgTrace(("%s", __func__));
    lampGroups.clear();
    lampGroupUpdates.clear();
}

LSFResponseCode LampGroupManager::GetAllLampGroups(LampGroupMap& lampGroupMap)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = lampGroupsLock.Lock();
    if (ER_OK == status) {
        lampGroupMap = lampGroups;
        status = lampGroupsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __func__));
    }

    return responseCode;
}

LSFResponseCode LampGroupManager::Reset(void)
{
    QCC_DbgPrintf(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    if (!controllerService.UpdatesAllowed()) {
        return LSF_ERR_BUSY;
    }

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
        lampGroupUpdates.clear();
        blobLength = 0;

        ScheduleFileWrite();

        tempStatus = lampGroupsLock.Unlock();
        if (ER_OK != tempStatus) {
            QCC_LogError(tempStatus, ("%s: lampGroupsLock.Unlock() failed", __func__));
        }

        /*
         * Send the LampGroups deleted signal
         */
        if (lampGroupsList.size()) {
            tempStatus = controllerService.SendSignal(ControllerServiceLampGroupInterfaceName, "LampGroupsDeleted", lampGroupsList);
            if (ER_OK != tempStatus) {
                QCC_LogError(tempStatus, ("%s: Unable to send LampGroupsDeleted signal", __func__));
            }
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(tempStatus, ("%s: lampGroupsLock.Lock() failed", __func__));
    }

    return responseCode;
}

LSFResponseCode LampGroupManager::IsDependentOnLampGroup(LSFString& lampGroupID)
{
    QCC_DbgTrace(("%s", __func__));
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
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __func__));
    }

    if (LSF_OK == responseCode) {
        responseCode = sceneElementManagerPtr->IsDependentOnLampGroup(lampGroupID);
    }

    return responseCode;
}

void LampGroupManager::GetAllLampGroupIDs(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFStringList idList;
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = lampGroupsLock.Lock();
    if (ER_OK == status) {
        for (LampGroupMap::iterator it = lampGroups.begin(); it != lampGroups.end(); ++it) {
            idList.push_back(it->first.c_str());
        }
        status = lampGroupsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __func__));
    }

    controllerService.SendMethodReplyWithResponseCodeAndListOfIDs(message, responseCode, idList);
}

void LampGroupManager::GetLampGroupName(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFString name;
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 2)  != LSF_OK) {
        return;
    }

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    LSFString lampGroupID(uniqueId);

    LSFString language = static_cast<LSFString>(args[1].v_string.str);
    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
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
                QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDLanguageAndName(message, responseCode, lampGroupID, language, name);
}

void LampGroupManager::SetLampGroupName(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    bool nameChanged = false;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 3)  != LSF_OK) {
        return;
    }

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    LSFString lampGroupID(uniqueId);

    const char* name;
    args[1].Get("s", &name);

    LSFString language = static_cast<LSFString>(args[2].v_string.str);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeIDAndName(message, LSF_ERR_BUSY, lampGroupID, language);
        return;
    }

    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name[0] == '\0') {
        QCC_LogError(ER_FAIL, ("%s: group name is empty", __func__));
        responseCode = LSF_ERR_EMPTY_NAME;
    } else {
        if (strlen(name) > LSF_MAX_NAME_LENGTH) {
            responseCode = LSF_ERR_INVALID_ARGS;
            QCC_LogError(ER_FAIL, ("%s: strlen(name) > LSF_MAX_NAME_LENGTH", __func__));
        } else {
            QStatus status = lampGroupsLock.Lock();
            if (ER_OK == status) {
                LampGroupMap::iterator it = lampGroups.find(uniqueId);
                if (it != lampGroups.end()) {
                    LSFString newName = name;
                    size_t newlen = blobLength - it->second.first.length() + newName.length();

                    if (newlen < MAX_FILE_LEN) {
                        blobLength = newlen;
                        it->second.first = newName;
                        responseCode = LSF_OK;
                        nameChanged = true;
                        ScheduleFileWrite();
                    } else {
                        responseCode = LSF_ERR_RESOURCES;
                    }
                }
                status = lampGroupsLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __func__));
                }
            } else {
                responseCode = LSF_ERR_BUSY;
                QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __func__));
            }
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDAndName(message, responseCode, lampGroupID, language);

    if (nameChanged) {
        LSFStringList idList;
        idList.push_back(lampGroupID);
        controllerService.SendSignal(ControllerServiceLampGroupInterfaceName, "LampGroupsNameChanged", idList);
    }
}

void LampGroupManager::CreateLampGroup(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_OK;
    LSFString lampGroupID;

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, lampGroupID);
        return;
    }

    lampGroupID = GenerateUniqueID("LAMP_GROUP");

    bool created = false;

    const ajn::MsgArg* inputArgs;
    size_t numInputArgs;
    message->GetArgs(numInputArgs, inputArgs);

    if (controllerService.CheckNumArgsInMessage(numInputArgs, 4)  != LSF_OK) {
        return;
    }

    LampGroup lampGroup(inputArgs[0], inputArgs[1]);
    LSFString name = static_cast<LSFString>(inputArgs[2].v_string.str);
    LSFString language = static_cast<LSFString>(inputArgs[3].v_string.str);

    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name.empty()) {
        QCC_LogError(ER_FAIL, ("%s: group name is empty", __func__));
        responseCode = LSF_ERR_EMPTY_NAME;
    } else if (name.length() > LSF_MAX_NAME_LENGTH) {
        QCC_LogError(ER_FAIL, ("%s: name length exceeds %d", __func__, LSF_MAX_NAME_LENGTH));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (lampGroup.lamps.empty() && lampGroup.lampGroups.empty()) {
        QCC_LogError(ER_FAIL, ("%s: Empty Lamps and LampGroups list", __func__));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = lampGroupsLock.Lock();
        if (ER_OK == status) {
            if (lampGroups.size() < OEM_CS_MAX_SUPPORTED_NUM_LSF_ENTITY) {
                std::string newGroupStr = GetString(name, lampGroupID, lampGroup);
                /*
                 * We have to add the lampGroupID length because we need to store
                 * the IDs of the updated lamp groups to the updates file
                 */
                size_t newlen = blobLength + newGroupStr.length() + lampGroupID.length();
                if (newlen < MAX_FILE_LEN) {
                    blobLength = newlen;
                    lampGroups[lampGroupID].first = name;
                    lampGroups[lampGroupID].second = lampGroup;
                    created = true;
                    ScheduleFileWrite();
                } else {
                    responseCode = LSF_ERR_RESOURCES;
                }
            } else {
                QCC_LogError(ER_FAIL, ("%s: No slot for new LampGroup", __func__));
                responseCode = LSF_ERR_NO_SLOT;
            }
            status = lampGroupsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __func__));
        }
    }

    if (!created) {
        lampGroupID.clear();
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);

    if (created) {
        LSFStringList idList;
        idList.push_back(lampGroupID);
        controllerService.SendSignal(ControllerServiceLampGroupInterfaceName, "LampGroupsCreated", idList);
    }
}

void LampGroupManager::UpdateLampGroup(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    bool updated = false;

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 3) != LSF_OK) {
        return;
    }

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    LSFString lampGroupID(uniqueId);
    LampGroup lampGroup(args[1], args[2]);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, lampGroupID);
        return;
    }

    if (lampGroup.lamps.empty() && lampGroup.lampGroups.empty()) {
        QCC_LogError(ER_FAIL, ("%s: Empty Lamps and LampGroups list", __func__));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = lampGroupsLock.Lock();
        if (ER_OK == status) {
            LampGroupMap::iterator it = lampGroups.find(uniqueId);
            if (it != lampGroups.end()) {

                size_t newlen = blobLength;
                // sub len of old group, add len of new group
                newlen -= GetString(it->second.first, lampGroupID, it->second.second).length();
                newlen += GetString(it->second.first, lampGroupID, lampGroup).length();

                if (newlen < MAX_FILE_LEN) {
                    blobLength = newlen;
                    it->second.second = lampGroup;
                    responseCode = LSF_OK;
                    if (lampGroupUpdates.find(lampGroupID) == lampGroupUpdates.end()) {
                        lampGroupUpdates.insert(lampGroupID);
                    }
                    updated = true;
                    ScheduleFileWrite();
                } else {
                    responseCode = LSF_ERR_RESOURCES;
                }
            }
            status = lampGroupsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);

    if (updated) {
        LSFStringList idList;
        idList.push_back(lampGroupID);
        controllerService.SendSignal(ControllerServiceLampGroupInterfaceName, "LampGroupsUpdated", idList);
    }
}

void LampGroupManager::DeleteLampGroup(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;

    bool deleted = false;

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

    const char* lampGroupId;
    args[0].Get("s", &lampGroupId);

    LSFString lampGroupID(lampGroupId);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, lampGroupID);
        return;
    }

    responseCode = IsDependentOnLampGroup(lampGroupID);

    if (LSF_OK == responseCode) {
        QStatus status = lampGroupsLock.Lock();
        if (ER_OK == status) {
            LampGroupMap::iterator it = lampGroups.find(lampGroupId);
            if (it != lampGroups.end()) {
                blobLength -= ((GetString(it->second.first, lampGroupId, it->second.second).length()) + lampGroupID.length());

                lampGroups.erase(it);
                if (lampGroupUpdates.find(lampGroupID) != lampGroupUpdates.end()) {
                    lampGroupUpdates.erase(lampGroupID);
                }
                deleted = true;
                ScheduleFileWrite();
            } else {
                responseCode = LSF_ERR_NOT_FOUND;
            }
            status = lampGroupsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);

    if (deleted) {
        LSFStringList idList;
        idList.push_back(lampGroupID);
        controllerService.SendSignal(ControllerServiceLampGroupInterfaceName, "LampGroupsDeleted", idList);
    }
}

void LampGroupManager::GetLampGroup(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    MsgArg outArgs[4];

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

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
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __func__));
    }

    outArgs[0].Set("u", responseCode);
    outArgs[1].Set("s", uniqueId);

    controllerService.SendMethodReply(message, outArgs, 4);
}

void LampGroupManager::ResetLampGroupState(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

    LSFString lampGroupId = static_cast<LSFString>(args[0].v_string.str);
    QCC_DbgPrintf(("lampGroupId=%s", lampGroupId.c_str()));

    LSFStringList lamps;
    lamps.clear();

    LSFStringList lampGroupList;
    lampGroupList.push_back(lampGroupId);

    responseCode = GetAllGroupLamps(lampGroupList, lamps);
    QCC_DbgPrintf(("%s: Got a list of %d lamps", __func__, lamps.size()));

    if (lamps.empty()) {
        if (lampGroupId == AllLampsGroupIdentifier) {
            QCC_DbgPrintf(("%s: Processing All Lamps Operation", __func__));
            lampManager.ResetLampStateInternal(message, lamps, true, true);
        } else {
            controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupId);
        }
    } else {
        lampManager.ResetLampStateInternal(message, lamps, true);
    }
}

void LampGroupManager::TransitionLampGroupState(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 3)  != LSF_OK) {
        return;
    }

    LSFString lampGroupId = static_cast<LSFString>(args[0].v_string.str);
    LampState state(args[1]);
    uint32_t transitionPeriod = static_cast<uint32_t>(args[2].v_uint32);
    //QCC_DbgPrintf(("%s: lampGroupId=%s state=%s transitionPeriod=%d", __func__, lampGroupId.c_str(), state.c_str(), transitionPeriod));

    if (state.nullState) {
        QCC_LogError(ER_FAIL, ("%s: State cannot be NULL", __func__));
        LSFResponseCode responseCode = LSF_ERR_INVALID_ARGS;
        controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupId);
    } else {
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

        responseCode = ChangeLampGroupStateAndField(message, transitionToStateComponent, transitionToPresetComponent, pulseWithStateComponent, pulseWithPresetComponent, true, (lampGroupId == AllLampsGroupIdentifier));

        if (LSF_ERR_NOT_FOUND == responseCode) {
            controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupId);
        }
    }
}

void LampGroupManager::PulseLampGroupWithState(ajn::Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 6)  != LSF_OK) {
        return;
    }

    LSFString lampGroupID = static_cast<LSFString>(args[0].v_string.str);
    LampState fromLampGroupState(args[1]);
    LampState toLampGroupState(args[2]);
    uint32_t period = static_cast<uint32_t>(args[3].v_uint32);
    uint32_t duration = static_cast<uint32_t>(args[4].v_uint32);
    uint32_t numPulses = static_cast<uint32_t>(args[5].v_uint32);
    //QCC_DbgPrintf(("%s: lampGroupID=%s, fromLampGroupState=%s, period=%d, duration=%d, numPulses=%d",
    //__func__, lampGroupID.c_str(), fromLampGroupState.c_str(), period, duration, numPulses));
    //QCC_DbgPrintf(("%s: toLampGroupState=%s", __func__, toLampGroupState.c_str()));

    if (toLampGroupState.nullState) {
        QCC_LogError(ER_FAIL, ("%s: ToLampGroupState cannot be NULL", __func__));
        LSFResponseCode responseCode = LSF_ERR_INVALID_ARGS;
        controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);
    } else {
        LSFStringList lamps;
        lamps.clear();

        LSFStringList lampGroupList;
        lampGroupList.push_back(lampGroupID);

        responseCode = GetAllGroupLamps(lampGroupList, lamps);
        QCC_DbgPrintf(("%s: Got a list of %d lamps", __func__, lamps.size()));

        TransitionLampsLampGroupsToStateList transitionToStateComponent;
        TransitionLampsLampGroupsToPresetList transitionToPresetComponent;
        PulseLampsLampGroupsWithStateList pulseWithStateComponent;
        PulseLampsLampGroupsWithPresetList pulseWithPresetComponent;

        PulseLampsLampGroupsWithState component(lamps, lampGroupList, fromLampGroupState, toLampGroupState, period, duration, numPulses);
        pulseWithStateComponent.push_back(component);

        responseCode = ChangeLampGroupStateAndField(message, transitionToStateComponent, transitionToPresetComponent, pulseWithStateComponent, pulseWithPresetComponent, true, (lampGroupID == AllLampsGroupIdentifier));

        if (LSF_ERR_NOT_FOUND == responseCode) {
            controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);
        }
    }
}

void LampGroupManager::PulseLampGroupWithPreset(ajn::Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 6)  != LSF_OK) {
        return;
    }

    LSFString lampGroupID = static_cast<LSFString>(args[0].v_string.str);
    LSFString fromPresetID = static_cast<LSFString>(args[1].v_string.str);
    LSFString toPresetID = static_cast<LSFString>(args[2].v_string.str);
    uint32_t period = static_cast<uint32_t>(args[3].v_uint32);
    uint32_t duration = static_cast<uint32_t>(args[4].v_uint32);
    uint32_t numPulses = static_cast<uint32_t>(args[5].v_uint32);
    QCC_DbgPrintf(("%s: lampGroupID=%s, fromPresetID=%s, toPresetID=%s, period=%d, duration=%d, numPulses=%d",
                   __func__, lampGroupID.c_str(), fromPresetID.c_str(), toPresetID.c_str(), period, duration, numPulses));

    if (0 == strcmp(toPresetID.c_str(), CurrentStateIdentifier.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: ToPreset cannot be the current state", __func__));
        LSFResponseCode responseCode = LSF_ERR_INVALID_ARGS;
        controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);
    } else {
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

        responseCode = ChangeLampGroupStateAndField(message, transitionToStateComponent, transitionToPresetComponent, pulseWithStateComponent, pulseWithPresetComponent, true, (lampGroupID == AllLampsGroupIdentifier));

        if (LSF_ERR_NOT_FOUND == responseCode) {
            controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupID);
        }
    }
}

void LampGroupManager::TransitionLampGroupStateToPreset(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 3)  != LSF_OK) {
        return;
    }

    LSFString lampGroupId = static_cast<LSFString>(args[0].v_string.str);
    LSFString preset = static_cast<LSFString>(args[1].v_string.str);
    uint32_t transitionPeriod = static_cast<uint32_t>(args[2].v_uint32);
    QCC_DbgPrintf(("%s: lampGroupId=%s preset=%s transitionPeriod=%d", __func__, lampGroupId.c_str(), preset.c_str(), transitionPeriod));

    if (0 == strcmp(preset.c_str(), CurrentStateIdentifier.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Preset cannot be the current state", __func__));
        LSFResponseCode responseCode = LSF_ERR_INVALID_ARGS;
        controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupId);
    } else {
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

        responseCode = ChangeLampGroupStateAndField(message, transitionToStateComponent, transitionToPresetComponent, pulseWithStateComponent, pulseWithPresetComponent, true, (lampGroupId == AllLampsGroupIdentifier));

        if (LSF_ERR_NOT_FOUND == responseCode) {
            controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, lampGroupId);
        }
    }
}

void LampGroupManager::TransitionLampGroupStateField(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 4)  != LSF_OK) {
        return;
    }

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
    QCC_DbgPrintf(("%s: Got a list of %d lamps", __func__, lamps.size()));

    LampsAndStateList transitionToState;
    LampsAndPresetList transitionToPreset;
    LampsAndStateFieldList stateField;
    PulseLampsWithStateList pulseWithState;
    PulseLampsWithPresetList pulseWithPreset;

    if (lamps.empty() && (lampGroupId != AllLampsGroupIdentifier)) {
        controllerService.SendMethodReplyWithResponseCodeIDAndName(message, responseCode, lampGroupId, fieldName);
    } else {
        LampsAndStateField stateFieldComponent(lamps, fieldName, *varArg, transitionPeriod);
        stateField.push_back(stateFieldComponent);
        lampManager.ChangeLampStateAndField(message, transitionToState, transitionToPreset, stateField, pulseWithState, pulseWithPreset, true, (lampGroupId == AllLampsGroupIdentifier));
    }
}

LSFResponseCode LampGroupManager::ChangeLampGroupStateAndField(Message& message,
                                                               TransitionLampsLampGroupsToStateList& transitionToStateComponent,
                                                               TransitionLampsLampGroupsToPresetList& transitionToPresetComponent,
                                                               PulseLampsLampGroupsWithStateList& pulseWithStateComponent,
                                                               PulseLampsLampGroupsWithPresetList& pulseWithPresetComponent,
                                                               bool groupOperation, bool allLamps, bool sceneOperation, LSFString sceneOrMasterSceneID, bool effectOperation)
{
    QCC_DbgPrintf(("%s: allLamps=%u", __func__, allLamps));
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

        if (lampsAndState.lamps.size() || allLamps) {
            transitionToStateList.push_back(lampsAndState);
            numLamps += lampsAndState.lamps.size();
        }

        transitionToStateComponent.pop_front();
    }

    while (transitionToPresetComponent.size()) {
        TransitionLampsLampGroupsToPreset transitionToPresetComp = transitionToPresetComponent.front();
        LampsAndPreset lampsAndPreset(transitionToPresetComp.lamps, transitionToPresetComp.presetID, transitionToPresetComp.transitionPeriod);

        GetAllGroupLamps(transitionToPresetComp.lampGroups, lampsAndPreset.lamps);

        if (lampsAndPreset.lamps.size() || allLamps) {
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

        if (pulseWithState.lamps.size() || allLamps) {
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

        if (pulseWithPreset.lamps.size() || allLamps) {
            pulseWithPresetList.push_back(pulseWithPreset);
            numLamps += pulseWithPreset.lamps.size();
        }

        pulseWithPresetComponent.pop_front();
    }

    if (numLamps == 0 && (!allLamps)) {
        responseCode = LSF_ERR_NOT_FOUND;
    } else {
        lampManager.ChangeLampStateAndField(message, transitionToStateList, transitionToPresetList, stateFieldList, pulseWithStateList, pulseWithPresetList, groupOperation, allLamps, sceneOperation, sceneOrMasterSceneID, effectOperation);
    }

    return responseCode;
}

void LampGroupManager::ResetLampGroupStateField(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 2)  != LSF_OK) {
        return;
    }

    LSFString lampGroupId = static_cast<LSFString>(args[0].v_string.str);
    LSFString fieldName = static_cast<LSFString>(args[1].v_string.str);
    QCC_DbgPrintf(("%s: lampGroupId=%s fieldName=%s", __func__, lampGroupId.c_str(), fieldName.c_str()));

    LSFStringList lamps;
    lamps.clear();

    LSFStringList lampGroupList;
    lampGroupList.push_back(lampGroupId);

    responseCode = GetAllGroupLamps(lampGroupList, lamps);
    QCC_DbgPrintf(("%s: Got a list of %d lamps", __func__, lamps.size()));

    if (lamps.empty() && (lampGroupId != AllLampsGroupIdentifier)) {
        controllerService.SendMethodReplyWithResponseCodeIDAndName(message, responseCode, lampGroupId, fieldName);
    } else {
        lampManager.ResetLampStateFieldInternal(message, lamps, fieldName, true, (lampGroupId == AllLampsGroupIdentifier));
    }
}

LSFResponseCode LampGroupManager::GetAllGroupLampsInternal(LSFStringList& lampGroupList, LSFStringList& lamps, LSFStringList& refList)
{
    QCC_DbgPrintf(("%s: lampGroupList.size()(%d)", __func__, lampGroupList.size()));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = lampGroupsLock.Lock();
    if (ER_OK == status) {
        for (LSFStringList::iterator git = lampGroupList.begin(); git != lampGroupList.end(); git++) {
            LSFString lampGroupId = *git;
            if (std::find(refList.begin(), refList.end(), lampGroupId) == refList.end()) {
                LampGroupMap::iterator it = lampGroups.find(lampGroupId);
                if (it != lampGroups.end()) {
                    refList.push_back(lampGroupId);
                    QCC_DbgPrintf(("%s: Lamp list size = %d", __func__, it->second.second.lamps.size()));
                    CreateUniqueList(lamps, it->second.second.lamps);

                    QCC_DbgPrintf(("%s: Lamp Groups list size = %d", __func__, it->second.second.lampGroups.size()));
                    if (it->second.second.lampGroups.size()) {
                        LSFStringList groupList = it->second.second.lampGroups;
                        LSFResponseCode tempResponseCode = GetAllGroupLampsInternal(groupList, lamps, refList);
                        if (LSF_ERR_NOT_FOUND == tempResponseCode) {
                            responseCode = LSF_ERR_PARTIAL;
                        } else {
                            responseCode = tempResponseCode;
                        }
                    }
                } else {
                    QCC_DbgPrintf(("%s: Lamp Group %s not found", __func__, lampGroupId.c_str()));
                    responseCode = LSF_ERR_NOT_FOUND;
                }
            } else {
                QCC_DbgPrintf(("%s: Lamp Group %s already processed", __func__, lampGroupId.c_str()));
            }
        }
        status = lampGroupsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lampGroupsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: lampGroupsLock.Lock() failed", __func__));
    }

    return responseCode;
}

void LampGroupManager::ReadSavedData()
{
    QCC_DbgTrace(("%s", __func__));
    std::istringstream stream;
    if (!ValidateFileAndRead(stream)) {
        /*
         * If there is no file present / CRC check failed on the file create a new
         * file with initialState entry
         */
        lampGroupsLock.Lock();
        ScheduleFileWrite(false, true);
        lampGroupsLock.Unlock();
        return;
    }

    blobLength = stream.str().size();
    ReplaceMap(stream);

    std::istringstream updateStream;
    if (ValidateUpdateFileAndRead(updateStream)) {
        ReplaceUpdatesList(updateStream);
    }
}

void LampGroupManager::ReplaceMap(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));
    bool firstIteration = true;
    while (!stream.eof()) {
        std::string token = ParseString(stream);

        if (token == resetID) {
            QCC_DbgPrintf(("The file has a reset entry. Clearing the map"));
            lampGroups.clear();
        } else if (token == "LampGroup") {
            std::string id = ParseString(stream);
            std::string name = ParseString(stream);

            if (0 == strcmp(id.c_str(), resetID.c_str())) {
                QCC_DbgPrintf(("The file has a reset entry. Clearing the map"));
                lampGroups.clear();
            } else if (0 == strcmp(id.c_str(), initialStateID.c_str())) {
                QCC_DbgPrintf(("The file has a initialState entry. So we ignore it"));
            } else {
                if (firstIteration) {
                    lampGroups.clear();
                    firstIteration = false;
                }
                blobLength += id.length();
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
}

void LampGroupManager::ReplaceUpdatesList(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));
    bool firstIteration = true;
    while (!stream.eof()) {
        std::string id = ParseString(stream);
        if (0 == strcmp(id.c_str(), resetID.c_str())) {
            QCC_DbgPrintf(("The file has a reset entry. Clearing the list"));
            lampGroupUpdates.clear();
        } else if (0 == strcmp(id.c_str(), initialStateID.c_str())) {
            QCC_DbgPrintf(("The file has a initialState entry. So we ignore it"));
        } else {
            if (firstIteration) {
                lampGroupUpdates.clear();
                firstIteration = false;
            }
            if (!id.empty()) {
                lampGroupUpdates.insert(id);
                QCC_DbgPrintf(("%s: Add %s to updates list", __func__, id.c_str()));
            }
        }
    }
}

std::string LampGroupManager::GetString(const std::string& name, const std::string& id, const LampGroup& group)
{
    std::ostringstream stream;
    stream << "LampGroup " << id << " \"" << name << "\"";

    for (LSFStringList::const_iterator lit = group.lamps.begin(); lit != group.lamps.end(); ++lit) {
        stream << " Lamp " << *lit;
    }
    for (LSFStringList::const_iterator lit = group.lampGroups.begin(); lit != group.lampGroups.end(); ++lit) {
        stream << " LampGroup " << *lit;
    }

    stream << " EndLampGroup" << std::endl;
    return stream.str();
}

std::string LampGroupManager::GetString(const LampGroupMap& items)
{
    QCC_DbgTrace(("%s", __func__));
    // (LampGroup id "name" (Lamp id)* (SubGroup id)* EndLampGroup)*
    std::ostringstream stream;
    if (0 == items.size()) {
        if (initialState) {
            QCC_DbgPrintf(("%s: This is the initial state entry", __func__));
            const LSFString& id = initialStateID;
            const LSFString& name = initialStateID;

            stream << "LampGroup " << id << " \"" << name << "\"";
            stream << " EndLampGroup" << std::endl;
        } else {
            QCC_DbgPrintf(("%s: File is being reset", __func__));
            const LSFString& id = resetID;
            const LSFString& name = resetID;

            stream << "LampGroup " << id << " \"" << name << "\"";
            stream << " EndLampGroup" << std::endl;
        }
    } else {
        for (LampGroupMap::const_iterator it = items.begin(); it != items.end(); ++it) {
            const LSFString& id = it->first;
            const LSFString& name = it->second.first;
            const LampGroup& group = it->second.second;

            stream << GetString(name, id, group);
        }
    }

    return stream.str();
}

std::string LampGroupManager::GetUpdatesString(const std::set<LSFString>& updates)
{
    QCC_DbgTrace(("%s", __func__));
    std::ostringstream stream;
    if (0 == updates.size()) {
        if (initialState) {
            QCC_DbgPrintf(("%s: This is the initial state entry", __func__));
            stream << initialStateID << std::endl;
        } else {
            QCC_DbgPrintf(("%s: File is being reset", __func__));
            stream << resetID << std::endl;
        }
    } else {
        for (std::set<LSFString>::const_iterator it = updates.begin(); it != updates.end(); ++it) {
            stream << *it << std::endl;
        }
    }

    return stream.str();
}

bool LampGroupManager::GetString(std::string& output, std::string& updates, uint32_t& checksum, uint64_t& timestamp, uint32_t& updatesChksum, uint64_t& updatesTs)
{
    QCC_DbgTrace(("%s", __func__));
    LampGroupMap mapCopy;
    mapCopy.clear();
    std::set<LSFString> updatesCopy;
    updatesCopy.clear();
    bool ret = false;
    output.clear();
    updates.clear();

    lampGroupsLock.Lock();
    // we can't hold this lock for the entire time!
    if (updated) {
        mapCopy = lampGroups;
        updatesCopy = lampGroupUpdates;
        updated = false;
        ret = true;
    }
    lampGroupsLock.Unlock();

    if (ret) {
        output = GetString(mapCopy);
        updates = GetUpdatesString(updatesCopy);
        lampGroupsLock.Lock();
        if (blobUpdateCycle) {
            timestamp = timeStamp;
            updatesTs = updatesTimeStamp;
            blobUpdateCycle = false;
        } else {
            if (initialState) {
                timeStamp = timestamp = 0UL;
                updatesTimeStamp = updatesTs = 0UL;
                initialState = false;
            } else {
                timeStamp = timestamp = GetTimestampInMs();
                updatesTimeStamp = updatesTs = GetTimestampInMs();
            }
        }
        checkSum = checksum = GetChecksum(output);
        updatesCheckSum = updatesChksum = GetChecksum(updates);
        lampGroupsLock.Unlock();
    }

    return ret;
}

void LampGroupManager::HandleReceivedBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    uint64_t currentTimestamp = GetTimestampInMs();
    lampGroupsLock.Lock();
    if (((timeStamp == 0) || ((currentTimestamp - timeStamp) > timestamp)) && (checkSum != checksum)) {
        std::istringstream stream(blob.c_str());
        ReplaceMap(stream);
        timeStamp = currentTimestamp;
        checkSum = checksum;
        ScheduleFileWrite(true);
    }
    lampGroupsLock.Unlock();
}

void LampGroupManager::HandleReceivedUpdateBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    uint64_t currentTimestamp = GetTimestampInMs();
    lampGroupsLock.Lock();
    if (((updatesTimeStamp == 0) || ((currentTimestamp - updatesTimeStamp) > timestamp)) && (updatesCheckSum != checksum)) {
        std::istringstream stream(blob.c_str());
        ReplaceUpdatesList(stream);
        updatesTimeStamp = currentTimestamp;
        updatesCheckSum = checksum;
        ScheduleFileWrite(true);
    }
    lampGroupsLock.Unlock();
}

void LampGroupManager::ReadWriteFile()
{
    QCC_DbgPrintf(("%s", __func__));

    if (filePath.empty() || updateFilePath.empty()) {
        return;
    }

    std::string output;
    uint32_t checksum;
    uint64_t timestamp;
    std::string updates;
    uint32_t updateChecksum;
    uint64_t updateTimestamp;
    bool status = false;

    status = GetString(output, updates, checksum, timestamp, updateChecksum, updateTimestamp);

    if (status) {
        WriteFileWithChecksumAndTimestamp(output, checksum, timestamp);
        if (timestamp != 0UL) {
            uint64_t currentTime = GetTimestampInMs();
            controllerService.SendBlobUpdate(LSF_LAMP_GROUP, output, checksum, (currentTime - timestamp));
        }

        WriteUpdatesFileWithChecksumAndTimestamp(updates, updateChecksum, updateTimestamp);
        if (updateTimestamp != 0UL) {
            uint64_t currentTime = GetTimestampInMs();
            controllerService.SendBlobUpdate(LSF_LAMP_GROUP_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
        }
    }

    std::list<ajn::Message> tempMessageList;
    std::list<ajn::Message> tempUpdateMessageList;

    readMutex.Lock();
    if (read) {
        tempMessageList = readBlobMessages;
        readBlobMessages.clear();
        tempUpdateMessageList = readUpdateBlobMessages;
        readUpdateBlobMessages.clear();
        read = false;
    }
    readMutex.Unlock();

    if ((tempMessageList.size() || tempUpdateMessageList.size() || sendUpdate) && !status) {
        std::istringstream stream;
        bool fileStatus = ValidateFileAndReadInternal(checksum, timestamp, stream);
        if (fileStatus) {
            output = stream.str();
            while (!tempMessageList.empty()) {
                uint64_t currentTime = GetTimestampInMs();
                controllerService.SendGetBlobReply(tempMessageList.front(), LSF_LAMP_GROUP, output, checksum, (currentTime - timestamp));
                tempMessageList.pop_front();
            }
        } else {
            QCC_LogError(ER_FAIL, ("%s: Lamp Group persistent store corrupted", __func__));
        }

        std::istringstream updateStream;
        bool updateStatus = ValidateUpdateFileAndReadInternal(updateChecksum, updateTimestamp, updateStream);
        if (updateStatus) {
            updates = updateStream.str();
            while (!tempUpdateMessageList.empty()) {
                uint64_t currentTime = GetTimestampInMs();
                controllerService.SendGetBlobReply(tempUpdateMessageList.front(), LSF_LAMP_GROUP_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
                tempUpdateMessageList.pop_front();
            }
        } else {
            QCC_LogError(ER_FAIL, ("%s: Lamp Group Update persistent store corrupted", __func__));
        }
    }

    if (sendUpdate) {
        sendUpdate = false;
        uint64_t currentTime = GetTimestampInMs();
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_LAMP_GROUP, output, checksum, (currentTime - timestamp));
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_LAMP_GROUP_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
    }
}

uint32_t LampGroupManager::GetControllerServiceLampGroupInterfaceVersion(void)
{
    QCC_DbgPrintf(("%s: controllerLampGroupInterfaceVersion=%d", __func__, ControllerServiceLampGroupInterfaceVersion));
    return ControllerServiceLampGroupInterfaceVersion;
}