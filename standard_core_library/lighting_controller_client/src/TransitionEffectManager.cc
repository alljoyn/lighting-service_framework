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

#include <TransitionEffectManager.h>
#include <ControllerClient.h>
#include <qcc/Debug.h>

using namespace lsf;
using namespace ajn;

#define QCC_MODULE "TRANSITION_EFFECT_MANAGER"

TransitionEffectManager::TransitionEffectManager(ControllerClient& controllerClient, TransitionEffectManagerCallback& callback) :
    Manager(controllerClient),
    callback(callback)
{
    controllerClient.transitionEffectManagerPtr = this;
}

ControllerClientStatus TransitionEffectManager::GetAllTransitionEffectIDs(void)
{
    QCC_DbgPrintf(("%s", __func__));
    return controllerClient.MethodCallAsyncForReplyWithResponseCodeAndListOfIDs(
               ControllerServiceTransitionEffectInterfaceName,
               "GetAllTransitionEffectIDs");
}

ControllerClientStatus TransitionEffectManager::GetTransitionEffect(const LSFString& transitionEffectID)
{
    QCC_DbgPrintf(("%s: transitionEffectID=%s", __func__, transitionEffectID.c_str()));
    MsgArg arg;
    arg.Set("s", transitionEffectID.c_str());

    ControllerClientStatus status = controllerClient.MethodCallAsync(
        ControllerServiceTransitionEffectInterfaceName,
        "GetTransitionEffect",
        this,
        &TransitionEffectManager::GetTransitionEffectReply,
        &arg,
        1);

    return status;
}

ControllerClientStatus TransitionEffectManager::ApplyTransitionEffectOnLamps(const LSFString& transitionEffectID, const LSFStringList& lampIDs)
{
    QCC_DbgPrintf(("%s: transitionEffectID=%s", __func__, transitionEffectID.c_str()));
    MsgArg arg[2];
    arg[0].Set("s", transitionEffectID.c_str());

    size_t idsVecSize = lampIDs.size();

    if (idsVecSize) {
        const char** idsVec = new const char*[idsVecSize];
        size_t i = 0;
        for (LSFStringList::const_iterator it = lampIDs.begin(); it != lampIDs.end(); it++) {
            idsVec[i++] = it->c_str();
        }
        arg[1].Set("as", idsVecSize, idsVec);
        delete [] idsVec;
        arg[1].SetOwnershipFlags(MsgArg::OwnsArgs);
    } else {
        QCC_LogError(ER_BAD_ARG_2, ("%s: Size of lampIDs list is 0", __func__));
        return CONTROLLER_CLIENT_ERR_FAILURE;
    }

    ControllerClientStatus status = controllerClient.MethodCallAsync(
        ControllerServiceTransitionEffectInterfaceName,
        "ApplyTransitionEffectOnLamps",
        this,
        &TransitionEffectManager::ApplyTransitionEffectOnLampsReply,
        arg,
        2);

    return status;
}

ControllerClientStatus TransitionEffectManager::ApplyTransitionEffectOnLampGroups(const LSFString& transitionEffectID, const LSFStringList& lampGroupIDs)
{
    QCC_DbgPrintf(("%s: transitionEffectID=%s", __func__, transitionEffectID.c_str()));
    MsgArg arg[2];
    arg[0].Set("s", transitionEffectID.c_str());

    size_t idsVecSize = lampGroupIDs.size();

    if (idsVecSize) {
        const char** idsVec = new const char*[idsVecSize];
        size_t i = 0;
        for (LSFStringList::const_iterator it = lampGroupIDs.begin(); it != lampGroupIDs.end(); it++) {
            idsVec[i++] = it->c_str();
        }
        arg[1].Set("as", idsVecSize, idsVec);
        delete [] idsVec;
        arg[1].SetOwnershipFlags(MsgArg::OwnsArgs);
    } else {
        QCC_LogError(ER_BAD_ARG_2, ("%s: Size of lampGroupIDs list is 0", __func__));
        return CONTROLLER_CLIENT_ERR_FAILURE;
    }

    ControllerClientStatus status = controllerClient.MethodCallAsync(
        ControllerServiceTransitionEffectInterfaceName,
        "ApplyTransitionEffectOnLampGroups",
        this,
        &TransitionEffectManager::ApplyTransitionEffectOnLampGroupsReply,
        arg,
        2);

    return status;
}

void TransitionEffectManager::ApplyTransitionEffectOnLampsReply(Message& message)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __func__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerClient.CheckNumArgsInMessage(numArgs, 3) != LSF_OK) {
        return;
    }

    LSFResponseCode responseCode = static_cast<LSFResponseCode>(args[0].v_uint32);
    LSFString transitionEffectID = static_cast<LSFString>(args[1].v_string.str);
    LSFStringList lampIDs;
    MsgArg* idsArray;
    size_t idsSize;
    args[2].Get("as", &idsSize, &idsArray);
    for (size_t i = 0; i < idsSize; i++) {
        char* gid;
        idsArray[i].Get("s", &gid);
        lampIDs.push_back(LSFString(gid));
    }
    callback.ApplyTransitionEffectOnLampsReplyCB(responseCode, transitionEffectID, lampIDs);
}

