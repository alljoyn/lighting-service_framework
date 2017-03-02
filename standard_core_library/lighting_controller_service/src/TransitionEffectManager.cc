/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF), AllJoyn Open Source
 *    Project (AJOSP) Contributors and others.
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
#include <lsf/controllerservice/TransitionEffectManager.h>
#include <lsf/controllerservice/ControllerService.h>
#include <lsf/controllerservice/SceneElementManager.h>
#include <lsf/controllerservice/OEM_CS_Config.h>
#include <lsf/controllerservice/FileParser.h>
#include <lsf/controllerservice/LampGroupManager.h>
#else
#include <TransitionEffectManager.h>
#include <ControllerService.h>
#include <SceneElementManager.h>
#include <OEM_CS_Config.h>
#include <FileParser.h>
#include <LampGroupManager.h>
#endif

#include <qcc/Debug.h>

using namespace lsf;
using namespace ajn;

#ifdef LSF_BINDINGS
using namespace controllerservice;
#define QCC_MODULE "CONTROLLER_TRANSITION_EFFECT_MANAGER"
#else
#define QCC_MODULE "TRANSITION_EFFECT_MANAGER"
#endif

TransitionEffectManager::TransitionEffectManager(ControllerService& controllerSvc, LampGroupManager* lampGroupMgrPtr, SceneElementManager* sceneElementMgrPtr, const std::string& transitionEffectFile) :
    Manager(controllerSvc, transitionEffectFile), lampGroupManagerPtr(lampGroupMgrPtr), sceneElementManagerPtr(sceneElementMgrPtr), blobLength(0)
{
    QCC_DbgTrace(("%s", __func__));
    transitionEffects.clear();
    transitionEffectUpdates.clear();
}

LSFResponseCode TransitionEffectManager::GetAllTransitionEffects(TransitionEffectMap& transitionEffectMap)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = transitionEffectsLock.Lock();
    if (ER_OK == status) {
        transitionEffectMap = transitionEffects;
        status = transitionEffectsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: transitionEffectsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: transitionEffectsLock.Lock() failed", __func__));
    }

    return responseCode;
}

LSFResponseCode TransitionEffectManager::Reset(void)
{
    QCC_DbgPrintf(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    if (!controllerService.UpdatesAllowed()) {
        return LSF_ERR_BUSY;
    }

    QStatus tempStatus = transitionEffectsLock.Lock();
    if (ER_OK == tempStatus) {
        /*
         * Record the IDs of all the TransitionEffects that are being deleted
         */
        LSFStringList transitionEffectsList;
        for (TransitionEffectMap::iterator it = transitionEffects.begin(); it != transitionEffects.end(); ++it) {
            transitionEffectsList.push_back(it->first);
        }

        /*
         * Clear the TransitionEffects
         */
        transitionEffects.clear();
        transitionEffectUpdates.clear();
        blobLength = 0;
        ScheduleFileWrite();
        tempStatus = transitionEffectsLock.Unlock();
        if (ER_OK != tempStatus) {
            QCC_LogError(tempStatus, ("%s: transitionEffectsLock.Unlock() failed", __func__));
        }

        /*
         * Send the TransitionEffects deleted signal
         */
        if (transitionEffectsList.size()) {
            tempStatus = controllerService.SendSignal(ControllerServiceTransitionEffectInterfaceName, "TransitionEffectsDeleted", transitionEffectsList);
            if (ER_OK != tempStatus) {
                QCC_LogError(tempStatus, ("%s: Unable to send TransitionEffectsDeleted signal", __func__));
            }
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(tempStatus, ("%s: transitionEffectsLock.Lock() failed", __func__));
    }

    return responseCode;
}

LSFResponseCode TransitionEffectManager::GetTransitionEffectInternal(const LSFString& transitionEffectID, TransitionEffect& transitionEffect)
{
    QCC_DbgPrintf(("%s", __func__));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    QStatus status = transitionEffectsLock.Lock();
    if (ER_OK == status) {
        TransitionEffectMap::iterator it = transitionEffects.find(transitionEffectID);
        if (it != transitionEffects.end()) {
            transitionEffect = it->second.second;
            //QCC_DbgPrintf(("%s: Found TransitionEffect %s", __func__, transitionEffect.c_str()));
            responseCode = LSF_OK;
        }
        status = transitionEffectsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: transitionEffectsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: transitionEffectsLock.Lock() failed", __func__));
    }

    QCC_DbgPrintf(("%s: %s", __func__, LSFResponseCodeText(responseCode)));
    return responseCode;
}

void TransitionEffectManager::GetAllTransitionEffectIDs(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));

    LSFStringList idList;
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = transitionEffectsLock.Lock();
    if (ER_OK == status) {
        for (TransitionEffectMap::iterator it = transitionEffects.begin(); it != transitionEffects.end(); ++it) {
            idList.push_back(it->first.c_str());
        }
        status = transitionEffectsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: transitionEffectsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: transitionEffectsLock.Lock() failed", __func__));
    }

    controllerService.SendMethodReplyWithResponseCodeAndListOfIDs(msg, responseCode, idList);
}

