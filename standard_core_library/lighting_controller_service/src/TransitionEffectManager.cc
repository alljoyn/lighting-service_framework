/******************************************************************************
 * Copyright AllSeen Alliance. All rights reserved.
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

#ifdef LSF_BINDINGS
#include <lsf/controllerservice/TransitionEffectManager.h>
#include <lsf/controllerservice/ControllerService.h>
#include <lsf/controllerservice/SceneManager.h>
#include <lsf/controllerservice/OEM_CS_Config.h>
#include <lsf/controllerservice/FileParser.h>
#else
#include <TransitionEffectManager.h>
#include <ControllerService.h>
#include <SceneManager.h>
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

TransitionEffectManager::TransitionEffectManager(ControllerService& controllerSvc, LampGroupManager* lampGroupMgrPtr, SceneManager* sceneMgrPtr, const std::string& transitionEffectFile) :
    Manager(controllerSvc, transitionEffectFile), lampGroupManagerPtr(lampGroupMgrPtr), sceneManagerPtr(sceneMgrPtr), blobLength(0)
{
    QCC_DbgTrace(("%s", __func__));
    transitionEffects.clear();
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

    transitionEffectID = GenerateUniqueID("TRANSITION_EFFECT");

    bool created = false;

    const ajn::MsgArg* inputArgs;
    size_t numInputArgs;
    msg->GetArgs(numInputArgs, inputArgs);

    if (controllerService.CheckNumArgsInMessage(numInputArgs, 5)  != LSF_OK) {
        return;
    }

    TransitionEffect transitionEffect(inputArgs[0], inputArgs[1], inputArgs[2]);
    LSFString name = static_cast<LSFString>(inputArgs[3].v_string.str);
    LSFString language = static_cast<LSFString>(inputArgs[4].v_string.str);

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
                size_t newlen = blobLength + newTransitionEffectStr.length();
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

    controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, transitionEffectID);

    if (created) {
        LSFStringList idList;
        idList.push_back(transitionEffectID);
        controllerService.SendSignal(ControllerServiceTransitionEffectInterfaceName, "TransitionEffectsCreated", idList);
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
                                                                         false, false, LSFString(), true);
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
                                                                         false, false, LSFString(), true);
    }

    if (LSF_ERR_NOT_FOUND == responseCode) {
        controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, transitionEffectID);
    }
}

void TransitionEffectManager::DeleteTransitionEffect(Message& msg)
{
    QCC_DbgPrintf(("%s: %s", __func__, msg->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;

    bool deleted = false;

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

    responseCode = sceneManagerPtr->IsDependentOnTransitionEffect(transitionEffectID);

    if (LSF_OK == responseCode) {
        QStatus status = transitionEffectsLock.Lock();
        if (ER_OK == status) {
            TransitionEffectMap::iterator it = transitionEffects.find(transitionEffectId);
            if (it != transitionEffects.end()) {
                blobLength -= GetString(it->second.first, transitionEffectId, it->second.second).length();
                transitionEffects.erase(it);
                deleted = true;
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

    controllerService.SendMethodReplyWithResponseCodeAndID(msg, responseCode, transitionEffectID);

    if (deleted) {
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

void TransitionEffectManager::ReplaceMap(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));
    bool firstIteration = true;
    while (!stream.eof()) {
        std::string token = ParseString(stream);

        // keep searching for "TransitionEffect", which indicates the beginning of a saved TransitionEffect state
        if (token == "TransitionEffect") {
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

bool TransitionEffectManager::GetString(std::string& output, uint32_t& checksum, uint64_t& timestamp)
{
    TransitionEffectMap mapCopy;
    mapCopy.clear();
    bool ret = false;
    output.clear();

    transitionEffectsLock.Lock();
    // we can't hold this lock for the entire time!
    if (updated) {
        mapCopy = transitionEffects;
        updated = false;
        ret = true;
    }
    transitionEffectsLock.Unlock();

    if (ret) {
        output = GetString(mapCopy);
        transitionEffectsLock.Lock();
        if (blobUpdateCycle) {
            checksum = checkSum;
            timestamp = timeStamp;
            blobUpdateCycle = false;
        } else {
            if (initialState) {
                timeStamp = timestamp = 0UL;
                initialState = false;
            } else {
                timeStamp = timestamp = GetTimestampInMs();
            }
            checkSum = checksum = GetChecksum(output);
        }
        transitionEffectsLock.Unlock();
    }

    return ret;
}

void TransitionEffectManager::ReadWriteFile()
{
    QCC_DbgPrintf(("%s", __func__));

    if (filePath.empty()) {
        return;
    }

    std::string output;
    uint32_t checksum;
    uint64_t timestamp;
    bool status = false;

    status = GetString(output, checksum, timestamp);

    if (status) {
        WriteFileWithChecksumAndTimestamp(output, checksum, timestamp);
        if (timestamp != 0UL) {
            uint64_t currentTime = GetTimestampInMs();
            controllerService.SendBlobUpdate(LSF_TRANSITION_EFFECT, output, checksum, (currentTime - timestamp));
        }
    }

    std::list<ajn::Message> tempMessageList;

    readMutex.Lock();
    if (read) {
        tempMessageList = readBlobMessages;
        readBlobMessages.clear();
        read = false;
    }
    readMutex.Unlock();

    if ((tempMessageList.size() || sendUpdate) && !status) {
        std::istringstream stream;
        status = ValidateFileAndReadInternal(checksum, timestamp, stream);
        if (status) {
            output = stream.str();
        } else {
            QCC_LogError(ER_FAIL, ("%s: TransitionEffect persistent store corrupted", __func__));
        }
    }

    if (status) {
        while (tempMessageList.size()) {
            uint64_t currentTime = GetTimestampInMs();
            controllerService.SendGetBlobReply(tempMessageList.front(), LSF_TRANSITION_EFFECT, output, checksum, (currentTime - timestamp));
            tempMessageList.pop_front();
        }
    }

    if (sendUpdate) {
        sendUpdate = false;
        uint64_t currentTime = GetTimestampInMs();
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_TRANSITION_EFFECT, output, checksum, (currentTime - timestamp));
    }
}

uint32_t TransitionEffectManager::GetControllerServiceTransitionEffectInterfaceVersion(void)
{
    QCC_DbgPrintf(("%s: controllerTransitionEffectInterfaceVersion=%d", __func__, ControllerServiceTransitionEffectInterfaceVersion));
    return ControllerServiceTransitionEffectInterfaceVersion;
}
