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
#include <time.h>

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
    QCC_DbgPrintf(("%s:version=%u, port=%u, busName=%s", __func__, version, port, busName));
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

        QCC_DbgPrintf(("%s: About Data Dump", __func__));
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
    QCC_DbgTrace(("%s", __func__));

    LSFResponseCode responseCode = LSF_OK;

    bool tempConnectToLamps = false;
    connectToLampsLock.Lock();
    tempConnectToLamps = connectToLamps;
    connectToLampsLock.Unlock();

    if (!tempConnectToLamps) {
        responseCode = LSF_ERR_REJECTED;
    } else {
        QStatus status = getAllLampIDsLock.Lock();
        if (ER_OK != status) {
            QCC_LogError(ER_FAIL, ("%s: Failed to lock mutex", __func__));
            responseCode = LSF_ERR_FAILURE;
        } else {
            getAllLampIDsRequests.push_back(message);

            status = getAllLampIDsLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(ER_FAIL, ("%s: Failed to unlock mutex", __func__));
            }
            wakeUp.Post();
        }
    }

    if (LSF_OK != responseCode) {
        LSFStringList idList;
        controllerService.SendMethodReplyWithResponseCodeAndListOfIDs(message, responseCode, idList);
    }
}

void LampClients::GetAllLamps(LampNameMap& lamps)
{
    QCC_LogError(ER_OK, ("%s: PLEASE NOTE THIS FUNCTION IS NOT THREAD SAFE", __func__));
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
    connectToLamps(false)
{
    QCC_DbgTrace(("%s", __func__));
    keyListener.SetPassCode(INITIAL_PASSCODE);
    getMethodQueue.clear();
    setMethodQueue.clear();
    aboutsList.clear();
    activeLamps.clear();
    joinSessionCBList.clear();
    lostSessionList.clear();
    getAllLampIDsRequests.clear();
    lostLamps.clear();
    joinSessionInProgressList.clear();
    sentNGNSPings.clear();
}

LampClients::~LampClients()
{
    QCC_DbgTrace(("%s", __func__));

    if (serviceHandler) {
        delete serviceHandler;
        serviceHandler = NULL;
    }

    while (getMethodQueue.size()) {
        QueuedMethodCall* queuedCall = getMethodQueue.front();
        delete queuedCall;
        getMethodQueue.pop_front();
    }

    while (setMethodQueue.size()) {
        QueuedMethodCall* queuedCall = setMethodQueue.front();
        delete queuedCall;
        setMethodQueue.pop_front();
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
    QCC_DbgTrace(("%s", __func__));

    QCC_DbgPrintf(("%s: KeyStore location = %s", __func__, keyStoreFileLocation));
    QStatus status = controllerService.GetBusAttachment().EnablePeerSecurity("ALLJOYN_PIN_KEYX ALLJOYN_ECDHE_PSK", &keyListener, keyStoreFileLocation);
    QCC_DbgPrintf(("EnablePeerSecurity(): %s\n", QCC_StatusText(status)));

    if (ER_OK == status) {
        isRunning = true;
        status = Thread::Start();
        QCC_DbgPrintf(("%s: Thread::Start(): %s\n", __func__, QCC_StatusText(status)));
    }

    return status;
}

void LampClients::Join(void)
{
    QCC_DbgTrace(("%s", __func__));

    Thread::Join();
}

void LampClients::Stop(void)
{
    QCC_DbgTrace(("%s", __func__));
    DisconnectFromLamps();
    isRunning = false;
    wakeUp.Post();
}

void LampClients::HandleAboutAnnounce(const LSFString lampID, const LSFString& lampName, uint16_t port, const char* busName)
{
    QCC_DbgPrintf(("LampClients::HandleAboutAnnounce(%s,%s,%u,%s)\n", lampID.c_str(), lampName.c_str(), port, busName));
    bool added = false;
    LampConnection* connection = new LampConnection();
    if (connection) {
        connection->lampId = lampID;
        connection->busName = busName;
        connection->name = lampName;
        connection->port = port;
        connection->methodCallPending = false;

        QStatus status = aboutsListLock.Lock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: aboutsListLock.Lock() failed", __func__));
            delete connection;
            return;
        }

        LampMap::iterator it = aboutsList.find(lampID);
        if (it != aboutsList.end()) {
            QCC_DbgPrintf(("%s: Got another announcement for a lamp that we already know about", __func__));
            delete it->second;
            it->second = connection;
        } else {
            if (aboutsList.size() < (MAX_SUPPORTED_LAMPS + 25)) {
                aboutsList.insert(std::make_pair(lampID, connection));
                added = true;
            } else {
                QCC_LogError(status, ("%s: Controller Service can cache only a maximum of %d announcements. Max'ed out on the capacity. Ignoring the announcement", __func__, (MAX_SUPPORTED_LAMPS + 25)));
                delete connection;
            }
        }
        status = aboutsListLock.Unlock();
        if (ER_OK != status) {
            QCC_LogError(status, ("%s: aboutsListLock.Unlock() failed", __func__));
        }

        bool tempConnectToLamps = false;
        connectToLampsLock.Lock();
        tempConnectToLamps = connectToLamps;
        connectToLampsLock.Unlock();

        if (tempConnectToLamps && added) {
            wakeUp.Post();
        } else {
            QCC_DbgPrintf(("%s: connectToLamps is false", __func__));
        }
    } else {
        QCC_LogError(ER_FAIL, ("%s: Could not allocate memory for new LampConnection", __func__));
    }
}

void LampClients::ConnectToLamps(void)
{
    QCC_DbgTrace(("%s", __func__));
    connectToLampsLock.Lock();
    connectToLamps = true;
    connectToLampsLock.Unlock();
    wakeUp.Post();
}

void LampClients::DisconnectFromLamps(void)
{
    QCC_DbgTrace(("%s", __func__));
    connectToLampsLock.Lock();
    connectToLamps = false;
    connectToLampsLock.Unlock();
    wakeUp.Post();
}

void LampClients::JoinSessionCB(QStatus status, SessionId sessionId, const SessionOpts& opts, void* context)
{
    QCC_DbgTrace(("%s:status=%s sessionId=%u", __func__, QCC_StatusText(status), sessionId));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    /*
     * Only handle JoinSessionCB if we are still running
     */
    if (!isRunning) {
        QCC_DbgPrintf(("%s: Ignoring callback as we are stopping", __func__));
        return;
    }

    LampConnection* connection = static_cast<LampConnection*>(context);

    if (!connection) {
        QCC_LogError(ER_FAIL, ("%s: No context", __func__));
        return;
    }

    QStatus tempStatus = ER_OK;

    bool tempConnectToLamps = false;
    connectToLampsLock.Lock();
    tempConnectToLamps = connectToLamps;
    connectToLampsLock.Unlock();

    if (!tempConnectToLamps) {
        QCC_DbgPrintf(("%s: connectToLamps is false", __func__));
        tempStatus = ER_FAIL;
    } else {
        if (status != ER_OK) {
            QCC_LogError(status, ("%s: Join Session failed for lamp with ID = %s", __func__, connection->lampId.c_str()));
            joinSessionInProgressLock.Lock();
            joinSessionInProgressList.erase(connection->lampId);
            joinSessionInProgressLock.Unlock();

            sentNGNSPingsLock.Lock();
            LampMap::iterator lit = sentNGNSPings.find(connection->lampId);
            if (lit == sentNGNSPings.end()) {
                QCC_DbgPrintf(("%s: Retrying Ping due to JoinSession failure", __func__));
                if (controllerService.GetBusAttachment().PingAsync(connection->busName.c_str(), NGNS_PING_TIMEOUT_IN_MS, this, connection) != ER_OK) {
                    QCC_LogError(status, ("%s: Sending PingAsync failed for Lamp ID = %s", __func__, connection->lampId.c_str()));
                    delete connection;
                } else {
                    QCC_DbgPrintf(("%s: NGNS Ping successfully sent for lamp ID %s", __func__, connection->lampId.c_str()));
                    sentNGNSPings.insert(std::make_pair(connection->lampId, connection));
                }
            } else {
                QCC_DbgPrintf(("%s: NGNS Ping already in progress for lamp ID %s", __func__, connection->lampId.c_str()));
            }
            sentNGNSPingsLock.Unlock();

            return;
        }

        QCC_DbgPrintf(("%s: status = %s, sessionId = %u\n", __func__, QCC_StatusText(status), sessionId));
        QCC_DbgPrintf(("New connection to lamp ID [%s]\n", connection->lampId.c_str()));

        connection->sessionID = sessionId;
        connection->object = ProxyBusObject(controllerService.GetBusAttachment(), connection->busName.c_str(), LampServiceObjectPath, sessionId);
        connection->configObject = ProxyBusObject(controllerService.GetBusAttachment(), connection->busName.c_str(), ConfigServiceObjectPath, sessionId);
        connection->aboutObject = ProxyBusObject(controllerService.GetBusAttachment(), connection->busName.c_str(), AboutObjectPath, sessionId);
        connection->methodCallPending = false;

        QCC_DbgPrintf(("%s: Invoking IntrospectRemoteObjectAsync\n", __func__));
        tempStatus = connection->object.IntrospectRemoteObjectAsync(this, static_cast<ProxyBusObject::Listener::IntrospectCB>(&LampClients::IntrospectCB), context);
        QCC_DbgPrintf(("%s: IntrospectRemoteObjectAsync returns %s\n", __func__, QCC_StatusText(tempStatus)));
    }

    if (ER_OK != tempStatus) {
        /*
         * Tear down the session if the object setup was unsuccessful
         */
        controllerService.DoLeaveSessionAsync(sessionId);
        connection->sessionID = 0;
        connection->object = ProxyBusObject();
        connection->configObject = ProxyBusObject();
        connection->aboutObject = ProxyBusObject();
        connection->methodCallPending = false;
        if (tempConnectToLamps) {
            sentNGNSPingsLock.Lock();
            LampMap::iterator lit = sentNGNSPings.find(connection->lampId);
            if (lit == sentNGNSPings.end()) {
                QCC_DbgPrintf(("%s: Retrying Ping due to IntrospectRemoteObjectAsync method call failure", __func__));
                if (controllerService.GetBusAttachment().PingAsync(connection->busName.c_str(), NGNS_PING_TIMEOUT_IN_MS, this, connection) != ER_OK) {
                    QCC_LogError(status, ("%s: Sending PingAsync failed for Lamp ID = %s", __func__, connection->lampId.c_str()));
                    delete connection;
                } else {
                    QCC_DbgPrintf(("%s: NGNS Ping successfully sent for lamp ID %s", __func__, connection->lampId.c_str()));
                    sentNGNSPings.insert(std::make_pair(connection->lampId, connection));
                }
            } else {
                QCC_DbgPrintf(("%s: NGNS Ping already in progress for lamp ID %s", __func__, connection->lampId.c_str()));
            }
            sentNGNSPingsLock.Unlock();
        } else {
            delete connection;
        }

    }
}

void LampClients::SessionLost(SessionId sessionId, SessionLostReason reason)
{
    QCC_DbgPrintf(("%s: sessionId=0x%x reason=0x%x\n", __func__, sessionId, reason));

    bool tempConnectToLamps = false;
    connectToLampsLock.Lock();
    tempConnectToLamps = connectToLamps;
    connectToLampsLock.Unlock();

    if (!tempConnectToLamps) {
        QCC_DbgPrintf(("%s: connectToLamps is false", __func__));
        return;
    }

    QStatus status = lostSessionListLock.Lock();
    if (ER_OK != status) {
        QCC_LogError(status, ("%s: lostSessionListLock.Lock() failed", __func__));
        return;
    }
    lostSessionList.insert((uint32_t)sessionId);
    status = lostSessionListLock.Unlock();
    if (ER_OK != status) {
        QCC_LogError(status, ("%s: lostSessionListLock.Unlock() failed", __func__));
    }
    wakeUp.Post();
}

void LampClients::SendMethodReply(LSFResponseCode responseCode, ajn::Message msg, std::list<ajn::MsgArg>& stdArgs, std::list<ajn::MsgArg>& custArgs)
{
    QCC_DbgPrintf(("%s\n", __func__));
    size_t numArgs = stdArgs.size() + custArgs.size() + 1;
    QCC_DbgPrintf(("%s: NumArgs = %d", __func__, numArgs));
    MsgArg* args = new MsgArg[numArgs];
    if (args) {
        args[0] = MsgArg("u", responseCode);
        args[0].SetOwnershipFlags(MsgArg::OwnsData | MsgArg::OwnsArgs, true);
        uint8_t index = 1;
        while (stdArgs.size()) {
            args[index] = stdArgs.front();
            args[index].SetOwnershipFlags(MsgArg::OwnsData | MsgArg::OwnsArgs, true);
            stdArgs.pop_front();
            index++;
        }
        while (custArgs.size()) {
            args[index] = custArgs.front();
            args[index].SetOwnershipFlags(MsgArg::OwnsData | MsgArg::OwnsArgs, true);
            custArgs.pop_front();
            index++;
        }
        controllerService.SendMethodReply(msg, args, numArgs);
        delete [] args;
    } else {
        QCC_LogError(ER_OUT_OF_MEMORY, ("%s: Failed to allocate enough memory", __func__));
    }
}

void LampClients::QueueGetLampMethod(QueuedMethodCall* queuedCall)
{
    QCC_DbgPrintf(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    bool tempConnectToLamps = false;

    connectToLampsLock.Lock();
    tempConnectToLamps = connectToLamps;
    connectToLampsLock.Unlock();

    if (!tempConnectToLamps) {
        QCC_DbgPrintf(("%s: connectToLamps is false", __func__));
        responseCode = LSF_ERR_REJECTED;
    } else {
        QStatus status = getQueueLock.Lock();
        if (status != ER_OK) {
            QCC_LogError(status, ("%s: getQueueLock.Lock() failed or doNotQueue set", __func__));
            responseCode = LSF_ERR_BUSY;
        } else {
            if (getMethodQueue.size() < MAX_LAMP_CLIENTS_METHOD_QUEUE_SIZE) {
                getMethodQueue.push_back(queuedCall);
            } else {
                responseCode = LSF_ERR_NO_SLOT;
                QCC_LogError(ER_OUT_OF_MEMORY, ("%s: No slot for new method call", __func__));
            }

            status = getQueueLock.Unlock();
            if (status != ER_OK) {
                QCC_LogError(status, ("%s: getQueueLock.Unlock() failed", __func__));
            }
        }
    }

    if (LSF_OK == responseCode) {
        wakeUp.Post();
    } else {
        if (strstr(queuedCall->inMsg->GetInterface(), ApplySceneEventActionInterfaceName)) {
            QCC_DbgPrintf(("%s: Skipping the sending of reply because interface = %s", __func__, queuedCall->inMsg->GetInterface()));
        } else {
            SendMethodReply(responseCode, queuedCall->inMsg, queuedCall->responseCounter.standardReplyArgs, queuedCall->responseCounter.customReplyArgs);
        }
        delete queuedCall;
    }
}

void LampClients::QueueSetLampMethod(QueuedMethodCall* queuedCall)
{
    QCC_DbgPrintf(("%s", __func__));
    LSFResponseCode responseCode = LSF_OK;

    bool tempConnectToLamps = false;

    connectToLampsLock.Lock();
    tempConnectToLamps = connectToLamps;
    connectToLampsLock.Unlock();

    if (!tempConnectToLamps) {
        QCC_DbgPrintf(("%s: connectToLamps is false", __func__));
        responseCode = LSF_ERR_REJECTED;
    } else {
        QStatus status = setQueueLock.Lock();
        if (status != ER_OK) {
            QCC_LogError(status, ("%s: setQueueLock.Lock() failed or doNotQueue set", __func__));
            responseCode = LSF_ERR_BUSY;
        } else {
            if (setMethodQueue.size() < MAX_LAMP_CLIENTS_METHOD_QUEUE_SIZE) {
                setMethodQueue.push_back(queuedCall);
            } else {
                responseCode = LSF_ERR_NO_SLOT;
                QCC_LogError(ER_OUT_OF_MEMORY, ("%s: No slot for new method call", __func__));
            }

            status = setQueueLock.Unlock();
            if (status != ER_OK) {
                QCC_LogError(status, ("%s: setQueueLock.Unlock() failed", __func__));
            }
        }
    }

    if (LSF_OK == responseCode) {
        wakeUp.Post();
    } else {
        if (strstr(queuedCall->inMsg->GetInterface(), ApplySceneEventActionInterfaceName)) {
            QCC_DbgPrintf(("%s: Skipping the sending of reply because interface = %s", __func__, queuedCall->inMsg->GetInterface()));
        } else {
            SendMethodReply(responseCode, queuedCall->inMsg, queuedCall->responseCounter.standardReplyArgs, queuedCall->responseCounter.customReplyArgs);
        }
        delete queuedCall;
    }
}

static uint64_t GetTimeMsec()
{
    static time_t s_clockOffset = 0;
    struct timespec ts;
    uint64_t ret_val;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    if (0 == s_clockOffset) {
        s_clockOffset = ts.tv_sec;
    }

    ret_val = ((uint64_t)(ts.tv_sec - s_clockOffset)) * 1000;
    ret_val += (uint64_t)ts.tv_nsec / 1000000;

    return ret_val;

}

LSFResponseCode LampClients::DoMethodCallAsync(QueuedMethodCall* queuedCall, bool recordInPendingCallList)
{
    QCC_DbgPrintf(("%s", __func__));

    LSFResponseCode responseCode = LSF_OK;
    QStatus status = ER_OK;
    uint32_t notFound = 0;
    uint32_t failures = 0;
    uint32_t busy = 0;
    QueuedMethodCallElementList elementList = queuedCall->methodCallElements;
    QueuedMethodCallContext* ctx = NULL;
    std::list<LSFString> pingFailedList;

    while (elementList.size()) {
        QueuedMethodCallElement element = elementList.front();
        LSFStringList lamps = element.lamps;
        MethodCallDetail detail;

        for (LSFStringList::const_iterator it = lamps.begin(); it != lamps.end(); it++) {
            QCC_DbgPrintf(("%s: Processing for LampID=%s", __func__, (*it).c_str()));
            LampMap::iterator lit = activeLamps.find(*it);
            if (lit != activeLamps.end()) {
                QCC_DbgPrintf(("%s: Found Lamp", __func__));
                if (lit->second->methodCallPending) {
                    QCC_DbgPrintf(("%s: Method call already pending for lamp", __func__));
                    busy++;
                } else {
                    if (recordInPendingCallList) {
                        detail.interface = element.interface;
                        detail.method = element.method;
                        detail.numArgs = element.args.size();
                        detail.arg1 = element.args[0].v_string.str;
                        if (detail.numArgs == 2) {
                            detail.arg2 = element.args[1].v_string.str;
                        }
                    }
                    if (0 == strcmp(element.interface.c_str(), ConfigServiceInterfaceName)) {
                        QCC_DbgPrintf(("%s: Config Call", __func__));
                        ctx = new QueuedMethodCallContext((*it), queuedCall);
                        if (!ctx) {
                            QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for context", __func__));
                            status = ER_FAIL;
                        } else {
                            ctx->method = element.method;
                            ctx->timeSent = GetTimeMsec();
                            QCC_DbgTrace(("%s: Calling %s.%s on lamp %s", __func__, element.interface.c_str(), element.method.c_str(), ctx->lampID.c_str()));
                            status = lit->second->configObject.MethodCallAsync(
                                element.interface.c_str(),
                                element.method.c_str(),
                                this,
                                queuedCall->replyFunc,
                                &element.args[0],
                                element.args.size(),
                                ctx,
                                LAMP_METHOD_CALL_TIMEOUT
                                );
                        }
                    } else if (0 == strcmp(element.interface.c_str(), AboutInterfaceName)) {
                        QCC_DbgPrintf(("%s: About Call", __func__));
                        ctx = new QueuedMethodCallContext((*it), queuedCall);
                        if (!ctx) {
                            QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for context", __func__));
                            status = ER_FAIL;
                        } else {
                            ctx->method = element.method;
                            ctx->timeSent = GetTimeMsec();
                            QCC_DbgTrace(("%s: Calling %s.%s on lamp %s", __func__, element.interface.c_str(), element.method.c_str(), ctx->lampID.c_str()));
                            status = lit->second->aboutObject.MethodCallAsync(
                                element.interface.c_str(),
                                element.method.c_str(),
                                this,
                                queuedCall->replyFunc,
                                &element.args[0],
                                element.args.size(),
                                ctx,
                                LAMP_METHOD_CALL_TIMEOUT
                                );
                        }
                    } else {
                        QCC_DbgPrintf(("%s: LampService Call", __func__));
                        ctx = new QueuedMethodCallContext((*it), queuedCall);
                        if (!ctx) {
                            QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for context", __func__));
                            status = ER_FAIL;
                        } else {
                            ctx->method = element.method;
                            ctx->timeSent = GetTimeMsec();
                            QCC_DbgTrace(("%s: Calling %s.%s on lamp %s", __func__, element.interface.c_str(), element.method.c_str(), ctx->lampID.c_str()));
                            status = lit->second->object.MethodCallAsync(
                                element.interface.c_str(),
                                element.method.c_str(),
                                this,
                                queuedCall->replyFunc,
                                &element.args[0],
                                element.args.size(),
                                ctx,
                                LAMP_METHOD_CALL_TIMEOUT
                                );
                        }
                    }

                    if (status != ER_OK) {
                        QCC_LogError(status, ("%s: MethodCallAsync failed", __func__));
                        if (ctx) {
                            delete ctx;
                        }
                        pingFailedList.push_back(*it);
                        failures++;
                    } else {
                        lit->second->methodCallPending = true;
                        QCC_DbgPrintf(("%s: Set methodCallPending for Lamp ID %s to true", __func__, lit->first.c_str()));
                        if (recordInPendingCallList) {
                            std::list<ajn::Message> msgList;
                            msgList.clear();
                            pendingCallListLock.Lock();
                            QCC_DbgPrintf(("%s: Added LampID(%s) to pendingCallList", __func__, lit->first.c_str()));
                            pendingCallList.insert(std::make_pair(lit->first, std::make_pair(detail, msgList)));
                            pendingCallListLock.Unlock();
                        }
                    }
                }
            } else {
                notFound++;
            }
        }

        elementList.pop_front();
    }

    if (notFound || failures || busy) {
        DecrementWaitingAndSendResponse(queuedCall, 0, failures, notFound, busy);
    }

    while (pingFailedList.size()) {
        QCC_DbgPrintf(("%s: Sending Ping failed for lamp ID %s", __func__, pingFailedList.front().c_str()));
        SendPingReply(pingFailedList.front(), LSF_ERR_FAILURE, false);
        pingFailedList.pop_front();
    }

    return responseCode;
}

bool LampClients::CheckAndAddToPendingCallList(LSFString lampID, MethodCallDetail detail, Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));

    bool ret = false;
    pendingCallListLock.Lock();
    PendingCallMap::iterator it = pendingCallList.find(lampID);
    if (it == pendingCallList.end()) {
        QCC_DbgPrintf(("%s: No entry for Lamp ID %s found in pendingCallList", __func__, lampID.c_str()));
    } else {
        /*
         * Check if the interfaces are the same
         */
        QCC_DbgPrintf(("%s: Found entry for lamp %s", __func__, lampID.c_str()));
        QCC_DbgPrintf(("%s: Checking interface %s", __func__, detail.interface.c_str()));
        if (0 != strcmp(detail.interface.c_str(), it->second.first.interface.c_str())) {
            QCC_DbgPrintf(("%s: Interface %s != %s", __func__, detail.interface.c_str(), it->second.first.interface.c_str()));
        } else {
            /*
             * Check if the method call names are the same
             */
            if (0 != strcmp(detail.method.c_str(), it->second.first.method.c_str())) {
                QCC_DbgPrintf(("%s: Method Call %s != %s", __func__, detail.method.c_str(), it->second.first.method.c_str()));
            } else {
                /*
                 * Check if the first argument is the same
                 */
                if (0 != strcmp(detail.arg1.c_str(), it->second.first.arg1.c_str())) {
                    QCC_DbgPrintf(("%s: Argument %s != %s", __func__, detail.arg1.c_str(), it->second.first.arg1.c_str()));
                } else {
                    /*
                     * Check the second argument if there is one
                     */
                    if (detail.numArgs != it->second.first.numArgs) {
                        QCC_DbgPrintf(("%s: Argument list size %d != %d", __func__, detail.numArgs, it->second.first.numArgs));
                    } else {
                        if (detail.numArgs == 2) {
                            if (0 != strcmp(detail.arg2.c_str(), it->second.first.arg2.c_str())) {
                                QCC_DbgPrintf(("%s: Argument %s != %s", __func__, detail.arg2.c_str(), it->second.first.arg2.c_str()));
                            } else {
                                QCC_DbgPrintf(("%s: Found a pending call entry", __func__));
                                ret = true;
                                it->second.second.push_back(inMsg);
                            }
                        }
                    }
                }
            }
        }
    }
    pendingCallListLock.Unlock();

    return ret;
}

