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
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#ifdef LSF_BINDINGS
#include <lsf/controllerservice/PulseEffectManager.h>
#include <lsf/controllerservice/ControllerService.h>
#include <lsf/controllerservice/SceneElementManager.h>
#include <lsf/controllerservice/OEM_CS_Config.h>
#include <lsf/controllerservice/FileParser.h>
#else
#include <PulseEffectManager.h>
#include <ControllerService.h>
#include <SceneElementManager.h>
#include <OEM_CS_Config.h>
#include <FileParser.h>
#endif

#include <qcc/Debug.h>

using namespace lsf;
using namespace ajn;

#ifdef LSF_BINDINGS
using namespace controllerservice;
#define QCC_MODULE "CONTROLLER_PULSE_EFFECT_MANAGER"
#else
#define QCC_MODULE "PULSE_EFFECT_MANAGER"
#endif

PulseEffectManager::PulseEffectManager(ControllerService& controllerSvc, LampGroupManager* lampGroupMgrPtr, SceneElementManager* sceneElementMgrPtr, const std::string& pulseEffectFile) :
    Manager(controllerSvc, pulseEffectFile), lampGroupManagerPtr(lampGroupMgrPtr), sceneElementManagerPtr(sceneElementMgrPtr), blobLength(0)
{
    QCC_DbgTrace(("%s", __func__));
    pulseEffects.clear();
    pulseEffectUpdates.clear();
}

LSFResponseCode PulseEffectManager::GetAllPulseEffects(PulseEffectMap& pulseEffectMap)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = pulseEffectsLock.Lock();
    if (ER_OK == status) {
        pulseEffectMap = pulseEffects;
        status = pulseEffectsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: pulseEffectsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: pulseEffectsLock.Lock() failed", __func__));
    }

    return responseCode;
}

LSFResponseCode PulseEffectManager::Reset(void)
{
    QCC_DbgPrintf(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    if (!controllerService.UpdatesAllowed()) {
        return LSF_ERR_BUSY;
    }

    QStatus tempStatus = pulseEffectsLock.Lock();
    if (ER_OK == tempStatus) {
        /*
         * Record the IDs of all the PulseEffects that are being deleted
         */
        LSFStringList pulseEffectsList;
        for (PulseEffectMap::iterator it = pulseEffects.begin(); it != pulseEffects.end(); ++it) {
            pulseEffectsList.push_back(it->first);
        }

        /*
         * Clear the PulseEffects
         */
        pulseEffects.clear();
        pulseEffectUpdates.clear();
        blobLength = 0;
        ScheduleFileWrite();
        tempStatus = pulseEffectsLock.Unlock();
        if (ER_OK != tempStatus) {
            QCC_LogError(tempStatus, ("%s: pulseEffectsLock.Unlock() failed", __func__));
        }

        /*
         * Send the PulseEffects deleted signal
         */
        if (pulseEffectsList.size()) {
            tempStatus = controllerService.SendSignal(ControllerServicePulseEffectInterfaceName, "PulseEffectsDeleted", pulseEffectsList);
            if (ER_OK != tempStatus) {
                QCC_LogError(tempStatus, ("%s: Unable to send PulseEffectsDeleted signal", __func__));
            }
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(tempStatus, ("%s: pulseEffectsLock.Lock() failed", __func__));
    }

    return responseCode;
}

LSFResponseCode PulseEffectManager::GetPulseEffectInternal(const LSFString& pulseEffectID, PulseEffect& pulseEffect)
{
    QCC_DbgPrintf(("%s", __func__));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    QStatus status = pulseEffectsLock.Lock();
    if (ER_OK == status) {
        PulseEffectMap::iterator it = pulseEffects.find(pulseEffectID);
        if (it != pulseEffects.end()) {
            pulseEffect = it->second.second;
            //QCC_DbgPrintf(("%s: Found PulseEffect %s", __func__, pulseEffect.c_str()));
            responseCode = LSF_OK;
        }
        status = pulseEffectsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: pulseEffectsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: pulseEffectsLock.Lock() failed", __func__));
    }

    QCC_DbgPrintf(("%s: %s", __func__, LSFResponseCodeText(responseCode)));
    return responseCode;
}

void PulseEffectManager::GetAllPulseEffectIDs(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));

    LSFStringList idList;
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = pulseEffectsLock.Lock();
    if (ER_OK == status) {
        for (PulseEffectMap::iterator it = pulseEffects.begin(); it != pulseEffects.end(); ++it) {
            idList.push_back(it->first.c_str());
        }
        status = pulseEffectsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: pulseEffectsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: pulseEffectsLock.Lock() failed", __func__));
    }

    controllerService.SendMethodReplyWithResponseCodeAndListOfIDs(msg, responseCode, idList);
}

