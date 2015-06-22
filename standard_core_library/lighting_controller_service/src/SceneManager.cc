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
#include <lsf/controllerservice/SceneManager.h>
#include <lsf/controllerservice/ControllerService.h>
#include <lsf/controllerservice/MasterSceneManager.h>
#include <lsf/controllerservice/OEM_CS_Config.h>
#include <lsf/controllerservice/FileParser.h>
#else
#include <SceneManager.h>
#include <ControllerService.h>
#include <MasterSceneManager.h>
#include <OEM_CS_Config.h>
#include <FileParser.h>
#endif

#include <qcc/atomic.h>
#include <qcc/Debug.h>

using namespace lsf;
using namespace ajn;

#ifdef LSF_BINDINGS
using namespace controllerservice;
#define QCC_MODULE "CONTROLLER_SCENE_MANAGER"
#else
#define QCC_MODULE "SCENE_MANAGER"
#endif

const char* sceneEventActionObjId = "LightingControllerServiceObject";
const char* sceneEventActionObjDescription[] = { "This is the LSF EventAction object for scene " };
const char* sceneEventActionInterfaceId = "SceneEventActionInterface";
const char* sceneEventActionInterfaceDescription[] = { "This is the LSF EventAction interface for scene " };
const char* sceneAppliedId = "SceneApplied";
const char* sceneAppliedDescription[2] = { "When the scene ", " is executed" };
const char* applySceneId = "ApplyScene";
const char* applySceneDescription[] = { "Apply the scene " };

SceneObject::SceneObject(SceneManager& sceneMgr, LSFString& sceneid, Scene& tempScene, SceneWithSceneElements& tempSceneWithSceneElements, LSFString& name) :
    BusObject((LSFString(ApplySceneEventActionObjectPath) + sceneid).c_str()), sceneManager(sceneMgr), sceneId(sceneid), scene(tempScene), sceneWithSceneElements(tempSceneWithSceneElements), sceneName(name), appliedSceneMember(NULL)
{
    QCC_DbgPrintf(("%s", __func__));
    InterfaceDescription* intf = NULL;
    QStatus status = sceneManager.controllerService.GetBusAttachment().CreateInterface((LSFString(ApplySceneEventActionInterfaceName) + sceneid).c_str(), intf);

    if ((status == ER_OK) && (intf != NULL)) {
        intf->AddSignal("SceneApplied", NULL, NULL);
        intf->AddMethod("ApplyScene", NULL, NULL, NULL);

        intf->SetDescriptionLanguage("");
        intf->SetDescription(sceneEventActionInterfaceId);
        intf->SetMemberDescription("SceneApplied", sceneAppliedId, true);
        intf->SetMemberDescription("ApplyScene", applySceneId);

        intf->SetDescriptionTranslator(this);
        intf->Activate();

        status = AddInterface(*intf, ANNOUNCED);

        if (status == ER_OK) {
            /* Register the signal handler 'nameChanged' with the bus*/
            appliedSceneMember = intf->GetMember("SceneApplied");
            AddMethodHandler(intf->GetMember("ApplyScene"), static_cast<MessageReceiver::MethodHandler>(&SceneObject::ApplySceneHandler));
        } else {
            QCC_LogError(ER_FAIL, ("Failed to Add interface: %s", (LSFString(ApplySceneEventActionInterfaceName) + sceneid).c_str()));
        }

        SetDescription("", sceneEventActionObjId);
        SetDescriptionTranslator(this);

        sceneManager.controllerService.GetBusAttachment().RegisterBusObject(*this);
    } else {
        QCC_LogError(ER_FAIL, ("Failed to create interface %s\n", (LSFString(ApplySceneEventActionInterfaceName) + sceneid).c_str()));
    }
}

SceneObject::~SceneObject() {
    QCC_DbgPrintf(("%s", __func__));
    sceneManager.controllerService.GetBusAttachment().UnregisterBusObject(*this);
    sceneManager.controllerService.Announce();
}

void SceneObject::ApplySceneHandler(const InterfaceDescription::Member* member, Message& message)
{
    sceneManager.controllerService.GetBusAttachment().EnableConcurrentCallbacks();
    QCC_DbgPrintf(("%s: Received Method call %s from interface %s", __func__, message->GetMemberName(), message->GetInterface()));

    LSFStringList sceneIDs;
    sceneIDs.push_back(sceneId);

    sceneManager.ApplySceneNestedInternal(message, sceneIDs, sceneId);
    MethodReply(message);
}

const char* SceneObject::Translate(const char* sourceLanguage, const char* targetLanguage, const char* source, qcc::String& buffer)
{
    QCC_DbgPrintf(("SceneObject::Translate(%s, %s, %s)", sourceLanguage, targetLanguage, source));

    if (0 == strcmp(targetLanguage, "en")) {
        if (0 == strcmp(source, sceneEventActionObjId)) {
            buffer = sceneEventActionObjDescription[0];
        } else if (0 == strcmp(source, sceneEventActionInterfaceId)) {
            buffer = sceneEventActionInterfaceDescription[0];
            sceneNameMutex.Lock();
            buffer.append(sceneName.c_str());
            sceneNameMutex.Unlock();
        } else if (0 == strcmp(source, sceneAppliedId)) {
            buffer = sceneAppliedDescription[0];
            sceneNameMutex.Lock();
            buffer.append(sceneName.c_str());
            sceneNameMutex.Unlock();
            buffer.append(sceneAppliedDescription[1]);
        } else if (0 == strcmp(source, applySceneId)) {
            buffer = applySceneDescription[0];
            sceneNameMutex.Lock();
            buffer.append(sceneName.c_str());
            sceneNameMutex.Unlock();
        } else {
            // shouldn't happen!
            return NULL;
        }

        QCC_DbgPrintf(("Translate: %s", sourceLanguage, targetLanguage, source, buffer.c_str()));
        return buffer.c_str();
    }

    return NULL;
}

void SceneObject::SendSceneAppliedSignal(void)
{
    QCC_DbgPrintf(("%s", __func__));
    uint8_t flags = ALLJOYN_FLAG_GLOBAL_BROADCAST | ALLJOYN_FLAG_SESSIONLESS;
    if (appliedSceneMember) {
        QStatus status = Signal(NULL, 0, *appliedSceneMember, NULL, 0, 0, flags);
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: Unable to send the applied scene event", __func__));
        }
    } else {
        QCC_LogError(ER_FAIL, ("%s: appliedSceneMember not initialized", __func__));
    }
}

void SceneObject::ObjectRegistered(void)
{
    QCC_DbgPrintf(("%s", __func__));
    sceneManager.controllerService.Announce();
}

SceneFileManager::SceneFileManager(ControllerService& controllerSvc, const std::string& filePathToManager, const std::string& scene2filePath) :
    Manager(controllerSvc, filePathToManager), scene2FilePath(scene2filePath), scene2CheckSum(0), scene2TimeStamp(0)
{
    QCC_DbgPrintf(("%s", __func__));
    readScene2BlobMessages.clear();
}

void SceneFileManager::ScheduleScene2FileRead(ajn::Message& message)
{
    QCC_DbgPrintf(("%s", __func__));
    readMutex.Lock();
    readScene2BlobMessages.push_back(message);
    read = true;
    controllerService.ScheduleFileReadWrite(this);
    readMutex.Unlock();
}

bool SceneFileManager::ValidateScene2FileAndRead(std::istringstream& filestream)
{
    QCC_DbgTrace(("%s", __func__));
    uint32_t checksum;
    uint64_t timestamp;

    bool b = ValidateScene2FileAndReadInternal(checksum, timestamp, filestream);

    if (b) {
        scene2CheckSum = checksum;
        scene2TimeStamp = timestamp;
    }

    return b;
}

bool SceneFileManager::ValidateScene2FileAndReadInternal(uint32_t& checksum, uint64_t& timestamp, std::istringstream& filestream)
{
    QCC_DbgPrintf(("%s: scene2FilePath=%s", __func__, scene2FilePath.c_str()));

    if (scene2FilePath.empty()) {
        return false;
    }

    std::ifstream stream(scene2FilePath.c_str());

    if (!stream.is_open()) {
        QCC_LogError(ER_FAIL, ("File not found: %s\n", scene2FilePath.c_str()));
        return false;
    }

    stream >> timestamp;

    uint64_t currenttime = GetTimestampInMs();
    QCC_DbgPrintf(("%s: timestamp=%llu", __func__, timestamp));
    QCC_DbgPrintf(("%s: Updated %llu ticks ago", __func__, (currenttime - timestamp)));

    stream >> checksum;

    QCC_DbgPrintf(("%s: checksum=%u", __func__, checksum));

    // put the rest of the file into the filestream
    std::stringbuf rest;
    stream >> &rest;
    std::string data = rest.str();
    filestream.str(data);

    // check the adler checksum
    uint32_t adler = GetChecksum(data);

    stream.close();
    return adler == checksum;
}

