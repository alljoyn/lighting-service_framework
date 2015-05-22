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
#include <lsf/controllerservice/ControllerService.h>
#include <lsf/controllerservice/SceneElementManager.h>
#include <lsf/controllerservice/OEM_CS_Config.h>
#include <lsf/controllerservice/FileParser.h>
#else
#include <ControllerService.h>
#include <SceneElementManager.h>
#include <OEM_CS_Config.h>
#include <FileParser.h>
#endif

#include <qcc/atomic.h>
#include <qcc/Debug.h>

using namespace lsf;
using namespace ajn;

#ifdef LSF_BINDINGS
using namespace controllerservice;
#define QCC_MODULE "CONTROLLER_SCENE_ELEMENT_MANAGER"
#else
#define QCC_MODULE "SCENE_ELEMENT_MANAGER"
#endif

SceneElementManager::SceneElementManager(ControllerService& controllerSvc, TransitionEffectManager* transistionEffectMgr, PulseEffectManager* pulseEffectMgr, const std::string& sceneElementFile) :
    Manager(controllerSvc, sceneElementFile), transitionEffectManagerPtr(transistionEffectMgr), pulseEffectManagerPtr(pulseEffectMgr)
{
    QCC_DbgPrintf(("%s", __func__));
    sceneElements.clear();
}

LSFResponseCode SceneElementManager::Reset(void)
{
    QCC_DbgPrintf(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    if (!controllerService.UpdatesAllowed()) {
        return LSF_ERR_BUSY;
    }

    QStatus tempStatus = sceneElementsLock.Lock();
    if (ER_OK == tempStatus) {
        /*
         * Record the IDs of all the SceneElements that are being deleted
         */
        LSFStringList sceneElementsList;
        for (SceneElementMap::iterator it = sceneElements.begin(); it != sceneElements.end(); ++it) {
            sceneElementsList.push_back(it->first);
        }

        /*
         * Clear the SceneElements
         */
        sceneElements.clear();
        blobLength = 0;

        ScheduleFileWrite();

        tempStatus = sceneElementsLock.Unlock();
        if (ER_OK != tempStatus) {
            QCC_LogError(tempStatus, ("%s: sceneElementsLock.Unlock() failed", __func__));
        }

        /*
         * Send the SceneElements deleted signal
         */
        if (sceneElementsList.size()) {
            tempStatus = controllerService.SendSignal(ControllerServiceSceneElementInterfaceName, "SceneElementsDeleted", sceneElementsList);
            if (ER_OK != tempStatus) {
                QCC_LogError(tempStatus, ("%s: Unable to send SceneElementsDeleted signal", __func__));
            }
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(tempStatus, ("%s: sceneElementsLock.Lock() failed", __func__));
    }

    return responseCode;
}

LSFResponseCode SceneElementManager::IsDependentOnLampGroup(LSFString& lampGroupID)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = sceneElementsLock.Lock();
    if (ER_OK == status) {
        for (SceneElementMap::iterator it = sceneElements.begin(); it != sceneElements.end(); ++it) {
            responseCode = it->second.second.IsDependentOnLampGroup(lampGroupID);
            if (LSF_OK != responseCode) {
                break;
            }
        }
        status = sceneElementsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: sceneElementsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: sceneElementsLock.Lock() failed", __func__));
    }

    return responseCode;
}

LSFResponseCode SceneElementManager::GetAllSceneElements(SceneElementMap& sceneElementMap)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = sceneElementsLock.Lock();
    if (ER_OK == status) {
        sceneElementMap = sceneElements;
        status = sceneElementsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: sceneElementsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: sceneElementsLock.Lock() failed", __func__));
    }

    return responseCode;
}

void SceneElementManager::GetAllSceneElementIDs(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFStringList idList;
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = sceneElementsLock.Lock();
    if (ER_OK == status) {
        for (SceneElementMap::iterator it = sceneElements.begin(); it != sceneElements.end(); ++it) {
            idList.push_back(it->first.c_str());
        }
        status = sceneElementsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: sceneElementsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: sceneElementsLock.Lock() failed", __func__));
    }

    controllerService.SendMethodReplyWithResponseCodeAndListOfIDs(message, responseCode, idList);
}

void SceneElementManager::GetSceneElementName(Message& message)
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

    LSFString sceneElementID(uniqueId);

    LSFString language = static_cast<LSFString>(args[1].v_string.str);
    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = sceneElementsLock.Lock();
        if (ER_OK == status) {
            SceneElementMap::iterator it = sceneElements.find(uniqueId);
            if (it != sceneElements.end()) {
                name = it->second.first;
                responseCode = LSF_OK;
            }
            status = sceneElementsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: sceneElementsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: sceneElementsLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDLanguageAndName(message, responseCode, sceneElementID, language, name);
}