void LampClients::GetLampState(const LSFString& lampID, Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));

    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "GetAll");
    element.args.push_back(MsgArg("s", LampServiceStateInterfaceName));

    MethodCallDetail detail;
    detail.interface = org::freedesktop::DBus::Properties::InterfaceName;
    detail.method = "GetAll";
    detail.numArgs = 1;
    detail.arg1 = LampServiceStateInterfaceName;

    if (CheckAndAddToPendingCallList(lampID, detail, inMsg)) {
        QCC_DbgPrintf(("%s: Same Method Call already pending on the same lamp", __func__));
        return;
    }

    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("a{sv}", 0, NULL));
    QueueGetLampMethod(queuedCall);
}

void LampClients::GetLampStateField(const LSFString& lampID, const LSFString& field, Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "Get");
    element.args.push_back(MsgArg("s", LampServiceStateInterfaceName));
    element.args.push_back(MsgArg("s", field.c_str()));

    MethodCallDetail detail;
    detail.interface = org::freedesktop::DBus::Properties::InterfaceName;
    detail.method = "Get";
    detail.numArgs = 2;
    detail.arg1 = LampServiceStateInterfaceName;
    detail.arg2 = field;

    if (CheckAndAddToPendingCallList(lampID, detail, inMsg)) {
        QCC_DbgPrintf(("%s: Same Method Call already pending on the same lamp", __func__));
        return;
    }

    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", field.c_str()));
    MsgArg arg("u", 0);
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("v", &arg));
    QueueGetLampMethod(queuedCall);
}

