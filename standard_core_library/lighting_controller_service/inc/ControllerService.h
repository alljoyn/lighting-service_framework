#ifndef _CONTROLLER_SERVICE_H_
#define _CONTROLLER_SERVICE_H_
/**
 * \ingroup ControllerService
 */
/**
 * @file
 * This file provides definitions for ControllerService
 */

/******************************************************************************
 *  *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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

#include <map>
#include <string>

#include <alljoyn/BusAttachment.h>
#include <alljoyn/ProxyBusObject.h>
#include <alljoyn/notification/NotificationSender.h>
#include <alljoyn/config/ConfigService.h>

#include <LSFTypes.h>
#include <Mutex.h>

#ifdef LSF_BINDINGS
#include <lsf/controllerservice/PersistenceThread.h>
#include <lsf/controllerservice/LampManager.h>
#include <lsf/controllerservice/LampGroupManager.h>
#include <lsf/controllerservice/PresetManager.h>
#include <lsf/controllerservice/TransitionEffectManager.h>
#include <lsf/controllerservice/PulseEffectManager.h>
#include <lsf/controllerservice/SceneManager.h>
#include <lsf/controllerservice/SceneElementManager.h>
#include <lsf/controllerservice/MasterSceneManager.h>
#include <lsf/controllerservice/LeaderElectionObject.h>
#include <lsf/controllerservice/LampClients.h>
#include <lsf/controllerservice/ControllerServiceRank.h>
#include <lsf/controllerservice/LSFAboutDataStore.h>
#else
#include <PersistenceThread.h>
#include <LampManager.h>
#include <LampGroupManager.h>
#include <PresetManager.h>
#include <TransitionEffectManager.h>
#include <PulseEffectManager.h>
#include <SceneManager.h>
#include <SceneElementManager.h>
#include <MasterSceneManager.h>
#include <LeaderElectionObject.h>
#include <LampClients.h>
#include <ControllerServiceRank.h>
#include <LSFAboutDataStore.h>
#endif

#include <alljoyn/AboutIconObj.h>
#include <alljoyn/AboutObj.h>
#include <alljoyn/AboutIcon.h>
#include <alljoyn/AboutData.h>
#include "LSFNamespaceSpecifier.h"

namespace lsf {

OPTIONAL_NAMESPACE_CONTROLLER_SERVICE

/*
 * Pre-Declaration of a deprecated 14.06 release specific class
 */
class LSFPropertyStore;

/**
 * This class functions as the message dispatcher. It receives the messages from AllJoyn \n
 * and forwards it to the appropriate manager and receives a reply from a manager and \n
 * passes it on to AllJoyn
 */
class ControllerService : public ajn::BusObject, public ajn::services::ConfigService::Listener {
    friend class ControllerServiceManager;
    friend class LampClients;
    friend class LeaderElectionObject;
  public:

    /**
     * Deprecated Constructor
     * @param factoryConfigFile - path of factory config file
     * @param configFile - path of config file
     * @param lampGroupFile - path of lamp group file
     * @param presetFile - path of preset file
     * @param sceneElementsFile - path of scene elements file
     * @param sceneFile - path of scene file
     * @param masterSceneFile - path of master scene file
     */
    ControllerService(
        const std::string& factoryConfigFile,
        const std::string& configFile,
        const std::string& lampGroupFile,
        const std::string& presetFile,
        const std::string& sceneFile,
        const std::string& masterSceneFile);

    /**
     * Constructor
     * @param factoryConfigFile - path of factory config file
     * @param configFile - path of config file
     * @param lampGroupFile - path of lamp group file
     * @param presetFile - path of preset file
     * @param transitionEffectFile - path of transitionEffect file
     * @param pulseEffectFile - path of pulseEffect file
     * @param sceneElementsFile - path to sceneElements file
     * @param sceneFile - path of scene file
     * @param sceneWithSceneElementsFile - path of scene with scene elements file
     * @param masterSceneFile - path of master scene file
     */
    ControllerService(
        const std::string& factoryConfigFile,
        const std::string& configFile,
        const std::string& lampGroupFile,
        const std::string& presetFile,
        const std::string& transitionEffectFile,
        const std::string& pulseEffectFile,
        const std::string& sceneElementsFile,
        const std::string& sceneFile,
        const std::string& sceneWithSceneElementsFile,
        const std::string& masterSceneFile);