void SceneElementManager::SetSceneElementName(Message& message)
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

    LSFString sceneElementID(uniqueId);

    const char* name;
    args[1].Get("s", &name);

    LSFString language = static_cast<LSFString>(args[2].v_string.str);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeIDAndName(message, LSF_ERR_BUSY, sceneElementID, language);
        return;
    }

    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name[0] == '\0') {
        QCC_LogError(ER_FAIL, ("%s: scene element name is empty", __func__));
        responseCode = LSF_ERR_EMPTY_NAME;
    } else {
        if (strlen(name) > LSF_MAX_NAME_LENGTH) {
            responseCode = LSF_ERR_INVALID_ARGS;
            QCC_LogError(ER_FAIL, ("%s: strlen(name) > LSF_MAX_NAME_LENGTH", __func__));
        } else {
            QStatus status = sceneElementsLock.Lock();
            if (ER_OK == status) {
                SceneElementMap::iterator it = sceneElements.find(uniqueId);
                if (it != sceneElements.end()) {
                    LSFString newName = name;
                    size_t newlen = blobLength - it->second.first.length() + newName.length();

                    if (newlen < MAX_FILE_LEN) {
                        blobLength = newlen;
                        it->second.first = newName;
                        responseCode = LSF_OK;
                        nameChanged = true;
                        ScheduleFileWrite();
                    } else {
                        QCC_LogError(ER_FAIL, ("%s: blob too big: %d >= %d", __func__, newlen, MAX_FILE_LEN));
                        responseCode = LSF_ERR_RESOURCES;
                    }
                }
                status = sceneElementsLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: sceneElementsLock.Unlock() failed", __func__));
                }
            } else {
                responseCode = LSF_ERR_BUSY;
                QCC_LogError(status, ("%s: sceneElementsLock.Lock() failed", __func__));
            }
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDAndName(message, responseCode, sceneElementID, language);

    if (nameChanged) {
        LSFStringList idList;
        idList.push_back(sceneElementID);
        controllerService.SendSignal(ControllerServiceSceneElementInterfaceName, "SceneElementsNameChanged", idList);
    }
}

void SceneElementManager::CreateSceneElement(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_OK;
    LSFString sceneElementID;

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, sceneElementID);
        return;
    }

    sceneElementID = GenerateUniqueID("SCENE_ELEMENT");

    bool created = false;

    const ajn::MsgArg* inputArgs;
    size_t numInputArgs;
    message->GetArgs(numInputArgs, inputArgs);

    if (controllerService.CheckNumArgsInMessage(numInputArgs, 5)  != LSF_OK) {
        return;
    }

    SceneElement sceneElement(inputArgs[0], inputArgs[1], inputArgs[2]);
    LSFString name = static_cast<LSFString>(inputArgs[3].v_string.str);
    LSFString language = static_cast<LSFString>(inputArgs[4].v_string.str);

    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name.empty()) {
        QCC_LogError(ER_FAIL, ("%s: scene element name is empty", __func__));
        responseCode = LSF_ERR_EMPTY_NAME;
    } else if (name.length() > LSF_MAX_NAME_LENGTH) {
        QCC_LogError(ER_FAIL, ("%s: name length exceeds %d", __func__, LSF_MAX_NAME_LENGTH));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (sceneElement.lamps.empty() && sceneElement.lampGroups.empty()) {
        QCC_LogError(ER_FAIL, ("%s: Empty Lamps and LampGroups list", __func__));
           responseCode = LSF_ERR_INVALID_ARGS;
    } else if (sceneElement.effectID.empty()) {
        QCC_LogError(ER_FAIL, ("%s: effect ID is empty", __func__));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = sceneElementsLock.Lock();
        if (ER_OK == status) {
            if (sceneElements.size() < OEM_CS_MAX_SUPPORTED_NUM_LSF_ENTITY) {
                std::string newElementStr = GetString(name, sceneElementID, sceneElement);
                size_t newlen = blobLength + newElementStr.length();
                if (newlen < MAX_FILE_LEN) {
                    blobLength = newlen;
                    sceneElements[sceneElementID].first = name;
                    sceneElements[sceneElementID].second = sceneElement;
                    created = true;
                       ScheduleFileWrite();
                } else {
                    QCC_LogError(ER_FAIL, ("%s: blob too big: %d >= %d", __func__, newlen, MAX_FILE_LEN));
                    responseCode = LSF_ERR_RESOURCES;
                }
            } else {
                QCC_LogError(ER_FAIL, ("%s: No slot for new SceneElement", __func__));
                responseCode = LSF_ERR_NO_SLOT;
            }
            status = sceneElementsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: sceneElementLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: sceneElementLock.Lock() failed", __func__));
        }
    }

    if (!created) {
        sceneElementID.clear();
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, sceneElementID);

    if (created) {
        LSFStringList idList;
        idList.push_back(sceneElementID);
        controllerService.SendSignal(ControllerServiceSceneElementInterfaceName, "SceneElementsCreated", idList);
    }
}