void LampClients::GetLampDetails(const LSFString& lampID, Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "GetAll");
    element.args.push_back(MsgArg("s", LampServiceDetailsInterfaceName));

    MethodCallDetail detail;
    detail.interface = org::freedesktop::DBus::Properties::InterfaceName;
    detail.method = "GetAll";
    detail.numArgs = 1;
    detail.arg1 = LampServiceDetailsInterfaceName;

    if (CheckAndAddToPendingCallList(lampID, detail, inMsg)) {
        QCC_DbgPrintf(("%s: Same Method Call already pending on the same lamp", __func__));
        return;
    }

    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("a{sv}", 0, NULL));
    QueueGetLampMethod(queuedCall);
}

void LampClients::GetLampParameters(const LSFString& lampID, Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "GetAll");
    element.args.push_back(MsgArg("s", LampServiceParametersInterfaceName));

    MethodCallDetail detail;
    detail.interface = org::freedesktop::DBus::Properties::InterfaceName;
    detail.method = "GetAll";
    detail.numArgs = 1;
    detail.arg1 = LampServiceParametersInterfaceName;

    if (CheckAndAddToPendingCallList(lampID, detail, inMsg)) {
        QCC_DbgPrintf(("%s: Same Method Call already pending on the same lamp", __func__));
        return;
    }

    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("a{sv}", 0, NULL));
    QueueGetLampMethod(queuedCall);
}

void LampClients::GetLampParametersField(const LSFString& lampID, const LSFString& field, Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "Get");
    element.args.push_back(MsgArg("s", LampServiceParametersInterfaceName));
    element.args.push_back(MsgArg("s", field.c_str()));

    MethodCallDetail detail;
    detail.interface = org::freedesktop::DBus::Properties::InterfaceName;
    detail.method = "Get";
    detail.numArgs = 2;
    detail.arg1 = LampServiceParametersInterfaceName;
    detail.arg2 = field;

    if (CheckAndAddToPendingCallList(lampID, detail, inMsg)) {
        QCC_DbgPrintf(("%s: Same Method Call already pending on the same lamp", __func__));
        return;
    }

    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", field.c_str()));
    MsgArg arg("u", 0);
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("v", &arg));
    QueueGetLampMethod(queuedCall);
}

void LampClients::HandleGetReply(ajn::Message& message, void* context)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __func__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    /*
     * Handle only if we are still running
     */
    if (!isRunning) {
        QCC_DbgPrintf(("%s: Ignoring reply as we are stopping", __func__));
        return;
    }

    QueuedMethodCallContext* queuedCall = static_cast<QueuedMethodCallContext*>(context);

    if (queuedCall == NULL) {
        QCC_LogError(ER_FAIL, ("%s: Received NULL context", __func__));
        return;
    }

    QCC_DbgPrintf(("%s: Lamp ID %s", __func__, queuedCall->lampID.c_str()));

    if (MESSAGE_METHOD_RET == message->GetType()) {
        size_t numArgs;
        const MsgArg* args;
        message->GetArgs(numArgs, args);

        if (numArgs == 0) {
            DecrementWaitingAndSendResponse(queuedCall->queuedCallPtr, 1, 0, 0);
        } else {
            DecrementWaitingAndSendResponse(queuedCall->queuedCallPtr, 1, 0, 0, 0, args);
        }

        QCC_DbgPrintf(("%s: Ping successful for lamp ID %s", __func__, queuedCall->lampID.c_str()));
        SendPingReply(queuedCall->lampID, LSF_OK);
    } else {
        DecrementWaitingAndSendResponse(queuedCall->queuedCallPtr, 0, 1, 0);
        QCC_DbgPrintf(("%s: Ping failed for lamp ID %s", __func__, queuedCall->lampID.c_str()));
        SendPingReply(queuedCall->lampID, LSF_ERR_FAILURE);
    }

    delete queuedCall;
}

void LampClients::ChangeLampState(const ajn::Message& inMsg, bool groupOperation, bool sceneOperation, TransitionStateParamsList& transitionStateParams,
                                  TransitionStateFieldParamsList& transitionStateFieldparams, PulseStateParamsList& pulseParams, LSFString sceneOrMasterSceneID)
{
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithLampResponseCode));

    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }

    if (groupOperation || (sceneOperation && ((0 == strcmp(ControllerServiceSceneInterfaceName, inMsg->GetInterface())) || (0 == strcmp(ControllerServiceMasterSceneInterfaceName, inMsg->GetInterface()))))) {
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
        arrayVals[0].Set("{sv}", strdupnew(transitionStateFieldParam.field), new MsgArg(transitionStateFieldParam.value));
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

    if (!sceneOrMasterSceneID.empty()) {
        QCC_DbgPrintf(("%s: Recording sceneOrMasterSceneID %s", __func__, sceneOrMasterSceneID.c_str()));
        queuedCall->responseCounter.sceneOrMasterSceneID = sceneOrMasterSceneID;
    }

    QueueSetLampMethod(queuedCall);
}