    /**
     * Deprecated Constructor
     * @param propStore - path of property store
     * @param factoryConfigFile - path of factory config file
     * @param configFile - path of config file
     * @param lampGroupFile - path of lamp group file
     * @param presetFile - path of preset file
     * @param sceneElementsFile - path of scene elements file
     * @param sceneFile - path of scene file
     * @param masterSceneFile - path of master scene file
     */
    ControllerService(
        LSFPropertyStore& propStore,
        const std::string& factoryConfigFile,
        const std::string& configFile,
        const std::string& lampGroupFile,
        const std::string& presetFile,
        const std::string& sceneFile,
        const std::string& masterSceneFile);

    /**
     * Constructor
     * @param aboutData - About Data passed in by the application
     * @param factoryConfigFile - path of factory config file
     * @param configFile - path of config file
     * @param lampGroupFile - path of lamp group file
     * @param presetFile - path of preset file
     * @param transitionEffectFile - path of transitionEffect file
     * @param pulseEffectFile - path of pulseEffect file
     * @param sceneElementsFile - path to sceneElements file
     * @param sceneFile - path of scene file
     * @param sceneWithSceneElementsFile - path of scene with scene elements file
     * @param masterSceneFile - path of master scene file
     */
    ControllerService(
        LSFAboutDataStore& aboutData,
        const std::string& factoryConfigFile,
        const std::string& configFile,
        const std::string& lampGroupFile,
        const std::string& presetFile,
        const std::string& transitionEffectFile,
        const std::string& pulseEffectFile,
        const std::string& sceneElementsFile,
        const std::string& sceneFile,
        const std::string& sceneWithSceneElementsFile,
        const std::string& masterSceneFile);

    /**
     * Destructor
     */
    ~ControllerService();

    /**
     * Starts the ControllerService
     *
     * @param  keyStoreFileLocation Absolute path of the location to put the AllJoyn keystore file in. If this is not specified, the
     *                              default location will be used. Android applications running the Controller Service should pass in the
     *                              location returned by Context.getFileStreamPath("alljoyn_keystore").getAbsolutePath()
     * @return ER_OK if successful, error otherwise
     */
    QStatus Start(const char* keyStoreFileLocation);
    /**
     * Register Method Handlers
     */
    QStatus RegisterMethodHandlers(void);

    /**
     * Stops the ControllerService
     *
     * @return ER_OK if successful, error otherwise
     */
    QStatus Stop(void);
    /**
     * join thread
     */
    QStatus Join(void);