void SceneFileManager::GetScene2BlobInfoInternal(uint32_t& checksum, uint64_t& time)
{
    QCC_DbgTrace(("%s", __func__));
    checksum = scene2CheckSum;
    time = scene2TimeStamp;
}

void SceneFileManager::WriteScene2FileWithChecksumAndTimestamp(const std::string& str, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgTrace(("%s", __func__));
    std::ofstream fstream(scene2FilePath.c_str(), std::ios_base::out);
    if (!fstream.is_open()) {
        QCC_LogError(ER_FAIL, ("File not found: %s\n", scene2FilePath.c_str()));
        return;
    }

    fstream << timestamp << std::endl;
    fstream << checksum << std::endl;
    fstream << str;
    fstream.close();
}

SceneManager::SceneManager(ControllerService& controllerSvc, SceneElementManager* sceneElementMgr, MasterSceneManager* masterSceneMgr, const std::string& sceneFile, const std::string& sceneWithSceneElementsFile) :
    SceneFileManager(controllerSvc, sceneFile, sceneWithSceneElementsFile), sceneElementManager(sceneElementMgr), masterSceneManager(masterSceneMgr), blobLength(0)
{
    QCC_DbgPrintf(("%s", __func__));
    scenes.clear();
    sceneUpdates.clear();
    oldSceneMap.clear();
}

void SceneManager::UnregisterSceneEventActionObjects(void)
{
    QCC_DbgPrintf(("%s", __func__));
    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
            delete it->second;
        }
        scenes.clear();
        status = scenesLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: scenesLock.Unlock() failed", __func__));
        }
    } else {
        QCC_LogError(status, ("%s: Failed to lock scenesLock", __func__));
    }
}

LSFResponseCode SceneManager::GetAllScenes(SceneMap& sceneMap)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    sceneMap.clear();

    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); it++) {
            it->second->sceneNameMutex.Lock();
            sceneMap.insert(std::make_pair(it->first, std::make_pair(it->second->sceneName, it->second->scene)));
            it->second->sceneNameMutex.Unlock();
        }
        status = scenesLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: scenesLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: scenesLock.Lock() failed", __func__));
    }

    return responseCode;
}

LSFResponseCode SceneManager::Reset(void)
{
    QCC_DbgPrintf(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    if (!controllerService.UpdatesAllowed()) {
        return LSF_ERR_BUSY;
    }

    QStatus tempStatus = scenesLock.Lock();
    if (ER_OK == tempStatus) {
        /*
         * Record the IDs of all the Scenes that are being deleted
         */
        LSFStringList scenesList;
        std::list<SceneObject*> sceneObjectList;
        for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
            scenesList.push_back(it->first);
            sceneObjectList.push_back(it->second);
        }

        /*
         * Clear the Scenes
         */
        scenes.clear();
        sceneUpdates.clear();
        oldSceneMap.clear();
        blobLength = 0;
        ScheduleFileWrite();
        tempStatus = scenesLock.Unlock();
        if (ER_OK != tempStatus) {
            QCC_LogError(tempStatus, ("%s: scenesLock.Unlock() failed", __func__));
        }

        /*
         * Delete all SceneObjects
         */
        while (sceneObjectList.size()) {
            delete sceneObjectList.front();
            sceneObjectList.pop_front();
        }

        /*
         * Send the Scenes deleted signal
         */
        if (scenesList.size()) {
            tempStatus = controllerService.SendSignal(ControllerServiceSceneInterfaceName, "ScenesDeleted", scenesList);
            if (ER_OK != tempStatus) {
                QCC_LogError(tempStatus, ("%s: Unable to send ScenesDeleted signal", __func__));
            }
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(tempStatus, ("%s: scenesLock.Lock() failed", __func__));
    }

    return responseCode;
}

LSFResponseCode SceneManager::IsDependentOnSceneElement(LSFString& sceneElementID)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
            responseCode = it->second->IsDependentOnSceneElement(sceneElementID);
            if (LSF_OK != responseCode) {
                break;
            }
        }
        status = scenesLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: scenesLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: scenesLock.Lock() failed", __func__));
    }

    return responseCode;
}

void SceneManager::GetAllSceneIDs(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFStringList idList;
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
            idList.push_back(it->first.c_str());
        }
        status = scenesLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: scenesLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: scenesLock.Lock() failed", __func__));
    }

    controllerService.SendMethodReplyWithResponseCodeAndListOfIDs(message, responseCode, idList);
}

void SceneManager::GetSceneName(Message& message)
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

    LSFString sceneID(uniqueId);

    LSFString language = static_cast<LSFString>(args[1].v_string.str);
    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = scenesLock.Lock();
        if (ER_OK == status) {
            SceneObjectMap::iterator it = scenes.find(uniqueId);
            if (it != scenes.end()) {
                it->second->sceneNameMutex.Lock();
                name = it->second->sceneName;
                it->second->sceneNameMutex.Unlock();
                responseCode = LSF_OK;
            }
            status = scenesLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: scenesLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: scenesLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDLanguageAndName(message, responseCode, sceneID, language, name);
}

void SceneManager::SetSceneName(Message& message)
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

    LSFString sceneID(uniqueId);

    const char* name;
    args[1].Get("s", &name);

    LSFString language = static_cast<LSFString>(args[2].v_string.str);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeIDAndName(message, LSF_ERR_BUSY, sceneID, language);
        return;
    }

    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name[0] == '\0') {
        QCC_LogError(ER_FAIL, ("%s: scene name is empty", __func__));
        responseCode = LSF_ERR_EMPTY_NAME;
    } else {
        if (strlen(name) > LSF_MAX_NAME_LENGTH) {
            responseCode = LSF_ERR_INVALID_ARGS;
            QCC_LogError(ER_FAIL, ("%s: strlen(name) > LSF_MAX_NAME_LENGTH", __func__));
        } else {
            QStatus status = scenesLock.Lock();
            if (ER_OK == status) {
                SceneObjectMap::iterator it = scenes.find(uniqueId);
                if (it != scenes.end()) {

                    it->second->sceneNameMutex.Lock();
                    LSFString newName = name;
                    /*
                     * Multiply by 2 because we have 2 Scene Files
                     */
                    size_t newlen = blobLength - (2*(it->second->sceneName.length() + newName.length()));

                    if (newlen < MAX_FILE_LEN) {
                        it->second->sceneName = newName;
                        it->second->sceneNameMutex.Unlock();
                        responseCode = LSF_OK;
                        nameChanged = true;
                        ScheduleFileWrite();
                    } else {
                        it->second->sceneNameMutex.Unlock();
                        responseCode = LSF_ERR_RESOURCES;
                    }
                }
                status = scenesLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: scenesLock.Unlock() failed", __func__));
                }
            } else {
                responseCode = LSF_ERR_BUSY;
                QCC_LogError(status, ("%s: scenesLock.Lock() failed", __func__));
            }
        }
    }

    controllerService.SendMethodReplyWithResponseCodeIDAndName(message, responseCode, sceneID, language);

    if (nameChanged) {
        LSFStringList idList;
        idList.push_back(sceneID);
        controllerService.SendSignal(ControllerServiceSceneInterfaceName, "ScenesNameChanged", idList);
    }
}

void SceneManager::CreateScene(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_OK;

    LSFString sceneID;

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, sceneID);
        return;
    }

    const ajn::MsgArg* inputArgs;
    size_t numInputArgs;
    size_t numInputArgsExpected = 6;
    message->GetArgs(numInputArgs, inputArgs);

    if (controllerService.CheckNumArgsInMessage(numInputArgs, numInputArgsExpected) != LSF_OK) {
        return;
    }

    Scene scene = Scene(inputArgs[0], inputArgs[1], inputArgs[2], inputArgs[3]);

    LSFString name = static_cast<LSFString>(inputArgs[numInputArgsExpected-2].v_string.str);
    LSFString language = static_cast<LSFString>(inputArgs[numInputArgsExpected-1].v_string.str);

    SceneWithSceneElements sceneWithSceneElements;

    responseCode = CreateSceneWithSceneElements(scene, sceneWithSceneElements);

    if (responseCode == LSF_OK) {
        responseCode = CreateSceneInternal(scene, sceneWithSceneElements, name, language, sceneID);
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, sceneID);

    if (responseCode == LSF_OK) {
        LSFStringList idList;
        idList.push_back(sceneID);
        controllerService.SendSignal(ControllerServiceSceneInterfaceName, "ScenesCreated", idList);
    }
}