void LampClients::DecrementWaitingAndSendResponse(QueuedMethodCall* queuedCall, uint32_t success, uint32_t failure, uint32_t notFound, uint32_t busyCount, const ajn::MsgArg* arg)
{
    QCC_DbgPrintf(("%s", __func__));
    LSFResponseCode responseCode = LSF_ERR_UNEXPECTED;
    bool sendResponse = false;

    queuedCall->responseCounterMutex.Lock();

    queuedCall->responseCounter.notFoundCount += notFound;
    queuedCall->responseCounter.numWaiting -= notFound;
    queuedCall->responseCounter.successCount += success;
    queuedCall->responseCounter.numWaiting -= success;
    queuedCall->responseCounter.failCount += failure;
    queuedCall->responseCounter.numWaiting -= failure;
    queuedCall->responseCounter.busyCount += busyCount;
    queuedCall->responseCounter.numWaiting -= busyCount;

    if (arg) {
        queuedCall->responseCounter.customReplyArgs.clear();
        queuedCall->responseCounter.customReplyArgs.push_back(arg[0]);
    }

    if (queuedCall->responseCounter.numWaiting == 0) {
        if (queuedCall->responseCounter.notFoundCount == queuedCall->responseCounter.total) {
            responseCode = LSF_ERR_NOT_FOUND;
            QCC_DbgPrintf(("%s: Response is LSF_ERR_NOT_FOUND for method %s", __func__, queuedCall->inMsg->GetMemberName()));
        } else if (queuedCall->responseCounter.successCount == queuedCall->responseCounter.total) {
            responseCode = LSF_OK;
            QCC_DbgPrintf(("%s: Response is LSF_OK for method %s", __func__, queuedCall->inMsg->GetMemberName()));
        } else if (queuedCall->responseCounter.failCount == queuedCall->responseCounter.total) {
            responseCode = LSF_ERR_FAILURE;
            QCC_DbgPrintf(("%s: Response is LSF_ERR_FAILURE for method %s", __func__, queuedCall->inMsg->GetMemberName()));
        } else if (queuedCall->responseCounter.busyCount == queuedCall->responseCounter.total) {
            responseCode = LSF_ERR_BUSY;
            QCC_DbgPrintf(("%s: Response is LSF_ERR_BUSY for method %s", __func__, queuedCall->inMsg->GetMemberName()));
        } else if ((queuedCall->responseCounter.notFoundCount + queuedCall->responseCounter.successCount + queuedCall->responseCounter.failCount + queuedCall->responseCounter.busyCount) == queuedCall->responseCounter.total) {
            responseCode = LSF_ERR_PARTIAL;
            QCC_DbgPrintf(("%s: Response is LSF_ERR_PARTIAL for method %s", __func__, queuedCall->inMsg->GetMemberName()));
        }

        sendResponse = true;
    }

    if (sendResponse) {
        if (strstr(queuedCall->inMsg->GetInterface(), ApplySceneEventActionInterfaceName)) {
            QCC_DbgPrintf(("%s: Skipping the sending of reply because interface = %s", __func__, queuedCall->inMsg->GetInterface()));
        } else {
            SendMethodReply(responseCode, queuedCall->inMsg, queuedCall->responseCounter.standardReplyArgs, queuedCall->responseCounter.customReplyArgs);

            /*
             * Check if we should respond to the same get request from other controller clients
             */
            if (queuedCall->responseCounter.total == 1) {
                std::list<ajn::Message> messageListCopy;
                messageListCopy.clear();
                pendingCallListLock.Lock();
                PendingCallMap::iterator it = pendingCallList.find(queuedCall->methodCallElements.front().lamps.front());
                if (it != pendingCallList.end()) {
                    QCC_DbgPrintf(("%s: Found the same method call pending", __func__));
                    messageListCopy = it->second.second;
                    pendingCallList.erase(it);
                }
                pendingCallListLock.Unlock();

                while (messageListCopy.size()) {
                    QCC_DbgPrintf(("%s: Sent reply for same method call pending", __func__));
                    SendMethodReply(responseCode, messageListCopy.front(), queuedCall->responseCounter.standardReplyArgs, queuedCall->responseCounter.customReplyArgs);
                    messageListCopy.pop_front();
                }
            }
        }

        if ((!(queuedCall->responseCounter.sceneOrMasterSceneID.empty())) && ((responseCode == LSF_OK) || (responseCode == LSF_ERR_PARTIAL))) {
            controllerService.SendSceneOrMasterSceneAppliedSignal(queuedCall->responseCounter.sceneOrMasterSceneID);
        }

        queuedCall->responseCounterMutex.Unlock();
        delete queuedCall;
    } else {
        queuedCall->responseCounterMutex.Unlock();
    }
}

void LampClients::HandleReplyWithLampResponseCode(Message& message, void* context)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __func__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    /*
     * Handle only if we are still running
     */
    if (!isRunning) {
        QCC_DbgPrintf(("%s: Ignoring reply as we are stopping", __func__));
        return;
    }

    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    QueuedMethodCallContext* queuedCall = static_cast<QueuedMethodCallContext*>(context);

    if (queuedCall == NULL) {
        QCC_LogError(ER_FAIL, ("%s: Received NULL context", __func__));
        return;
    }

    QCC_DbgPrintf(("%s: Lamp ID %s", __func__, queuedCall->lampID.c_str()));

    if (MESSAGE_METHOD_RET == message->GetType()) {
        size_t numArgs;
        const MsgArg* args;
        message->GetArgs(numArgs, args);

        QCC_DbgTrace(("%s: Received reply to call %s on lamp %s in %lu msec", __func__,
                      queuedCall->method.c_str(), queuedCall->lampID.c_str(), GetTimeMsec() - queuedCall->timeSent));

        LampResponseCode responseCode;
        args[0].Get("u", &responseCode);

        uint32_t success = 0;
        uint32_t failure = 0;

        if (responseCode == LAMP_OK) {
            success++;
        } else {
            failure++;
        }

        DecrementWaitingAndSendResponse(queuedCall->queuedCallPtr, success, failure, 0);
        QCC_DbgPrintf(("%s: Ping successful for lamp ID %s", __func__, queuedCall->lampID.c_str()));
        SendPingReply(queuedCall->lampID, LSF_OK);
    } else {
        DecrementWaitingAndSendResponse(queuedCall->queuedCallPtr, 0, 1, 0);
        QCC_DbgPrintf(("%s: Ping failed for lamp ID %s", __func__, queuedCall->lampID.c_str()));
        SendPingReply(queuedCall->lampID, LSF_ERR_FAILURE);
    }

    delete queuedCall;
}

void LampClients::GetLampFaults(const LSFString& lampID, ajn::Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "Get");
    element.args.push_back(MsgArg("s", LampServiceInterfaceName));
    element.args.push_back(MsgArg("s", "LampFaults"));

    MethodCallDetail detail;
    detail.interface = org::freedesktop::DBus::Properties::InterfaceName;
    detail.method = "Get";
    detail.numArgs = 2;
    detail.arg1 = LampServiceInterfaceName;
    detail.arg2 = "LampFaults";

    if (CheckAndAddToPendingCallList(lampID, detail, inMsg)) {
        QCC_DbgPrintf(("%s: Same Method Call already pending on the same lamp", __func__));
        return;
    }

    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithVariant));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("au", 0, NULL));
    QueueGetLampMethod(queuedCall);
}

void LampClients::GetLampVersion(const LSFString& lampID, ajn::Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, org::freedesktop::DBus::Properties::InterfaceName, "Get");
    element.args.push_back(MsgArg("s", LampServiceInterfaceName));
    element.args.push_back(MsgArg("s", "LampServiceVersion"));

    MethodCallDetail detail;
    detail.interface = org::freedesktop::DBus::Properties::InterfaceName;
    detail.method = "Get";
    detail.numArgs = 2;
    detail.arg1 = LampServiceInterfaceName;
    detail.arg2 = "LampServiceVersion";

    if (CheckAndAddToPendingCallList(lampID, detail, inMsg)) {
        QCC_DbgPrintf(("%s: Same Method Call already pending on the same lamp", __func__));
        return;
    }

    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithVariant));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("u", 0, NULL));
    QueueGetLampMethod(queuedCall);
}

void LampClients::HandleReplyWithVariant(ajn::Message& message, void* context)
{
    QCC_DbgTrace(("%s", __func__));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    /*
     * Handle only if we are still running
     */
    if (!isRunning) {
        QCC_DbgPrintf(("%s: Ignoring reply as we are stopping", __func__));
        return;
    }

    QueuedMethodCallContext* queuedCall = static_cast<QueuedMethodCallContext*>(context);

    if (queuedCall == NULL) {
        QCC_LogError(ER_FAIL, ("%s: Received NULL context", __func__));
        return;
    }

    QCC_DbgPrintf(("%s: Lamp ID %s", __func__, queuedCall->lampID.c_str()));

    if (MESSAGE_METHOD_RET == message->GetType()) {
        size_t numArgs;
        const MsgArg* args;
        message->GetArgs(numArgs, args);

        MsgArg* verArg;
        args[0].Get("v", &verArg);

        DecrementWaitingAndSendResponse(queuedCall->queuedCallPtr, 1, 0, 0, 0, verArg);
        QCC_DbgPrintf(("%s: Ping successful for lamp ID %s", __func__, queuedCall->lampID.c_str()));
        SendPingReply(queuedCall->lampID, LSF_OK);
    } else {
        DecrementWaitingAndSendResponse(queuedCall->queuedCallPtr, 0, 1, 0);
        QCC_DbgPrintf(("%s: Ping failed for lamp ID %s", __func__, queuedCall->lampID.c_str()));
        SendPingReply(queuedCall->lampID, LSF_ERR_FAILURE);
    }

    delete queuedCall;
}

void LampClients::ClearLampFault(const LSFString& lampID, LampFaultCode faultCode, ajn::Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithLampResponseCode));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, LampServiceInterfaceName, "ClearLampFault");
    element.args.push_back(MsgArg("u", faultCode));
    queuedCall->AddMethodCallElement(element);

    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("u", faultCode));

    QueueSetLampMethod(queuedCall);
}

void LampClients::LampStateChangedSignalHandler(const InterfaceDescription::Member* member, const char* sourcePath, Message& message)
{
    QCC_DbgTrace(("%s", __func__));
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
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, AboutInterfaceName, "GetAboutData");
    element.args.push_back(MsgArg("s", "en"));

    MethodCallDetail detail;
    detail.interface = AboutInterfaceName;
    detail.method = "GetAboutData";
    detail.numArgs = 1;
    detail.arg1 = "en";

    if (CheckAndAddToPendingCallList(lampID, detail, inMsg)) {
        QCC_DbgPrintf(("%s: Same Method Call already pending on the same lamp", __func__));
        return;
    }

    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithKeyValuePairs));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    queuedCall->AddMethodCallElement(element);

    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("as", 0, NULL));

    QueueGetLampMethod(queuedCall);
}

void LampClients::HandleReplyWithKeyValuePairs(Message& message, void* context)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __func__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    /*
     * Handle only if we are still running
     */
    if (!isRunning) {
        QCC_DbgPrintf(("%s: Ignoring reply as we are stopping", __func__));
        return;
    }

    QueuedMethodCallContext* queuedCall = static_cast<QueuedMethodCallContext*>(context);

    if (queuedCall == NULL) {
        QCC_LogError(ER_FAIL, ("%s: Received NULL context", __func__));
        return;
    }

    QCC_DbgPrintf(("%s: Lamp ID %s", __func__, queuedCall->lampID.c_str()));

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
            QCC_DbgPrintf(("%s: %s", __func__, key));
            if (((0 == strcmp(key, "SupportedLanguages")) && (0 == strcmp(queuedCall->queuedCallPtr->inMsg->GetMemberName(), "GetLampSupportedLanguages"))) ||
                ((0 == strcmp(key, "DeviceName")) && (0 == strcmp(queuedCall->queuedCallPtr->inMsg->GetMemberName(), "GetLampName"))) ||
                ((0 == strcmp(key, "Manufacturer")) && (0 == strcmp(queuedCall->queuedCallPtr->inMsg->GetMemberName(), "GetLampManufacturer")))) {
                DecrementWaitingAndSendResponse(queuedCall->queuedCallPtr, 1, 0, 0, 0, value);
                break;
            }
        }
        QCC_DbgPrintf(("%s: Ping successful for lamp ID %s", __func__, queuedCall->lampID.c_str()));
        SendPingReply(queuedCall->lampID, LSF_OK);
    } else {
        DecrementWaitingAndSendResponse(queuedCall->queuedCallPtr, 0, 1, 0);
        QCC_DbgPrintf(("%s: Ping failed for lamp ID %s", __func__, queuedCall->lampID.c_str()));
        SendPingReply(queuedCall->lampID, LSF_ERR_FAILURE);
    }

    delete queuedCall;
}