void TransitionEffectManager::GetTransitionEffectName(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));
    LSFString name;
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 2)  != LSF_OK) {
        return;
    }

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    LSFString transitionEffectID(uniqueId);

    LSFString language = static_cast<LSFString>(args[1].v_string.str);
    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = transitionEffectsLock.Lock();
        if (ER_OK == status) {
            TransitionEffectMap::iterator it = transitionEffects.find(uniqueId);
            if (it != transitionEffects.end()) {
                name = it->second.first;
                responseCode = LSF_OK;
            }
            status = transitionEffectsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: transitionEffectsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: transitionEffectsLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDLanguageAndName(msg, responseCode, transitionEffectID, language, name);
}

void TransitionEffectManager::SetTransitionEffectName(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    bool nameChanged = false;
    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 3)  != LSF_OK) {
        return;
    }

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    LSFString transitionEffectID(uniqueId);

    const char* name;
    args[1].Get("s", &name);

    LSFString language = static_cast<LSFString>(args[2].v_string.str);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeIDAndName(msg, LSF_ERR_BUSY, transitionEffectID, language);
        return;
    }

    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name[0] == '\0') {
        QCC_LogError(ER_FAIL, ("%s: transitionEffect name is empty", __func__));
        responseCode = LSF_ERR_EMPTY_NAME;
    } else {
        if (strlen(name) > LSF_MAX_NAME_LENGTH) {
            responseCode = LSF_ERR_INVALID_ARGS;
            QCC_LogError(ER_FAIL, ("%s: strlen(name) > LSF_MAX_NAME_LENGTH", __func__));
        } else {
            QStatus status = transitionEffectsLock.Lock();
            if (ER_OK == status) {
                TransitionEffectMap::iterator it = transitionEffects.find(uniqueId);
                if (it != transitionEffects.end()) {
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
                status = transitionEffectsLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: transitionEffectsLock.Unlock() failed", __func__));
                }
            } else {
                responseCode = LSF_ERR_BUSY;
                QCC_LogError(status, ("%s: transitionEffectsLock.Lock() failed", __func__));
            }
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDAndName(msg, responseCode, transitionEffectID, language);

    if (nameChanged) {
        LSFStringList idList;
        idList.push_back(transitionEffectID);
        controllerService.SendSignal(ControllerServiceTransitionEffectInterfaceName, "TransitionEffectsNameChanged", idList);
    }
}

void TransitionEffectManager::CreateTransitionEffect(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));

    LSFResponseCode responseCode = LSF_OK;

    LSFString transitionEffectID;

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(msg, LSF_ERR_BUSY, transitionEffectID);
        return;
    }

    const ajn::MsgArg* inputArgs;
    size_t numInputArgs;
    msg->GetArgs(numInputArgs, inputArgs);

    if (controllerService.CheckNumArgsInMessage(numInputArgs, 5)  != LSF_OK) {
        return;
    }

    TransitionEffect transitionEffect(inputArgs[0], inputArgs[1], inputArgs[2]);
    LSFString name = static_cast<LSFString>(inputArgs[3].v_string.str);
    LSFString language = static_cast<LSFString>(inputArgs[4].v_string.str);

    responseCode = CreateTransitionEffectInternal(transitionEffect, name, language, transitionEffectID);

    controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, transitionEffectID);

    if (responseCode == LSF_OK) {
        LSFStringList idList;
        idList.push_back(transitionEffectID);
        SendTransitionEffectsCreatedSignal(idList);
    }
}