void PulseEffectManager::GetPulseEffectName(Message& msg)
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

    LSFString pulseEffectID(uniqueId);

    LSFString language = static_cast<LSFString>(args[1].v_string.str);
    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = pulseEffectsLock.Lock();
        if (ER_OK == status) {
            PulseEffectMap::iterator it = pulseEffects.find(uniqueId);
            if (it != pulseEffects.end()) {
                name = it->second.first;
                responseCode = LSF_OK;
            }
            status = pulseEffectsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: pulseEffectsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: pulseEffectsLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDLanguageAndName(msg, responseCode, pulseEffectID, language, name);
}

void PulseEffectManager::SetPulseEffectName(Message& msg)
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

    LSFString pulseEffectID(uniqueId);

    const char* name;
    args[1].Get("s", &name);

    LSFString language = static_cast<LSFString>(args[2].v_string.str);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeIDAndName(msg, LSF_ERR_BUSY, pulseEffectID, language);
        return;
    }

    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name[0] == '\0') {
        QCC_LogError(ER_FAIL, ("%s: pulseEffect name is empty", __func__));
        responseCode = LSF_ERR_EMPTY_NAME;
    } else {
        if (strlen(name) > LSF_MAX_NAME_LENGTH) {
            responseCode = LSF_ERR_INVALID_ARGS;
            QCC_LogError(ER_FAIL, ("%s: strlen(name) > LSF_MAX_NAME_LENGTH", __func__));
        } else {
            QStatus status = pulseEffectsLock.Lock();
            if (ER_OK == status) {
                PulseEffectMap::iterator it = pulseEffects.find(uniqueId);
                if (it != pulseEffects.end()) {
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
                status = pulseEffectsLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: pulseEffectsLock.Unlock() failed", __func__));
                }
            } else {
                responseCode = LSF_ERR_BUSY;
                QCC_LogError(status, ("%s: pulseEffectsLock.Lock() failed", __func__));
            }
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDAndName(msg, responseCode, pulseEffectID, language);

    if (nameChanged) {
        LSFStringList idList;
        idList.push_back(pulseEffectID);
        controllerService.SendSignal(ControllerServicePulseEffectInterfaceName, "PulseEffectsNameChanged", idList);
    }
}

void PulseEffectManager::CreatePulseEffect(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));

    LSFResponseCode responseCode = LSF_OK;

    LSFString pulseEffectID;

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(msg, LSF_ERR_BUSY, pulseEffectID);
        return;
    }

    const ajn::MsgArg* inputArgs;
    size_t numInputArgs;
    msg->GetArgs(numInputArgs, inputArgs);

    if (controllerService.CheckNumArgsInMessage(numInputArgs, 9)  != LSF_OK) {
        return;
    }

    PulseEffect pulseEffect(inputArgs[0], inputArgs[1], inputArgs[2], inputArgs[3], inputArgs[4], inputArgs[5], inputArgs[6]);
    LSFString name = static_cast<LSFString>(inputArgs[7].v_string.str);
    LSFString language = static_cast<LSFString>(inputArgs[8].v_string.str);

    responseCode = CreatePulseEffectInternal(pulseEffect, name, language, pulseEffectID);

    controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, pulseEffectID);

    if (responseCode == LSF_OK) {
        LSFStringList idList;
        idList.push_back(pulseEffectID);
        SendPulseEffectsCreatedSignal(idList);
    }
}