void LampClients::GetLampName(const LSFString& lampID, const LSFString& language, Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, ConfigServiceInterfaceName, "GetConfigurations");
    element.args.push_back(MsgArg("s", language.c_str()));

    MethodCallDetail detail;
    detail.interface = ConfigServiceInterfaceName;
    detail.method = "GetConfigurations";
    detail.numArgs = 1;
    detail.arg1 = language;

    if (CheckAndAddToPendingCallList(lampID, detail, inMsg)) {
        QCC_DbgPrintf(("%s: Same Method Call already pending on the same lamp", __func__));
        return;
    }

    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithKeyValuePairs));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", language.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("s", "<ERROR>"));
    QueueGetLampMethod(queuedCall);
}

void LampClients::PingLamp(const LSFString& lampID, Message& inMsg)
{
    QCC_DbgTrace(("%s: lampID=%s", __func__, lampID.c_str()));

    bool tempConnectToLamps = false;

    connectToLampsLock.Lock();
    tempConnectToLamps = connectToLamps;
    connectToLampsLock.Unlock();

    if (!tempConnectToLamps) {
        QCC_DbgPrintf(("%s: connectToLamps is false", __func__));
        controllerService.SendMethodReplyWithResponseCodeAndID(inMsg, LSF_ERR_REJECTED, lampID);
    } else {
        pingMapLock.Lock();
        PingMap::iterator it = pingMap.find(lampID);
        if (it != pingMap.end()) {
            QCC_DbgPrintf(("%s: Other controller clients are also waiting for a ping response from this lamp. Adding current message to the list.", __func__));
            it->second.push_back(inMsg);
        } else {
            QCC_DbgPrintf(("%s: Adding a new entry to the pingMap", __func__));
            std::list<ajn::Message> messageList;
            messageList.push_back(inMsg);
            pingMap.insert(std::make_pair(lampID, messageList));
        }
        pingMapLock.Unlock();

        wakeUp.Post();
    }
}

void LampClients::GetLampManufacturer(const LSFString& lampID, const LSFString& language, ajn::Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, ConfigServiceInterfaceName, "GetConfigurations");
    element.args.push_back(MsgArg("s", language.c_str()));

    MethodCallDetail detail;
    detail.interface = ConfigServiceInterfaceName;
    detail.method = "GetConfigurations";
    detail.numArgs = 1;
    detail.arg1 = language;

    if (CheckAndAddToPendingCallList(lampID, detail, inMsg)) {
        QCC_DbgPrintf(("%s: Same Method Call already pending on the same lamp", __func__));
        return;
    }

    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleReplyWithKeyValuePairs));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    queuedCall->AddMethodCallElement(element);
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", language.c_str()));
    queuedCall->responseCounter.customReplyArgs.push_back(MsgArg("s", "<ERROR>"));
    QueueGetLampMethod(queuedCall);
}

void LampClients::SetLampName(const LSFString& lampID, const LSFString& name, const LSFString& language, Message& inMsg)
{
    QCC_DbgTrace(("%s", __func__));
    QueuedMethodCall* queuedCall = new QueuedMethodCall(inMsg, static_cast<MessageReceiver::ReplyHandler>(&LampClients::HandleGetReply));
    if (!queuedCall) {
        QCC_LogError(ER_FAIL, ("%s: Unable to allocate memory for call", __func__));
        return;
    }
    QueuedMethodCallElement element = QueuedMethodCallElement(lampID, ConfigServiceInterfaceName, "UpdateConfigurations");

    MsgArg name_arg("s", name.c_str());
    MsgArg arg("{sv}", "DeviceName", &name_arg);

    element.args.push_back(MsgArg("s", language.c_str()));
    element.args.push_back(MsgArg("a{sv}", 1, &arg));
    queuedCall->AddMethodCallElement(element);

    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", lampID.c_str()));
    queuedCall->responseCounter.standardReplyArgs.push_back(MsgArg("s", language.c_str()));

    QueueSetLampMethod(queuedCall);
}

void LampClients::IntrospectCB(QStatus status, ajn::ProxyBusObject* obj, void* context)
{
    QCC_DbgPrintf(("%s: status = %s", __func__, QCC_StatusText(status)));

    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    /*
     * Handle only if we are still running
     */
    if (!isRunning) {
        QCC_DbgPrintf(("%s: Ignoring callback as we are stopping", __func__));
        return;
    }

    LampConnection* connection = static_cast<LampConnection*>(context);

    bool tempConnectToLamps = false;
    connectToLampsLock.Lock();
    tempConnectToLamps = connectToLamps;
    connectToLampsLock.Unlock();

    if (!tempConnectToLamps) {
        QCC_DbgPrintf(("%s: connectToLamps is false", __func__));
        delete connection;
        return;
    }

    if (ER_OK != status) {
        controllerService.DoLeaveSessionAsync(connection->sessionID);
        connection->sessionID = 0;
        connection->object = ProxyBusObject();
        connection->configObject = ProxyBusObject();
        connection->aboutObject = ProxyBusObject();
        connection->methodCallPending = false;
        sentNGNSPingsLock.Lock();
        LampMap::iterator lit = sentNGNSPings.find(connection->lampId);
        if (lit == sentNGNSPings.end()) {
            QCC_DbgPrintf(("%s: Retrying Ping due to IntrospectCB failure", __func__));
            if (controllerService.GetBusAttachment().PingAsync(connection->busName.c_str(), NGNS_PING_TIMEOUT_IN_MS, this, connection) != ER_OK) {
                QCC_LogError(status, ("%s: Sending PingAsync failed for Lamp ID = %s", __func__, connection->lampId.c_str()));
                delete connection;
            } else {
                QCC_DbgPrintf(("%s: NGNS Ping successfully sent for lamp ID %s", __func__, connection->lampId.c_str()));
                sentNGNSPings.insert(std::make_pair(connection->lampId, connection));
            }
        } else {
            QCC_DbgPrintf(("%s: NGNS Ping already in progress for lamp ID %s", __func__, connection->lampId.c_str()));
        }
        sentNGNSPingsLock.Unlock();
        return;
    }

    /*
     * Do not introspect the remote Config and About object!
     */
    const InterfaceDescription* intf = controllerService.GetBusAttachment().GetInterface(ConfigServiceInterfaceName);
    QStatus tempStatus = connection->configObject.AddInterface(*intf);
    QCC_DbgPrintf(("%s: connection->configObject.AddInterface returns %s\n", __func__, QCC_StatusText(tempStatus)));
    if (ER_OK == tempStatus) {
        intf = controllerService.GetBusAttachment().GetInterface(AboutInterfaceName);
        tempStatus = connection->aboutObject.AddInterface(*intf);
        QCC_DbgPrintf(("%s: connection->aboutObject.AddInterface returns %s\n", __func__, QCC_StatusText(tempStatus)));

        if (ER_OK == tempStatus) {
            if (!lampStateChangedSignalHandlerRegistered) {
                intf = connection->object.GetInterface(LampServiceStateInterfaceName);
                if (intf) {
                    const InterfaceDescription::Member* sig = intf->GetMember("LampStateChanged");
                    if (sig) {
                        tempStatus = controllerService.GetBusAttachment().RegisterSignalHandler(this, static_cast<MessageReceiver::SignalHandler>(&LampClients::LampStateChangedSignalHandler), sig, LampServiceObjectPath);
                        QCC_DbgPrintf(("%s: RegisterSignalHandler returns %s\n", __func__, QCC_StatusText(tempStatus)));

                        if (ER_OK == tempStatus) {
                            lampStateChangedSignalHandlerRegistered = true;
                        }
                    }
                }
            }

            if (ER_OK == tempStatus) {
                tempStatus = joinSessionCBListLock.Lock();
                if (ER_OK != tempStatus) {
                    QCC_LogError(tempStatus, ("%s: joinSessionCBListLock.Lock() failed", __func__));
                } else {
                    joinSessionCBList.push_back(connection);
                    QCC_DbgPrintf(("%s: Add %s to joinSessionCBList", __func__, connection->lampId.c_str()));
                    if (ER_OK != joinSessionCBListLock.Unlock()) {
                        QCC_LogError(tempStatus, ("%s: joinSessionCBListLock.Unlock() failed", __func__));
                    }
                    wakeUp.Post();
                }
            }
        }
    }

    if (ER_OK != tempStatus) {
        controllerService.DoLeaveSessionAsync(connection->sessionID);
        connection->sessionID = 0;
        connection->object = ProxyBusObject();
        connection->configObject = ProxyBusObject();
        connection->aboutObject = ProxyBusObject();
        connection->methodCallPending = false;

        sentNGNSPingsLock.Lock();
        LampMap::iterator lit = sentNGNSPings.find(connection->lampId);
        if (lit == sentNGNSPings.end()) {
            QCC_DbgPrintf(("%s: Retrying Ping due to ProxyBusObject setup failure", __func__));
            if (controllerService.GetBusAttachment().PingAsync(connection->busName.c_str(), NGNS_PING_TIMEOUT_IN_MS, this, connection) != ER_OK) {
                QCC_LogError(status, ("%s: Sending PingAsync failed for Lamp ID = %s", __func__, connection->lampId.c_str()));
                delete connection;
            } else {
                QCC_DbgPrintf(("%s: NGNS Ping successfully sent for lamp ID %s", __func__, connection->lampId.c_str()));
                sentNGNSPings.insert(std::make_pair(connection->lampId, connection));
            }
        } else {
            QCC_DbgPrintf(("%s: NGNS Ping already in progress for lamp ID %s", __func__, connection->lampId.c_str()));
        }
        sentNGNSPingsLock.Unlock();
    }
}

void LampClients::SessionPingCB(Message& reply, void* context)
{
    QCC_DbgPrintf(("%s", __func__));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    /*
     * Handle only if we are still running
     */
    if (!isRunning) {
        QCC_DbgPrintf(("%s: Ignoring callback as we are stopping", __func__));
        return;
    }

    bool tempConnectToLamps = false;
    connectToLampsLock.Lock();
    tempConnectToLamps = connectToLamps;
    connectToLampsLock.Unlock();

    if (context && tempConnectToLamps) {
        LampConnection* connection = static_cast<LampConnection*>(context);
        if (reply->GetType() == MESSAGE_ERROR) {
            QCC_LogError(ER_FAIL, ("%s: Session Ping un-successful for Lamp ID = %s", __func__, connection->lampId.c_str()));
            SendPingReply(connection->lampId, LSF_ERR_FAILURE);
        } else {
            QCC_DbgPrintf(("%s: Session Ping successful for Lamp ID = %s", __func__, connection->lampId.c_str()));
            SendPingReply(connection->lampId, LSF_OK);
        }
    } else {
        QCC_DbgPrintf(("%s: connectToLamps is false or context is NULL", __func__));
    }
}