void SceneManager::CreateSceneWithSceneElements(ajn::Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_OK;

    LSFString sceneID;

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, sceneID);
        return;
    }

    const ajn::MsgArg* inputArgs;
    size_t numInputArgs;
    size_t numInputArgsExpected = 3;
    message->GetArgs(numInputArgs, inputArgs);

    if (controllerService.CheckNumArgsInMessage(numInputArgs, numInputArgsExpected) != LSF_OK) {
        return;
    }

    SceneWithSceneElements sceneWithSceneElements = SceneWithSceneElements(inputArgs[0]);

    LSFString name = static_cast<LSFString>(inputArgs[numInputArgsExpected-2].v_string.str);
    LSFString language = static_cast<LSFString>(inputArgs[numInputArgsExpected-1].v_string.str);

    Scene scene;

    responseCode = CreateScene(scene, sceneWithSceneElements);

    if (responseCode == LSF_OK) {
        responseCode = CreateSceneInternal(scene, sceneWithSceneElements, name, language, sceneID);
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, sceneID);

    if (responseCode == LSF_OK) {
        LSFStringList idList;
        idList.push_back(sceneID);
        controllerService.SendSignal(ControllerServiceSceneInterfaceName, "ScenesCreated", idList);
    }
}

void SceneManager::UpdateScene(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    size_t numArgs;
    size_t numArgsExpected = 5;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, numArgsExpected)  != LSF_OK) {
        return;
    }

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    LSFString sceneID(uniqueId);

    SceneWithSceneElements sceneWithSceneElements;
    Scene scene = Scene(args[1], args[2], args[3], args[4]);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, sceneID);
        return;
    }

    responseCode = CreateSceneWithSceneElements(scene, sceneWithSceneElements);

    if (responseCode == LSF_OK) {
        responseCode = UpdateSceneInternal(scene, sceneWithSceneElements, sceneID);
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, sceneID);

    if (responseCode == LSF_OK) {
        LSFStringList idList;
        idList.push_back(sceneID);
        controllerService.SendSignal(ControllerServiceSceneInterfaceName, "ScenesUpdated", idList);
    }
}

void SceneManager::UpdateSceneWithSceneElements(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    size_t numArgs;
    size_t numArgsExpected = 2;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, numArgsExpected)  != LSF_OK) {
        return;
    }

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    LSFString sceneID(uniqueId);
    SceneWithSceneElements sceneWithSceneElements = SceneWithSceneElements(args[1]);
    Scene scene;

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, sceneID);
        return;
    }

    responseCode = CreateScene(scene, sceneWithSceneElements);

    if (responseCode == LSF_OK) {
        responseCode = UpdateSceneInternal(scene, sceneWithSceneElements, sceneID);
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, sceneID);

    if (responseCode == LSF_OK) {
        LSFStringList idList;
        idList.push_back(sceneID);
        controllerService.SendSignal(ControllerServiceSceneInterfaceName, "ScenesUpdated", idList);
    }
}

void SceneManager::DeleteScene(Message& message)
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

    const char* sceneId;
    args[0].Get("s", &sceneId);

    LSFString sceneID(sceneId);

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, sceneID);
        return;
    }

    responseCode = masterSceneManager->IsDependentOnScene(sceneID);

    if (LSF_OK == responseCode) {
        QStatus status = scenesLock.Lock();
        SceneObject* sceneObjPtr = NULL;
        if (ER_OK == status) {
            SceneObjectMap::iterator it = scenes.find(sceneId);
            if (it != scenes.end()) {
                blobLength -= (GetString(it->second->sceneName, sceneId, it->second->sceneWithSceneElements).length() + GetString(it->second->sceneName, sceneId, it->second->scene).length());
                sceneObjPtr = it->second;
                scenes.erase(it);
                deleted = true;
                ScheduleFileWrite();
            } else {
                responseCode = LSF_ERR_NOT_FOUND;
            }
            status = scenesLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: scenesLock.Unlock() failed", __func__));
            }
            if (sceneObjPtr) {
                delete sceneObjPtr;
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: scenesLock.Lock() failed", __func__));
        }
    }

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, sceneID);

    if (deleted) {
        LSFStringList idList;
        idList.push_back(sceneID);
        controllerService.SendSignal(ControllerServiceSceneInterfaceName, "ScenesDeleted", idList);
    }
}

void SceneManager::GetScene(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_OK;

    MsgArg outArgs[6];

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    Scene scene;
    SceneWithSceneElements sceneWithSceneElements;
    LSFString sceneID = LSFString(uniqueId);

    responseCode = GetSceneInternal(scene, sceneWithSceneElements, sceneID);

    if (responseCode == LSF_OK) {
        scene.Get(&outArgs[2], &outArgs[3], &outArgs[4], &outArgs[5]);
    } else {
        outArgs[2].Set("a(asasa{sv}u)", 0, NULL);
        outArgs[3].Set("a(asassu)", 0, NULL);
        outArgs[4].Set("a(asasa{sv}a{sv}uuu)", 0, NULL);
        outArgs[5].Set("a(asasssuuu)", 0, NULL);
    }

    outArgs[0].Set("u", responseCode);
    outArgs[1].Set("s", uniqueId);

    controllerService.SendMethodReply(message, outArgs, 6);
}

void SceneManager::GetSceneWithSceneElements(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    MsgArg outArgs[3];

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    Scene scene;
    SceneWithSceneElements sceneWithSceneElements;
    LSFString sceneID = LSFString(uniqueId);

    responseCode = GetSceneInternal(scene, sceneWithSceneElements, sceneID);

    if (responseCode == LSF_OK) {
        sceneWithSceneElements.Get(&outArgs[2]);
    } else {
        outArgs[2].Set("as", 0, NULL);
    }

    outArgs[0].Set("u", responseCode);
    outArgs[1].Set("s", uniqueId);

    controllerService.SendMethodReply(message, outArgs, 3);
}

void SceneManager::SendSceneOrMasterSceneAppliedSignal(LSFString& sceneorMasterSceneId)
{
    QCC_DbgPrintf(("%s: %s", __func__, sceneorMasterSceneId.c_str()));
    bool notScene = false;
    SceneObject* sceneObj = NULL;
    scenesLock.Lock();
    SceneObjectMap::iterator it = scenes.find(sceneorMasterSceneId);
    if (it != scenes.end()) {
        sceneObj = it->second;
    } else {
        notScene = true;
    }
    scenesLock.Unlock();
    if (sceneObj) {
        sceneObj->SendSceneAppliedSignal();
    }

    if (notScene) {
        masterSceneManager->SendMasterSceneAppliedSignal(sceneorMasterSceneId);
    } else {
        LSFStringList sceneList;
        sceneList.push_back(sceneorMasterSceneId);
        controllerService.SendSignal(ControllerServiceSceneInterfaceName, "ScenesApplied", sceneList);
    }
}

void SceneManager::ApplyScene(Message& message)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_OK;

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

    const char* sceneId;
    args[0].Get("s", &sceneId);

    LSFString sceneID(sceneId);

    QCC_DbgPrintf(("%s: sceneID=%s", __func__, sceneId));

    LSFStringList scenesList;
    scenesList.push_back(sceneID);

    responseCode = ApplySceneNestedInternal(message, scenesList, sceneID);

    if (LSF_OK != responseCode) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, sceneID);
    }
}