void TransitionEffectManager::UpdateTransitionEffect(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    bool updated = false;

    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 4)  != LSF_OK) {
        return;
    }

    const char* transitionEffectId;
    args[0].Get("s", &transitionEffectId);

    LSFString transitionEffectID(transitionEffectId);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(msg, LSF_ERR_BUSY, transitionEffectID);
        return;
    }

    TransitionEffect transitionEffect(args[1], args[2], args[3]);

    if (transitionEffect.invalidArgs) {
        QCC_LogError(ER_FAIL, ("%s: Invalid Transition Effect", __func__));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = transitionEffectsLock.Lock();
        if (ER_OK == status) {
            TransitionEffectMap::iterator it = transitionEffects.find(transitionEffectId);
            if (it != transitionEffects.end()) {
                size_t newlen = blobLength;
                // sub len of old group, add len of new group
                newlen -= GetString(it->second.first, transitionEffectID, it->second.second).length();
                newlen += GetString(it->second.first, transitionEffectID, transitionEffect).length();

                if (newlen < MAX_FILE_LEN) {
                    blobLength = newlen;
                    transitionEffects[transitionEffectID].second = transitionEffect;
                    responseCode = LSF_OK;
                    if (transitionEffectUpdates.find(transitionEffectID) == transitionEffectUpdates.end()) {
                        transitionEffectUpdates.insert(transitionEffectID);
                    }
                    updated = true;
                    ScheduleFileWrite();
                } else {
                    responseCode = LSF_ERR_RESOURCES;
                }
            }
            status = transitionEffectsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: transitionEffectsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: transitionEffectsLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, transitionEffectID);

    if (updated) {
        LSFStringList idList;
        idList.push_back(transitionEffectID);
        controllerService.SendSignal(ControllerServiceTransitionEffectInterfaceName, "TransitionEffectsUpdated", idList);
        responseCode = sceneElementManagerPtr->IsDependentOnEffect(transitionEffectID);
        if (LSF_ERR_DEPENDENCY == responseCode) {
            controllerService.GetSceneManager().RefreshSceneData();
        }
    }
}

void TransitionEffectManager::ApplyTransitionEffectOnLamps(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 2)  != LSF_OK) {
        return;
    }

    const char* transitionEffectId;
    args[0].Get("s", &transitionEffectId);

    LSFString transitionEffectID(transitionEffectId);

    TransitionEffect transitionEffect;

    responseCode = GetTransitionEffectInternal(transitionEffectID, transitionEffect);

    if (responseCode == LSF_OK) {
        LSFStringList lamps;
        LSFStringList lampGroups;
        lamps.clear();
        lampGroups.clear();

        MsgArg* idsArray;
        size_t idsSize;
        args[1].Get("as", &idsSize, &idsArray);
        CreateUniqueList(lamps, idsArray, idsSize);

        TransitionLampsLampGroupsToStateList transitionToStateComponent;
        TransitionLampsLampGroupsToPresetList transitionToPresetComponent;
        PulseLampsLampGroupsWithStateList pulseWithStateComponent;
        PulseLampsLampGroupsWithPresetList pulseWithPresetComponent;

        if (transitionEffect.state.nullState) {
            TransitionLampsLampGroupsToPreset component(lamps, lampGroups, transitionEffect.presetID, transitionEffect.transitionPeriod);
            transitionToPresetComponent.push_back(component);
        } else {
            TransitionLampsLampGroupsToState component(lamps, lampGroups, transitionEffect.state, transitionEffect.transitionPeriod);
            transitionToStateComponent.push_back(component);
        }

        responseCode = lampGroupManagerPtr->ChangeLampGroupStateAndField(msg, transitionToStateComponent, transitionToPresetComponent,
                                                                         pulseWithStateComponent, pulseWithPresetComponent,
                                                                         false, false, false, LSFString(), true);
    }

    if (LSF_ERR_NOT_FOUND == responseCode) {
        controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, transitionEffectID);
    }
}