void PulseEffectManager::UpdatePulseEffect(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    bool updated = false;

    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 8)  != LSF_OK) {
        return;
    }

    const char* pulseEffectId;
    args[0].Get("s", &pulseEffectId);

    LSFString pulseEffectID(pulseEffectId);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(msg, LSF_ERR_BUSY, pulseEffectID);
        return;
    }

    PulseEffect pulseEffect(args[1], args[2], args[3], args[4], args[5], args[6], args[7]);

    if (pulseEffect.invalidArgs) {
        QCC_LogError(ER_FAIL, ("%s: Invalid Pulse Effect", __func__));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = pulseEffectsLock.Lock();
        if (ER_OK == status) {
            PulseEffectMap::iterator it = pulseEffects.find(pulseEffectId);
            if (it != pulseEffects.end()) {
                size_t newlen = blobLength;
                // sub len of old pulse effect, add len of new pulse effect
                newlen -= GetString(it->second.first, pulseEffectID, it->second.second).length();
                newlen += GetString(it->second.first, pulseEffectID, pulseEffect).length();

                if (newlen < MAX_FILE_LEN) {
                    blobLength = newlen;
                    pulseEffects[pulseEffectID].second = pulseEffect;
                    responseCode = LSF_OK;
                    if (pulseEffectUpdates.find(pulseEffectID) == pulseEffectUpdates.end()) {
                        pulseEffectUpdates.insert(pulseEffectID);
                    }
                    updated = true;
                    ScheduleFileWrite();
                } else {
                    responseCode = LSF_ERR_RESOURCES;
                }
            }
            status = pulseEffectsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: pulseEffectsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: pulseEffectsLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, pulseEffectID);

    if (updated) {
        LSFStringList idList;
        idList.push_back(pulseEffectID);
        controllerService.SendSignal(ControllerServicePulseEffectInterfaceName, "PulseEffectsUpdated", idList);
        responseCode = sceneElementManagerPtr->IsDependentOnEffect(pulseEffectID);
        if (LSF_ERR_DEPENDENCY == responseCode) {
            controllerService.GetSceneManager().RefreshSceneData();
        }
    }
}

void PulseEffectManager::ApplyPulseEffectOnLamps(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 2)  != LSF_OK) {
        return;
    }

    const char* pulseEffectId;
    args[0].Get("s", &pulseEffectId);

    LSFString pulseEffectID(pulseEffectId);

    PulseEffect pulseEffect;

    responseCode = GetPulseEffectInternal(pulseEffectID, pulseEffect);

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

        if (pulseEffect.toState.nullState) {
            PulseLampsLampGroupsWithPreset component(lamps, lampGroups, pulseEffect.fromPreset, pulseEffect.toPreset, pulseEffect.pulsePeriod, pulseEffect.pulseDuration, pulseEffect.numPulses);
            pulseWithPresetComponent.push_back(component);
        } else {
            PulseLampsLampGroupsWithState component(lamps, lampGroups, pulseEffect.fromState, pulseEffect.toState, pulseEffect.pulsePeriod, pulseEffect.pulseDuration, pulseEffect.numPulses);
            pulseWithStateComponent.push_back(component);
        }

        responseCode = lampGroupManagerPtr->ChangeLampGroupStateAndField(msg, transitionToStateComponent, transitionToPresetComponent,
                                                                         pulseWithStateComponent, pulseWithPresetComponent,
                                                                         false, false, false, LSFString(), true);
    }

    if (LSF_ERR_NOT_FOUND == responseCode) {
        controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, pulseEffectID);
    }
}

void PulseEffectManager::ApplyPulseEffectOnLampGroups(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 2)  != LSF_OK) {
        return;
    }

    const char* pulseEffectId;
    args[0].Get("s", &pulseEffectId);

    LSFString pulseEffectID(pulseEffectId);

    PulseEffect pulseEffect;

    responseCode = GetPulseEffectInternal(pulseEffectID, pulseEffect);

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

        if (pulseEffect.toState.nullState) {
            PulseLampsLampGroupsWithPreset component(lamps, lampGroups, pulseEffect.fromPreset, pulseEffect.toPreset, pulseEffect.pulsePeriod, pulseEffect.pulseDuration, pulseEffect.numPulses);
            pulseWithPresetComponent.push_back(component);
        } else {
            PulseLampsLampGroupsWithState component(lamps, lampGroups, pulseEffect.fromState, pulseEffect.toState, pulseEffect.pulsePeriod, pulseEffect.pulseDuration, pulseEffect.numPulses);
            pulseWithStateComponent.push_back(component);
        }

        responseCode = lampGroupManagerPtr->ChangeLampGroupStateAndField(msg, transitionToStateComponent, transitionToPresetComponent,
                                                                         pulseWithStateComponent, pulseWithPresetComponent,
                                                                         false, (lampGroups.front() == AllLampsGroupIdentifier), false, LSFString(), true);
    }

    if (LSF_ERR_NOT_FOUND == responseCode) {
        controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, pulseEffectID);
    }
}