LSFResponseCode SceneManager::ApplySceneNestedInternal(ajn::Message message, LSFStringList& sceneList, LSFString sceneOrMasterSceneId)
{
    QCC_DbgPrintf(("%s: sceneList.size() = %d", __func__, sceneList.size()));
    LSFResponseCode responseCode = LSF_OK;

    std::list<SceneWithSceneElements> scenesWithSceneElements;

    scenesWithSceneElements.clear();

    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        while (sceneList.size()) {
            LSFString sceneId = sceneList.front();
            SceneObjectMap::iterator it = scenes.find(sceneId);
            if (it != scenes.end()) {
                QCC_DbgPrintf(("%s: Found sceneID=%s", __func__, sceneId.c_str()));
                scenesWithSceneElements.push_back(it->second->sceneWithSceneElements);
            } else {
                QCC_DbgPrintf(("%s: Scene %s not found", __func__, sceneId.c_str()));
            }
            sceneList.pop_front();
        }
        status = scenesLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: scenesLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: scenesLock.Lock() failed", __func__));
    }

    LSFStringList sceneElementIDs;

    if (scenesWithSceneElements.empty()) {
        responseCode = LSF_ERR_NOT_FOUND;
    } else {

        while (scenesWithSceneElements.size()) {
            SceneWithSceneElements sceneWithSceneElements = scenesWithSceneElements.front();
            sceneElementIDs.insert(sceneElementIDs.end(), sceneWithSceneElements.sceneElements.begin(), sceneWithSceneElements.sceneElements.end());
            scenesWithSceneElements.pop_front();
        }

        if (!(sceneElementIDs.empty())) {
            QCC_DbgTrace(("%s: Calling SceneElementManager::ApplySceneElementInternal()", __func__));
            LSFResponseCode tempResponseCode = sceneElementManager->ApplySceneElementInternal(message, sceneElementIDs, sceneOrMasterSceneId);
            if (tempResponseCode != LSF_OK) {
                responseCode = LSF_ERR_FAILURE;
            }
        }
    }

    return responseCode;
}

LSFResponseCode SceneManager::CreateSceneWithSceneElements(Scene& scene, SceneWithSceneElements& sceneWithSceneElements)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    LSFString name = "AutoGenerated";
    LSFString language = "en";

    sceneWithSceneElements.sceneElements.clear();

    LSFStringList transitionEffectIds;
    LSFStringList pulseEffectIds;

    QCC_DbgPrintf(("%s", __func__));
    for (TransitionLampsLampGroupsToStateList::iterator it = scene.transitionToStateComponent.begin(); it != scene.transitionToStateComponent.end(); it++) {
        if (responseCode == LSF_OK) {
            LSFString effectId;
            TransitionEffect transitionEffect(it->state, it->transitionPeriod);
            responseCode = controllerService.GetTransitionEffectManager().CreateTransitionEffectInternal(transitionEffect, name, language, effectId);
            if (responseCode == LSF_OK) {
                transitionEffectIds.push_back(effectId);
                SceneElement sceneElement(it->lamps, it->lampGroups, effectId);
                LSFString sceneElementId;
                responseCode = sceneElementManager->CreateSceneElementInternal(sceneElement, name, language, sceneElementId);
                if (responseCode == LSF_OK) {
                    sceneWithSceneElements.sceneElements.push_back(sceneElementId);
                }
            }
        }
    }

    for (TransitionLampsLampGroupsToPresetList::iterator it = scene.transitionToPresetComponent.begin(); it != scene.transitionToPresetComponent.end(); it++) {
        if (responseCode == LSF_OK) {
            LSFString effectId;
            TransitionEffect transitionEffect(it->presetID, it->transitionPeriod);
            responseCode = controllerService.GetTransitionEffectManager().CreateTransitionEffectInternal(transitionEffect, name, language, effectId);
            if (responseCode == LSF_OK) {
                transitionEffectIds.push_back(effectId);
                SceneElement sceneElement(it->lamps, it->lampGroups, effectId);
                LSFString sceneElementId;
                responseCode = sceneElementManager->CreateSceneElementInternal(sceneElement, name, language, sceneElementId);
                if (responseCode == LSF_OK) {
                    sceneWithSceneElements.sceneElements.push_back(sceneElementId);
                }
            }
        }
    }

    for (PulseLampsLampGroupsWithStateList::iterator it = scene.pulseWithStateComponent.begin(); it != scene.pulseWithStateComponent.end(); it++) {
        if (responseCode == LSF_OK) {
            LSFString effectId;
            PulseEffect pulseEffect(it->toState, it->pulsePeriod, it->pulseDuration, it->numPulses, it->fromState);
            responseCode = controllerService.GetPulseEffectManager().CreatePulseEffectInternal(pulseEffect, name, language, effectId);
            if (responseCode == LSF_OK) {
                pulseEffectIds.push_back(effectId);
                SceneElement sceneElement(it->lamps, it->lampGroups, effectId);
                LSFString sceneElementId;
                responseCode = sceneElementManager->CreateSceneElementInternal(sceneElement, name, language, sceneElementId);
                if (responseCode == LSF_OK) {
                    sceneWithSceneElements.sceneElements.push_back(sceneElementId);
                }
            }
        }
    }

    for (PulseLampsLampGroupsWithPresetList::iterator it = scene.pulseWithPresetComponent.begin(); it != scene.pulseWithPresetComponent.end(); it++) {
        if (responseCode == LSF_OK) {
            LSFString effectId;
            PulseEffect pulseEffect(it->toPreset, it->pulsePeriod, it->pulseDuration, it->numPulses, it->fromPreset);
            responseCode = controllerService.GetPulseEffectManager().CreatePulseEffectInternal(pulseEffect, name, language, effectId);
            if (responseCode == LSF_OK) {
                pulseEffectIds.push_back(effectId);
                SceneElement sceneElement(it->lamps, it->lampGroups, effectId);
                LSFString sceneElementId;
                responseCode = sceneElementManager->CreateSceneElementInternal(sceneElement, name, language, sceneElementId);
                if (responseCode == LSF_OK) {
                    sceneWithSceneElements.sceneElements.push_back(sceneElementId);
                }
            }
        }
    }

    if (responseCode != LSF_OK) {
        while (transitionEffectIds.size()) {
            controllerService.GetTransitionEffectManager().DeleteTransitionEffectInternal(transitionEffectIds.front());
            transitionEffectIds.pop_front();
        }

        while (pulseEffectIds.size()) {
            controllerService.GetPulseEffectManager().DeletePulseEffectInternal(pulseEffectIds.front());
            pulseEffectIds.pop_front();
        }

        while (sceneWithSceneElements.sceneElements.size()) {
            sceneElementManager->DeleteSceneElementInternal(sceneWithSceneElements.sceneElements.front());
            sceneWithSceneElements.sceneElements.pop_front();
        }
    } else {
        controllerService.GetPulseEffectManager().SendPulseEffectsCreatedSignal(pulseEffectIds);
        controllerService.GetTransitionEffectManager().SendTransitionEffectsCreatedSignal(transitionEffectIds);
        sceneElementManager->SendSceneElementsCreatedSignal(sceneWithSceneElements.sceneElements);
    }

    return responseCode;
}

LSFResponseCode SceneManager::CreateScene(Scene& scene, SceneWithSceneElements& sceneWithSceneElements)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    TransitionLampsLampGroupsToStateList transitionToStateComponentList;
    TransitionLampsLampGroupsToPresetList transitionToPresetComponentList;
    PulseLampsLampGroupsWithStateList pulseWithStateComponentList;
    PulseLampsLampGroupsWithPresetList pulseWithPresetComponentList;

    for (LSFStringList::const_iterator it = sceneWithSceneElements.sceneElements.begin(); it != sceneWithSceneElements.sceneElements.end(); it++) {
        if (responseCode == LSF_OK) {
            SceneElement sceneElement;
            LSFString sceneElementId(*it);
            responseCode = sceneElementManager->GetSceneElementInternal(sceneElementId, sceneElement);
            if (responseCode == LSF_OK) {
                if (sceneElement.effectID.find("PRESET") != std::string::npos) {
                    uint32_t transitionPeriod = 0;
                    TransitionLampsLampGroupsToPreset component(sceneElement.lamps, sceneElement.lampGroups, sceneElement.effectID, transitionPeriod);
                    transitionToPresetComponentList.push_back(component);
                } else if (sceneElement.effectID.find("TRANSITION_EFFECT") != std::string::npos) {
                    TransitionEffect transitionEffect;
                    responseCode = controllerService.GetTransitionEffectManager().GetTransitionEffectInternal(sceneElement.effectID, transitionEffect);
                    if (responseCode == LSF_OK) {
                        if (!transitionEffect.state.nullState) {
                            TransitionLampsLampGroupsToState component(sceneElement.lamps, sceneElement.lampGroups, transitionEffect.state, transitionEffect.transitionPeriod);
                            transitionToStateComponentList.push_back(component);
                        } else {
                            TransitionLampsLampGroupsToPreset component(sceneElement.lamps, sceneElement.lampGroups, transitionEffect.presetID, transitionEffect.transitionPeriod);
                            transitionToPresetComponentList.push_back(component);
                        }
                    }
                } else if (sceneElement.effectID.find("PULSE_EFFECT") != std::string::npos) {
                    PulseEffect pulseEffect;
                    responseCode = controllerService.GetPulseEffectManager().GetPulseEffectInternal(sceneElement.effectID, pulseEffect);
                    if (responseCode == LSF_OK) {
                        if (!pulseEffect.toState.nullState) {
                            PulseLampsLampGroupsWithState component(sceneElement.lamps, sceneElement.lampGroups, pulseEffect.fromState, pulseEffect.toState, pulseEffect.pulsePeriod, pulseEffect.pulseDuration, pulseEffect.numPulses);
                            pulseWithStateComponentList.push_back(component);
                        } else {
                            PulseLampsLampGroupsWithPreset component(sceneElement.lamps, sceneElement.lampGroups, pulseEffect.fromPreset, pulseEffect.toPreset, pulseEffect.pulsePeriod, pulseEffect.pulseDuration, pulseEffect.numPulses);
                            pulseWithPresetComponentList.push_back(component);
                        }
                    }
                }
            }
        }
    }

    if (responseCode == LSF_OK) {
        scene = Scene(transitionToStateComponentList, transitionToPresetComponentList, pulseWithStateComponentList, pulseWithPresetComponentList);
    }

    return responseCode;
}

