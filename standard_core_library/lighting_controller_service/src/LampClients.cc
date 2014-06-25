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

#include <LampClients.h>
#include <alljoyn/Status.h>
#include <alljoyn/AllJoynStd.h>
#include <qcc/Debug.h>
#include <algorithm>
#include <ControllerService.h>
#include <OEM_CS_Config.h>

using namespace lsf;
using namespace ajn;

#define QCC_MODULE "LAMP_CLIENTS"

class LampClients::ServiceHandler : public services::AnnounceHandler {
  public:
    ServiceHandler(LampClients& mgr) : manager(mgr) { }

    virtual void Announce(uint16_t version, uint16_t port, const char* busName, const ObjectDescriptions& objectDescs, const AboutData& aboutData);

    LampClients& manager;
};

void LampClients::ServiceHandler::Announce(
    uint16_t version,
    uint16_t port,
    const char* busName,
    const ObjectDescriptions& objectDescs,
    const AboutData& aboutData)
{
    LSFString lampID;
    LSFString lampName;

    ObjectDescriptions::const_iterator oit = objectDescs.find(LampServiceObjectPath);
    if (oit != objectDescs.end()) {
        AboutData::const_iterator ait = aboutData.find("DeviceId");
        if (ait != aboutData.end()) {
            const char* uniqueId;
            ait->second.Get("s", &uniqueId);
            lampID = uniqueId;
        }

        ait = aboutData.find("DeviceName");
        if (ait != aboutData.end()) {
            const char* name;
            ait->second.Get("s", &name);
            lampName = name;
        }

        QCC_DbgPrintf(("%s: About Data Dump", __FUNCTION__));
        for (ait = aboutData.begin(); ait != aboutData.end(); ait++) {
            QCC_DbgPrintf(("%s: %s", ait->first.c_str(), ait->second.ToString().c_str()));
        }
    }

    if (!lampID.empty()) {
        manager.HandleAboutAnnounce(lampID, lampName, port, busName);
    }
}

void LampClients::RequestAllLampIDs(Message& message)
{
    QStatus status = getAllLampIDsLock.Lock();
    if (ER_OK != status) {
        QCC_LogError(ER_FAIL, ("%s: Failed to lock mutex", __FUNCTION__));
        LSFStringList idList;
        LSFResponseCode responseCode = LSF_ERR_FAILURE;
        controllerService.SendMethodReplyWithResponseCodeAndListOfIDs(message, responseCode, idList);
    } else {
        getAllLampIDsRequests.push_back(message);

        status = getAllLampIDsLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(ER_FAIL, ("%s: Failed to unlock mutex", __FUNCTION__));
        }
        wakeUp.Post();
    }
}

void LampClients::GetAllLamps(LampNameMap& lamps)
{
    QCC_LogError(ER_OK, ("%s: PLEASE NOTE THIS FUNCTION IS NOT THREAD SAFE", __FUNCTION__));
    lamps.clear();
    LampMap tempMap = activeLamps;
    for (LampMap::const_iterator it = tempMap.begin(); it != tempMap.end(); ++it) {
        lamps.insert(std::make_pair(it->first, it->second->name));
    }
}

LampClients::LampClients(ControllerService& controllerSvc)
    : Manager(controllerSvc),
    serviceHandler(new ServiceHandler(*this)),
    isRunning(false),
    lampStateChangedSignalHandlerRegistered(false),
    sentNGNSPings(0),
    sentSessionPings(0),
    receivedNGNSPingResponses(0),
    receivedSessionPingResponses(0),
    pingsInProgress(false)
{
    keyListener.SetPassCode(INITIAL_PASSCODE);
    methodQueue.clear();
    aboutsList.clear();
    activeLamps.clear();
    joinSessionCBList.clear();
    lostSessionList.clear();
    getAllLampIDsRequests.clear();
    probableLostLamps.clear();
    lostLamps.clear();
    joinSessionInProgressList.clear();
}

LampClients::~LampClients()
{
    delete serviceHandler;

    while (methodQueue.size()) {
        QueuedMethodCall* queuedCall = methodQueue.front();
        delete queuedCall;
        methodQueue.pop_front();
    }

    aboutsListLock.Lock();
    aboutsList.clear();
    aboutsListLock.Unlock();

    joinSessionCBListLock.Lock();
    joinSessionCBList.clear();
    joinSessionCBListLock.Unlock();

    lostSessionListLock.Lock();
    lostSessionList.clear();
    lostSessionListLock.Unlock();
}

static const char* interfaces[] =
{
    LampServiceInterfaceName,
    LampServiceStateInterfaceName,
    LampServiceParametersInterfaceName,
    LampServiceDetailsInterfaceName,
    ConfigServiceInterfaceName,
    AboutInterfaceName
};

QStatus LampClients::Start(const char* keyStoreFileLocation)
{
    QCC_DbgPrintf(("%s", __FUNCTION__));
    // to receive About data
    QStatus status = services::AnnouncementRegistrar::RegisterAnnounceHandler(controllerService.GetBusAttachment(), *serviceHandler, interfaces, sizeof(interfaces) / sizeof(interfaces[0]));
    QCC_DbgPrintf(("RegisterAnnounceHandler(): %s\n", QCC_StatusText(status)));
    if (ER_OK == status) {
        QCC_DbgPrintf(("%s: KeyStore location = %s", __FUNCTION__, keyStoreFileLocation));
        status = controllerService.GetBusAttachment().EnablePeerSecurity("ALLJOYN_PIN_KEYX ALLJOYN_ECDHE_PSK", &keyListener, keyStoreFileLocation);
        QCC_DbgPrintf(("EnablePeerSecurity(): %s\n", QCC_StatusText(status)));

        if (ER_OK == status) {
            isRunning = true;
            status = Thread::Start();
            QCC_DbgPrintf(("%s: Thread::Start(): %s\n", __FUNCTION__, QCC_StatusText(status)));
        }
    }

    return status;
}