void PulseEffectManager::DeletePulseEffect(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;

    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

    const char* pulseEffectId;
    args[0].Get("s", &pulseEffectId);

    LSFString pulseEffectID(pulseEffectId);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(msg, LSF_ERR_BUSY, pulseEffectID);
        return;
    }

    responseCode = DeletePulseEffectInternal(pulseEffectID);

    controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, pulseEffectID);

    if (responseCode == LSF_OK) {
        LSFStringList idList;
        idList.push_back(pulseEffectID);
        controllerService.SendSignal(ControllerServicePulseEffectInterfaceName, "PulseEffectsDeleted", idList);
    }
}

void PulseEffectManager::GetPulseEffect(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));

    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    MsgArg outArgs[9];

    size_t numArgs;
    const MsgArg* args;
    msg->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

    const char* pulseEffectId;
    args[0].Get("s", &pulseEffectId);

    PulseEffect pulseEffect;

    responseCode = GetPulseEffectInternal(pulseEffectId, pulseEffect);

    if (LSF_OK == responseCode) {
        pulseEffect.Get(&outArgs[2], &outArgs[3], &outArgs[4], &outArgs[5], &outArgs[6], &outArgs[7], &outArgs[8]);
    } else {
        outArgs[2].Set("a{sv}", 0, NULL);
        outArgs[3].Set("u", 0);
        outArgs[4].Set("u", 0);
        outArgs[5].Set("u", 0);
        outArgs[6].Set("a{sv}", 0, NULL);
        outArgs[7].Set("s", "");
        outArgs[8].Set("s", "");
    }

    outArgs[0].Set("u", responseCode);
    outArgs[1].Set("s", pulseEffectId);

    controllerService.SendMethodReply(msg, outArgs, 9);
}

// never call this when the ControllerService is up; it isn't thread-safe!
// PulseEffects follow the form:
// (PulseEffect id "name" LampState/PresetID pulsePeriod)*
void PulseEffectManager::ReadSavedData(void)
{
    QCC_DbgTrace(("%s", __func__));
    std::istringstream stream;
    if (!ValidateFileAndRead(stream)) {
        /*
         * If there is no file present / CRC check failed on the file create a new
         * file with initialState entry
         */
        pulseEffectsLock.Lock();
        ScheduleFileWrite(false, true);
        pulseEffectsLock.Unlock();
        return;
    }

    blobLength = stream.str().size();
    ReplaceMap(stream);

    std::istringstream updateStream;
    if (ValidateUpdateFileAndRead(updateStream)) {
        ReplaceUpdatesList(updateStream);
    }
}

void PulseEffectManager::HandleReceivedBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    uint64_t currentTimestamp = GetTimestampInMs();
    pulseEffectsLock.Lock();
    if (((timeStamp == 0) || ((currentTimestamp - timeStamp) > timestamp)) && (checkSum != checksum)) {
        std::istringstream stream(blob.c_str());
        ReplaceMap(stream);
        timeStamp = currentTimestamp;
        checkSum = checksum;
        ScheduleFileWrite(true);
    }
    pulseEffectsLock.Unlock();
}

void PulseEffectManager::HandleReceivedUpdateBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    uint64_t currentTimestamp = GetTimestampInMs();
    pulseEffectsLock.Lock();
    if (((updatesTimeStamp == 0) || ((currentTimestamp - updatesTimeStamp) > timestamp)) && (updatesCheckSum != checksum)) {
        std::istringstream stream(blob.c_str());
        ReplaceUpdatesList(stream);
        updatesTimeStamp = currentTimestamp;
        updatesCheckSum = checksum;
        ScheduleFileWrite(true);
    }
    pulseEffectsLock.Unlock();
}