LSFResponseCode SceneManager::GetSceneInternal(Scene& scene, SceneWithSceneElements& sceneWithSceneElements, LSFString& sceneID)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        SceneObjectMap::iterator it = scenes.find(sceneID.c_str());
        if (it != scenes.end()) {
            scene = it->second->scene;
            sceneWithSceneElements = it->second->sceneWithSceneElements;
            responseCode = LSF_OK;
        }
        status = scenesLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: scenesLock.Unlock() failed", __func__));
        }
    } else {
        responseCode = LSF_ERR_BUSY;
        QCC_LogError(status, ("%s: scenesLock.Lock() failed", __func__));
    }

    return responseCode;
}

LSFResponseCode SceneManager::CreateSceneInternal(Scene& scene, SceneWithSceneElements& sceneWithSceneElements, LSFString& name, LSFString& language, LSFString& sceneID)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    bool created = false;
    sceneID = GenerateUniqueID("SCENE");
    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name.empty()) {
        QCC_LogError(ER_FAIL, ("%s: scene name is empty", __func__));
        responseCode = LSF_ERR_EMPTY_NAME;
    } else if (scene.invalidArgs) {
        QCC_LogError(ER_FAIL, ("%s: Invalid Scene components specified", __func__));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name.length() > LSF_MAX_NAME_LENGTH) {
        QCC_LogError(ER_FAIL, ("%s: name length exceeds %d", __func__, LSF_MAX_NAME_LENGTH));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = scenesLock.Lock();
        if (ER_OK == status) {
            if (scenes.size() < OEM_CS_MAX_SUPPORTED_NUM_LSF_ENTITY) {
                std::string newSceneStr = (GetString(name, sceneID, sceneWithSceneElements) + GetString(name, sceneID, scene));
                size_t newlen = blobLength + newSceneStr.length() + sceneID.length();

                if (newlen < MAX_FILE_LEN) {
                    SceneObject* newObj = new SceneObject(*this, sceneID, scene, sceneWithSceneElements, name);
                    if (newObj) {
                        blobLength = newlen;
                        scenes.insert(std::make_pair(sceneID, newObj));
                        created = true;
                        ScheduleFileWrite();
                    } else {
                        QCC_LogError(ER_FAIL, ("%s: Could not allocate memory for new SceneObject", __func__));
                        responseCode = LSF_ERR_RESOURCES;
                    }
                } else {
                    responseCode = LSF_ERR_RESOURCES;
                }
            } else {
                QCC_LogError(ER_FAIL, ("%s: No slot for new Scene", __func__));
                responseCode = LSF_ERR_NO_SLOT;
            }
            status = scenesLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: scenesLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: scenesLock.Lock() failed", __func__));
        }
    }

    if (!created) {
        sceneID.clear();
    }

    return responseCode;
}

LSFResponseCode SceneManager::UpdateSceneInternal(Scene& scene, SceneWithSceneElements& sceneWithSceneElements, LSFString& sceneID)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    if (scene.invalidArgs) {
        QCC_LogError(ER_FAIL, ("%s: Invalid Scene arguments", __func__));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = scenesLock.Lock();
        if (ER_OK == status) {
            SceneObjectMap::iterator it = scenes.find(sceneID);
            if (it != scenes.end()) {
                size_t newlen = blobLength;
                // sub len of old scene, add len of new scene
                newlen -= (GetString(it->second->sceneName, sceneID, it->second->sceneWithSceneElements).length() + GetString(it->second->sceneName, sceneID, it->second->scene).length());
                newlen += (GetString(it->second->sceneName, sceneID, sceneWithSceneElements).length() + GetString(it->second->sceneName, sceneID, scene).length());

                if (newlen < MAX_FILE_LEN) {
                    blobLength = newlen;
                    it->second->scene = scene;
                    it->second->sceneWithSceneElements = sceneWithSceneElements;
                    responseCode = LSF_OK;
                    ScheduleFileWrite();
                } else {
                    responseCode = LSF_ERR_RESOURCES;
                }
            }
            status = scenesLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: scenesLock.Unlock() failed", __func__));
            }
        } else {
            responseCode = LSF_ERR_BUSY;
            QCC_LogError(status, ("%s: scenesLock.Lock() failed", __func__));
        }
    }

    return responseCode;
}

void SceneManager::ReadSavedData()
{
    QCC_DbgTrace(("%s", __func__));

    std::istringstream stream1;
    stream1.clear();
    if (ValidateScene2FileAndRead(stream1)) {
        ReplaceScene2List(stream1);
    } else {
        std::ostringstream stream;
        stream << "Scene " << initialStateID << " \"" << initialStateID << "\"\n";
        stream << "EndScene\n";
        WriteScene2FileWithChecksumAndTimestamp(stream.str(), GetChecksum(stream.str()), 0UL);
    }

    std::istringstream stream2;
    stream2.clear();
    if (ValidateFileAndRead(stream2)) {
        ReplaceMap(stream2);
    } else {
        std::ostringstream stream;
        stream << "Scene " << initialStateID << " \"" << initialStateID << "\"\n";
        stream << "EndScene\n";
        WriteFileWithChecksumAndTimestamp(stream.str(), GetChecksum(stream.str()), 0UL);
    }

    std::istringstream stream3;
    stream3.clear();
    if (ValidateUpdateFileAndRead(stream3)) {
        ReplaceUpdatesList(stream3);
    } else {
        std::ostringstream stream;
        stream << initialStateID << std::endl;
        WriteUpdatesFileWithChecksumAndTimestamp(stream.str(), GetChecksum(stream.str()), 0UL);
    }

    SyncSceneData();
}