void SceneElementManager::UpdateSceneElement(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    bool updated = false;

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 4)  != LSF_OK) {
        return;
    }

    const char* sceneElementId;
    args[0].Get("s", &sceneElementId);

    LSFString sceneElementID(sceneElementId);
    SceneElement sceneElement(args[1], args[2], args[3]);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, sceneElementID);
        return;
    }

    if (sceneElement.lamps.empty() && sceneElement.lampGroups.empty()) {
        QCC_LogError(ER_FAIL, ("%s: Empty Lamps and LampGroups list", __func__));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = sceneElementsLock.Lock();
        if (ER_OK == status) {
            SceneElementMap::iterator it = sceneElements.find(sceneElementId);
            if (it != sceneElements.end()) {

                size_t newlen = blobLength;
                // sub len of old element, add len of new element
                newlen -= GetString(it->second.first, sceneElementID, it->second.second).length();
                newlen += GetString(it->second.first, sceneElementID, sceneElement).length();

                if (newlen < MAX_FILE_LEN) {
                    blobLength = newlen;
                    it->second.second = sceneElement;
                    responseCode = LSF_OK;
                    updated = true;
                    ScheduleFileWrite();
                } else {
                    QCC_LogError(ER_FAIL, ("%s: blob too big: %d >= %d", __func__, newlen, MAX_FILE_LEN));
                    responseCode = LSF_ERR_RESOURCES;
                }
            }
            status = sceneElementsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: sceneElementsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: sceneElementsLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, sceneElementID);

    if (updated) {
        LSFStringList idList;
        idList.push_back(sceneElementID);
        controllerService.SendSignal(ControllerServiceSceneElementInterfaceName, "SceneElementsUpdated", idList);
    }
}

void SceneElementManager::DeleteSceneElement(Message& message)
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

    const char* sceneElementId;
    args[0].Get("s", &sceneElementId);

    LSFString sceneElementID(sceneElementId);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, sceneElementID);
        return;
    }

    responseCode = LSF_OK; //TODO-IMPL Implement IsDependentOnLampGroup(), IsDependentOnSceneElement(sceneElementID);

    if (LSF_OK == responseCode) {
        QStatus status = sceneElementsLock.Lock();
        if (ER_OK == status) {
            SceneElementMap::iterator it = sceneElements.find(sceneElementId);
            if (it != sceneElements.end()) {
                blobLength -= GetString(it->second.first, sceneElementId, it->second.second).length();

                sceneElements.erase(it);
                deleted = true;
                ScheduleFileWrite();
            } else {
                responseCode = LSF_ERR_NOT_FOUND;
            }
            status = sceneElementsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: sceneElementsLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: sceneElementsLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, sceneElementID);

    if (deleted) {
        LSFStringList idList;
        idList.push_back(sceneElementID);
        controllerService.SendSignal(ControllerServiceSceneElementInterfaceName, "SceneElementsDeleted", idList);
    }
}

void SceneElementManager::GetSceneElement(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    MsgArg outArgs[5];

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

    const char* sceneElementId;
    args[0].Get("s", &sceneElementId);

    QStatus status = sceneElementsLock.Lock();
    if (ER_OK == status) {
        SceneElementMap::iterator it = sceneElements.find(sceneElementId);
        if (it != sceneElements.end()) {
            it->second.second.Get(&outArgs[2], &outArgs[3], &outArgs[4]);
            responseCode = LSF_OK;
        } else {
            outArgs[2].Set("as", 0, NULL);
            outArgs[3].Set("as", 0, NULL);
            outArgs[4].Set("s", 0, NULL);
        }
        status = sceneElementsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: sceneElementsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: sceneElementsLock.Lock() failed", __func__));
    }

    outArgs[0].Set("u", responseCode);
    outArgs[1].Set("s", sceneElementId);

    controllerService.SendMethodReply(message, outArgs, 5);
}