QStatus LampClients::Join(void)
{
    QCC_DbgPrintf(("%s", __FUNCTION__));

    Stop();
    wakeUp.Post();
    Thread::Join();

    services::AnnouncementRegistrar::UnRegisterAnnounceHandler(controllerService.GetBusAttachment(), *serviceHandler, interfaces, sizeof(interfaces) / sizeof(interfaces[0]));

    for (LampMap::iterator it = activeLamps.begin(); it != activeLamps.end(); ++it) {
        LampConnection* conn = it->second;
        controllerService.GetBusAttachment().LeaveSession(conn->object.GetSessionId());
        delete conn;
    }
    activeLamps.clear();

    return ER_OK;
}

void LampClients::Stop(void)
{
    QCC_DbgPrintf(("%s", __FUNCTION__));
    isRunning = false;
}

void LampClients::HandleAboutAnnounce(const LSFString lampID, const LSFString& lampName, uint16_t port, const char* busName)
{
    QCC_DbgPrintf(("LampClients::HandleAboutAnnounce(%s,%s,%u,%s)\n", lampID.c_str(), lampName.c_str(), port, busName));
    LampConnection* connection = new LampConnection();
    if (connection) {
        connection->lampId = lampID;
        connection->busName = busName;
        connection->name = lampName;
        connection->port = port;
    }
    QStatus status = aboutsListLock.Lock();
    if (ER_OK != status) {
        QCC_LogError(status, ("%s: aboutsListLock.Lock() failed", __FUNCTION__));
        return;
    }
    aboutsList.push_back(connection);
    status = aboutsListLock.Unlock();
    if (ER_OK != status) {
        QCC_LogError(status, ("%s: aboutsListLock.Unlock() failed", __FUNCTION__));
    }

    wakeUp.Post();
}

void LampClients::JoinSessionCB(QStatus status, SessionId sessionId, const SessionOpts& opts, void* context)
{
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    LampConnection* connection = static_cast<LampConnection*>(context);

    QCC_DbgPrintf(("Controller::JoinSessionCB(%s, %u)\n", QCC_StatusText(status), sessionId));

    if (status != ER_OK) {
        QCC_LogError(status, ("%s: Join Session failed for lamp with ID = %s", __FUNCTION__, connection->lampId.c_str()));
        delete connection;
        return;
    }

    QCC_DbgPrintf(("New connection to lamp ID [%s]\n", connection->lampId.c_str()));

    connection->sessionID = sessionId;
    connection->object = ProxyBusObject(controllerService.GetBusAttachment(), connection->busName.c_str(), LampServiceObjectPath, sessionId);
    connection->configObject = ProxyBusObject(controllerService.GetBusAttachment(), connection->busName.c_str(), ConfigServiceObjectPath, sessionId);
    connection->aboutObject = ProxyBusObject(controllerService.GetBusAttachment(), connection->busName.c_str(), AboutObjectPath, sessionId);

    QCC_DbgPrintf(("%s: Invoking IntrospectRemoteObjectAsync\n", __FUNCTION__));
    QStatus tempStatus = connection->object.IntrospectRemoteObjectAsync(this, static_cast<ProxyBusObject::Listener::IntrospectCB>(&LampClients::IntrospectCB), context);
    QCC_DbgPrintf(("%s: IntrospectRemoteObjectAsync returns %s\n", __FUNCTION__, QCC_StatusText(tempStatus)));
    if (ER_OK != tempStatus) {
        /*
         * Tear down the session if the object setup was unsuccessful
         */
        tempStatus = controllerService.GetBusAttachment().LeaveSession(sessionId);
        QCC_DbgPrintf(("%s: controllerService.GetBusAttachment().LeaveSession returns %s\n", __FUNCTION__, QCC_StatusText(tempStatus)));
        delete connection;
    }
}

void LampClients::SessionLost(SessionId sessionId, SessionLostReason reason)
{
    QCC_DbgPrintf(("%s: sessionId=0x%x reason=0x%x\n", __FUNCTION__, sessionId, reason));
    QStatus status = lostSessionListLock.Lock();
    if (ER_OK != status) {
        QCC_LogError(status, ("%s: lostSessionListLock.Lock() failed", __FUNCTION__));
        return;
    }
    lostSessionList.insert((uint32_t)sessionId);
    status = lostSessionListLock.Unlock();
    if (ER_OK != status) {
        QCC_LogError(status, ("%s: lostSessionListLock.Unlock() failed", __FUNCTION__));
    }
    wakeUp.Post();
}

void LampClients::SendMethodReply(LSFResponseCode responseCode, ajn::Message msg, std::list<ajn::MsgArg>& stdArgs, std::list<ajn::MsgArg>& custArgs)
{
    QCC_DbgPrintf(("%s\n", __FUNCTION__));
    size_t numArgs = stdArgs.size() + custArgs.size() + 1;
    QCC_DbgPrintf(("%s: NumArgs = %d", __FUNCTION__, numArgs));
    MsgArg* args = new MsgArg[numArgs];
    if (args) {
        args[0] = MsgArg("u", responseCode);
        uint8_t index = 1;
        while (stdArgs.size()) {
            args[index] = stdArgs.front();
            stdArgs.pop_front();
            index++;
        }
        while (custArgs.size()) {
            args[index] = custArgs.front();
            custArgs.pop_front();
            index++;
        }
        controllerService.SendMethodReply(msg, args, numArgs);
    } else {
        QCC_LogError(ER_OUT_OF_MEMORY, ("%s: Failed to allocate enough memory", __FUNCTION__));
    }
}