void SceneManager::ReplaceMap(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));
    while (!stream.eof()) {
        std::string token;
        std::string id;
        std::string name;
        Scene scene;

        token = ParseString(stream);
        if (token == resetID) {
            QCC_DbgPrintf(("The file has a reset entry. Clearing the map"));
            for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
                delete it->second;
            }
            scenes.clear();
        } else if (token == "Scene") {
            token.clear();
            id.clear();
            name.clear();
            scene = Scene();
            id = ParseString(stream);
            name = ParseString(stream);

            if (0 == strcmp(id.c_str(), resetID.c_str())) {
                QCC_DbgPrintf(("The file has a reset entry. Clearing the map"));
                for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
                    delete it->second;
                }
                scenes.clear();
            } else if (0 == strcmp(id.c_str(), initialStateID.c_str())) {
                QCC_DbgPrintf(("The file has a initialState entry. So we ignore it"));
            } else {
                do {
                    token = ParseString(stream);
                    if (token == "TransitionLampsLampGroupsToState") {
                        LSFStringList lampList;
                        LSFStringList lampGroupList;
                        LampState lampState;
                        uint32_t period = 0;

                        do {
                            token = ParseString(stream);
                            if (token == "Lamp") {
                                std::string id = ParseString(stream);
                                lampList.push_back(id);
                            } else if (token == "LampGroup") {
                                std::string id = ParseString(stream);
                                lampGroupList.push_back(id);
                            } else if (token == "LampState") {
                                ParseLampState(stream, lampState);
                            } else if (token == "Period") {
                                period = ParseValue<uint32_t>(stream);
                                scene.transitionToStateComponent.push_back(TransitionLampsLampGroupsToState(lampList, lampGroupList, lampState, period));
                                lampList.clear();
                                lampGroupList.clear();
                                lampState = LampState();
                                period = 0;
                            }
                        } while (token != "EndTransitionLampsLampGroupsToState");
                    } else if (token == "TransitionLampsLampGroupsToPreset") {
                        LSFStringList lampList;
                        LSFStringList lampGroupList;
                        LSFString preset;
                        uint32_t period = 0;

                        do {
                            token = ParseString(stream);

                            if (token == "Lamp") {
                                std::string id = ParseString(stream);
                                lampList.push_back(id);
                            } else if (token == "LampGroup") {
                                std::string id = ParseString(stream);
                                lampGroupList.push_back(id);
                            } else if (token == "LampState") {
                                preset = ParseString(stream);
                            } else if (token == "Period") {
                                period = ParseValue<uint32_t>(stream);
                                scene.transitionToPresetComponent.push_back(TransitionLampsLampGroupsToPreset(lampList, lampGroupList, preset, period));
                                lampList.clear();
                                lampGroupList.clear();
                                preset.clear();
                                period = 0;
                            }
                        } while (token != "EndTransitionLampsLampGroupsToPreset");
                    } else if (token == "PulseLampsLampGroupsWithState") {
                        LSFStringList lampList;
                        LSFStringList lampGroupList;
                        LampState fromState, toState;
                        uint32_t period = 0, duration = 0, pulses = 0;

                        do {
                            token = ParseString(stream);
                            if (token == "Lamp") {
                                std::string id = ParseString(stream);
                                lampList.push_back(id);
                            } else if (token == "LampGroup") {
                                std::string id = ParseString(stream);
                                lampGroupList.push_back(id);
                            } else if (token == "FromState") {
                                ParseLampState(stream, fromState);
                            } else if (token == "ToState") {
                                ParseLampState(stream, toState);
                            } else if (token == "Period") {
                                period = ParseValue<uint32_t>(stream);
                            } else if (token == "Duration") {
                                duration = ParseValue<uint32_t>(stream);
                            } else if (token == "Pulses") {
                                pulses = ParseValue<uint32_t>(stream);
                                scene.pulseWithStateComponent.push_back(PulseLampsLampGroupsWithState(lampList, lampGroupList, fromState, toState, period, duration, pulses));
                                lampList.clear();
                                lampGroupList.clear();
                                fromState = LampState();
                                toState = LampState();
                                period = 0;
                                duration = 0;
                                pulses = 0;
                            }
                        } while (token != "EndPulseLampsLampGroupsWithState");
                    } else if (token == "PulseLampsLampGroupsWithPreset") {
                        LSFStringList lampList;
                        LSFStringList lampGroupList;
                        LSFString fromState, toState;
                        uint32_t period = 0, duration = 0, pulses = 0;

                        do {
                            token = ParseString(stream);
                            if (token == "Lamp") {
                                std::string id = ParseString(stream);
                                lampList.push_back(id);
                            } else if (token == "LampGroup") {
                                std::string id = ParseString(stream);
                                lampGroupList.push_back(id);
                            } else if (token == "FromState") {
                                fromState = ParseString(stream);
                            } else if (token == "ToState") {
                                toState = ParseString(stream);
                            } else if (token == "Period") {
                                period = ParseValue<uint32_t>(stream);
                            } else if (token == "Duration") {
                                duration = ParseValue<uint32_t>(stream);
                            } else if (token == "Pulses") {
                                pulses = ParseValue<uint32_t>(stream);
                                scene.pulseWithPresetComponent.push_back(PulseLampsLampGroupsWithPreset(lampList, lampGroupList, fromState, toState, period, duration, pulses));
                                lampList.clear();
                                lampGroupList.clear();
                                fromState.clear();
                                toState.clear();
                                period = 0;
                                duration = 0;
                                pulses = 0;
                            }
                        } while (token != "EndPulseLampsLampGroupsWithPreset");
                    }
                } while (token != "EndScene");

                oldSceneMap.insert(std::make_pair(id, std::make_pair(name, scene)));
            }
        }
    }
}

static void OutputLamps(std::ostream& stream, const std::string& name, const LSFStringList& list)
{
    for (LSFStringList::const_iterator it = list.begin(); it != list.end(); ++it) {
        stream << "\t\t" << name << ' ' << *it << '\n';
    }
}

static void OutputState(std::ostream& stream, const std::string& name, const LampState& state)
{
    if (!state.nullState) {
        stream << "\t\t" << name << ' '
               << (state.nullState ? 1 : 0) << ' '
               << (state.onOff ? 1 : 0) << ' '
               << state.hue << ' ' << state.saturation << ' '
               << state.colorTemp << ' ' << state.brightness << '\n';
    } else {
        stream << "\t\t" << name << ' '
               << (state.nullState ? 1 : 0) << '\n';
    }
}

void SceneManager::ReplaceUpdatesList(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));
    while (!stream.eof()) {
        std::string id = ParseString(stream);
        if (0 == strcmp(id.c_str(), resetID.c_str())) {
            QCC_DbgPrintf(("The file has a reset entry. So we ignore it"));
        } else if (0 == strcmp(id.c_str(), initialStateID.c_str())) {
            QCC_DbgPrintf(("The file has a initialState entry. So we ignore it"));
        } else {
            if (!id.empty()) {
                sceneUpdates.push_back(id);
                QCC_DbgPrintf(("%s: Add %s to updates list", __func__, id.c_str()));
            }
        }
    }
}

void SceneManager::ReplaceScene2List(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));

    sceneWithSceneElementsMap.clear();

    while (!stream.eof()) {
        std::string token;
        std::string id;
        std::string name;
        SceneWithSceneElements sceneWithSceneElements;

        token = ParseString(stream);
        if (token == "Scene") {
            token.clear();
            id.clear();
            name.clear();
            sceneWithSceneElements = SceneWithSceneElements();
            id = ParseString(stream);
            name = ParseString(stream);

            if (0 == strcmp(id.c_str(), resetID.c_str())) {
                QCC_DbgPrintf(("The file has a reset entry. Clearing the map"));
                for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
                    delete it->second;
                }
                scenes.clear();
            } else if (0 == strcmp(id.c_str(), initialStateID.c_str())) {
                QCC_DbgPrintf(("The file has a initialState entry. So we ignore it"));
            } else {
                do {
                    token = ParseString(stream);
                    if (token == "SceneElements") {
                        std::string id = ParseString(stream);
                        while (id != "EndSceneElements") {
                            sceneWithSceneElements.sceneElements.push_back(id);
                            id = ParseString(stream);
                        }
                    }
                } while (token != "EndScene");
                sceneWithSceneElementsMap.insert(std::make_pair(id, std::make_pair(name, sceneWithSceneElements)));
            }
        }
    }
}