void TransitionEffectManager::ApplyTransitionEffectOnLampGroups(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 2)  != LSF_OK) {
        return;
    }

    const char* transitionEffectId;
    args[0].Get("s", &transitionEffectId);

    LSFString transitionEffectID(transitionEffectId);

    TransitionEffect transitionEffect;

    responseCode = GetTransitionEffectInternal(transitionEffectID, transitionEffect);

    if (responseCode == LSF_OK) {
        LSFStringList lamps;
        LSFStringList lampGroups;
        lamps.clear();
        lampGroups.clear();

        MsgArg* idsArray;
        size_t idsSize;
        args[1].Get("as", &idsSize, &idsArray);
        CreateUniqueList(lampGroups, idsArray, idsSize);

        TransitionLampsLampGroupsToStateList transitionToStateComponent;
        TransitionLampsLampGroupsToPresetList transitionToPresetComponent;
        PulseLampsLampGroupsWithStateList pulseWithStateComponent;
        PulseLampsLampGroupsWithPresetList pulseWithPresetComponent;

        if (transitionEffect.state.nullState) {
            TransitionLampsLampGroupsToPreset component(lamps, lampGroups, transitionEffect.presetID, transitionEffect.transitionPeriod);
            transitionToPresetComponent.push_back(component);
        } else {
            TransitionLampsLampGroupsToState component(lamps, lampGroups, transitionEffect.state, transitionEffect.transitionPeriod);
            transitionToStateComponent.push_back(component);
        }
        responseCode = lampGroupManagerPtr->ChangeLampGroupStateAndField(msg, transitionToStateComponent, transitionToPresetComponent, pulseWithStateComponent, pulseWithPresetComponent,
                                                                         false, (lampGroups.front() == AllLampsGroupIdentifier), false, LSFString(), true);
    }

    if (LSF_ERR_NOT_FOUND == responseCode) {
        controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, transitionEffectID);
    }
}

void TransitionEffectManager::DeleteTransitionEffect(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;

    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

    const char* transitionEffectId;
    args[0].Get("s", &transitionEffectId);

    LSFString transitionEffectID(transitionEffectId);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(msg, LSF_ERR_BUSY, transitionEffectID);
        return;
    }

    responseCode = DeleteTransitionEffectInternal(transitionEffectID);

    controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, transitionEffectID);

    if (responseCode == LSF_OK) {
        LSFStringList idList;
        idList.push_back(transitionEffectID);
        controllerService.SendSignal(ControllerServiceTransitionEffectInterfaceName, "TransitionEffectsDeleted", idList);
    }
}

void TransitionEffectManager::GetTransitionEffect(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));

    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    MsgArg outArgs[5];

    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

    const char* transitionEffectId;
    args[0].Get("s", &transitionEffectId);

    TransitionEffect transitionEffect;

    responseCode = GetTransitionEffectInternal(transitionEffectId, transitionEffect);

    if (LSF_OK == responseCode) {
        transitionEffect.Get(&outArgs[2], &outArgs[3], &outArgs[4]);
    } else {
        outArgs[2].Set("a{sv}", 0, NULL);
        outArgs[3].Set("s", NULL);
        outArgs[4].Set("u", 0);
    }

    outArgs[0].Set("u", responseCode);
    outArgs[1].Set("s", transitionEffectId);

    controllerService.SendMethodReply(msg, outArgs, 5);
}

// never call this when the ControllerService is up; it isn't thread-safe!
// TransitionEffects follow the form:
// (TransitionEffect id "name" LampState/PresetID transitionPeriod)*
void TransitionEffectManager::ReadSavedData(void)
{
    QCC_DbgTrace(("%s", __func__));
    std::istringstream stream;
    if (!ValidateFileAndRead(stream)) {
        /*
         * If there is no file present / CRC check failed on the file create a new
         * file with initialState entry
         */
        transitionEffectsLock.Lock();
        ScheduleFileWrite(false, true);
        transitionEffectsLock.Unlock();
        return;
    }

    blobLength = stream.str().size();
    ReplaceMap(stream);

    std::istringstream updateStream;
    if (ValidateUpdateFileAndRead(updateStream)) {
        ReplaceUpdatesList(updateStream);
    }
}