void SceneElementManager::ApplySceneElement(ajn::Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1) != LSF_OK) {
        return;
    }

    const char* sceneElementId;
    args[0].Get("s", &sceneElementId);

    LSFString uniqueId(sceneElementId);
    LSFStringList sceneElementIDs;
    sceneElementIDs.push_back(uniqueId);

    responseCode = ApplySceneElementInternal(message, sceneElementIDs);

    if (LSF_OK != responseCode) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, uniqueId);
    }
}

LSFResponseCode SceneElementManager::ApplySceneElementInternal(ajn::Message& message, LSFStringList& sceneElementIDs)
{
    QCC_DbgPrintf(("%s: sceneElementIDs.size() = %d", __func__, sceneElementIDs.size()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;
    std::list<SceneElement> sceneElementList;
    uint8_t notfound = 0;

    QStatus status = sceneElementsLock.Lock();
    if (ER_OK == status) {
        for (LSFStringList::iterator it = sceneElementIDs.begin(); it != sceneElementIDs.end(); it++) {
            SceneElementMap::iterator it2 = sceneElements.find(*it);
            if (it2 != sceneElements.end()) {
                QCC_DbgPrintf(("%s: Found sceneElementID=%s", __func__, it->c_str()));
                sceneElementList.push_back(it2->second.second);
                responseCode = LSF_OK;
            } else {
                QCC_DbgPrintf(("%s: Missing sceneElementID=%s", __func__, it->c_str()));
                notfound++;
            }
        }
        status = sceneElementsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: sceneElementsLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: sceneElementsLock.Lock() failed", __func__));
    }

    if (LSF_OK == responseCode) {
        if (notfound) {
            responseCode = LSF_ERR_PARTIAL;
        }

        while (sceneElementList.size()) {
            TransitionEffect transitionEffect;
            PulseEffect pulseEffect;
            LSFResponseCode tempResponseCode = LSF_OK;

            QCC_DbgPrintf(("%s: Applying sceneElementID with effectID=%s", __func__, sceneElementList.front().effectID.c_str()));
            if (transitionEffectManagerPtr->GetTransitionEffectInternal(sceneElementList.front().effectID, transitionEffect) == LSF_OK) {
                tempResponseCode = transitionEffectManagerPtr->ApplyTransitionEffectInternal(message, transitionEffect, sceneElementList.front().lamps, sceneElementList.front().lampGroups, true);
            } else if (pulseEffectManagerPtr->GetPulseEffectInternal(sceneElementList.front().effectID, pulseEffect) == LSF_OK) {
                tempResponseCode = pulseEffectManagerPtr->ApplyPulseEffectInternal(message, pulseEffect, sceneElementList.front().lamps, sceneElementList.front().lampGroups, true);
            } else {
                tempResponseCode = LSF_ERR_NOT_FOUND;
            }

            if (tempResponseCode != LSF_OK) {
                responseCode = LSF_ERR_FAILURE;
            }

            sceneElementList.pop_front();
        }
    }

    return responseCode;
}

void SceneElementManager::ReadWriteFile()
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
            controllerService.SendBlobUpdate(LSF_SCENE_ELEMENT, output, checksum, (currentTime - timestamp));
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
            QCC_LogError(ER_FAIL, ("%s: Scene element persistent store corrupted", __func__));
        }
    }

    if (status) {
        while (!tempMessageList.empty()) {
            uint64_t currentTime = GetTimestampInMs();
            controllerService.SendGetBlobReply(tempMessageList.front(), LSF_SCENE_ELEMENT, output, checksum, (currentTime - timestamp));
            tempMessageList.pop_front();
        }
    }

    if (sendUpdate) {
        sendUpdate = false;
        uint64_t currentTime = GetTimestampInMs();
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_SCENE_ELEMENT, output, checksum, (currentTime - timestamp));
    }
}

void SceneElementManager::ReadSavedData()
{
    QCC_DbgTrace(("%s", __func__));
    std::istringstream stream;
    if (!ValidateFileAndRead(stream)) {
        /*
         * If there is no file present / CRC check failed on the file create a new
         * file with initialState entry
         */
        sceneElementsLock.Lock();
        ScheduleFileWrite(false, true);
        sceneElementsLock.Unlock();
        return;
    }

    blobLength = stream.str().size();
    ReplaceMap(stream);
}