void LampClients::QueueLampMethod(QueuedMethodCall* queuedCall)
{
    QCC_DbgPrintf(("%s", __FUNCTION__));
    LSFResponseCode responseCode = LSF_OK;

    QStatus status = queueLock.Lock();
    if (status != ER_OK) {
        QCC_LogError(status, ("%s: queueLock.Lock() failed", __FUNCTION__));
        responseCode = LSF_ERR_BUSY;
    } else {
        if (methodQueue.size() < MAX_LAMP_CLIENTS_METHOD_QUEUE_SIZE) {
            methodQueue.push_back(queuedCall);
        } else {
            responseCode = LSF_ERR_NO_SLOT;
            QCC_LogError(ER_OUT_OF_MEMORY, ("%s: No slot for new method call", __FUNCTION__));
        }

        status = queueLock.Unlock();
        if (status != ER_OK) {
            QCC_LogError(status, ("%s: queueLock.Unlock() failed", __FUNCTION__));
        }
    }

    if (LSF_OK == responseCode) {
        QCC_DbgPrintf(("%s: Adding response counter with ID=%s to response map", __FUNCTION__, queuedCall->responseID.c_str()));
        responseLock.Lock();
        responseMap.insert(std::make_pair(queuedCall->responseID, queuedCall->responseCounter));
        responseLock.Unlock();
        wakeUp.Post();
    } else {
        SendMethodReply(responseCode, queuedCall->inMsg, queuedCall->responseCounter.standardReplyArgs, queuedCall->responseCounter.customReplyArgs);
        delete queuedCall;
    }
}

LSFResponseCode LampClients::DoMethodCallAsync(QueuedMethodCall* queuedCall)
{
    QCC_DbgPrintf(("%s", __FUNCTION__));
    LSFResponseCode responseCode = LSF_OK;
    QStatus status = ER_OK;
    uint32_t notFound = 0;
    QueuedMethodCallElementList elementList = queuedCall->methodCallElements;

    while (elementList.size()) {
        QueuedMethodCallElement element = elementList.front();
        LSFStringList lamps = element.lamps;

        for (LSFStringList::const_iterator it = lamps.begin(); it != lamps.end(); it++) {
            QCC_DbgPrintf(("%s: Processing for LampID=%s", __FUNCTION__, (*it).c_str()));
            LampMap::iterator lit = activeLamps.find(*it);
            if (lit != activeLamps.end()) {
                QCC_DbgPrintf(("%s: Found Lamp", __FUNCTION__));
                if (0 == strcmp(element.interface.c_str(), ConfigServiceInterfaceName)) {
                    QCC_DbgPrintf(("%s: Config Call", __FUNCTION__));
                    status = lit->second->configObject.MethodCallAsync(
                        element.interface.c_str(),
                        element.method.c_str(),
                        this,
                        queuedCall->replyFunc,
                        &element.args[0],
                        element.args.size(),
                        queuedCall
                        );
                } else if (0 == strcmp(element.interface.c_str(), AboutInterfaceName)) {
                    QCC_DbgPrintf(("%s: About Call", __FUNCTION__));
                    status = lit->second->aboutObject.MethodCallAsync(
                        element.interface.c_str(),
                        element.method.c_str(),
                        this,
                        queuedCall->replyFunc,
                        &element.args[0],
                        element.args.size(),
                        queuedCall
                        );
                } else {
                    QCC_DbgPrintf(("%s: LampService Call", __FUNCTION__));
                    status = lit->second->object.MethodCallAsync(
                        element.interface.c_str(),
                        element.method.c_str(),
                        this,
                        queuedCall->replyFunc,
                        &element.args[0],
                        element.args.size(),
                        queuedCall
                        );
                }

                if (status != ER_OK) {
                    QCC_LogError(status, ("%s: MethodCallAsync failed", __FUNCTION__));
                }
            } else {
                notFound++;
            }
        }

        elementList.pop_front();
    }

    if (notFound) {
        DecrementWaitingAndSendResponse(queuedCall, 0, 0, notFound);
    }

    return responseCode;
}

void LampClients::GetLampState(const LSFString& lampID, Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "GetAll");
    element.args.push_back(MsgArg("s", LampServiceStateInterfaceName));
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("a{sv}", 0, NULL));
    QueueLampMethod(queuedCall);
}

void LampClients::GetLampStateField(const LSFString& lampID, const LSFString& field, Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "Get");
    element.args.push_back(MsgArg("s", LampServiceStateInterfaceName));
    element.args.push_back(MsgArg("s", field.c_str()));
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", field.c_str()));
    MsgArg arg("u", 0);
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("v", &arg));
    QueueLampMethod(queuedCall);
}

void LampClients::GetLampDetails(const LSFString& lampID, Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "GetAll");
    element.args.push_back(MsgArg("s", LampServiceDetailsInterfaceName));
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("a{sv}", 0, NULL));
    QueueLampMethod(queuedCall);
}

void LampClients::GetLampParameters(const LSFString& lampID, Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "GetAll");
    element.args.push_back(MsgArg("s", LampServiceParametersInterfaceName));
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("a{sv}", 0, NULL));
    QueueLampMethod(queuedCall);
}

void LampClients::GetLampParametersField(const LSFString& lampID, const LSFString& field, Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "Get");
    element.args.push_back(MsgArg("s", LampServiceParametersInterfaceName));
    element.args.push_back(MsgArg("s", field.c_str()));
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", field.c_str()));
    MsgArg arg("u", 0);
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("v", &arg));
    QueueLampMethod(queuedCall);
}

void LampClients::HandleGetReply(ajn::Message& message, void* context)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __FUNCTION__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();
    QueuedMethodCall* queuedCall = static_cast<QueuedMethodCall*>(context);

    if (MESSAGE_METHOD_RET == message->GetType()) {
        size_t numArgs;
        const MsgArg* args;
        message->GetArgs(numArgs, args);

        if (numArgs == 0) {
            DecrementWaitingAndSendResponse(queuedCall, 1, 0, 0);
        } else {
            DecrementWaitingAndSendResponse(queuedCall, 1, 0, 0, args);
        }
    } else {
        DecrementWaitingAndSendResponse(queuedCall, 0, 1, 0);
    }
}