    /**
     * Returns a reference to the AllJoyn BusAttachment
     *
     * @return reference to the AllJoyn BusAttachment
     */
    ajn::BusAttachment& GetBusAttachment(void) { return bus; }
    /**
     * Get reference to Lamp Manager object
     * @return LampManager
     */
    LampManager& GetLampManager(void) { return lampManager; };
    /**
     * Get reference to Lamp Group Manager object
     * @return LampGroupManager
     */
    LampGroupManager& GetLampGroupManager(void) { return lampGroupManager; };
    /**
     * Get reference to Preset Manager object
     * @return PresetManager
     */
    PresetManager& GetPresetManager(void) { return presetManager; };
    /**
     * Get reference to Scene Manager object
     * @return SceneManager
     */
    SceneManager& GetSceneManager(void) { return sceneManager; };
    /**
     * Get reference to Scene Element Manager object
     * @return SceneElementManager
     */
    SceneElementManager& GetSceneElementManager(void) { return sceneElementManager; };
    /**
     * Get reference to Master Scene Manager
     * @return MasterSceneManager
     */
    MasterSceneManager& GetMasterSceneManager(void) { return masterSceneManager; };
    /**
     * Get reference to Transition Effect Manager object
     * @return TransitionEffectManager
     */
    TransitionEffectManager& GetTransitionEffectManager(void) { return transitionEffectManager; };
    /**
     * Get reference to Pulse Effect Manager object
     * @return PulseEffectManager
     */
    PulseEffectManager& GetPulseEffectManager(void) { return pulseEffectManager; };
    /**
     * Send Method Reply \n
     * Reply for asynchronous method call \n
     * @param msg      The method call message
     * @param args     The reply arguments (can be NULL)
     * @param numArgs  The number of arguments
     * @return
     *      - ER_OK if successful
     *      - ER_BUS_OBJECT_NOT_REGISTERED if bus object has not yet been registered
     *      - An error status otherwise
     */
    void SendMethodReply(const ajn::Message& msg, const ajn::MsgArg* args = NULL, size_t numArgs = 0);
    /**
     * Send Method Reply With Response Code And List Of IDs \n
     * Reply for asynchronous method call that needs LSFResponseCode and string of IDs of some list \n
     * @param msg      The method call message
     * @param responseCode type LSFResponseCode
     * @param idList - string of IDs
     */
    void SendMethodReplyWithResponseCodeAndListOfIDs(const ajn::Message& msg, LSFResponseCode responseCode, const LSFStringList& idList);
    /**
     * Send Method Reply With Response Code ID And Name \n
     * Reply for asynchronous method call that needs LSFResponseCode and ID and name \n
     * @param msg      The method call message
     * @param responseCode type LSFResponseCode
     * @param lsfId - id as a string
     * @param lsfName - name
     */
    void SendMethodReplyWithResponseCodeIDAndName(const ajn::Message& msg, LSFResponseCode responseCode, const LSFString& lsfId, const LSFString& lsfName);
    /**
     * Send Method Reply With Response Code And ID \n
     * Reply for asynchronous method call that needs LSFResponseCode and ID \n
     * @param msg      The method call message
     * @param responseCode type LSFResponseCode
     * @param lsfId - id as a string
     */
    void SendMethodReplyWithResponseCodeAndID(const ajn::Message& msg, LSFResponseCode responseCode, const LSFString& lsfId);
    /**
     * Send Method Reply With Uint32 Value \n
     * Reply for asynchronous method call that needs uint32_t \n
     * @param msg      The method call message
     * @param value    The uint32_t value need to be sent
     */
    void SendMethodReplyWithUint32Value(const ajn::Message& msg, const uint32_t value);
    /**
     * Send Method Reply With Response Code ID Language And Name \n
     * Reply for asynchronous method call that needs LSFResponseCode, ID, language and name \n
     * @param msg      The method call message
     * @param responseCode type LSFResponseCode
     * @param lsfId - id as a string
     * @param language of the name to be sent
     * @param name to send
     */
    void SendMethodReplyWithResponseCodeIDLanguageAndName(const ajn::Message& msg, LSFResponseCode responseCode, const LSFString& lsfId, const LSFString& language, const LSFString& name);
    /**
     * Send Signal with list of IDs
     * @param ifaceName - interface that the signal is located
     * @param methodName - signal method name
     * @param idList - The list of IDs needed to be sent
     * @return QStatus
     */
    QStatus SendSignal(const char* ifaceName, const char* signalName, const LSFStringList& idList);

    /**
     * Send Name Changed signal
     * @param ifaceName  - interface that the signal is located
     * @param methodName - signal method name
     * @param lampId     - The Lamp ID
     * @param lampName   - The Lamp Name
     * @return QStatus
     */
    QStatus SendNameChangedSignal(const char* ifaceName, const char* signalName, const LSFString& lampID, const LSFString& lampName);

    /**
     * Send State Changed signal
     * @param ifaceName  - interface that the signal is located
     * @param methodName - signal method name
     * @param lampId     - The Lamp ID
     * @param lampState  - The Lamp State
     * @return QStatus
     */
    QStatus SendStateChangedSignal(const char* ifaceName, const char* signalName, const LSFString& lampID, const LampState& lampState);