void TransitionEffectManager::HandleReceivedBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    uint64_t currentTimestamp = GetTimestampInMs();
    transitionEffectsLock.Lock();
    if (((timeStamp == 0) || ((currentTimestamp - timeStamp) > timestamp)) && (checkSum != checksum)) {
        std::istringstream stream(blob.c_str());
        ReplaceMap(stream);
        timeStamp = currentTimestamp;
        checkSum = checksum;
        ScheduleFileWrite(true);
    }
    transitionEffectsLock.Unlock();
}

void TransitionEffectManager::HandleReceivedUpdateBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    uint64_t currentTimestamp = GetTimestampInMs();
    transitionEffectsLock.Lock();
    if (((updatesTimeStamp == 0) || ((currentTimestamp - updatesTimeStamp) > timestamp)) && (updatesCheckSum != checksum)) {
        std::istringstream stream(blob.c_str());
        ReplaceUpdatesList(stream);
        updatesTimeStamp = currentTimestamp;
        updatesCheckSum = checksum;
        ScheduleFileWrite(true);
    }
    transitionEffectsLock.Unlock();
}

void TransitionEffectManager::ReplaceMap(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));
    bool firstIteration = true;
    while (!stream.eof()) {
        std::string token = ParseString(stream);

        // keep searching for "TransitionEffect", which indicates the beginning of a saved TransitionEffect state
        if (token == resetID) {
            QCC_DbgPrintf(("The file has a reset entry. Clearing the map"));
            transitionEffects.clear();
        } else if (token == "TransitionEffect") {
            std::string transitionEffectId = ParseString(stream);
            std::string transitionEffectName = ParseString(stream);

            QCC_DbgPrintf(("TransitionEffect id=%s, name=[%s]\n", transitionEffectId.c_str(), transitionEffectName.c_str()));

            if (0 == strcmp(transitionEffectId.c_str(), resetID.c_str())) {
                QCC_DbgPrintf(("The file has a reset entry. Clearing the map"));
                transitionEffects.clear();
            } else if (0 == strcmp(transitionEffectId.c_str(), initialStateID.c_str())) {
                QCC_DbgPrintf(("The file has a initialState entry. So we ignore it"));
            } else {
                if (firstIteration) {
                    transitionEffects.clear();
                    firstIteration = false;
                }
                blobLength += transitionEffectId.length();
                bool containsLampState = ParseValue<bool>(stream);

                LampState state;
                std::string presetID;
                TransitionEffect transitionEffect;

                if (containsLampState) {
                    ParseLampState(stream, state);
                } else {
                    presetID = ParseString(stream);
                }

                uint32_t transitionPeriod = ParseValue<uint32_t>(stream);

                if (containsLampState) {
                    transitionEffect = TransitionEffect(state, transitionPeriod);
                } else {
                    transitionEffect = TransitionEffect(presetID, transitionPeriod);
                }

                std::pair<LSFString, TransitionEffect> thePair = std::make_pair(transitionEffectName, transitionEffect);
                transitionEffects[transitionEffectId] = thePair;
                //QCC_DbgPrintf(("%s: Added ID=%s, Name=%s, State=%s", __func__, transitionEffectId.c_str(), transitionEffects[transitionEffectId].first.c_str(), transitionEffects[transitionEffectId].second.c_str()));
            }
        }
    }
}

void TransitionEffectManager::ReplaceUpdatesList(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));
    bool firstIteration = true;
    while (!stream.eof()) {
        std::string id = ParseString(stream);
        if (0 == strcmp(id.c_str(), resetID.c_str())) {
            QCC_DbgPrintf(("The file has a reset entry. Clearing the list"));
            transitionEffectUpdates.clear();
        } else if (0 == strcmp(id.c_str(), initialStateID.c_str())) {
            QCC_DbgPrintf(("The file has a initialState entry. So we ignore it"));
        } else {
            if (firstIteration) {
                transitionEffectUpdates.clear();
                firstIteration = false;
            }
            if (!id.empty()) {
                transitionEffectUpdates.insert(id);
                QCC_DbgPrintf(("%s: Add %s to updates list", __func__, id.c_str()));
            }
        }
    }
}