void LampClients::ChangeLampState(const ajn::Message& inMsg, bool groupOperation, bool sceneOperation, TransitionStateParamsList& transitionStateParams,
                                  TransitionStateFieldParamsList& transitionStateFieldparams, PulseStateParamsList& pulseParams)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithLampResponseCode));

    if (groupOperation || sceneOperation) {
        size_t numArgs;
        const MsgArg* args;
        Message tempMsg = inMsg;
        tempMsg->GetArgs(numArgs, args);
        queuedCall->responseCounter.standardReplyArgs.push_back(args[0]);
    }

    while (transitionStateFieldparams.size()) {
        bool firstIteration = true;
        TransitionStateFieldParams transitionStateFieldParam = transitionStateFieldparams.front();

        if (firstIteration) {
            if (!groupOperation && !sceneOperation) {
                queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", transitionStateFieldParam.lamps.front().c_str()));
            }

            if (!sceneOperation) {
                queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", transitionStateFieldParam.field));
            }
            firstIteration = false;
        }

        QueuedMethodCallElement element = QueuedMethodCallElement(transitionStateFieldParam.lamps, LampServiceStateInterfaceName, "TransitionLampState");
        MsgArg* arrayVals = new MsgArg[1];
        arrayVals[0].Set("{sv}", strdup(transitionStateFieldParam.field), new MsgArg(transitionStateFieldParam.value));
        arrayVals[0].SetOwnershipFlags(MsgArg::OwnsArgs | MsgArg::OwnsData);

        element.args.push_back(MsgArg("t", transitionStateFieldParam.timestamp));
        element.args.push_back(MsgArg("a{sv}", 1, arrayVals));
        element.args.push_back(MsgArg("u", transitionStateFieldParam.period));
        queuedCall->AddMethodCallElement(element);

        transitionStateFieldparams.pop_front();
    }

    while (transitionStateParams.size()) {
        bool firstIteration = true;
        TransitionStateParams transitionStateParam = transitionStateParams.front();

        if (firstIteration) {
            if (!groupOperation && !sceneOperation) {
                queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", transitionStateParam.lamps.front().c_str()));
            }
            firstIteration = false;
        }

        QueuedMethodCallElement element = QueuedMethodCallElement(transitionStateParam.lamps, LampServiceStateInterfaceName, "TransitionLampState");

        element.args.push_back(MsgArg("t", transitionStateParam.timestamp));
        element.args.push_back(transitionStateParam.state);
        element.args.push_back(MsgArg("u", transitionStateParam.period));
        queuedCall->AddMethodCallElement(element);

        transitionStateParams.pop_front();
    }

    while (pulseParams.size()) {
        bool firstIteration = true;
        PulseStateParams pulseParam = pulseParams.front();

        if (firstIteration) {
            if (!groupOperation && !sceneOperation) {
                queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", pulseParam.lamps.front().c_str()));
            }
            firstIteration = false;
        }

        QueuedMethodCallElement element = QueuedMethodCallElement(pulseParam.lamps, LampServiceStateInterfaceName, "ApplyPulseEffect");

        element.args.push_back(pulseParam.oldState);
        element.args.push_back(pulseParam.newState);
        element.args.push_back(MsgArg("u", pulseParam.period));
        element.args.push_back(MsgArg("u", pulseParam.duration));
        element.args.push_back(MsgArg("u", pulseParam.numPulses));
        element.args.push_back(MsgArg("t", pulseParam.timestamp));
        queuedCall->AddMethodCallElement(element);

        pulseParams.pop_front();
    }

    QueueLampMethod(queuedCall);
}

void LampClients::DecrementWaitingAndSendResponse(QueuedMethodCall* queuedCall, uint32_t success, uint32_t failure, uint32_t notFound, const ajn::MsgArg* arg)
{
    QCC_DbgPrintf(("%s: responseID=%s ", __FUNCTION__, queuedCall->responseID.c_str()));
    LSFResponseCode responseCode = LSF_ERR_UNEXPECTED;
    ResponseCounter responseCounter;
    bool sendResponse = false;

    responseLock.Lock();

    ResponseMap::iterator it = responseMap.find(queuedCall->responseID);

    if (it != responseMap.end()) {
        it->second.notFoundCount += notFound;
        it->second.numWaiting -= notFound;
        it->second.successCount += success;
        it->second.numWaiting -= success;
        it->second.failCount += failure;
        it->second.numWaiting -= failure;

        if (arg) {
            it->second.customReplyArgs.clear();
            it->second.customReplyArgs.push_back(arg[0]);
        }

        if (it->second.numWaiting == 0) {
            if (it->second.notFoundCount == it->second.total) {
                responseCode = LSF_ERR_NOT_FOUND;
            } else if (it->second.successCount == it->second.total) {
                responseCode = LSF_OK;
            } else if (it->second.failCount == it->second.total) {
                responseCode = LSF_ERR_FAILURE;
            } else if ((it->second.notFoundCount + it->second.successCount + it->second.failCount) == it->second.total) {
                responseCode = LSF_ERR_PARTIAL;
            }

            responseCounter = it->second;
            responseMap.erase(it);
            sendResponse = true;
        }
    } else {
        QCC_LogError(ER_FAIL, ("%s: Response map entry not found for response ID = %s", __FUNCTION__, queuedCall->responseID.c_str()));
    }
    responseLock.Unlock();

    if (sendResponse) {
        SendMethodReply(responseCode, queuedCall->inMsg, responseCounter.standardReplyArgs, responseCounter.customReplyArgs);
        delete queuedCall;
    }
}

void LampClients::HandleReplyWithLampResponseCode(Message& message, void* context)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __FUNCTION__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    QueuedMethodCall* queuedCall = static_cast<QueuedMethodCall*>(context);

    if (MESSAGE_METHOD_RET == message->GetType()) {
        size_t numArgs;
        const MsgArg* args;
        message->GetArgs(numArgs, args);

        LampResponseCode responseCode;
        args[0].Get("u", &responseCode);

        uint32_t success = 0;
        uint32_t failure = 0;

        if (responseCode == LAMP_OK) {
            success++;
        } else {
            failure++;
        }

        DecrementWaitingAndSendResponse(queuedCall, success, failure, 0);
    } else {
        DecrementWaitingAndSendResponse(queuedCall, 0, 1, 0);
    }
}