void LampClients::PingCB(QStatus status, void* context)
{
    QCC_DbgPrintf(("%s: status=%s", __func__, QCC_StatusText(status)));
    controllerService.GetBusAttachment().EnableConcurrentCallbacks();

    /*
     * Only handle ping callback if we are still running
     */
    if (!isRunning) {
        QCC_DbgPrintf(("%s: Ignoring callback as we are stopping", __func__));
        return;
    }

    bool tempConnectToLamps = false;
    connectToLampsLock.Lock();
    tempConnectToLamps = connectToLamps;
    connectToLampsLock.Unlock();

    LampConnection* connection = static_cast<LampConnection*>(context);

    if (connection) {
        if (!tempConnectToLamps) {
            delete connection;
            return;
        } else {
            if (ER_OK == status) {
                QCC_DbgPrintf(("%s: Ping successful for Lamp ID = %s", __func__, connection->lampId.c_str()));

                sentNGNSPingsLock.Lock();
                LampMap::iterator lit = sentNGNSPings.find(connection->lampId);
                if (lit != sentNGNSPings.end()) {
                    QCC_DbgPrintf(("%s: Removing entry for lamp ID %s from sentNGNSPings", __func__, connection->lampId.c_str()));
                    sentNGNSPings.erase(lit);
                }
                sentNGNSPingsLock.Unlock();

                if (connection->sessionID == 0) {
                    QCC_DbgPrintf(("%s: Adding the connection details for lamp ID %s to aboutsList", __func__, connection->lampId.c_str()));
                    QStatus tempStatus = aboutsListLock.Lock();
                    if (ER_OK != tempStatus) {
                        QCC_LogError(tempStatus, ("%s: aboutsListLock.Lock() failed", __func__));
                        delete connection;
                        return;
                    }
                    LampMap::iterator it = aboutsList.find(connection->lampId);
                    if (it == aboutsList.end()) {
                        if (aboutsList.size() < (MAX_SUPPORTED_LAMPS + 25)) {
                            aboutsList.insert(std::make_pair(connection->lampId, connection));
                            wakeUp.Post();
                        } else {
                            QCC_LogError(status, ("%s: Controller Service can cache only a maximum of %d announcements. Max'ed out on the capacity. Ignoring the successful ping to announcement", __func__, (MAX_SUPPORTED_LAMPS + 25)));
                            delete connection;
                        }
                    } else {
                        QCC_DbgPrintf(("%s: Entry already exists for Lamp ID = %s", __func__, connection->lampId.c_str()));
                        delete connection;
                    }
                    tempStatus = aboutsListLock.Unlock();
                    if (ER_OK != tempStatus) {
                        QCC_LogError(tempStatus, ("%s: aboutsListLock.Unlock() failed", __func__));
                    }
                }
            } else {
                if (ER_ALLJOYN_PING_REPLY_TIMEOUT == status) {
                    sentNGNSPingsLock.Lock();
                    LampMap::iterator lit = sentNGNSPings.find(connection->lampId);
                    if (lit == sentNGNSPings.end()) {
                        QCC_DbgPrintf(("%s: Retrying Ping due to timeout", __func__));
                        if (controllerService.GetBusAttachment().PingAsync(connection->busName.c_str(), NGNS_PING_TIMEOUT_IN_MS, this, connection) != ER_OK) {
                            QCC_LogError(status, ("%s: Sending PingAsync failed for Lamp ID = %s", __func__, connection->lampId.c_str()));

                            if (connection->sessionID == 0) {
                                QCC_DbgPrintf(("%s: Sending PingAsync failed for lamp %s which is not in the active lamps list", __func__, connection->lampId.c_str()));
                                delete connection;
                            } else {
                                QCC_DbgPrintf(("%s: Sending PingAsync failed for lamp %s which is in the active lamps list", __func__, connection->lampId.c_str()));
                                lostLampsLock.Lock();
                                lostLamps.push_back(connection->lampId);
                                lostLampsLock.Unlock();
                                wakeUp.Post();
                            }
                        } else {
                            QCC_DbgPrintf(("%s: NGNS Ping successfully sent for lamp ID %s", __func__, connection->lampId.c_str()));
                            sentNGNSPings.insert(std::make_pair(connection->lampId, connection));
                        }
                    } else {
                        QCC_DbgPrintf(("%s: NGNS Ping already in progress for lamp ID %s", __func__, connection->lampId.c_str()));
                    }
                    sentNGNSPingsLock.Unlock();
                } else {
                    QCC_LogError(status, ("%s: NGNS Ping un-successful for Lamp ID = %s", __func__, connection->lampId.c_str()));
                    sentNGNSPingsLock.Lock();
                    LampMap::iterator it = sentNGNSPings.find(connection->lampId);
                    if (it != sentNGNSPings.end()) {
                        QCC_LogError(status, ("%s: Removed Lamp ID = %s from sentNGNSPings", __func__, connection->lampId.c_str()));
                        sentNGNSPings.erase(it);
                    }
                    sentNGNSPingsLock.Unlock();

                    if (connection->sessionID == 0) {
                        QCC_DbgPrintf(("%s: NGNS ping failed for lamp %s which is not in the active lamps list", __func__, connection->lampId.c_str()));
                        delete connection;
                    } else {
                        QCC_DbgPrintf(("%s: NGNS ping failed for lamp %s which is in the active lamps list", __func__, connection->lampId.c_str()));
                        lostLampsLock.Lock();
                        lostLamps.push_back(connection->lampId);
                        lostLampsLock.Unlock();
                        wakeUp.Post();
                    }
                }
            }
        }
    } else {
        QCC_DbgPrintf(("%s: context is NULL", __func__));
    }
}

void LampClients::SendPingReply(LSFString lampID, LSFResponseCode responseCode, bool decrementMethodCallCount)
{
    QCC_DbgPrintf(("%s: Lamp ID = %s", __func__, lampID.c_str()));

    bool tempConnectToLamps = false;

    connectToLampsLock.Lock();
    tempConnectToLamps = connectToLamps;
    connectToLampsLock.Unlock();

    if (!tempConnectToLamps) {
        QCC_DbgPrintf(("%s: connectToLamps is false", __func__));
    } else {
        std::list<ajn::Message> messageList;

        pingMapLock.Lock();
        PingMap::iterator it = pingMap.find(lampID);
        if (it != pingMap.end()) {
            QCC_DbgPrintf(("%s: Copied the messages to respond to", __func__));
            messageList = it->second;
            pingMap.erase(it);
        } else {
            QCC_DbgPrintf(("%s: No waiting ping requests for Lamp ID = %s", __func__, lampID.c_str()));
        }
        pingMapLock.Unlock();

        while (messageList.size()) {
            controllerService.SendMethodReplyWithResponseCodeAndID(messageList.front(), responseCode, lampID);
            messageList.pop_front();
        }

        if (LSF_ERR_FAILURE == responseCode) {
            methodCallsFailedLock.Lock();
            methodCallsFailed.push_back(lampID);
            methodCallsFailedLock.Unlock();
        }

        if (decrementMethodCallCount) {
            methodRepliesReceivedLock.Lock();
            methodRepliesReceived.push_back(lampID);
            methodRepliesReceivedLock.Unlock();
        }

        wakeUp.Post();
    }
}

QStatus LampClients::UnregisterAnnounceHandler(void)
{
    QCC_DbgPrintf(("%s", __func__));
    QStatus status = services::AnnouncementRegistrar::UnRegisterAnnounceHandler(controllerService.GetBusAttachment(), *serviceHandler, interfaces, sizeof(interfaces) / sizeof(interfaces[0]));
    if (ER_OK == status) {
        QCC_DbgPrintf(("%s: UnRegisterAnnounceHandler successful", __func__));
    } else {
        QCC_LogError(status, ("%s: UnRegisterAnnounceHandler unsuccessful", __func__));
    }
    return status;
}

QStatus LampClients::RegisterAnnounceHandler(void)
{
    QCC_DbgPrintf(("%s", __func__));
    QStatus status = services::AnnouncementRegistrar::RegisterAnnounceHandler(controllerService.GetBusAttachment(), *serviceHandler, interfaces, sizeof(interfaces) / sizeof(interfaces[0]));
    if (ER_OK == status) {
        QCC_DbgPrintf(("%s: RegisterAnnounceHandler successful", __func__));
    } else {
        QCC_LogError(status, ("%s: RegisterAnnounceHandler unsuccessful", __func__));
    }
    return status;
}