void TransitionEffectManager::ApplyTransitionEffectOnLampGroupsReply(Message& message)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __func__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerClient.CheckNumArgsInMessage(numArgs, 3) != LSF_OK) {
        return;
    }

    LSFResponseCode responseCode = static_cast<LSFResponseCode>(args[0].v_uint32);
    LSFString transitionEffectID = static_cast<LSFString>(args[1].v_string.str);
    LSFStringList lampGroupIDs;
    MsgArg* idsArray;
    size_t idsSize;
    args[2].Get("as", &idsSize, &idsArray);
    for (size_t i = 0; i < idsSize; i++) {
        char* gid;
        idsArray[i].Get("s", &gid);
        lampGroupIDs.push_back(LSFString(gid));
    }
    callback.ApplyTransitionEffectOnLampGroupsReplyCB(responseCode, transitionEffectID, lampGroupIDs);
}

void TransitionEffectManager::GetTransitionEffectReply(Message& message)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __func__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerClient.CheckNumArgsInMessage(numArgs, 5) != LSF_OK) {
        return;
    }

    LSFResponseCode responseCode = static_cast<LSFResponseCode>(args[0].v_uint32);
    LSFString transitionEffectID = static_cast<LSFString>(args[1].v_string.str);
    TransitionEffect transitionEffect(args[2], args[3], args[4]);
    callback.GetTransitionEffectReplyCB(responseCode, transitionEffectID, transitionEffect);
}

ControllerClientStatus TransitionEffectManager::GetTransitionEffectName(const LSFString& transitionEffectID, const LSFString& language)
{
    QCC_DbgPrintf(("%s: transitionEffectID=%s", __func__, transitionEffectID.c_str()));
    MsgArg args[2];
    args[0].Set("s", transitionEffectID.c_str());
    args[1].Set("s", language.c_str());
    return controllerClient.MethodCallAsyncForReplyWithResponseCodeIDLanguageAndName(
               ControllerServiceTransitionEffectInterfaceName,
               "GetTransitionEffectName",
               args,
               2);
}

ControllerClientStatus TransitionEffectManager::SetTransitionEffectName(const LSFString& transitionEffectID, const LSFString& transitionEffectName, const LSFString& language)
{
    QCC_DbgPrintf(("%s: transitionEffectID=%s transitionEffectName=%s language=%s", __func__, transitionEffectID.c_str(), transitionEffectName.c_str(), language.c_str()));

    MsgArg args[3];
    args[0].Set("s", transitionEffectID.c_str());
    args[1].Set("s", transitionEffectName.c_str());
    args[2].Set("s", language.c_str());

    return controllerClient.MethodCallAsyncForReplyWithResponseCodeIDAndName(
               ControllerServiceTransitionEffectInterfaceName,
               "SetTransitionEffectName",
               args,
               3);
}

ControllerClientStatus TransitionEffectManager::CreateTransitionEffect(uint32_t& trackingID, const TransitionEffect& transitionEffect, const LSFString& transitionEffectName, const LSFString& language)
{
    //QCC_DbgPrintf(("%s: transitionEffect=%s", __func__, transitionEffect.c_str()));

    MsgArg arg[5];
    transitionEffect.Get(&arg[0], &arg[1], &arg[2]);
    arg[3].Set("s", transitionEffectName.c_str());
    arg[4].Set("s", language.c_str());

    return controllerClient.MethodCallAsyncForReplyWithResponseCodeIDAndTrackingID(
               trackingID,
               ControllerServiceTransitionEffectInterfaceName,
               "CreateTransitionEffect",
               arg,
               5);
}

ControllerClientStatus TransitionEffectManager::UpdateTransitionEffect(const LSFString& transitionEffectID, const TransitionEffect& transitionEffect)
{
    //QCC_DbgPrintf(("%s: transitionEffectID=%s transitionEffect=%s", __func__, transitionEffectID.c_str(), transitionEffect.c_str()));
    MsgArg args[4];
    args[0].Set("s", transitionEffectID.c_str());
    transitionEffect.Get(&args[1], &args[2], &args[3]);

    return controllerClient.MethodCallAsyncForReplyWithResponseCodeAndID(
               ControllerServiceTransitionEffectInterfaceName,
               "UpdateTransitionEffect",
               args,
               4);
}

ControllerClientStatus TransitionEffectManager::DeleteTransitionEffect(const LSFString& transitionEffectID)
{
    QCC_DbgPrintf(("%s: transitionEffectID=%s", __func__, transitionEffectID.c_str()));
    MsgArg arg;
    arg.Set("s", transitionEffectID.c_str());

    return controllerClient.MethodCallAsyncForReplyWithResponseCodeAndID(
               ControllerServiceTransitionEffectInterfaceName,
               "DeleteTransitionEffect",
               &arg,
               1);
}

ControllerClientStatus TransitionEffectManager::GetTransitionEffectDataSet(const LSFString& transitionEffectID, const LSFString& language)
{
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;

    status = GetTransitionEffect(transitionEffectID);

    if (CONTROLLER_CLIENT_OK == status) {
        status = GetTransitionEffectName(transitionEffectID, language);
    }

    return status;
}