void LampClients::GetLampFaults(const LSFString& lampID, ajn::Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithVariant));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "Get");
    element.args.push_back(MsgArg("s", LampServiceInterfaceName));
    element.args.push_back(MsgArg("s", "LampFaults"));
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("au", 0, NULL));
    QueueLampMethod(queuedCall);
}

void LampClients::GetLampVersion(const LSFString& lampID, ajn::Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithVariant));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "Get");
    element.args.push_back(MsgArg("s", LampServiceInterfaceName));
    element.args.push_back(MsgArg("s", "LampServiceVersion"));
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("u", 0, NULL));
    QueueLampMethod(queuedCall);
}

void LampClients::HandleReplyWithVariant(ajn::Message& message, void* context)
{
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();
    QueuedMethodCall* queuedCall = static_cast<QueuedMethodCall*>(context);

    if (MESSAGE_METHOD_RET == message->GetType()) {
        size_t numArgs;
        const MsgArg* args;
        message->GetArgs(numArgs, args);

        MsgArg* verArg;
        args[0].Get("v", &verArg);

        DecrementWaitingAndSendResponse(queuedCall, 1, 0, 0, verArg);
    } else {
        DecrementWaitingAndSendResponse(queuedCall, 0, 1, 0);
    }
}

void LampClients::ClearLampFault(const LSFString& lampID, LampFaultCode faultCode, ajn::Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithLampResponseCode));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, LampServiceInterfaceName, "ClearLampFault");
    element.args.push_back(MsgArg("u", faultCode));
    queuedCall->AddMethodCallElement(element);

    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("u", faultCode));

    QueueLampMethod(queuedCall);
}

void LampClients::LampStateChangedSignalHandler(const InterfaceDescription::Member* member, const char* sourcePath, Message& message)
{
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    const char* uniqueId;
    args[0].Get("s", &uniqueId);
    LSFStringList ids;
    ids.push_back(uniqueId);

    controllerService.SendSignal(ControllerServiceLampInterfaceName, "LampsStateChanged", ids);
}

void LampClients::GetLampSupportedLanguages(const LSFString& lampID, ajn::Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithKeyValuePairs));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, AboutInterfaceName, "GetAboutData");
    element.args.push_back(MsgArg("s", "en"));
    queuedCall->AddMethodCallElement(element);

    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("as", 0, NULL));

    QueueLampMethod(queuedCall);
}

void LampClients::HandleReplyWithKeyValuePairs(Message& message, void* context)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __FUNCTION__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();
    QueuedMethodCall* queuedCall = static_cast<QueuedMethodCall*>(context);

    if (MESSAGE_METHOD_RET == message->GetType()) {
        const MsgArg* args;
        size_t numArgs;
        message->GetArgs(numArgs, args);

        size_t numEntries;
        MsgArg* entries;

        args[0].Get("a{sv}", &numEntries, &entries);
        for (size_t i = 0; i < numEntries; ++i) {
            char* key;
            MsgArg* value;
            entries[i].Get("{sv}", &key, &value);
            QCC_DbgPrintf(("%s: %s", __FUNCTION__, key));
            if (((0 == strcmp(key, "SupportedLanguages")) && (0 == strcmp(queuedCall->inMsg->GetMemberName(), "GetLampSupportedLanguages"))) ||
                ((0 == strcmp(key, "DeviceName")) && (0 == strcmp(queuedCall->inMsg->GetMemberName(), "GetLampName"))) ||
                ((0 == strcmp(key, "Manufacturer")) && (0 == strcmp(queuedCall->inMsg->GetMemberName(), "GetLampManufacturer")))) {
                DecrementWaitingAndSendResponse(queuedCall, 1, 0, 0, value);
                break;
            }
        }
    } else {
        DecrementWaitingAndSendResponse(queuedCall, 0, 1, 0);
    }
}

void LampClients::GetLampName(const LSFString& lampID, const LSFString& language, Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithKeyValuePairs));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, ConfigServiceInterfaceName, "GetConfigurations");
    element.args.push_back(MsgArg("s", language.c_str()));
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", language.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("s", "<ERROR>"));
    QueueLampMethod(queuedCall);
}

void LampClients::GetLampManufacturer(const LSFString& lampID, const LSFString& language, ajn::Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithKeyValuePairs));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, ConfigServiceInterfaceName, "GetConfigurations");
    element.args.push_back(MsgArg("s", language.c_str()));
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", language.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("s", "<ERROR>"));
    QueueLampMethod(queuedCall);
}

void LampClients::SetLampName(const LSFString& lampID, const LSFString& name, const LSFString& language, Message& inMsg)
{
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, ConfigServiceInterfaceName, "UpdateConfigurations");

    MsgArg name_arg("s", name.c_str());
    MsgArg arg("{sv}", "DeviceName", &name_arg);

    element.args.push_back(MsgArg("s", language.c_str()));
    element.args.push_back(MsgArg("a{sv}", 1, &arg));
    queuedCall->AddMethodCallElement(element);

    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", language.c_str()));

    QueueLampMethod(queuedCall);
}