void SceneManager::SyncSceneData(bool isLeader)
{
    QCC_DbgTrace(("%s", __func__));

    if (sceneWithSceneElementsMap.empty()) {
        QCC_DbgPrintf(("%s: sceneWithSceneElementsMap empty. Making a copy of scenes", __func__));
        for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
            sceneWithSceneElementsMap.insert(std::make_pair(it->first, std::make_pair(it->second->sceneName, it->second->sceneWithSceneElements)));
        }
    }

    // Add new entries in oldSceneMap to sceneWithSceneElementsMap and update name as required.
    if (isLeader) {
        // Remove the entries that do no exist in the old scene map if we fetched the old scene map
        if (!oldSceneMap.empty()) {
            for (SceneWithSceneElementsMap::iterator it = sceneWithSceneElementsMap.begin(); it != sceneWithSceneElementsMap.end();) {
                if (oldSceneMap.find(it->first) == oldSceneMap.end()) {
                    it = sceneWithSceneElementsMap.erase(it);
                } else {
                    it++;
                }
            }
        }

        for (SceneMap::iterator it = oldSceneMap.begin(); it != oldSceneMap.end(); it++) {
            SceneWithSceneElementsMap::iterator eit = sceneWithSceneElementsMap.find(it->first);
            if (eit == sceneWithSceneElementsMap.end()) {
                SceneWithSceneElements sceneWithSceneElements;
                LSFResponseCode responseCode = CreateSceneWithSceneElements(it->second.second, sceneWithSceneElements);
                if (responseCode == LSF_OK) {
                    sceneWithSceneElementsMap.insert(std::make_pair(it->first, std::make_pair(it->second.first, sceneWithSceneElements)));
                }
            } else {
                if (eit->second.first != it->second.first) {
                    eit->second.first = it->second.first;
                }
            }
        }

        // Update the sceneWithSceneElementsMap as per sceneUpdates
        while (sceneUpdates.size()) {
            SceneMap::iterator oit = oldSceneMap.find(sceneUpdates.front());
            if (oit != oldSceneMap.end()) {
                SceneWithSceneElementsMap::iterator nit = sceneWithSceneElementsMap.find(sceneUpdates.front());
                if (nit != sceneWithSceneElementsMap.end()) {
                    sceneWithSceneElementsMap.erase(nit);
                }
                SceneWithSceneElements sceneWithSceneElements;
                LSFResponseCode responseCode = CreateSceneWithSceneElements(oit->second.second, sceneWithSceneElements);
                if (responseCode == LSF_OK) {
                    sceneWithSceneElementsMap.insert(std::make_pair(oit->first, std::make_pair(oit->second.first, sceneWithSceneElements)));
                }
            }
            sceneUpdates.pop_front();
        }
    }

    oldSceneMap.clear();
    sceneUpdates.clear();

    /*
     * Add entries corresponding to the contents of sceneWithSceneElementsMap
     */
    for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end();) {
        if (sceneWithSceneElementsMap.find(it->first) == sceneWithSceneElementsMap.end()) {
            QCC_DbgPrintf(("%s: Deleting %s from scenes as it is not present in sceneWithSceneElementsMap", __func__, it->first.c_str()));
            delete it->second;
            it = scenes.erase(it);
        } else {
            ++it;
        }
    }

    for (SceneWithSceneElementsMap::iterator it = sceneWithSceneElementsMap.begin(); it != sceneWithSceneElementsMap.end(); it++) {
        Scene scene;
        LSFResponseCode responseCode = CreateScene(scene, it->second.second);
        if (responseCode == LSF_OK) {
            SceneObjectMap::iterator sit = scenes.find(it->first);
            if (sit == scenes.end()) {
                LSFString id = it->first;
                LSFString name = it->second.first;
                SceneObject* newObj = new SceneObject(*this, id, scene, it->second.second, name);
                if (newObj) {
                    QCC_DbgPrintf(("%s: Adding %s to scenes as it is present in sceneWithSceneElementsMap", __func__, it->first.c_str()));
                    scenes.insert(std::make_pair(id, newObj));
                } else {
                    QCC_LogError(ER_FAIL, ("%s: Could not allocate memory for new SceneObject", __func__));
                }
            } else {
                QCC_DbgPrintf(("%s: Updating %s details in scenes", __func__, it->first.c_str()));
                sit->second->scene = scene;
                sit->second->sceneWithSceneElements = it->second.second;
                sit->second->sceneName = it->second.first;
            }
        }
    }

    sceneWithSceneElementsMap.clear();

    std::string output;
    std::string scene2;
    GetString(output, scene2, scenes);
    blobLength = scene2.size() + output.size();
}

std::string SceneManager::GetString(const std::string& name, const std::string& id, const Scene& scene)
{
    std::ostringstream stream;
    stream << "Scene " << id << " \"" << name << "\"\n";

    // TransitionLampsLampGroupsToState
    if (!scene.transitionToStateComponent.empty()) {
        stream << "\tTransitionLampsLampGroupsToState\n";
        for (TransitionLampsLampGroupsToStateList::const_iterator cit = scene.transitionToStateComponent.begin(); cit != scene.transitionToStateComponent.end(); ++cit) {
            const TransitionLampsLampGroupsToState& comp = *cit;

            OutputLamps(stream, "Lamp", comp.lamps);
            OutputLamps(stream, "LampGroup", comp.lampGroups);
            OutputState(stream, "LampState", comp.state);
            stream << "\t\tPeriod " << comp.transitionPeriod << '\n';
        }

        stream << "\tEndTransitionLampsLampGroupsToState\n";
    }

    if (!scene.transitionToPresetComponent.empty()) {
        stream << "\tTransitionLampsLampGroupsToPreset\n";
        for (TransitionLampsLampGroupsToPresetList::const_iterator cit = scene.transitionToPresetComponent.begin(); cit != scene.transitionToPresetComponent.end(); ++cit) {
            const TransitionLampsLampGroupsToPreset& comp = *cit;

            OutputLamps(stream, "Lamp", comp.lamps);
            OutputLamps(stream, "LampGroup", comp.lampGroups);
            stream << "\t\tLampState " << comp.presetID << "\n\t\tPeriod " << comp.transitionPeriod << '\n';
        }

        stream << "\tEndTransitionLampsLampGroupsToPreset\n";
    }


    if (!scene.pulseWithStateComponent.empty()) {
        stream << "\tPulseLampsLampGroupsWithState\n";
        for (PulseLampsLampGroupsWithStateList::const_iterator cit = scene.pulseWithStateComponent.begin(); cit != scene.pulseWithStateComponent.end(); ++cit) {
            const PulseLampsLampGroupsWithState& comp = *cit;

            OutputLamps(stream, "Lamp", comp.lamps);
            OutputLamps(stream, "LampGroup", comp.lampGroups);
            OutputState(stream, "FromState", comp.fromState);
            OutputState(stream, "ToState", comp.toState);
            stream << "\t\tPeriod " << comp.pulsePeriod << "\n\t\tDuration " << comp.pulseDuration << "\n\t\tPulses " << comp.numPulses << '\n';
        }

        stream << "\tEndPulseLampsLampGroupsWithState\n";
    }

    if (!scene.pulseWithPresetComponent.empty()) {
        stream << "\tPulseLampsLampGroupsWithPreset\n";
        for (PulseLampsLampGroupsWithPresetList::const_iterator cit = scene.pulseWithPresetComponent.begin(); cit != scene.pulseWithPresetComponent.end(); ++cit) {
            const PulseLampsLampGroupsWithPreset& comp = *cit;

            OutputLamps(stream, "Lamp", comp.lamps);
            OutputLamps(stream, "LampGroup", comp.lampGroups);
            stream << "\t\tFromState " << comp.fromPreset << "\n\t\tToState " << comp.toPreset
                   << "\n\t\tPeriod " << comp.pulsePeriod << "\n\t\tDuration " << comp.pulseDuration << "\n\t\tPulses " << comp.numPulses << '\n';
        }

        stream << "\tEndPulseLampsLampGroupsWithPreset\n";
    }

    stream << "EndScene\n";
    return stream.str();
}

std::string SceneManager::GetString(const std::string& name, const std::string& id, const SceneWithSceneElements& sceneWithSceneElements)
{
    std::ostringstream stream;
    stream << "Scene " << id << " \"" << name << "\"\n";

    if (!sceneWithSceneElements.sceneElements.empty()) {
        stream << "\tSceneElements\n\t\t";

        for (LSFStringList::const_iterator lit = sceneWithSceneElements.sceneElements.begin(); lit != sceneWithSceneElements.sceneElements.end(); ++lit) {
            stream << " " << *lit;
        }

        stream << "\n\tEndSceneElements\n";
    }

    stream << "EndScene\n";
    return stream.str();
}

void SceneManager::GetString(std::string& scenes, std::string& scene2, const SceneObjectMap& items)
{
    std::ostringstream stream1;
    std::ostringstream stream2;
    if (0 == items.size()) {
        QCC_DbgPrintf(("%s: File is being reset", __func__));
        const LSFString& id = resetID;
        const LSFString& name = resetID;

        stream1 << "Scene " << id << " \"" << name << "\"\n";
        stream1 << "EndScene\n";

        stream2 << "Scene " << id << " \"" << name << "\"\n";
        stream2 << "EndScene\n";
    } else {
        for (SceneObjectMap::const_iterator it = items.begin(); it != items.end(); it++) {
            const LSFString& id = it->first;
            const LSFString& name = it->second->sceneName;

            const SceneWithSceneElements& sceneWithSceneElements = it->second->sceneWithSceneElements;
            stream2 << GetString(name, id, sceneWithSceneElements);

            const Scene& scene = it->second->scene;
            stream1 << GetString(name, id, scene);
        }
    }

    scenes = stream1.str();
    scene2 = stream2.str();
}