    /**
     * Send Signal Without Arg - just an empty signal
     * @param ifaceName - interface that the signal is located
     * @param signalName - signal method name
     * @return QStatus
     */
    QStatus SendSignalWithoutArg(const char* ifaceName, const char* signalName);
    /**
     * Send Scene Or Master Scene Applied Signal \n
     * Sends signal for event - ScenesApplied signal or MasterScenesApplied signal \n
     * @param sceneorMasterSceneId - Scene, MasterScene
     */
    void SendSceneOrMasterSceneAppliedSignal(LSFString& sceneorMasterSceneId) {
        sceneManager.SendSceneOrMasterSceneAppliedSignal(sceneorMasterSceneId);
    }
    /**
     * Schedule File Read Write \n
     * a trigger to synchronize all lighting service meta data with persistent storage.\n
     * Meta data includes lamp groups, scenes, master scenes, presets, transition effects and pulse effects.
     * @param manager - parameter not in use
     */
    void ScheduleFileReadWrite(Manager* manager);
    /**
     * Send Blob Update \n
     * Updating the leader controller service about the current controller service meta data \n
     * @param type - which kind of meta data is this
     * @param blob - the information to update
     * @param checksum
     * @param timestamp
     */
    QStatus SendBlobUpdate(LSFBlobType type, std::string blob, uint32_t checksum, uint64_t timestamp);
    /**
     * Send Get Blob Reply \n
     * Replay to Get blob request \n
     * @param message - the request message
     * @param type - the type of the requested blob
     * @param blob - the requested meta data information
     * @param checksum
     * @param timestamp
     */
    void SendGetBlobReply(ajn::Message& message, LSFBlobType type, std::string blob, uint32_t checksum, uint64_t timestamp);
    /**
     * Is Running
     */
    bool IsRunning();

    /**
     * Is Leader
     */
    bool IsLeader();
    /**
     * Set Is Leader
     */
    void SetIsLeader(bool val);
    /**
     * Set Allow Updates
     */
    void SetAllowUpdates(bool allow);
    /**
     * Updates Allowed
     */
    bool UpdatesAllowed(void);
    /**
     * Do Leave Session Async
     */
    void DoLeaveSessionAsync(ajn::SessionId sessionId);
    /**
     * Leave Session Async Reply Handler
     */
    void LeaveSessionAsyncReplyHandler(ajn::Message& message, void* context);

    /**
     * Get Leader Election Obj
     */
    LeaderElectionObject& GetLeaderElectionObj(void) {
        return elector;
    }

    /**
     * Check if the receivedNumArgs is same as expectedNumArgs and return a response
     * code accordingly
     */
    LSFResponseCode CheckNumArgsInMessage(uint32_t receivedNumArgs, uint32_t expectedNumArgs);

    /**
     * Send out a Controller Service announcement
     */
    QStatus Announce(void);

    /**
     * Return the aboutDataStore instance
     */
    LSFAboutDataStore& GetAboutDataStore(void) { return aboutDataStore; };

    /**
     * Developer API to Lighting Reset the Controller Service.
     * This function will reset all the persistent store
     * files - so all the lighting data will be lost once
     * this API is invoked
     *
     * Please note that this is a blocking function call and
     * the Controller Service will NOT guarantee any coherence
     * when this API is used by apps that incorporate
     * the Controller Service and these apps would need to manage
     * the coherence of data.
     *
     * The operation of the LSF system with this API has not been
     * tested and any undesirable effects that may arise due to the
     * use of this API are unknown.
     *
     * @param  None
     * @return LSFResponseCode indicating the status of the operation
     *         LSF_OK - If reset was successful
     *         LSF_ERR_FAILURE - If reset failed
     *         LSF_ERR_PARTIAL - If the reset was partially successful
     */
    LSFResponseCode LightingResetAPI(void);

    /**
     * Developer API to Factory Reset the Controller Service.
     * This function will factory reset the onboarding daemon
     * and reset the Controller Service About Data to factory
     * settings. It also stops the Controller Service. The
     * Controller Service needs to be restarted after invoking
     * this API to resume normal operation if the controller
     * service has been started in the foreground. If it has
     * been started in the default background mode, the monitor
     * task will restart the Controller Service automatically.
     *
     * Please note that this is a blocking function call and
     * the Controller Service will NOT guarantee any coherence
     * when this API is used by apps that incorporate
     * the Controller Service and these apps would need to manage
     * the coherence of data.
     *
     * The operation of the LSF system with this API has not been
     * tested and any undesirable effects that may arise due to the
     * use of this API are unknown.
     *
     * @param  None
     * @return LSFResponseCode indicating the status of the operation
     *         LSF_OK - If reset was successful
     *         LSF_ERR_FAILURE - If reset failed
     */
    LSFResponseCode FactoryResetAPI(void);
  private:

    void Initialize();

    uint32_t GetControllerServiceInterfaceVersion(void);

    /**
     * Handles the GetPropery request
     * @param ifcName  interface name
     * @param propName the name of the propery
     * @param val reference of MsgArg out parameter.
     * @return status - success/failure
     */
    QStatus Get(const char* ifcName, const char* propName, ajn::MsgArg& val);