void LampClients::IntrospectCB(QStatus status, ajn::ProxyBusObject* obj, void* context)
{
    QCC_DbgPrintf(("%s: status = %s", __FUNCTION__, QCC_StatusText(status)));

    LampConnection* connection = static_cast<LampConnection*>(context);

    if (ER_OK != status) {
        delete connection;
        return;
    }

    /*
     * Do not introspect the remote Config and About object!
     */
    const InterfaceDescription* intf = controllerService.GetBusAttachment().GetInterface(ConfigServiceInterfaceName);
    QStatus tempStatus = connection->configObject.AddInterface(*intf);
    QCC_DbgPrintf(("%s: connection->configObject.AddInterface returns %s\n", __FUNCTION__, QCC_StatusText(tempStatus)));
    if (ER_OK == tempStatus) {
        intf = controllerService.GetBusAttachment().GetInterface(AboutInterfaceName);
        tempStatus = connection->aboutObject.AddInterface(*intf);
        QCC_DbgPrintf(("%s: connection->aboutObject.AddInterface returns %s\n", __FUNCTION__, QCC_StatusText(tempStatus)));

        if (ER_OK == tempStatus) {
            if (!lampStateChangedSignalHandlerRegistered) {
                intf = connection->object.GetInterface(LampServiceStateInterfaceName);
                const InterfaceDescription::Member* sig = intf->GetMember("LampStateChanged");
                tempStatus = controllerService.GetBusAttachment().RegisterSignalHandler(this, static_cast<MessageReceiver::SignalHandler>(&LampClients::LampStateChangedSignalHandler), sig, LampServiceObjectPath);
                QCC_DbgPrintf(("%s: RegisterSignalHandler returns %s\n", __FUNCTION__, QCC_StatusText(tempStatus)));

                if (ER_OK == tempStatus) {
                    lampStateChangedSignalHandlerRegistered = true;
                }
            }

            if (ER_OK == tempStatus) {
                tempStatus = joinSessionCBListLock.Lock();
                if (ER_OK != tempStatus) {
                    QCC_LogError(tempStatus, ("%s: joinSessionCBListLock.Lock() failed", __FUNCTION__));
                } else {
                    joinSessionCBList.push_back(connection);
                    QCC_DbgPrintf(("%s: Add %s to joinSessionCBList", __FUNCTION__, connection->lampId.c_str()));
                    tempStatus = joinSessionCBListLock.Unlock();
                    if (ER_OK != tempStatus) {
                        QCC_LogError(tempStatus, ("%s: joinSessionCBListLock.Unlock() failed", __FUNCTION__));
                    }
                    wakeUp.Post();
                }
            }
        }
    }

    if (ER_OK != tempStatus) {
        delete connection;
    }
}

void LampClients::PingCB(QStatus status, void* context)
{
    QCC_DbgPrintf(("%s: status=%s", __FUNCTION__, QCC_StatusText(status)));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    if (context) {
        ngnsPingResponseLock.Lock();
        if (ER_OK == status) {
            QCC_DbgPrintf(("%s: Ping successful for Lamp ID = %s", __FUNCTION__, ((LSFString*)context)->c_str()));
        } else {
            QCC_LogError(status, ("%s: Ping un-successful for Lamp ID = %s", __FUNCTION__, ((LSFString*)context)->c_str()));
            LSFString tempString = *((LSFString*)context);
            probableLostLamps.push_back(tempString);
        }
        receivedNGNSPingResponses++;
        ngnsPingResponseLock.Unlock();
    }
    delete ((LSFString*)context);

    wakeUp.Post();
}

void LampClients::SessionPingCB(Message& reply, void* context)
{
    QCC_DbgPrintf(("%s", __FUNCTION__));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    if (context) {
        sessionPingResponseLock.Lock();
        if (reply->GetType() == MESSAGE_ERROR) {
            QCC_LogError(ER_FAIL, ("%s: Session Ping un-successful for Lamp ID = %s", __FUNCTION__, ((LSFString*)context)->c_str()));
            LSFString tempString = *((LSFString*)context);
            lostLamps.push_back(tempString);
        } else {
            QCC_DbgPrintf(("%s: Session Ping successful for Lamp ID = %s", __FUNCTION__, ((LSFString*)context)->c_str()));
        }
        receivedSessionPingResponses++;
        sessionPingResponseLock.Unlock();
    }
    delete ((LSFString*)context);

    wakeUp.Post();
}