LSFResponseCode TransitionEffectManager::CreateTransitionEffectInternal(TransitionEffect& transitionEffect, LSFString& name, LSFString& language, LSFString& transitionEffectID)
{
    QCC_DbgTrace(("%s", __func__));
    bool created = false;
    LSFResponseCode responseCode = LSF_OK;
    transitionEffectID = GenerateUniqueID("TRANSITION_EFFECT");

    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name.empty()) {
        QCC_LogError(ER_FAIL, ("%s: transition effect name is empty", __func__));
        responseCode = LSF_ERR_EMPTY_NAME;
    } else if (transitionEffect.invalidArgs) {
        QCC_LogError(ER_FAIL, ("%s: Cannot save invalid TransitionEffect", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name.length() > LSF_MAX_NAME_LENGTH) {
        QCC_LogError(ER_FAIL, ("%s: name length exceeds %d", __func__, LSF_MAX_NAME_LENGTH));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = transitionEffectsLock.Lock();
        if (ER_OK == status) {
            if (transitionEffects.size() < OEM_CS_MAX_SUPPORTED_NUM_LSF_ENTITY) {
                std::string newTransitionEffectStr = GetString(name, transitionEffectID, transitionEffect);
                size_t newlen = blobLength + newTransitionEffectStr.length() + transitionEffectID.length();
                if (newlen < MAX_FILE_LEN) {
                    blobLength = newlen;
                    transitionEffects[transitionEffectID].first = name;
                    transitionEffects[transitionEffectID].second = transitionEffect;
                    created = true;
                    ScheduleFileWrite();
                } else {
                    responseCode = LSF_ERR_RESOURCES;
                }
            } else {
                QCC_LogError(ER_FAIL, ("%s: No slot for new TransitionEffect", __func__));
                responseCode = LSF_ERR_NO_SLOT;
            }
            status = transitionEffectsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: transitionEffectsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: transitionEffectsLock.Lock() failed", __func__));
        }
    }

    if (!created) {
        transitionEffectID.clear();
    }

    return responseCode;
}

void TransitionEffectManager::SendTransitionEffectsCreatedSignal(LSFStringList& transitionEffectIds)
{
    QCC_DbgTrace(("%s", __func__));
    controllerService.SendSignal(ControllerServiceTransitionEffectInterfaceName, "TransitionEffectsCreated", transitionEffectIds);
}

LSFResponseCode TransitionEffectManager::DeleteTransitionEffectInternal(LSFString& transitionEffectID)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    responseCode = sceneElementManagerPtr->IsDependentOnEffect(transitionEffectID);

    if (LSF_OK == responseCode) {
        QStatus status = transitionEffectsLock.Lock();
        if (ER_OK == status) {
            TransitionEffectMap::iterator it = transitionEffects.find(transitionEffectID);
            if (it != transitionEffects.end()) {
                blobLength -= ((GetString(it->second.first, transitionEffectID, it->second.second).length()) + transitionEffectID.length());
                transitionEffects.erase(it);
                if (transitionEffectUpdates.find(transitionEffectID) != transitionEffectUpdates.end()) {
                    transitionEffectUpdates.erase(transitionEffectID);
                }
                ScheduleFileWrite();
            } else {
                responseCode = LSF_ERR_NOT_FOUND;
            }
            status = transitionEffectsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: transitionEffectsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: transitionEffectsLock.Lock() failed", __func__));
        }
    }

    return responseCode;
}

std::string TransitionEffectManager::GetUpdatesString(const std::set<LSFString>& updates)
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

std::string TransitionEffectManager::GetString(const std::string& name, const std::string& id, const TransitionEffect& transitionEffect)
{
    std::ostringstream stream;
    if (!(transitionEffect.state.nullState)) {
        stream << "TransitionEffect " << id << " \"" << name << "\" "
               << 1 << ' '
               << (transitionEffect.state.nullState ? 1 : 0) << ' '
               << (transitionEffect.state.onOff ? 1 : 0) << ' '
               << transitionEffect.state.hue << ' ' << transitionEffect.state.saturation << ' '
               << transitionEffect.state.colorTemp << ' ' << transitionEffect.state.brightness << ' '
               << transitionEffect.transitionPeriod << '\n';
    } else {
        stream << "TransitionEffect " << id << " \"" << name << "\" "
               << 0 << ' ' << transitionEffect.presetID << ' ' << transitionEffect.transitionPeriod << '\n';
    }
    return stream.str();
}

