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

SceneObject::SceneObject(SceneManager& sceneMgr, LSFString& sceneid, Scene& tempScene, SceneWithSceneElements& tempSceneWithSceneElements, bool hasElements, LSFString& name) :
    BusObject((LSFString(ApplySceneEventActionObjectPath) + sceneid).c_str()), sceneManager(sceneMgr), sceneId(sceneid), scene(tempScene), sceneWithSceneElements(tempSceneWithSceneElements), hasElements(hasElements), sceneName(name), appliedSceneMember(NULL)
{
    QCC_DbgPrintf(("%s", __func__));
    InterfaceDescription* intf = NULL;
    QStatus status = sceneManager.controllerService.GetBusAttachment().CreateInterface((LSFString(ApplySceneEventActionInterfaceName) + sceneid).c_str(), intf);
    if (status == ER_OK) {
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
    qcc::String path = ApplySceneEventActionObjectPath;
    path.append(sceneId.c_str());
    qcc::String intf = ApplySceneEventActionInterfaceName;
    intf.append(sceneId.c_str());
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
    qcc::String path = ApplySceneEventActionObjectPath;
    path.append(sceneId.c_str());
    qcc::String intf = ApplySceneEventActionInterfaceName;
    intf.append(sceneId.c_str());
    sceneManager.controllerService.Announce();
}

SceneManager::SceneManager(ControllerService& controllerSvc, LampGroupManager& lampGroupMgr, SceneElementManager* sceneElementMgr, MasterSceneManager* masterSceneMgr, const std::string& sceneFile) :
    Manager(controllerSvc, sceneFile), lampGroupManager(lampGroupMgr), sceneElementManager(sceneElementMgr), masterSceneManager(masterSceneMgr), blobLength(0)
{
    QCC_DbgPrintf(("%s", __func__));
    scenes.clear();
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

LSFResponseCode SceneManager::IsDependentOnPreset(LSFString& presetID)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
            responseCode = it->second->IsDependentOnPreset(presetID);
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

LSFResponseCode SceneManager::IsDependentOnLampGroup(LSFString& lampGroupID)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
            responseCode = it->second->IsDependentOnLampGroup(lampGroupID);
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

LSFResponseCode SceneManager::IsDependentOnTransitionEffect(LSFString& transitionEffectID)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
            responseCode = it->second->IsDependentOnTransitionEffect(transitionEffectID);
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

LSFResponseCode SceneManager::IsDependentOnPulseEffect(LSFString& pulseEffectID)
{
    QCC_DbgTrace(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
            responseCode = it->second->IsDependentOnPulseEffect(pulseEffectID);
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

void SceneManager::GetAllSceneIDsInternal(Message& message, bool isSceneElement)
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

void SceneManager::GetSceneNameInternal(Message& message, bool isSceneElement)
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

void SceneManager::SetSceneNameInternal(Message& message, bool isSceneElement)
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
                    size_t newlen = blobLength - it->second->sceneName.length() + newName.length();

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

void SceneManager::CreateSceneInternal(Message& message, bool isSceneElement)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_OK;

    LSFString sceneID;

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, sceneID);
        return;
    }

    bool created = false;
    sceneID = GenerateUniqueID("SCENE");

    const ajn::MsgArg* inputArgs;
    size_t numInputArgs;
    size_t numInputArgsExpected = isSceneElement ? 3 : 6;
    message->GetArgs(numInputArgs, inputArgs);

    if (controllerService.CheckNumArgsInMessage(numInputArgs, numInputArgsExpected) != LSF_OK) {
        return;
    }

    SceneWithSceneElements sceneWithSceneElements = isSceneElement ? SceneWithSceneElements(inputArgs[0]) : SceneWithSceneElements();
    Scene scene = !isSceneElement ? Scene(inputArgs[0], inputArgs[1], inputArgs[2], inputArgs[3]) : Scene();

    LSFString name = static_cast<LSFString>(inputArgs[numInputArgsExpected-2].v_string.str);
    LSFString language = static_cast<LSFString>(inputArgs[numInputArgsExpected-1].v_string.str);

    if (0 != strcmp("en", language.c_str())) {
        QCC_LogError(ER_FAIL, ("%s: Language %s not supported", __func__, language.c_str()));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name.empty()) {
        QCC_LogError(ER_FAIL, ("%s: scene name is empty", __func__));
        responseCode = LSF_ERR_EMPTY_NAME;
    } else if (!isSceneElement && scene.invalidArgs) {
        QCC_LogError(ER_FAIL, ("%s: Invalid Scene components specified", __func__));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else if (name.length() > LSF_MAX_NAME_LENGTH) {
        QCC_LogError(ER_FAIL, ("%s: name length exceeds %d", __func__, LSF_MAX_NAME_LENGTH));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = scenesLock.Lock();
        if (ER_OK == status) {
            if (scenes.size() < OEM_CS_MAX_SUPPORTED_NUM_LSF_ENTITY) {
                std::string newSceneStr = isSceneElement ? GetString(name, sceneID, sceneWithSceneElements) : GetString(name, sceneID, scene);
                size_t newlen = blobLength + newSceneStr.length();

                if (newlen < MAX_FILE_LEN) {
                    SceneObject* newObj = new SceneObject(*this, sceneID, scene, sceneWithSceneElements, isSceneElement, name);
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

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, sceneID);

    if (created) {
        LSFStringList idList;
        idList.push_back(sceneID);
        controllerService.SendSignal(ControllerServiceSceneInterfaceName, "ScenesCreated", idList);
    }
}

void SceneManager::UpdateSceneInternal(Message& message, bool isSceneElement)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));
    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    bool updated = false;

    size_t numArgs;
    size_t numArgsExpected = isSceneElement ? 2 : 5;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, numArgsExpected)  != LSF_OK) {
        return;
    }

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    LSFString sceneID(uniqueId);
    SceneWithSceneElements sceneWithSceneElements = isSceneElement ? SceneWithSceneElements(args[1]) : SceneWithSceneElements();
    Scene scene = !isSceneElement ? Scene(args[1], args[2], args[3], args[4]) : Scene();

    if (!controllerService.UpdatesAllowed()) {
        controllerService.SendMethodReplyWithResponseCodeAndID(message, LSF_ERR_BUSY, sceneID);
        return;
    }

    if (!isSceneElement && scene.invalidArgs) {
        QCC_LogError(ER_FAIL, ("%s: Invalid Scene arguments", __func__));
        responseCode = LSF_ERR_INVALID_ARGS;
    } else {
        QStatus status = scenesLock.Lock();
        if (ER_OK == status) {
            SceneObjectMap::iterator it = scenes.find(uniqueId);
            if (it != scenes.end()) {
                size_t newlen = blobLength;
                // sub len of old group, add len of new group
                newlen -= it->second->hasElements ? GetString(it->second->sceneName, uniqueId, it->second->sceneWithSceneElements).length() : GetString(it->second->sceneName, uniqueId, it->second->scene).length();
                newlen += isSceneElement ? GetString(it->second->sceneName, uniqueId, sceneWithSceneElements).length() : GetString(it->second->sceneName, uniqueId, scene).length();

                if (newlen < MAX_FILE_LEN) {
                    blobLength = newlen;
                    it->second->scene = scene;
                    it->second->sceneWithSceneElements = sceneWithSceneElements;
                    it->second->hasElements = isSceneElement;
                    responseCode = LSF_OK;
                    updated = true;
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

    controllerService.SendMethodReplyWithResponseCodeAndID(message, responseCode, sceneID);

    if (updated) {
        LSFStringList idList;
        idList.push_back(sceneID);
        controllerService.SendSignal(ControllerServiceSceneInterfaceName, "ScenesUpdated", idList);
    }
}

void SceneManager::DeleteSceneInternal(Message& message, bool isSceneElement)
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
                blobLength -= it->second->hasElements ? GetString(it->second->sceneName, sceneId, it->second->sceneWithSceneElements).length() : GetString(it->second->sceneName, sceneId, it->second->scene).length();
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

void SceneManager::GetSceneInternal(Message& message, bool isSceneElement)
{
    QCC_DbgPrintf(("%s: %s", __func__, message->ToString().c_str()));

    LSFResponseCode responseCode = LSF_ERR_NOT_FOUND;

    MsgArg outArgs[6];

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerService.CheckNumArgsInMessage(numArgs, 1)  != LSF_OK) {
        return;
    }

    const char* uniqueId;
    args[0].Get("s", &uniqueId);

    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        SceneObjectMap::iterator it = scenes.find(uniqueId);
        if (isSceneElement) {
            if (it != scenes.end()) {
                it->second->sceneWithSceneElements.Get(&outArgs[2]);
                responseCode = LSF_OK;
            } else {
                outArgs[2].Set("as", 0, NULL);
            }
        } else {
            if (it != scenes.end()) {
                it->second->scene.Get(&outArgs[2], &outArgs[3], &outArgs[4], &outArgs[5]);
                responseCode = LSF_OK;
            } else {
                outArgs[2].Set("a(asasa{sv}u)", 0, NULL);
                outArgs[3].Set("a(asassu)", 0, NULL);
                outArgs[4].Set("a(asasa{sv}a{sv}uuu)", 0, NULL);
                outArgs[5].Set("a(asasssuuu)", 0, NULL);
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

    outArgs[0].Set("u", responseCode);
    outArgs[1].Set("s", uniqueId);

    controllerService.SendMethodReply(message, outArgs, isSceneElement ? 3 : 6);
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

void SceneManager::ApplySceneInternal(Message& message, bool isSceneElement)
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
    bool invokeSceneElementsApply = false;
    bool invokeEmbeddedStateApply = false;

    uint8_t notfound = 0;

    std::list<SceneWithSceneElements> scenesWithSceneElements;
    std::list<Scene> scenesWithEmbeddedState;

    scenesWithSceneElements.clear();
    scenesWithEmbeddedState.clear();

    QStatus status = scenesLock.Lock();
    if (ER_OK == status) {
        while (sceneList.size()) {
            LSFString sceneId = sceneList.front();
            SceneObjectMap::iterator it = scenes.find(sceneId);
            if (it != scenes.end()) {
                QCC_DbgPrintf(("%s: Found sceneID=%s", __func__, sceneId.c_str()));
                if (it->second->hasElements) {
                    scenesWithSceneElements.push_back(it->second->sceneWithSceneElements);
                } else {
                    scenesWithEmbeddedState.push_back(it->second->scene);
                }
            } else {
                QCC_DbgPrintf(("%s: Scene %s not found", __func__, sceneId.c_str()));
                notfound++;
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
    TransitionLampsLampGroupsToStateList transitionToStateComponent;
    TransitionLampsLampGroupsToPresetList transitionToPresetComponent;
    PulseLampsLampGroupsWithStateList pulseWithStateComponent;
    PulseLampsLampGroupsWithPresetList pulseWithPresetComponent;

    if (scenesWithSceneElements.size() == 0 && scenesWithEmbeddedState.size() == 0) {
        responseCode = LSF_ERR_NOT_FOUND;
    } else {
        if (notfound) {
            responseCode = LSF_ERR_PARTIAL;
        }

        while (scenesWithSceneElements.size()) {
            SceneWithSceneElements sceneWithSceneElements = scenesWithSceneElements.front();
            sceneElementIDs.insert(sceneElementIDs.end(), sceneWithSceneElements.sceneElements.begin(), sceneWithSceneElements.sceneElements.end());
            invokeSceneElementsApply = true;
            scenesWithSceneElements.pop_front();
        }

        while (scenesWithEmbeddedState.size()) {
            Scene scene = scenesWithEmbeddedState.front();
            for (TransitionLampsLampGroupsToStateList::iterator it = scene.transitionToStateComponent.begin(); it != scene.transitionToStateComponent.end();
                    it++) {
                transitionToStateComponent.push_back(*it);
            }
            for (TransitionLampsLampGroupsToPresetList::iterator it = scene.transitionToPresetComponent.begin(); it != scene.transitionToPresetComponent.end();
                    it++) {
                transitionToPresetComponent.push_back(*it);
            }
            for (PulseLampsLampGroupsWithStateList::iterator it = scene.pulseWithStateComponent.begin(); it != scene.pulseWithStateComponent.end(); it++) {
                pulseWithStateComponent.push_back(*it);
            }
            for (PulseLampsLampGroupsWithPresetList::iterator it = scene.pulseWithPresetComponent.begin(); it != scene.pulseWithPresetComponent.end(); it++) {
                pulseWithPresetComponent.push_back(*it);
            }
            invokeEmbeddedStateApply = true;
            scenesWithEmbeddedState.pop_front();
        }

        if (invokeSceneElementsApply) {
            QCC_DbgTrace(("%s: Calling SceneElementManager::ApplySceneElementInternal()", __func__));
            LSFResponseCode tempResponseCode = sceneElementManager->ApplySceneElementInternal(message, sceneElementIDs);
            if (tempResponseCode != LSF_OK) {
                responseCode = LSF_ERR_FAILURE;
            }
        }

        if (invokeEmbeddedStateApply) {
            QCC_DbgTrace(("%s: Calling LampGroupManager::ChangeLampGroupStateAndField()", __func__));
            LSFResponseCode tempResponseCode = lampGroupManager.ChangeLampGroupStateAndField(message, transitionToStateComponent, transitionToPresetComponent, pulseWithStateComponent, pulseWithPresetComponent, false, true, sceneOrMasterSceneId);
            if (tempResponseCode != LSF_OK) {
                responseCode = LSF_ERR_FAILURE;
            }
        }
    }

    return responseCode;
}

void SceneManager::ReadSavedData()
{
    QCC_DbgTrace(("%s", __func__));
    std::istringstream stream;
    if (!ValidateFileAndRead(stream)) {
        /*
         * If there is no file present / CRC check failed on the file create a new
         * file with initialState entry
         */
        scenesLock.Lock();
        ScheduleFileWrite(false, true);
        scenesLock.Unlock();
        return;
    }

    blobLength = stream.str().size();
    ReplaceMap(stream);
}

void SceneManager::ReplaceMap(std::istringstream& stream)
{
    QCC_DbgTrace(("%s", __func__));
    bool firstIteration = true;
    while (!stream.eof()) {
        std::string token;
        std::string id;
        std::string name;
        SceneWithSceneElements sceneWithSceneElements;
        Scene scene;

        token = ParseString(stream);
        if (token == "Scene") {
            token.clear();
            id.clear();
            name.clear();
            sceneWithSceneElements = SceneWithSceneElements();
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
                if (firstIteration) {
                    for (SceneObjectMap::iterator it = scenes.begin(); it != scenes.end(); ++it) {
                        delete it->second;
                    }
                    scenes.clear();
                    firstIteration = false;
                }
                do {
                    token = ParseString(stream);
                    if (token == "SceneElements") {
                        std::string id = ParseString(stream);
                        while (id != "EndSceneElements") {
                            sceneWithSceneElements.sceneElements.push_back(id);
                            id = ParseString(stream);
                        }
                    } else if (token == "TransitionLampsLampGroupsToState") {
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

                SceneObject* newObj = new SceneObject(*this, id, scene, sceneWithSceneElements, !sceneWithSceneElements.sceneElements.empty(), name);
                if (newObj) {
                    scenes.insert(std::make_pair(id, newObj));
                } else {
                    QCC_LogError(ER_FAIL, ("%s: Could not allocate memory for new SceneObject", __func__));
                }
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

std::string SceneManager::GetString(const SceneObjectMap& items)
{
    std::ostringstream stream;
    if (0 == items.size()) {
        if (initialState) {
            QCC_DbgPrintf(("%s: This is the initial state entry", __func__));
            const LSFString& id = initialStateID;
            const LSFString& name = initialStateID;

            stream << "Scene " << id << " \"" << name << "\"\n";
            stream << "EndScene\n";
        } else {
            QCC_DbgPrintf(("%s: File is being reset", __func__));
            const LSFString& id = resetID;
            const LSFString& name = resetID;

            stream << "Scene " << id << " \"" << name << "\"\n";
            stream << "EndScene\n";
        }
    } else {
        for (SceneObjectMap::const_iterator it = items.begin(); it != items.end(); it++) {
            const LSFString& id = it->first;
            const LSFString& name = it->second->sceneName;
            if (it->second->hasElements) {
                const SceneWithSceneElements& sceneWithSceneElements = it->second->sceneWithSceneElements;
                stream << GetString(name, id, sceneWithSceneElements);
            } else {
                const Scene& scene = it->second->scene;
                stream << GetString(name, id, scene);
            }
        }
    }

    return stream.str();
}

bool SceneManager::GetString(std::string& output, uint32_t& checksum, uint64_t& timestamp)
{
    SceneObjectMap mapCopy;
    mapCopy.clear();
    bool ret = false;
    output.clear();

    scenesLock.Lock();
    // we can't hold this lock for the entire time!
    if (updated) {
        mapCopy = scenes;
        updated = false;
        ret = true;
    }
    scenesLock.Unlock();

    if (ret) {
        output = GetString(mapCopy);
        scenesLock.Lock();
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
        ScheduleFileWrite(true);
    }
    scenesLock.Unlock();
}

void SceneManager::ReadWriteFile()
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
            controllerService.SendBlobUpdate(LSF_SCENE, output, checksum, (currentTime - timestamp));
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
            QCC_LogError(ER_FAIL, ("%s: Scene persistent store corrupted", __func__));
        }
    }

    if (status) {
        while (!tempMessageList.empty()) {
            uint64_t currentTime = GetTimestampInMs();
            controllerService.SendGetBlobReply(tempMessageList.front(), LSF_SCENE, output, checksum, (currentTime - timestamp));
            tempMessageList.pop_front();
        }
    }

    if (sendUpdate) {
        sendUpdate = false;
        uint64_t currentTime = GetTimestampInMs();
        controllerService.GetLeaderElectionObj().SendBlobUpdate(LSF_SCENE, output, checksum, (currentTime - timestamp));
    }
}

uint32_t SceneManager::GetControllerServiceSceneInterfaceVersion(void)
{
    QCC_DbgPrintf(("%s: controllerSceneInterfaceVersion=%d", __func__, ControllerServiceSceneInterfaceVersion));
    return ControllerServiceSceneInterfaceVersion;
}