void PulseEffectManager::ReplaceMap(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));
    bool firstIteration = true;
    while (!stream.eof()) {
        std::string token = ParseString(stream);

        // keep searching for "PulseEffect", which indicates the beginning of a saved PulseEffect state
        if (token == resetID) {
            QCC_DbgPrintf(("The file has a reset entry. Clearing the map"));
            pulseEffects.clear();
        } else if (token == "PulseEffect") {
            std::string pulseEffectId = ParseString(stream);
            std::string pulseEffectName = ParseString(stream);

            QCC_DbgPrintf(("PulseEffect id=%s, name=[%s]\n", pulseEffectId.c_str(), pulseEffectName.c_str()));

            if (0 == strcmp(pulseEffectId.c_str(), resetID.c_str())) {
                QCC_DbgPrintf(("The file has a reset entry. Clearing the map"));
                pulseEffects.clear();
            } else if (0 == strcmp(pulseEffectId.c_str(), initialStateID.c_str())) {
                QCC_DbgPrintf(("The file has a initialState entry. So we ignore it"));
            } else {
                if (firstIteration) {
                    pulseEffects.clear();
                    firstIteration = false;
                }
                blobLength += pulseEffectId.length();

                bool containsLampState = ParseValue<bool>(stream);

                LampState fromState, toState;
                std::string fromPresetID, toPresetID;
                PulseEffect pulseEffect;

                if (containsLampState) {
                    ParseLampState(stream, fromState);
                    ParseLampState(stream, toState);
                } else {
                    fromPresetID = ParseString(stream);
                    toPresetID = ParseString(stream);
                }

                uint32_t pulsePeriod = ParseValue<uint32_t>(stream);
                uint32_t pulseDuration = ParseValue<uint32_t>(stream);
                uint32_t numPulses = ParseValue<uint32_t>(stream);

                if (containsLampState) {
                    pulseEffect = PulseEffect(toState, pulsePeriod, pulseDuration, numPulses, fromState);
                } else {
                    pulseEffect = PulseEffect(toPresetID, pulsePeriod, pulseDuration, numPulses, fromPresetID);
                }

                std::pair<LSFString, PulseEffect> thePair = std::make_pair(pulseEffectName, pulseEffect);
                pulseEffects[pulseEffectId] = thePair;
                //QCC_DbgPrintf(("%s: Added ID=%s, Name=%s, State=%s", __func__, pulseEffectId.c_str(), pulseEffects[pulseEffectId].first.c_str(), pulseEffects[pulseEffectId].second.c_str()));
            }
        }
    }
}

void PulseEffectManager::ReplaceUpdatesList(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));
    bool firstIteration = true;
    while (!stream.eof()) {
        std::string id = ParseString(stream);
        if (0 == strcmp(id.c_str(), resetID.c_str())) {
            QCC_DbgPrintf(("The file has a reset entry. Clearing the list"));
            pulseEffectUpdates.clear();
        } else if (0 == strcmp(id.c_str(), initialStateID.c_str())) {
            QCC_DbgPrintf(("The file has a initialState entry. So we ignore it"));
        } else {
            if (firstIteration) {
                pulseEffectUpdates.clear();
                firstIteration = false;
            }
            if (!id.empty()) {
                pulseEffectUpdates.insert(id);
                QCC_DbgPrintf(("%s: Add %s to updates list", __func__, id.c_str()));
            }
        }
    }
}