void SceneElementManager::ReplaceMap(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));
    bool firstIteration = true;
    while (!stream.eof()) {
        std::string token = ParseString(stream);

        if (token == "SceneElement") {
            std::string id = ParseString(stream);
            std::string name = ParseString(stream);

            if (0 == strcmp(id.c_str(), resetID.c_str())) {
                QCC_DbgPrintf(("The file has a reset entry. Clearing the map"));
                sceneElements.clear();
            } else if (0 == strcmp(id.c_str(), initialStateID.c_str())) {
                QCC_DbgPrintf(("The file has a initialState entry. So we ignore it"));
            } else {
                if (firstIteration) {
                    sceneElements.clear();
                    firstIteration = false;
                }
                SceneElement sceneElement;
                do {
                    token = ParseString(stream);

                    if (token == "Lamp") {
                        std::string member = ParseString(stream);
                        sceneElement.lamps.push_back(member);
                    } else if (token == "LampGroup") {
                        std::string member = ParseString(stream);
                        sceneElement.lampGroups.push_back(member);
                    } else if (token == "Effect") {
                        sceneElement.effectID = ParseString(stream);
                    } else {
                        break;
                    }
                } while (token != "EndSceneElement");

                std::pair<LSFString, SceneElement> thePair(name, sceneElement);
                sceneElements[id] = thePair;
            }
        }
    }
}

void SceneElementManager::HandleReceivedBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    uint64_t currentTimestamp = GetTimestampInMs();
    sceneElementsLock.Lock();
    if (((timeStamp == 0) || ((currentTimestamp - timeStamp) > timestamp)) && (checkSum != checksum)) {
        std::istringstream stream(blob.c_str());
        ReplaceMap(stream);
        timeStamp = currentTimestamp;
        checkSum = checksum;
        ScheduleFileWrite(true);
    }
    sceneElementsLock.Unlock();
}

uint32_t SceneElementManager::GetControllerServiceSceneElementInterfaceVersion(void)
{
    QCC_DbgPrintf(("%s: controllerSceneElementInterfaceVersion=%d", __func__, ControllerServiceSceneElementInterfaceVersion));
    return ControllerServiceSceneElementInterfaceVersion;
}

bool SceneElementManager::GetString(std::string& output, uint32_t& checksum, uint64_t& timestamp)
{
    QCC_DbgTrace(("%s", __func__));
    SceneElementMap mapCopy;
    mapCopy.clear();
    bool ret = false;
    output.clear();

    sceneElementsLock.Lock();
    // we can't hold this lock for the entire time!
    if (updated) {
        mapCopy = sceneElements;
        updated = false;
        ret = true;
    }
    sceneElementsLock.Unlock();

    if (ret) {
        output = GetString(mapCopy);
        sceneElementsLock.Lock();
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
        sceneElementsLock.Unlock();
    }

    return ret;
}

std::string SceneElementManager::GetString(const SceneElementMap& items)
{
    QCC_DbgTrace(("%s", __func__));

    std::ostringstream stream;
    if (0 == items.size()) {
        if (initialState) {
            QCC_DbgPrintf(("%s: This is the initial state entry", __func__));
            const LSFString& id = initialStateID;
            const LSFString& name = initialStateID;

            stream << "SceneElement " << id << " \"" << name << "\"";
            stream << " EndSceneElement" << std::endl;
        } else {
            QCC_DbgPrintf(("%s: File is being reset", __func__));
            const LSFString& id = resetID;
            const LSFString& name = resetID;

            stream << "SceneElement " << id << " \"" << name << "\"";
            stream << " EndSceneElement" << std::endl;
        }
    } else {
        for (SceneElementMap::const_iterator it = items.begin(); it != items.end(); ++it) {
            const LSFString& id = it->first;
            const LSFString& name = it->second.first;
            const SceneElement& sceneElement = it->second.second;

            stream << GetString(name, id, sceneElement);
        }
    }

    return stream.str();
}

std::string SceneElementManager::GetString(const std::string& name, const std::string& id, const SceneElement& sceneElement)
{
    std::ostringstream stream;
    stream << "SceneElement " << id << " \"" << name << "\"";

    for (LSFStringList::const_iterator lit = sceneElement.lamps.begin(); lit != sceneElement.lamps.end(); ++lit) {
        stream << " Lamp " << *lit;
    }
    for (LSFStringList::const_iterator lit = sceneElement.lampGroups.begin(); lit != sceneElement.lampGroups.end(); ++lit) {
        stream << " LampGroup " << *lit;
    }

    stream << " Effect " << sceneElement.effectID;

    stream << " EndSceneElement" << std::endl;
    return stream.str();
}
