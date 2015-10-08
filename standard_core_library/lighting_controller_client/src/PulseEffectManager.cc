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

#include <PulseEffectManager.h>
#include <ControllerClient.h>
#include <qcc/Debug.h>

using namespace lsf;
using namespace ajn;

#define QCC_MODULE "PULSE_EFFECT_MANAGER"

PulseEffectManager::PulseEffectManager(ControllerClient& controllerClient, PulseEffectManagerCallback& callback) :
    Manager(controllerClient),
    callback(callback)
{
    controllerClient.pulseEffectManagerPtr = this;
}

ControllerClientStatus PulseEffectManager::GetAllPulseEffectIDs(void)
{
    QCC_DbgPrintf(("%s", __func__));
    return controllerClient.MethodCallAsyncForReplyWithResponseCodeAndListOfIDs(
               ControllerServicePulseEffectInterfaceName,
               "GetAllPulseEffectIDs");
}

ControllerClientStatus PulseEffectManager::GetPulseEffect(const LSFString& pulseEffectID)
{
    QCC_DbgPrintf(("%s: pulseEffectID=%s", __func__, pulseEffectID.c_str()));
    MsgArg arg;
    arg.Set("s", pulseEffectID.c_str());

    ControllerClientStatus status = controllerClient.MethodCallAsync(
        ControllerServicePulseEffectInterfaceName,
        "GetPulseEffect",
        this,
        &PulseEffectManager::GetPulseEffectReply,
        &arg,
        1);

    return status;
}

ControllerClientStatus PulseEffectManager::ApplyPulseEffectOnLamps(const LSFString& pulseEffectID, const LSFStringList& lampIDs)
{
    QCC_DbgPrintf(("%s: pulseEffectID=%s", __func__, pulseEffectID.c_str()));
    MsgArg arg[2];
    arg[0].Set("s", pulseEffectID.c_str());

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
        ControllerServicePulseEffectInterfaceName,
        "ApplyPulseEffectOnLamps",
        this,
        &PulseEffectManager::ApplyPulseEffectOnLampsReply,
        arg,
        2);

    return status;
}

ControllerClientStatus PulseEffectManager::ApplyPulseEffectOnLampGroups(const LSFString& pulseEffectID, const LSFStringList& lampGroupIDs)
{
    QCC_DbgPrintf(("%s: pulseEffectID=%s", __func__, pulseEffectID.c_str()));
    MsgArg arg[2];
    arg[0].Set("s", pulseEffectID.c_str());

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
        ControllerServicePulseEffectInterfaceName,
        "ApplyPulseEffectOnLampGroups",
        this,
        &PulseEffectManager::ApplyPulseEffectOnLampGroupsReply,
        arg,
        2);

    return status;
}

void PulseEffectManager::ApplyPulseEffectOnLampsReply(Message& message)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __func__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerClient.CheckNumArgsInMessage(numArgs, 3) != LSF_OK) {
        return;
    }

    LSFResponseCode responseCode = static_cast<LSFResponseCode>(args[0].v_uint32);
    LSFString pulseEffectID = static_cast<LSFString>(args[1].v_string.str);
    LSFStringList lampIDs;
    MsgArg* idsArray;
    size_t idsSize;
    args[2].Get("as", &idsSize, &idsArray);
    for (size_t i = 0; i < idsSize; i++) {
        char* gid;
        idsArray[i].Get("s", &gid);
        lampIDs.push_back(LSFString(gid));
    }
    callback.ApplyPulseEffectOnLampsReplyCB(responseCode, pulseEffectID, lampIDs);
}

void PulseEffectManager::ApplyPulseEffectOnLampGroupsReply(Message& message)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __func__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerClient.CheckNumArgsInMessage(numArgs, 3) != LSF_OK) {
        return;
    }

    LSFResponseCode responseCode = static_cast<LSFResponseCode>(args[0].v_uint32);
    LSFString pulseEffectID = static_cast<LSFString>(args[1].v_string.str);
    LSFStringList lampGroupIDs;
    MsgArg* idsArray;
    size_t idsSize;
    args[2].Get("as", &idsSize, &idsArray);
    for (size_t i = 0; i < idsSize; i++) {
        char* gid;
        idsArray[i].Get("s", &gid);
        lampGroupIDs.push_back(LSFString(gid));
    }
    callback.ApplyPulseEffectOnLampGroupsReplyCB(responseCode, pulseEffectID, lampGroupIDs);
}