LSFResponseCode PulseEffectManager::CreatePulseEffectInternal(PulseEffect& pulseEffect, LSFString& name, LSFString& language, LSFString& pulseEffectID)
{
    QCC_DbgTrace(("%s", __func__));

    bool created = false;
    LSFResponseCode responseCode = LSF_OK;

    pulseEffectID = GenerateUniqueID("PULSE_EFFECT");
    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name.empty()) {
        QCC_LogError(ER_FAIL, ("%s: pulse effect name is empty", __func__));
        responseCode = LSF_ERR_EMPTY_NAME;
    } else if (pulseEffect.invalidArgs) {
        QCC_LogError(ER_FAIL, ("%s: Cannot save invalid PulseEffect", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name.length() > LSF_MAX_NAME_LENGTH) {
        QCC_LogError(ER_FAIL, ("%s: name length exceeds %d", __func__, LSF_MAX_NAME_LENGTH));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = pulseEffectsLock.Lock();
        if (ER_OK == status) {
            if (pulseEffects.size() < OEM_CS_MAX_SUPPORTED_NUM_LSF_ENTITY) {
                std::string newPulseEffectStr = GetString(name, pulseEffectID, pulseEffect);
                size_t newlen = blobLength + newPulseEffectStr.length() + pulseEffectID.length();
                if (newlen < MAX_FILE_LEN) {
                    blobLength = newlen;
                    pulseEffects[pulseEffectID].first = name;
                    pulseEffects[pulseEffectID].second = pulseEffect;
                    created = true;
                    ScheduleFileWrite();
                } else {
                    responseCode = LSF_ERR_RESOURCES;
                }
            } else {
                QCC_LogError(ER_FAIL, ("%s: No slot for new PulseEffect", __func__));
                responseCode = LSF_ERR_NO_SLOT;
            }
            status = pulseEffectsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: pulseEffectsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: pulseEffectsLock.Lock() failed", __func__));
        }
    }

    if (!created) {
        pulseEffectID.clear();
    }

    return responseCode;
}

void PulseEffectManager::SendPulseEffectsCreatedSignal(LSFStringList& pulseEffectIds)
{
    QCC_DbgTrace(("%s", __func__));
    controllerService.SendSignal(ControllerServicePulseEffectInterfaceName, "PulseEffectsCreated", pulseEffectIds);
}

LSFResponseCode PulseEffectManager::DeletePulseEffectInternal(LSFString& pulseEffectID)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    responseCode = sceneElementManagerPtr->IsDependentOnEffect(pulseEffectID);

    if (LSF_OK == responseCode) {
        QStatus status = pulseEffectsLock.Lock();
        if (ER_OK == status) {
            PulseEffectMap::iterator it = pulseEffects.find(pulseEffectID);
            if (it != pulseEffects.end()) {
                blobLength -= ((GetString(it->second.first, pulseEffectID, it->second.second).length()) + pulseEffectID.length());
                pulseEffects.erase(it);
                if (pulseEffectUpdates.find(pulseEffectID) != pulseEffectUpdates.end()) {
                    pulseEffectUpdates.erase(pulseEffectID);
                }
                ScheduleFileWrite();
            } else {
                responseCode = LSF_ERR_NOT_FOUND;
            }
            status = pulseEffectsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: pulseEffectsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: pulseEffectsLock.Lock() failed", __func__));
        }
    }

    return responseCode;
}

std::string PulseEffectManager::GetUpdatesString(const std::set<LSFString>& updates)
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

std::string PulseEffectManager::GetString(const std::string& name, const std::string& id, const PulseEffect& pulseEffect)
{
    std::ostringstream stream;
    if (!(pulseEffect.toState.nullState)) {
        stream << "PulseEffect " << id << " \"" << name << "\" "
               << 1 << ' '
               << (pulseEffect.fromState.nullState ? 1 : 0) << ' '
               << (pulseEffect.fromState.onOff ? 1 : 0) << ' '
               << pulseEffect.fromState.hue << ' ' << pulseEffect.fromState.saturation << ' '
               << pulseEffect.fromState.colorTemp << ' ' << pulseEffect.fromState.brightness << ' '
               << (pulseEffect.toState.nullState ? 1 : 0) << ' '
               << (pulseEffect.toState.onOff ? 1 : 0) << ' '
               << pulseEffect.toState.hue << ' ' << pulseEffect.toState.saturation << ' '
               << pulseEffect.toState.colorTemp << ' ' << pulseEffect.toState.brightness << ' '
               << pulseEffect.pulsePeriod << ' ' << pulseEffect.pulseDuration << ' ' << pulseEffect.numPulses << '\n';
    } else {
        stream << "PulseEffect " << id << " \"" << name << "\" "
               << 0 << ' '
               << pulseEffect.fromPreset << ' '
               << pulseEffect.toPreset << ' '
               << pulseEffect.pulsePeriod << ' ' << pulseEffect.pulseDuration << ' ' << pulseEffect.numPulses << '\n';
    }
    return stream.str();
}