void LampClients::Run(void)
{
    QCC_DbgTrace(("%s", __func__));

    while (isRunning) {
        /*
         * Wait for something to happen
         */
        QCC_DbgPrintf(("%s: Waiting on wakeUp", __func__));
        wakeUp.Wait();
        QStatus status = ER_OK;

        bool tempConnectToLamps = false;

        connectToLampsLock.Lock();
        tempConnectToLamps = connectToLamps;
        connectToLampsLock.Unlock();

        if (tempConnectToLamps) {
            QCC_DbgPrintf(("%s: In the ConnectToLamps loop", __func__));

            /*
             * Handle all the lost sessions
             */
            std::set<uint32_t> tempLostSessionList;
            tempLostSessionList.clear();
            status = lostSessionListLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: lostSessionListLock.Lock() failed", __func__));
                return;
            }
            /*
             * Make a local copy and release lostSessionList for use by Session Losts
             */
            tempLostSessionList = lostSessionList;
            lostSessionList.clear();
            status = lostSessionListLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: lostSessionListLock.Unlock() failed", __func__));
            }

            if (tempLostSessionList.size()) {
                for (LampMap::iterator it = activeLamps.begin(); it != activeLamps.end();) {
                    if (tempLostSessionList.find((uint32_t)it->second->sessionID) != tempLostSessionList.end()) {
                        bool okToErase = false;
                        sentNGNSPingsLock.Lock();
                        LampMap::iterator lit = sentNGNSPings.find(it->second->lampId);
                        if (lit == sentNGNSPings.end()) {
                            okToErase = true;
                            it->second->sessionID = 0;
                            it->second->object = ProxyBusObject();
                            it->second->configObject = ProxyBusObject();
                            it->second->aboutObject = ProxyBusObject();
                            it->second->methodCallPending = false;
                            QCC_DbgPrintf(("%s: Retrying Ping due to lost session", __func__));
                            if (controllerService.GetBusAttachment().PingAsync(it->second->busName.c_str(), NGNS_PING_TIMEOUT_IN_MS, this, it->second) != ER_OK) {
                                QCC_LogError(status, ("%s: Sending PingAsync failed for Lamp ID = %s", __func__, it->second->lampId.c_str()));
                                delete it->second;
                            } else {
                                QCC_DbgPrintf(("%s: NGNS Ping successfully sent for lamp ID %s", __func__, it->second->lampId.c_str()));
                                sentNGNSPings.insert(std::make_pair(it->second->lampId, it->second));
                            }
                        } else {
                            QCC_DbgPrintf(("%s: NGNS Ping already in progress for lamp ID %s", __func__, it->second->lampId.c_str()));
                        }
                        sentNGNSPingsLock.Unlock();
                        QCC_DbgPrintf(("%s: Removing %s from activeLamps", __func__, it->second->lampId.c_str()));
                        if (okToErase) {
                            activeLamps.erase(it++);
                        }
                    } else {
                        ++it;
                    }
                }
            }

            LSFStringList lostLampsList;
            lostLampsList.clear();

            LSFStringList lostLampsBackup;
            lostLampsBackup.clear();

            lostLampsLock.Lock();
            lostLampsList = lostLamps;
            lostLampsBackup = lostLamps;
            lostLamps.clear();
            lostLampsLock.Unlock();

            if (lostLampsBackup.size()) {
                controllerService.SendSignal(ControllerServiceLampInterfaceName, "LampsLost", lostLampsBackup);
            }

            /*
             * Clean up all Lamps
             */
            while (lostLampsList.size()) {
                LampMap::iterator it = activeLamps.find(lostLampsList.front());
                if (it != activeLamps.end()) {
                    LampConnection* connection = it->second;
                    controllerService.DoLeaveSessionAsync(connection->sessionID);
                    activeLamps.erase(it);
                    delete connection;
                }
                lostLampsList.pop_front();
            }

            /*
             * Update method call replies count
             */
            LSFStringList methodRepliesReceivedCopy;
            methodRepliesReceivedLock.Lock();
            methodRepliesReceivedCopy = methodRepliesReceived;
            methodRepliesReceived.clear();
            methodRepliesReceivedLock.Unlock();

            while (methodRepliesReceivedCopy.size()) {
                LampMap::iterator it = activeLamps.find(methodRepliesReceivedCopy.front());
                if (it != activeLamps.end()) {
                    it->second->methodCallPending = false;
                    QCC_DbgPrintf(("%s: Set methodCallPending for lamp ID %s to false", __func__, methodRepliesReceivedCopy.front().c_str()));
                }
                methodRepliesReceivedCopy.pop_front();
            }

            LSFStringList methodCallsFailedCopy;
            methodCallsFailedCopy.clear();

            /*
             * Send NGNS pings to lamps for which method calls failed
             */
            methodCallsFailedLock.Lock();
            methodCallsFailedCopy = methodCallsFailed;
            methodCallsFailed.clear();
            methodCallsFailedLock.Unlock();

            sentNGNSPingsLock.Lock();
            while (methodCallsFailedCopy.size()) {
                LampMap::iterator it = activeLamps.find(methodCallsFailedCopy.front());
                if (it != activeLamps.end()) {
                    LampConnection* connection = it->second;
                    connection->methodCallPending = false;

                    LampMap::iterator lit = sentNGNSPings.find(it->first);
                    if (lit == sentNGNSPings.end()) {
                        QCC_LogError(status, ("%s: Retrying NGNS Ping as method call failed", __func__));
                        if (controllerService.GetBusAttachment().PingAsync(it->second->busName.c_str(), NGNS_PING_TIMEOUT_IN_MS, this, it->second) != ER_OK) {
                            QCC_LogError(status, ("%s: Sending PingAsync failed for Lamp ID = %s", __func__, it->second->lampId.c_str()));
                            controllerService.DoLeaveSessionAsync(connection->sessionID);
                            activeLamps.erase(it);
                            delete it->second;
                        } else {
                            QCC_DbgPrintf(("%s: NGNS Ping successfully sent for lamp ID %s", __func__, it->first.c_str()));
                            sentNGNSPings.insert(std::make_pair(it->first, it->second));
                        }
                    } else {
                        QCC_DbgPrintf(("%s: NGNS Ping already in progress for lamp ID %s", __func__, it->first.c_str()));
                    }
                }
                methodCallsFailedCopy.pop_front();
            }
            sentNGNSPingsLock.Unlock();

            /*
             * Handle all received About Announcements as appropriate
             */
            LampMap tempAboutList;
            tempAboutList.clear();

            /*
             * Handle announcements
             */
            status = aboutsListLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: aboutsListLock.Lock() failed", __func__));
            } else {
                /*
                 * Make a local copy of aboutsList and release the list for use by the About Handler
                 */
                tempAboutList = aboutsList;
                aboutsList.clear();
                status = aboutsListLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: aboutsListLock.Unlock() failed", __func__));
                }
            }

            LSFStringList nameChangedList;
            std::list<LampConnection*> joinSessionList;
            joinSessionList.clear();
            for (LampMap::iterator it = tempAboutList.begin(); it != tempAboutList.end(); it++) {
                LampConnection* newConn = it->second;
                LampMap::const_iterator lit = activeLamps.find(newConn->lampId);
                if (lit != activeLamps.end()) {
                    QCC_DbgPrintf(("%s: Got another announcement for a lamp that we already know about", __func__));
                    /*
                     * We already know about this Lamp
                     */
                    LampConnection* conn = lit->second;
                    if (conn->name != newConn->name) {
                        conn->name = newConn->name;
                        nameChangedList.push_back(conn->lampId);
                        QCC_DbgPrintf(("%s: Name Changed for %s", __func__, newConn->lampId.c_str()));
                    }
                    delete newConn;
                } else {
                    /*
                     * We should only support a maximum of MAX_SUPPORTED_LAMPS lamps at
                     * any given point in time
                     */
                    joinSessionInProgressLock.Lock();
                    if (joinSessionInProgressList.find(newConn->lampId) != joinSessionInProgressList.end()) {
                        QCC_DbgPrintf(("%s: Got another announcement for a lamp that we already know about", __func__));
                    }

                    if ((joinSessionInProgressList.find(newConn->lampId) == joinSessionInProgressList.end()) && (activeLamps.size() < MAX_SUPPORTED_LAMPS)) {
                        joinSessionInProgressList.insert(newConn->lampId);
                        joinSessionList.push_back(newConn);
                        QCC_DbgPrintf(("%s: Added %s to JoinSession List", __func__, newConn->lampId.c_str()));
                    } else {
                        QCC_DbgPrintf(("%s: No slot for connection with a new lamp or connect attempt to Lamp already in progress", __func__));
                        delete newConn;
                    }
                    joinSessionInProgressLock.Unlock();
                }
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
                    joinSessionInProgressLock.Lock();
                    joinSessionInProgressList.erase(newConn->lampId);
                    joinSessionInProgressLock.Unlock();
                    delete newConn;
                }
                joinSessionList.pop_front();
            }

            /*
             * Handle all the successful Join Sessions
             */
            std::list<LampConnection*> tempJoinList;
            tempJoinList.clear();

            /*
             * Update the active lamps list
             */
            status = joinSessionCBListLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: joinSessionCBListLock.Lock() failed", __func__));
            } else {
                /*
                 * Make a local copy of joinSessionCBList and release the list for use by the Join Session callbacks
                 */
                tempJoinList = joinSessionCBList;
                joinSessionCBList.clear();
                status = joinSessionCBListLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: joinSessionCBListLock.Unlock() failed", __func__));
                }
            }

            std::list<LampConnection*> leaveSessionList;
            leaveSessionList.clear();

            LSFStringList foundLamps;
            foundLamps.clear();

            while (tempJoinList.size()) {
                LampConnection* newConn = tempJoinList.front();
                /*
                 * We should only support a maximum of MAX_SUPPORTED_LAMPS lamps at
                 * any given point in time
                 */
                if (activeLamps.size() < MAX_SUPPORTED_LAMPS) {
                    activeLamps.insert(std::make_pair(newConn->lampId, newConn));
                    joinSessionInProgressLock.Lock();
                    joinSessionInProgressList.erase(newConn->lampId);
                    joinSessionInProgressLock.Unlock();
                    QCC_DbgPrintf(("%s: Added new connection for %s to activeLamps", __func__, newConn->lampId.c_str()));
                    foundLamps.push_back(newConn->lampId);
                } else {
                    QCC_DbgPrintf(("%s: No slot for connection with a new lamp", __func__));
                    leaveSessionList.push_back(newConn);
                    QCC_DbgPrintf(("%s: Added connection for %s to leaveSessionList", __func__, newConn->lampId.c_str()));
                }
                tempJoinList.pop_front();
            }

            /*
             * Send the found lamps signal if required
             */
            if (foundLamps.size()) {
                controllerService.SendSignal(ControllerServiceLampInterfaceName, "LampsFound", foundLamps);
            }

            /*
             * Tear down the unwanted sessions
             */
            while (leaveSessionList.size()) {
                LampConnection* connection = leaveSessionList.front();
                controllerService.DoLeaveSessionAsync(connection->sessionID);
                joinSessionInProgressLock.Lock();
                joinSessionInProgressList.erase(connection->lampId);
                joinSessionInProgressLock.Unlock();
                delete connection;
                leaveSessionList.pop_front();
            }

            /*
             * Handle all the incoming Set requests
             */
            std::list<QueuedMethodCall*> tempMethodQueue;
            tempMethodQueue.clear();
            status = setQueueLock.Lock();
            if (status != ER_OK) {
                QCC_LogError(status, ("%s: setQueueLock.Lock() failed", __func__));
            } else {
                /*
                 * Make a local copy and release setMethodQueue
                 */
                tempMethodQueue = setMethodQueue;
                setMethodQueue.clear();
                status = setQueueLock.Unlock();
                if (status != ER_OK) {
                    QCC_LogError(status, ("%s: setQueueLock.Unlock() failed", __func__));
                }
            }

            while (tempMethodQueue.size()) {
                QueuedMethodCall* queuedCall = tempMethodQueue.front();
                QCC_DbgPrintf(("%s: Calling DoMethodCallAsync with tempMethodQueue.size() %d", __func__, tempMethodQueue.size()));
                DoMethodCallAsync(queuedCall);
                tempMethodQueue.pop_front();
            }

            /*
             * Handle all the incoming Get requests
             */
            tempMethodQueue.clear();
            status = getQueueLock.Lock();
            if (status != ER_OK) {
                QCC_LogError(status, ("%s: getQueueLock.Lock() failed", __func__));
            } else {
                /*
                 * Make a local copy and release getMethodQueue
                 */
                tempMethodQueue = getMethodQueue;
                getMethodQueue.clear();
                status = getQueueLock.Unlock();
                if (status != ER_OK) {
                    QCC_LogError(status, ("%s: getQueueLock.Unlock() failed", __func__));
                }
            }

            while (tempMethodQueue.size()) {
                QueuedMethodCall* queuedCall = tempMethodQueue.front();
                QCC_DbgPrintf(("%s: Calling DoMethodCallAsync with tempMethodQueue.size() %d", __func__, tempMethodQueue.size()));
                DoMethodCallAsync(queuedCall, true);
                tempMethodQueue.pop_front();
            }

            PingMap notFoundMap;
            LSFStringList sendSessionPingList;

            pingMapLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: pingMapLock.Lock() failed", __func__));
            } else {
                for (PingMap::iterator it = pingMap.begin(); it != pingMap.end();) {
                    LampMap::iterator lit = activeLamps.find(it->first);
                    if (lit != activeLamps.end()) {
                        if (!lit->second->methodCallPending) {
                            QCC_DbgPrintf(("%s: Adding lamp ID %s to sendSessionPingList", __func__, it->first.c_str()));
                            sendSessionPingList.push_back(it->first);
                        } else {
                            QCC_DbgPrintf(("%s: Method call already pending for lamp ID %s", __func__, it->first.c_str()));
                        }
                        it++;
                    } else {
                        QCC_DbgPrintf(("%s: Could not ping as lamp ID %s in not in activeLamps", __func__, it->first.c_str()));
                        notFoundMap.insert(std::make_pair(it->first, it->second));
                        pingMap.erase(it++);
                    }
                }
                status = pingMapLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: pingMapLock.Unlock() failed", __func__));
                }
            }

            LSFStringList failedPings;
            while (sendSessionPingList.size()) {
                LampMap::iterator it = activeLamps.find(sendSessionPingList.front());
                if (it != activeLamps.end()) {
                    status = it->second->object.MethodCallAsync(
                        org::freedesktop::DBus::Peer::InterfaceName,
                        "Ping",
                        this,
                        static_cast<MessageReceiver::ReplyHandler>(&LampClients::SessionPingCB),
                        NULL, 0,
                        it->second,
                        SESSION_PING_TIMEOUT_IN_MS
                        );

                    if (status != ER_OK) {
                        QCC_LogError(status, ("%s: Sending Session Ping failed", __func__));
                        failedPings.push_back(it->first);
                        sentNGNSPingsLock.Lock();
                        LampMap::iterator lit = sentNGNSPings.find(it->first);
                        if (lit == sentNGNSPings.end()) {
                            it->second->methodCallPending = false;
                            QCC_LogError(status, ("%s: Retrying NGNS Ping as Session Ping failed", __func__));
                            if (controllerService.GetBusAttachment().PingAsync(it->second->busName.c_str(), NGNS_PING_TIMEOUT_IN_MS, this, it->second) != ER_OK) {
                                QCC_LogError(status, ("%s: Sending PingAsync failed for Lamp ID = %s", __func__, it->second->lampId.c_str()));
                                controllerService.DoLeaveSessionAsync(it->second->sessionID);
                                delete it->second;
                                activeLamps.erase(it);
                            } else {
                                QCC_DbgPrintf(("%s: NGNS Ping successfully sent for lamp ID %s", __func__, it->first.c_str()));
                                sentNGNSPings.insert(std::make_pair(it->first, it->second));
                            }
                        } else {
                            QCC_DbgPrintf(("%s: NGNS Ping already in progress for lamp ID %s", __func__, it->first.c_str()));
                        }
                        sentNGNSPingsLock.Unlock();
                    } else {
                        QCC_DbgPrintf(("%s: Successfully sent a Session Ping", __func__));
                        it->second->methodCallPending = true;
                        QCC_DbgPrintf(("%s: Set methodCallPending for Lamp ID %s to true", __func__, it->first.c_str()));
                    }
                }
                sendSessionPingList.pop_front();
            }

            for (PingMap::iterator it = notFoundMap.begin(); it != notFoundMap.end(); it++) {
                while (it->second.size()) {
                    controllerService.SendMethodReplyWithResponseCodeAndID(it->second.front(), LSF_ERR_NOT_FOUND, it->first);
                    it->second.pop_front();
                }
            }

            PingMap failedMap;

            pingMapLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: pingMapLock.Lock() failed", __func__));
            } else {
                while (failedPings.size()) {
                    PingMap::iterator pit = pingMap.find(failedPings.front());
                    if (pit != pingMap.end()) {
                        failedMap.insert(std::make_pair(pit->first, pit->second));
                        pingMap.erase(pit);
                    }
                    failedPings.pop_front();
                }
                status = pingMapLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: pingMapLock.Unlock() failed", __func__));
                }
            }

            for (PingMap::iterator it = failedMap.begin(); it != failedMap.end(); it++) {
                while (it->second.size()) {
                    controllerService.SendMethodReplyWithResponseCodeAndID(it->second.front(), LSF_ERR_FAILURE, it->first);
                    it->second.pop_front();
                }
            }

            /*
             * Send all GetAllLampIDs responses
             */
            std::list<Message> tempGetAllLampIDsRequests;
            tempGetAllLampIDsRequests.clear();
            status = getAllLampIDsLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(ER_FAIL, ("%s: getAllLampIDsLock.Lock() failed", __func__));
            } else {
                tempGetAllLampIDsRequests = getAllLampIDsRequests;
                getAllLampIDsRequests.clear();
                status = getAllLampIDsLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(ER_FAIL, ("%s: getAllLampIDsLock.Unlock() failed", __func__));
                }
            }

            if (tempGetAllLampIDsRequests.size()) {
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
                    QCC_DbgPrintf(("%s: Sending GetAllLampIDs reply with with tempGetAllLampIDsRequests.size() %d", __func__, tempGetAllLampIDsRequests.size()));
                    controllerService.SendMethodReplyWithResponseCodeAndListOfIDs(tempGetAllLampIDsRequests.front(), responseCode, idList);
                    tempGetAllLampIDsRequests.pop_front();
                }
            }

        } else {
            QCC_DbgPrintf(("%s: In the DisconnectFromLamps loop", __func__));
            status = getQueueLock.Lock();
            if (status != ER_OK) {
                QCC_LogError(status, ("%s: getQueueLock.Lock() failed", __func__));
            } else {
                while (getMethodQueue.size()) {
                    QueuedMethodCall* queuedCall = getMethodQueue.front();
                    delete queuedCall;
                    getMethodQueue.pop_front();
                }
                QCC_DbgPrintf(("%s: Cleared getMethodQueue", __func__));
                status = getQueueLock.Unlock();
                if (status != ER_OK) {
                    QCC_LogError(status, ("%s: getQueueLock.Unlock() failed", __func__));
                }
            }

            status = setQueueLock.Lock();
            if (status != ER_OK) {
                QCC_LogError(status, ("%s: setQueueLock.Lock() failed", __func__));
            } else {
                while (setMethodQueue.size()) {
                    QueuedMethodCall* queuedCall = setMethodQueue.front();
                    delete queuedCall;
                    setMethodQueue.pop_front();
                }
                QCC_DbgPrintf(("%s: Cleared setMethodQueue", __func__));
                status = setQueueLock.Unlock();
                if (status != ER_OK) {
                    QCC_LogError(status, ("%s: setQueueLock.Unlock() failed", __func__));
                }
            }

            joinSessionInProgressLock.Lock();
            joinSessionInProgressList.clear();
            QCC_DbgPrintf(("%s: Cleared joinSessionInProgressList", __func__));
            joinSessionInProgressLock.Unlock();

            status = joinSessionCBListLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: joinSessionCBListLock.Lock() failed", __func__));
            } else {
                for (std::list<LampConnection*>::iterator it = joinSessionCBList.begin(); it != joinSessionCBList.end(); it++) {
                    controllerService.DoLeaveSessionAsync((*it)->sessionID);
                    delete (*it);
                }
                joinSessionCBList.clear();
                QCC_DbgPrintf(("%s: Cleared joinSessionCBList", __func__));
                status = joinSessionCBListLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: joinSessionCBListLock.Unlock() failed", __func__));
                }
            }

            status = lostSessionListLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: lostSessionListLock.Lock() failed", __func__));
                return;
            }
            lostSessionList.clear();
            QCC_DbgPrintf(("%s: Cleared lostSessionList", __func__));
            status = lostSessionListLock.Unlock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: lostSessionListLock.Unlock() failed", __func__));
            }

            status = getAllLampIDsLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(ER_FAIL, ("%s: getAllLampIDsLock.Lock() failed", __func__));
            } else {
                getAllLampIDsRequests.clear();
                QCC_DbgPrintf(("%s: Cleared getAllLampIDsRequests", __func__));
                status = getAllLampIDsLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(ER_FAIL, ("%s: getAllLampIDsLock.Unlock() failed", __func__));
                }
            }

            lostLampsLock.Lock();
            lostLamps.clear();
            QCC_DbgPrintf(("%s: Cleared lostLamps", __func__));
            lostLampsLock.Unlock();

            status = aboutsListLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: aboutsListLock.Lock() failed", __func__));
            } else {
                for (LampMap::iterator it = aboutsList.begin(); it != aboutsList.end(); ++it) {
                    LampConnection* conn = it->second;
                    delete conn;
                }
                aboutsList.clear();
                QCC_DbgPrintf(("%s: Cleared aboutsList", __func__));
                status = aboutsListLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: aboutsListLock.Unlock() failed", __func__));
                }
            }

            pingMapLock.Lock();
            if (ER_OK != status) {
                QCC_LogError(status, ("%s: pingMapLock.Lock() failed", __func__));
            } else {
                pingMap.clear();
                QCC_DbgPrintf(("%s: Cleared pingMap", __func__));
                status = pingMapLock.Unlock();
                if (ER_OK != status) {
                    QCC_LogError(status, ("%s: pingMapLock.Unlock() failed", __func__));
                }
            }

            methodRepliesReceivedLock.Lock();
            methodRepliesReceived.clear();
            methodRepliesReceivedLock.Unlock();

            LSFStringList clearedLamps;
            clearedLamps.clear();

            for (LampMap::iterator it = activeLamps.begin(); it != activeLamps.end(); ++it) {
                LampConnection* conn = it->second;
                controllerService.DoLeaveSessionAsync(conn->object.GetSessionId());
                delete conn;
                clearedLamps.push_back(it->first);
            }
            activeLamps.clear();
            QCC_DbgPrintf(("%s: Cleared activeLamps", __func__));

            sentNGNSPingsLock.Lock();
            while (clearedLamps.size()) {
                LampMap::iterator it = sentNGNSPings.find(clearedLamps.front());
                if (it != sentNGNSPings.end()) {
                    sentNGNSPings.erase(it);
                }
                clearedLamps.pop_front();
            }

            for (LampMap::iterator it = sentNGNSPings.begin(); it != sentNGNSPings.end(); ++it) {
                LampConnection* conn = it->second;
                delete conn;
            }
            sentNGNSPings.clear();
            sentNGNSPingsLock.Unlock();
            QCC_DbgPrintf(("%s: Cleared sentNGNSPings", __func__));
        }

        QCC_DbgPrintf(("%s: Exited", __func__));
    }
}