void LampClients::Run(void)
{
    while (isRunning) {

        /*
         * Wait for something to happen
         */
        QCC_DbgPrintf(("%s: Waiting on wakeUp", __FUNCTION__));
        wakeUp.Wait();

        /*
         * Handle all received About Announcements as appropriate
         */
        std::list<LampConnection*> tempAboutList;
        tempAboutList.clear();
        QStatus status = aboutsListLock.Lock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: aboutsListLock.Lock() failed", __FUNCTION__));
        } else {
            /*
             * Make a local copy of aboutsList and release the list for use by the About Handler
             */
            tempAboutList = aboutsList;
            aboutsList.clear();
            status = aboutsListLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: aboutsListLock.Unlock() failed", __FUNCTION__));
            }
        }

        LSFStringList nameChangedList;
        std::list<LampConnection*> joinSessionList;
        joinSessionList.clear();
        while (tempAboutList.size()) {
            LampConnection* newConn = tempAboutList.front();
            LampMap::const_iterator lit = activeLamps.find(newConn->lampId);
            if (lit != activeLamps.end()) {
                /*
                 * We already know about this Lamp
                 */
                LampConnection* conn = lit->second;
                if (conn->name != newConn->name) {
                    conn->name = newConn->name;
                    nameChangedList.push_back(conn->lampId);
                    QCC_DbgPrintf(("%s: Name Changed for %s", __FUNCTION__, newConn->lampId.c_str()));
                }
                delete newConn;
            } else {
                /*
                 * We should only support a maximum of MAX_SUPPORTED_LAMPS lamps at
                 * any given point in time
                 */
                if ((joinSessionInProgressList.find(newConn->lampId) == joinSessionInProgressList.end()) && (activeLamps.size() < MAX_SUPPORTED_LAMPS)) {
                    joinSessionInProgressList.insert(newConn->lampId);
                    joinSessionList.push_back(newConn);
                    QCC_DbgPrintf(("%s: Added %s to JoinSession List", __FUNCTION__, newConn->lampId.c_str()));
                } else {
                    QCC_DbgPrintf(("%s: No slot for connection with a new lamp or connect attempt to Lamp already in progress", __FUNCTION__));
                    delete newConn;
                }
            }
            tempAboutList.pop_front();
        }

        /*
         * Send out the LampsNameChanged signal if required
         */
        if (nameChangedList.size()) {
            controllerService.SendSignal(ControllerServiceLampInterfaceName, "LampsNameChanged", nameChangedList);
        }

        /*
         * Send out Join Session requests
         */
        while (joinSessionList.size()) {
            LampConnection* newConn = joinSessionList.front();
            SessionOpts opts;
            opts.isMultipoint = true;
            status = controllerService.GetBusAttachment().JoinSessionAsync(newConn->busName.c_str(), newConn->port, this, opts, this, newConn);
            QCC_DbgPrintf(("JoinSessionAsync(%s,%u): %s\n", newConn->busName.c_str(), newConn->port, QCC_StatusText(status)));
            if (status != ER_OK) {
                joinSessionInProgressList.erase(newConn->lampId);
                delete newConn;
            }
            joinSessionList.pop_front();
        }

        /*
         * Handle all the successful Join Sessions
         */
        std::list<LampConnection*> tempJoinList;
        tempJoinList.clear();
        status = joinSessionCBListLock.Lock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: joinSessionCBListLock.Lock() failed", __FUNCTION__));
        } else {
            /*
             * Make a local copy of joinSessionCBList and release the list for use by the Join Session callbacks
             */
            tempJoinList = joinSessionCBList;
            joinSessionCBList.clear();
            status = joinSessionCBListLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: joinSessionCBListLock.Unlock() failed", __FUNCTION__));
            }
        }

        std::list<LampConnection*> leaveSessionList;
        leaveSessionList.clear();
        while (tempJoinList.size()) {
            LampConnection* newConn = tempJoinList.front();
            /*
             * We should only support a maximum of MAX_SUPPORTED_LAMPS lamps at
             * any given point in time
             */
            if (activeLamps.size() < MAX_SUPPORTED_LAMPS) {
                activeLamps.insert(std::make_pair(newConn->lampId, newConn));
                joinSessionInProgressList.erase(newConn->lampId);
                QCC_DbgPrintf(("%s: Added new connection for %s to activeLamps", __FUNCTION__, newConn->lampId.c_str()));
            } else {
                QCC_DbgPrintf(("%s: No slot for connection with a new lamp", __FUNCTION__));
                leaveSessionList.push_back(newConn);
                QCC_DbgPrintf(("%s: Added connection for %s to leaveSessionList", __FUNCTION__, newConn->lampId.c_str()));
            }
            tempJoinList.pop_front();
        }

        /*
         * Tear down the unwanted sessions
         */
        while (leaveSessionList.size()) {
            LampConnection* connection = leaveSessionList.front();
            status = controllerService.GetBusAttachment().LeaveSession(connection->sessionID);
            QCC_DbgPrintf(("%s: controllerService.GetBusAttachment().LeaveSession for %s returns %s\n",
                           __FUNCTION__, connection->lampId.c_str(), QCC_StatusText(status)));
            joinSessionInProgressList.erase(connection->lampId);
            delete connection;
            leaveSessionList.pop_front();
        }

        /*
         * Handle all the lost sessions
         */
        std::set<uint32_t> tempLostSessionList;
        tempLostSessionList.clear();
        status = lostSessionListLock.Lock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lostSessionListLock.Lock() failed", __FUNCTION__));
            return;
        }
        /*
         * Make a local copy and release lostSessionList for use by Session Losts
         */
        tempLostSessionList = lostSessionList;
        lostSessionList.clear();
        status = lostSessionListLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: lostSessionListLock.Unlock() failed", __FUNCTION__));
        }

        if (tempLostSessionList.size()) {
            for (LampMap::iterator it = activeLamps.begin(); it != activeLamps.end(); it++) {
                if (tempLostSessionList.find((uint32_t)it->second->sessionID) != tempLostSessionList.end()) {
                    QCC_DbgPrintf(("%s: Removing %s from activeLamps", __FUNCTION__, it->second->lampId.c_str()));
                    delete it->second;
                    activeLamps.erase(it);
                }
            }
        }

        /*
         * Send NGNS pings if required
         */
        bool sendPings = false;
        if (!sentNGNSPings && !sentNGNSPings && !pingsInProgress) {
            status = getAllLampIDsLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(ER_FAIL, ("%s: getAllLampIDsLock.Lock() failed", __FUNCTION__));
            } else {
                if (getAllLampIDsRequests.size()) {
                    sendPings = true;
                }
                status = getAllLampIDsLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(ER_FAIL, ("%s: getAllLampIDsLock.Unlock() failed", __FUNCTION__));
                }
            }
        }

        if (sendPings) {
            pingsInProgress = true;
            for (LampMap::iterator it = activeLamps.begin(); it != activeLamps.end(); ++it) {
                LSFString* lampId = new LSFString(it->first);
                if (controllerService.GetBusAttachment().PingAsync(it->second->busName.c_str(), PING_TIMEOUT_IN_MS, this, lampId) != ER_OK) {
                    QCC_LogError(ER_FAIL, ("%s: NGNS Ping Async unsuccessful for Lamp ID %s", __FUNCTION__, it->first.c_str()));

                    status = it->second->object.MethodCallAsync(
                        org::freedesktop::DBus::Peer::InterfaceName,
                        "Ping",
                        this,
                        static_cast<MessageReceiver::ReplyHandler>(&LampClients::SessionPingCB),
                        NULL, 0,
                        lampId
                        );

                    if (status != ER_OK) {
                        QCC_LogError(status, ("%s: Sending Session Ping failed", __FUNCTION__));
                        status = controllerService.GetBusAttachment().LeaveSession(it->second->sessionID);
                        QCC_DbgPrintf(("%s: controllerService.GetBusAttachment().LeaveSession for %s returns %s\n",
                                       __FUNCTION__, it->second->lampId.c_str(), QCC_StatusText(status)));
                        delete it->second;
                        activeLamps.erase(it);
                        delete lampId;
                    } else {
                        QCC_DbgPrintf(("%s: Successfully sent a Session Ping", __FUNCTION__));
                        sentSessionPings++;
                    }
                } else {
                    sentNGNSPings++;
                    QCC_DbgPrintf(("%s: Ping Async successful for Lamp ID %s", __FUNCTION__, it->first.c_str()));
                }
            }
        }

        LSFStringList probableLostLampsList;
        probableLostLampsList.clear();

        if (sentNGNSPings) {
            ngnsPingResponseLock.Lock();
            probableLostLampsList = probableLostLamps;
            probableLostLamps.clear();
            if (receivedNGNSPingResponses == sentNGNSPings) {
                sentNGNSPings = 0;
                receivedNGNSPingResponses = 0;
            }
            ngnsPingResponseLock.Unlock();
        }

        /*
         * Send Session pings to the lamps for which NGNS pings failed
         */
        while (probableLostLampsList.size()) {
            LampMap::iterator it = activeLamps.find(probableLostLampsList.front());
            if (it != activeLamps.end()) {
                LampConnection* connection = it->second;

                LSFString* lampId = new LSFString(it->first);

                status = connection->object.MethodCallAsync(
                    org::freedesktop::DBus::Peer::InterfaceName,
                    "Ping",
                    this,
                    static_cast<MessageReceiver::ReplyHandler>(&LampClients::SessionPingCB),
                    NULL, 0,
                    lampId
                    );

                if (status != ER_OK) {
                    QCC_LogError(status, ("%s: Sending Session Ping failed", __FUNCTION__));
                    status = controllerService.GetBusAttachment().LeaveSession(it->second->sessionID);
                    QCC_DbgPrintf(("%s: controllerService.GetBusAttachment().LeaveSession for %s returns %s\n",
                                   __FUNCTION__, it->second->lampId.c_str(), QCC_StatusText(status)));
                    delete it->second;
                    activeLamps.erase(it);
                    delete lampId;
                } else {
                    QCC_DbgPrintf(("%s: Successfully sent a Session Ping", __FUNCTION__));
                    sentSessionPings++;
                }
            }
            probableLostLampsList.pop_front();
        }

        LSFStringList lostLampsList;
        lostLampsList.clear();

        if (sentSessionPings) {
            sessionPingResponseLock.Lock();
            lostLampsList = lostLamps;
            lostLamps.clear();
            if (receivedSessionPingResponses == sentSessionPings) {
                sentSessionPings = 0;
                receivedSessionPingResponses = 0;
            }
            sessionPingResponseLock.Unlock();
        }

        /*
         * Clean up all Lamps to which Pings did not succeed
         */
        while (lostLampsList.size()) {
            LampMap::iterator it = activeLamps.find(lostLampsList.front());
            if (it != activeLamps.end()) {
                LampConnection* connection = it->second;
                status = controllerService.GetBusAttachment().LeaveSession(connection->sessionID);
                QCC_DbgPrintf(("%s: controllerService.GetBusAttachment().LeaveSession for %s returns %s\n",
                               __FUNCTION__, connection->lampId.c_str(), QCC_StatusText(status)));
                delete connection;
                activeLamps.erase(it);
            }
            lostLampsList.pop_front();
        }

        /*
         * Send all GetAllLampIDs responses
         */
        if (!sentNGNSPings && !sentSessionPings) {
            std::list<Message> tempGetAllLampIDsRequests;
            tempGetAllLampIDsRequests.clear();
            status = getAllLampIDsLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(ER_FAIL, ("%s: getAllLampIDsLock.Lock() failed", __FUNCTION__));
            } else {
                tempGetAllLampIDsRequests = getAllLampIDsRequests;
                getAllLampIDsRequests.clear();
                status = getAllLampIDsLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(ER_FAIL, ("%s: getAllLampIDsLock.Unlock() failed", __FUNCTION__));
                }
            }

            LSFStringList idList;
            idList.clear();
            LSFResponseCode responseCode = LSF_OK;
            /*
             * Get all the Lamp IDs
             */
            for (LampMap::const_iterator it = activeLamps.begin(); it != activeLamps.end(); ++it) {
                idList.push_back(it->first);
            }

            while (tempGetAllLampIDsRequests.size()) {
                QCC_DbgPrintf(("%s: Sending GetAllLampIDs reply with with tempGetAllLampIDsRequests.size() %d", __FUNCTION__, tempGetAllLampIDsRequests.size()));
                controllerService.SendMethodReplyWithResponseCodeAndListOfIDs(tempGetAllLampIDsRequests.front(), responseCode, idList);
                tempGetAllLampIDsRequests.pop_front();
            }

            pingsInProgress = false;
        }

        /*
         * Handle all the incoming method requests
         */
        std::list<QueuedMethodCall*> tempMethodQueue;
        tempMethodQueue.clear();
        status = queueLock.Lock();
        if (status != ER_OK) {
            QCC_LogError(status, ("%s: queueLock.Lock() failed", __FUNCTION__));
        } else {
            /*
             * Make a local copy and release methodQueue
             */
            tempMethodQueue = methodQueue;
            methodQueue.clear();
            status = queueLock.Unlock();
            if (status != ER_OK) {
                QCC_LogError(status, ("%s: queueLock.Unlock() failed", __FUNCTION__));
            }
        }

        while (tempMethodQueue.size()) {
            QueuedMethodCall* queuedCall = tempMethodQueue.front();
            QCC_DbgPrintf(("%s: Calling DoMethodCallAsync with tempMethodQueue.size() %d", __FUNCTION__, tempMethodQueue.size()));
            DoMethodCallAsync(queuedCall);
            tempMethodQueue.pop_front();
        }
    }
}