std::string TransitionEffectManager::GetString(const TransitionEffectMap& items)
{
    std::ostringstream stream;

    if (0 == items.size()) {
        if (initialState) {
            QCC_DbgPrintf(("%s: This is the initial state entry", __func__));
            const LSFString& id = initialStateID;
            const LSFString& name = initialStateID;

            stream << "TransitionEffect " << id << " \"" << name << "\" " << '\n';
        } else {
            QCC_DbgPrintf(("%s: File is being reset", __func__));
            const LSFString& id = resetID;
            const LSFString& name = resetID;

            stream << "TransitionEffect " << id << " \"" << name << "\" " << '\n';
        }
    } else {
        // (TransitionEffect id "name" LampState/PresetID transitionPeriod)*
        for (TransitionEffectMap::const_iterator it = items.begin(); it != items.end(); ++it) {
            const LSFString& id = it->first;
            const LSFString& name = it->second.first;
            const TransitionEffect& effect = it->second.second;

            stream << GetString(name, id, effect);
        }
    }

    return stream.str();
}

bool TransitionEffectManager::GetString(std::string& output, std::string& updates, uint32_t& checksum, uint64_t& timestamp, uint32_t& updatesChksum, uint64_t& updatesTs)
{
    TransitionEffectMap mapCopy;
    mapCopy.clear();
    std::set<LSFString> updatesCopy;
    updatesCopy.clear();
    bool ret = false;
    output.clear();
    updates.clear();

    transitionEffectsLock.Lock();
    // we can't hold this lock for the entire time!
    if (updated) {
        mapCopy = transitionEffects;
        updatesCopy = transitionEffectUpdates;
        updated = false;
        ret = true;
    }
    transitionEffectsLock.Unlock();

    if (ret) {
        output = GetString(mapCopy);
        updates = GetUpdatesString(updatesCopy);
        transitionEffectsLock.Lock();
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
        transitionEffectsLock.Unlock();
    }

    return ret;
}

void TransitionEffectManager::ReadWriteFile()
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
            controllerService.SendBlobUpdate(LSF_TRANSITION_EFFECT, output, checksum, (currentTime - timestamp));
        }

        WriteUpdatesFileWithChecksumAndTimestamp(updates, updateChecksum, updateTimestamp);
        if (updateTimestamp != 0UL) {
            uint64_t currentTime = GetTimestampInMs();
            controllerService.SendBlobUpdate(LSF_TRANSITION_EFFECT_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
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
                controllerService.SendGetBlobReply(tempMessageList.front(), LSF_TRANSITION_EFFECT, output, checksum, (currentTime - timestamp));
                tempMessageList.pop_front();
            }
        } else {
            QCC_LogError(ER_FAIL, ("%s: TransitionEffect persistent store corrupted", __func__));
        }

        std::istringstream updateStream;
        bool updateStatus = ValidateUpdateFileAndReadInternal(updateChecksum, updateTimestamp, updateStream);
        if (updateStatus) {
            updates = updateStream.str();
            while (!tempUpdateMessageList.empty()) {
                uint64_t currentTime = GetTimestampInMs();
                controllerService.SendGetBlobReply(tempUpdateMessageList.front(), LSF_TRANSITION_EFFECT_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
                tempUpdateMessageList.pop_front();
            }
        } else {
            QCC_LogError(ER_FAIL, ("%s: TransitionEffect Update persistent store corrupted", __func__));
        }
    }

    if (sendUpdate) {
        sendUpdate = false;
        uint64_t currentTime = GetTimestampInMs();
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_TRANSITION_EFFECT, output, checksum, (currentTime - timestamp));
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_TRANSITION_EFFECT_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
    }
}

uint32_t TransitionEffectManager::GetControllerServiceTransitionEffectInterfaceVersion(void)
{
    QCC_DbgPrintf(("%s: controllerTransitionEffectInterfaceVersion=%d", __func__, ControllerServiceTransitionEffectInterfaceVersion));
    return ControllerServiceTransitionEffectInterfaceVersion;
}