    virtual void ObjectRegistered(void);

    typedef struct {
        std::string interfaceDescription;          /**< Interface Description */
        const char* interfaceName;                 /**< Interface Name */
    } InterfaceEntry;

    QStatus CreateAndAddInterface(std::string interfaceDescription, const char* interfaceName);

    QStatus CreateAndAddInterfaces(const InterfaceEntry* entries, size_t numEntries);

    Mutex updatesAllowedLock;
    bool updatesAllowed;
    ajn::BusAttachment bus;
    LeaderElectionObject elector;
    lsf::Mutex serviceSessionMutex;
    ajn::SessionId serviceSession;
    qcc::String multipointjoiner;

    class ControllerListener;
    ControllerListener* listener;

    class OBSJoiner;

    LampManager lampManager;
    LampGroupManager lampGroupManager;
    PresetManager presetManager;
    SceneManager sceneManager;
    SceneElementManager sceneElementManager;
    MasterSceneManager masterSceneManager;
    TransitionEffectManager transitionEffectManager;
    PulseEffectManager pulseEffectManager;

    void OnAccepMultipointSessionJoiner(const char* joiner);
    void SessionLost(ajn::SessionId sessionId);
    void SessionJoined(ajn::SessionId sessionId, const char* joiner);
    void LeaveSession(void);

    void FoundLocalOnboardingService(const char* busName, ajn::SessionPort port);

    LSFAboutDataStore internalAboutDataStore;
    LSFAboutDataStore& aboutDataStore;
    AboutIcon aboutIcon;
    AboutIconObj aboutIconObj;
    AboutObj aboutObj;
    ajn::services::ConfigService configService;
    ajn::services::NotificationSender* notificationSender;

    ajn::ProxyBusObject* obsObject;
    bool isObsObjectReady;
    Mutex obsObjectLock;

    volatile sig_atomic_t isRunning;

    // Interface for ajn::services::ConfigService::Listener
    QStatus Restart();
    QStatus FactoryReset();
    QStatus SetPassphrase(const char* daemonRealm, size_t passcodeSize, const char* passcode, ajn::SessionId sessionId);

    void MethodCallDispatcher(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    /*
     * Lighting Reset the Controller Service
     */
    void LightingResetControllerService(ajn::Message& msg);

    void GetControllerServiceVersion(ajn::Message& msg);

    /*
     * This function is not thread safe. it should not be called without locking messageHandlersLock
     */
    template <typename OBJ>
    void AddMethodHandler(const std::string& methodName, OBJ* obj, void (OBJ::* methodCall)(ajn::Message &))
    {
        MethodHandlerBase* handler = new MethodHandler<OBJ>(obj, methodCall);
        std::pair<DispatcherMap::iterator, bool> ins = messageHandlers.insert(std::make_pair(methodName, handler));
        if (ins.second == false) {
            // if this was already there, overwrite and delete the old handler
            delete ins.first->second;
            ins.first->second = handler;
        }
    }

    class MethodHandlerBase {
      public:
        virtual ~MethodHandlerBase() { }
        virtual void Handle(ajn::Message& msg) = 0;
    };

    template <typename OBJ>
    class MethodHandler : public MethodHandlerBase {
        typedef void (OBJ::* HandlerFunction)(ajn::Message&);

      public:
        MethodHandler(OBJ* obj, HandlerFunction handleFunc) :
            object(obj), handler(handleFunc) { }

        virtual ~MethodHandler() { }

        virtual void Handle(ajn::Message& msg) {
            (object->*(handler))(msg);
        }

        OBJ* object;
        HandlerFunction handler;
    };

    typedef std::map<std::string, MethodHandlerBase*> DispatcherMap;
    DispatcherMap messageHandlers;
    Mutex messageHandlersLock;


    PersistenceThread fileWriterThread;

    ControllerServiceRank rank;

    bool deprecatedConstructorUsed;
};
/**
 * controller service management class. \n
 * This is the class to create from the outside application that run the controller service.
 */
class ControllerServiceManager {
  public:
    /**
     * Deprecated ControllerServiceManager constructor
     */
    ControllerServiceManager(
        const std::string& factoryConfigFile,
        const std::string& configFile,
        const std::string& lampGroupFile,
        const std::string& presetFile,
        const std::string& sceneFile,
        const std::string& masterSceneFile) :
        controllerService(factoryConfigFile, configFile, lampGroupFile, presetFile, sceneFile, masterSceneFile) {
    }