std::string PulseEffectManager::GetString(const PulseEffectMap& items)
{
    std::ostringstream stream;

    if (0 == items.size()) {
        if (initialState) {
            QCC_DbgPrintf(("%s: This is the initial state entry", __func__));
            const LSFString& id = initialStateID;
            const LSFString& name = initialStateID;

            stream << "PulseEffect " << id << " \"" << name << "\" " << '\n';
        } else {
            QCC_DbgPrintf(("%s: File is being reset", __func__));
            const LSFString& id = resetID;
            const LSFString& name = resetID;

            stream << "PulseEffect " << id << " \"" << name << "\" " << '\n';
        }
    } else {
        // (PulseEffect id "name" LampState/PresetID pulsePeriod)*
        for (PulseEffectMap::const_iterator it = items.begin(); it != items.end(); ++it) {
            const LSFString& id = it->first;
            const LSFString& name = it->second.first;
            const PulseEffect& effect = it->second.second;

            stream << GetString(name, id, effect);
        }
    }

    return stream.str();
}

bool PulseEffectManager::GetString(std::string& output, std::string& updates, uint32_t& checksum, uint64_t& timestamp, uint32_t& updatesChksum, uint64_t& updatesTs)
{
    PulseEffectMap mapCopy;
    mapCopy.clear();
    std::set<LSFString> updatesCopy;
    updatesCopy.clear();
    bool ret = false;
    output.clear();
    updates.clear();

    pulseEffectsLock.Lock();
    // we can't hold this lock for the entire time!
    if (updated) {
        mapCopy = pulseEffects;
        updatesCopy = pulseEffectUpdates;
        updated = false;
        ret = true;
    }
    pulseEffectsLock.Unlock();

    if (ret) {
        output = GetString(mapCopy);
        updates = GetUpdatesString(updatesCopy);
        pulseEffectsLock.Lock();
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
        pulseEffectsLock.Unlock();
    }

    return ret;
}

void PulseEffectManager::ReadWriteFile()
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
            controllerService.SendBlobUpdate(LSF_PULSE_EFFECT, output, checksum, (currentTime - timestamp));
        }

        WriteUpdatesFileWithChecksumAndTimestamp(updates, updateChecksum, updateTimestamp);
        if (updateTimestamp != 0UL) {
            uint64_t currentTime = GetTimestampInMs();
            controllerService.SendBlobUpdate(LSF_PULSE_EFFECT_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
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
                controllerService.SendGetBlobReply(tempMessageList.front(), LSF_PULSE_EFFECT, output, checksum, (currentTime - timestamp));
                tempMessageList.pop_front();
            }
        } else {
            QCC_LogError(ER_FAIL, ("%s: PulseEffect persistent store corrupted", __func__));
        }

        std::istringstream updateStream;
        bool updateStatus = ValidateUpdateFileAndReadInternal(updateChecksum, updateTimestamp, updateStream);
        if (updateStatus) {
            updates = updateStream.str();
            while (!tempUpdateMessageList.empty()) {
                uint64_t currentTime = GetTimestampInMs();
                controllerService.SendGetBlobReply(tempUpdateMessageList.front(), LSF_PULSE_EFFECT_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
                tempUpdateMessageList.pop_front();
            }
        } else {
            QCC_LogError(ER_FAIL, ("%s: PulseEffect Update persistent store corrupted", __func__));
        }
    }

    if (sendUpdate) {
        sendUpdate = false;
        uint64_t currentTime = GetTimestampInMs();
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_PULSE_EFFECT, output, checksum, (currentTime - timestamp));
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_PULSE_EFFECT_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
    }
}

uint32_t PulseEffectManager::GetControllerServicePulseEffectInterfaceVersion(void)
{
    QCC_DbgPrintf(("%s: controllerPulseEffectInterfaceVersion=%d", __func__, ControllerServicePulseEffectInterfaceVersion));
    return ControllerServicePulseEffectInterfaceVersion;
}