std::string SceneManager::GetUpdatesString(const std::list<LSFString>& updates)
{
    QCC_DbgTrace(("%s", __func__));
    std::ostringstream stream;
    if (0 == updates.size()) {
        QCC_DbgPrintf(("%s: File is being reset", __func__));
        stream << resetID << std::endl;
    } else {
        for (std::list<LSFString>::const_iterator it = updates.begin(); it != updates.end(); ++it) {
            stream << *it << std::endl;
        }
    }

    return stream.str();
}

bool SceneManager::GetString(std::string& output, std::string& updates, std::string& scene2, uint32_t& checksum, uint64_t& timestamp, uint32_t& updatesChksum, uint64_t& updatesTs, uint32_t& scene2Chksum, uint64_t& scene2Ts)
{
    SceneObjectMap mapCopy;
    mapCopy.clear();
    std::list<LSFString> updatesCopy;
    updatesCopy.clear();
    bool ret = false;
    output.clear();
    updates.clear();

    scenesLock.Lock();
    // we can't hold this lock for the entire time!
    if (updated) {
        mapCopy = scenes;
        updatesCopy = sceneUpdates;
        updated = false;
        ret = true;
    }
    scenesLock.Unlock();

    if (ret) {
        GetString(output, scene2, mapCopy);
        updates = GetUpdatesString(updatesCopy);
        scenesLock.Lock();
        if (blobUpdateCycle) {
            timestamp = timeStamp;
            updatesTs = updatesTimeStamp;
            scene2Ts = scene2TimeStamp;
            blobUpdateCycle = false;
        } else {
            timeStamp = timestamp = GetTimestampInMs();
            updatesTimeStamp = updatesTs = GetTimestampInMs();
            scene2TimeStamp = scene2Ts = GetTimestampInMs();
        }
        checkSum = checksum = GetChecksum(output);
        updatesCheckSum = updatesChksum = GetChecksum(updates);
        scene2CheckSum = scene2Chksum = GetChecksum(scene2);
        scenesLock.Unlock();
    }

    return ret;
}

void SceneManager::HandleReceivedBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    uint64_t currentTimestamp = GetTimestampInMs();
    scenesLock.Lock();
    if (((timeStamp == 0) || ((currentTimestamp - timeStamp) > timestamp)) && (checkSum != checksum)) {
        std::istringstream stream(blob.c_str());
        ReplaceMap(stream);
        timeStamp = currentTimestamp;
        checkSum = checksum;
    }
    scenesLock.Unlock();
}

void SceneManager::HandleReceivedUpdateBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    uint64_t currentTimestamp = GetTimestampInMs();
    scenesLock.Lock();
    if (((updatesTimeStamp == 0) || ((currentTimestamp - updatesTimeStamp) > timestamp)) && (updatesCheckSum != checksum)) {
        std::istringstream stream(blob.c_str());
        ReplaceUpdatesList(stream);
        updatesTimeStamp = currentTimestamp;
        updatesCheckSum = checksum;
    }
    scenesLock.Unlock();
}

void SceneManager::HandleReceivedScene2Blob(const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    uint64_t currentTimestamp = GetTimestampInMs();
    scenesLock.Lock();
    if (((scene2TimeStamp == 0) || ((currentTimestamp - scene2TimeStamp) > timestamp)) && (scene2CheckSum != checksum)) {
        std::istringstream stream(blob.c_str());
        ReplaceScene2List(stream);
        scene2TimeStamp = currentTimestamp;
        scene2CheckSum = checksum;
    }
    scenesLock.Unlock();
}

void SceneManager::ReadWriteFile()
{
    QCC_DbgPrintf(("%s", __func__));

    if (filePath.empty() || updateFilePath.empty() || scene2FilePath.empty()) {
        return;
    }

    std::string output;
    uint32_t checksum;
    uint64_t timestamp;
    std::string updates;
    uint32_t updateChecksum;
    uint64_t updateTimestamp;
    std::string scene2;
    uint32_t scene2Checksum;
    uint64_t scene2Timestamp;
    bool status = false;

    status = GetString(output, updates, scene2, checksum, timestamp, updateChecksum, updateTimestamp, scene2Checksum, scene2Timestamp);

    if (status) {
        QCC_DbgPrintf(("%s: Writing Scenes file", __func__));
        WriteFileWithChecksumAndTimestamp(output, checksum, timestamp);
        if (timestamp != 0UL) {
            uint64_t currentTime = GetTimestampInMs();
            controllerService.SendBlobUpdate(LSF_SCENE, output, checksum, (currentTime - timestamp));
        }

        QCC_DbgPrintf(("%s: Writing Updates file", __func__));
        WriteUpdatesFileWithChecksumAndTimestamp(updates, updateChecksum, updateTimestamp);
        if (updateTimestamp != 0UL) {
            uint64_t currentTime = GetTimestampInMs();
            controllerService.SendBlobUpdate(LSF_SCENE_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
        }


        QCC_DbgPrintf(("%s: Writing Scene2 file", __func__));
        WriteScene2FileWithChecksumAndTimestamp(scene2, scene2Checksum, scene2Timestamp);
        if (scene2Timestamp != 0UL) {
            uint64_t currentTime = GetTimestampInMs();
            controllerService.SendBlobUpdate(LSF_SCENE_2, scene2, scene2Checksum, (currentTime - scene2Timestamp));
        }
    }

    std::list<ajn::Message> tempMessageList;
    std::list<ajn::Message> tempUpdateMessageList;
    std::list<ajn::Message> tempScene2MessageList;

    readMutex.Lock();
    if (read) {
        tempMessageList = readBlobMessages;
        readBlobMessages.clear();
        tempUpdateMessageList = readUpdateBlobMessages;
        readUpdateBlobMessages.clear();
        tempScene2MessageList = readScene2BlobMessages;
        readScene2BlobMessages.clear();
        read = false;
    }
    readMutex.Unlock();

    if ((tempMessageList.size() || tempUpdateMessageList.size() || tempScene2MessageList.size() || sendUpdate) && !status) {
        std::istringstream stream;
        bool fileStatus = ValidateFileAndReadInternal(checksum, timestamp, stream);
        if (fileStatus) {
            output = stream.str();
            while (!tempMessageList.empty()) {
                uint64_t currentTime = GetTimestampInMs();
                controllerService.SendGetBlobReply(tempMessageList.front(), LSF_SCENE, output, checksum, (currentTime - timestamp));
                tempMessageList.pop_front();
            }
        } else {
            QCC_LogError(ER_FAIL, ("%s: Scene persistent store corrupted", __func__));
        }

        std::istringstream updateStream;
        bool updateStatus = ValidateUpdateFileAndReadInternal(updateChecksum, updateTimestamp, updateStream);
        if (updateStatus) {
            updates = updateStream.str();
            while (!tempUpdateMessageList.empty()) {
                uint64_t currentTime = GetTimestampInMs();
                controllerService.SendGetBlobReply(tempUpdateMessageList.front(), LSF_SCENE_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
                tempUpdateMessageList.pop_front();
            }
        } else {
            QCC_LogError(ER_FAIL, ("%s: Scene Update persistent store corrupted", __func__));
        }

        std::istringstream scene2Stream;
        bool scene2Status = ValidateScene2FileAndReadInternal(scene2Checksum, scene2Timestamp, scene2Stream);
        if (scene2Status) {
            scene2 = scene2Stream.str();
            while (!tempScene2MessageList.empty()) {
                uint64_t currentTime = GetTimestampInMs();
                controllerService.SendGetBlobReply(tempScene2MessageList.front(), LSF_SCENE_2, scene2, scene2Checksum, (currentTime - scene2Timestamp));
                tempScene2MessageList.pop_front();
            }
        } else {
            QCC_LogError(ER_FAIL, ("%s: Scene2 persistent store corrupted", __func__));
        }
    }

    if (sendUpdate) {
        sendUpdate = false;
        uint64_t currentTime = GetTimestampInMs();
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_SCENE, output, checksum, (currentTime - timestamp));
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_SCENE_UPDATE, updates, updateChecksum, (currentTime - updateTimestamp));
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_SCENE_2, scene2, scene2Checksum, (currentTime - scene2Timestamp));
    }
}

uint32_t SceneManager::GetControllerServiceSceneInterfaceVersion(void)
{
    QCC_DbgPrintf(("%s: controllerSceneInterfaceVersion=%d", __func__, ControllerServiceSceneInterfaceVersion));
    return ControllerServiceSceneInterfaceVersion;
}