    /**
     * ControllerServiceManager constructor
     */
    ControllerServiceManager(
        const std::string& factoryConfigFile,
        const std::string& configFile,
        const std::string& lampGroupFile,
        const std::string& presetFile,
        const std::string& transitionEffectFile,
        const std::string& pulseEffectFile,
        const std::string& sceneElementsFile,
        const std::string& sceneFile,
        const std::string& sceneWithSceneElementsFile,
        const std::string& masterSceneFile) :
        controllerService(factoryConfigFile, configFile, lampGroupFile, presetFile, transitionEffectFile, pulseEffectFile, sceneElementsFile, sceneFile, sceneWithSceneElementsFile, masterSceneFile) {

    }

    /**
     * Deprecated Constructor
     * @param propStore - path of property store
     * @param factoryConfigFile - path of factory config file
     * @param configFile - path of config file
     * @param lampGroupFile - path of lamp group file
     * @param presetFile - path of preset file
     * @param sceneFile - path of scene file
     * @param masterSceneFile - path of master scene file
     */
    ControllerServiceManager(
        LSFPropertyStore& propStore,
        const std::string& factoryConfigFile,
        const std::string& configFile,
        const std::string& lampGroupFile,
        const std::string& presetFile,
        const std::string& sceneFile,
        const std::string& masterSceneFile) :
        controllerService(propStore, factoryConfigFile, configFile, lampGroupFile, presetFile, sceneFile, masterSceneFile) {
    }

    /**
     * Constructor
     * @param aboutData - About Data passed in by the application
     * @param factoryConfigFile - path of factory config file
     * @param configFile - path of config file
     * @param lampGroupFile - path of lamp group file
     * @param presetFile - path of preset file
     * @param transitionEffectFile - path of transitionEffect file
     * @param pulseEffectFile - path of pulseEffect file
     * @param sceneElementsFile - path to sceneElements file
     * @param sceneFile - path of scene file
     * @param sceneWithSceneElementsFile - path of scene with scene elements file
     * @param masterSceneFile - path of master scene file
     */
    ControllerServiceManager(
        LSFAboutDataStore& aboutData,
        const std::string& factoryConfigFile,
        const std::string& configFile,
        const std::string& lampGroupFile,
        const std::string& presetFile,
        const std::string& transitionEffectFile,
        const std::string& pulseEffectFile,
        const std::string& sceneElementsFile,
        const std::string& sceneFile,
        const std::string& sceneWithSceneElementsFile,
        const std::string& masterSceneFile) :
        controllerService(aboutData, factoryConfigFile, configFile, lampGroupFile, presetFile, transitionEffectFile, pulseEffectFile, sceneElementsFile, sceneFile, sceneWithSceneElementsFile, masterSceneFile) {

    }

    /**
     * ControllerServiceManager destructor
     */
    ~ControllerServiceManager() {
    }
    /**
     * Starts the ControllerService
     *
     * @param  keyStoreFileLocation Absolute path of the location to put the AllJoyn keystore file in. If this is not specified, the
     *                              default location will be used. Android applications running the Controller Service should pass in the
     *                              location returned by Context.getFileStreamPath("alljoyn_keystore").getAbsolutePath()
     * @return ER_OK if successful, error otherwise
     */
    QStatus Start(const char* keyStoreFileLocation) {
        return controllerService.Start(keyStoreFileLocation);
    }

    /**
     * Stops the ControllerService
     *
     * @return ER_OK if successful, error otherwise
     */
    QStatus Stop(void) {
        return controllerService.Stop();
    }
    /**
     * join thread
     */
    QStatus Join(void) {
        return controllerService.Join();
    }
    /**
     * is running
     */
    bool IsRunning() {
        return controllerService.IsRunning();
    }
    /**
     * Get a pointer to Controller Service
     */
    ControllerService* GetControllerServicePtr(void) { return &controllerService; };

  private:
    ControllerService controllerService;
};

OPTIONAL_NAMESPACE_CLOSE

} //lsf

#endif