void PulseEffectManager::GetPulseEffectReply(Message& message)
{
    QCC_DbgPrintf(("%s: Method Reply %s", __func__, (MESSAGE_METHOD_RET == message->GetType()) ? message->ToString().c_str() : "ERROR"));
    size_t numArgs;
    const MsgArg* args;
    message->GetArgs(numArgs, args);

    if (controllerClient.CheckNumArgsInMessage(numArgs, 9) != LSF_OK) {
        return;
    }

    LSFResponseCode responseCode = static_cast<LSFResponseCode>(args[0].v_uint32);
    LSFString pulseEffectID = static_cast<LSFString>(args[1].v_string.str);
    PulseEffect pulseEffect(args[2], args[3], args[4], args[5], args[6], args[7], args[8]);
    callback.GetPulseEffectReplyCB(responseCode, pulseEffectID, pulseEffect);
}

ControllerClientStatus PulseEffectManager::GetPulseEffectName(const LSFString& pulseEffectID, const LSFString& language)
{
    QCC_DbgPrintf(("%s: pulseEffectID=%s", __func__, pulseEffectID.c_str()));
    MsgArg args[2];
    args[0].Set("s", pulseEffectID.c_str());
    args[1].Set("s", language.c_str());
    return controllerClient.MethodCallAsyncForReplyWithResponseCodeIDLanguageAndName(
               ControllerServicePulseEffectInterfaceName,
               "GetPulseEffectName",
               args,
               2);
}

ControllerClientStatus PulseEffectManager::SetPulseEffectName(const LSFString& pulseEffectID, const LSFString& pulseEffectName, const LSFString& language)
{
    QCC_DbgPrintf(("%s: pulseEffectID=%s pulseEffectName=%s language=%s", __func__, pulseEffectID.c_str(), pulseEffectName.c_str(), language.c_str()));

    MsgArg args[3];
    args[0].Set("s", pulseEffectID.c_str());
    args[1].Set("s", pulseEffectName.c_str());
    args[2].Set("s", language.c_str());

    return controllerClient.MethodCallAsyncForReplyWithResponseCodeIDAndName(
               ControllerServicePulseEffectInterfaceName,
               "SetPulseEffectName",
               args,
               3);
}

ControllerClientStatus PulseEffectManager::CreatePulseEffect(uint32_t& trackingID, const PulseEffect& pulseEffect, const LSFString& pulseEffectName, const LSFString& language)
{
    //QCC_DbgPrintf(("%s: pulseEffect=%s", __func__, pulseEffect.c_str()));

    MsgArg arg[9];
    pulseEffect.Get(&arg[0], &arg[1], &arg[2], &arg[3], &arg[4], &arg[5], &arg[6]);
    arg[7].Set("s", pulseEffectName.c_str());
    arg[8].Set("s", language.c_str());

    return controllerClient.MethodCallAsyncForReplyWithResponseCodeIDAndTrackingID(
               trackingID,
               ControllerServicePulseEffectInterfaceName,
               "CreatePulseEffect",
               arg,
               9);
}

ControllerClientStatus PulseEffectManager::UpdatePulseEffect(const LSFString& pulseEffectID, const PulseEffect& pulseEffect)
{
    //QCC_DbgPrintf(("%s: pulseEffectID=%s pulseEffect=%s", __func__, pulseEffectID.c_str(), pulseEffect.c_str()));
    MsgArg args[8];
    args[0].Set("s", pulseEffectID.c_str());
    pulseEffect.Get(&args[1], &args[2], &args[3], &args[4], &args[5], &args[6], &args[7]);

    return controllerClient.MethodCallAsyncForReplyWithResponseCodeAndID(
               ControllerServicePulseEffectInterfaceName,
               "UpdatePulseEffect",
               args,
               8);
}

ControllerClientStatus PulseEffectManager::DeletePulseEffect(const LSFString& pulseEffectID)
{
    QCC_DbgPrintf(("%s: pulseEffectID=%s", __func__, pulseEffectID.c_str()));
    MsgArg arg;
    arg.Set("s", pulseEffectID.c_str());

    return controllerClient.MethodCallAsyncForReplyWithResponseCodeAndID(
               ControllerServicePulseEffectInterfaceName,
               "DeletePulseEffect",
               &arg,
               1);
}

ControllerClientStatus PulseEffectManager::GetPulseEffectDataSet(const LSFString& pulseEffectID, const LSFString& language)
{
    ControllerClientStatus status = CONTROLLER_CLIENT_OK;

    status = GetPulseEffect(pulseEffectID);

    if (CONTROLLER_CLIENT_OK == status) {
        status = GetPulseEffectName(pulseEffectID, language);
    }

    return status;
}