/******************************************************************************
 *  * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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

#include <UnknownBlobGroupManager.h>
#include <ControllerService.h>
#include <qcc/Debug.h>
#include <FileParser.h>

using namespace lsf;
using namespace ajn;

#define QCC_MODULE "UNKNOWN_BLOB_MANAGER"

UnknownBlobManager::UnknownBlobManager(ControllerService& controllerSvc, const std::string& unknownBlobFile, uint32_t blobTypeNum) :
    Manager(controllerSvc, unknownBlobFile), blobType(blobTypeNum)
{
    QCC_DbgTrace(("%s", __func__));
}

LSFResponseCode UnknownBlobManager::Reset(void)
{
    QCC_DbgPrintf(("%s", __func__));

    blobData = resetID;
    ScheduleFileWrite();

    return LSF_OK;
}

bool UnknownBlobManager::GetString(std::string& output, uint32_t& checksum, uint64_t& timestamp)
{
    bool ret = false;
    output.clear();

    if (updated) {
        output = blobData;
        updated = false;
        ret = true;
    }

    if (ret) {
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
    }

    return ret;
}

void UnknownBlobManager::ReadWriteFile(void)
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
            controllerService.SendBlobUpdate(static_cast<LSFBlobType>(blobType), output, checksum, (currentTime - timestamp));
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
        bool fileStatus = ValidateFileAndReadInternal(checksum, timestamp, stream);
        if (fileStatus) {
            output = stream.str();
            while (!tempMessageList.empty()) {
                uint64_t currentTime = GetTimestampInMs();
                controllerService.SendGetBlobReply(tempMessageList.front(), static_cast<LSFBlobType>(blobType), output, checksum, (currentTime - timestamp));
                tempMessageList.pop_front();
            }
        } else {
            QCC_LogError(ER_FAIL, ("%s: Persistent store for unknown blob type %u corrupted", __func__, blobType));
        }
    }

    if (sendUpdate) {
        sendUpdate = false;
        uint64_t currentTime = GetTimestampInMs();
        controllerService.GetLeaderElectionObj().SendBlobUpdate(static_cast<LSFBlobType>(blobType), output, checksum, (currentTime - timestamp));
    }
}

void UnknownBlobManager::HandleReceivedBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgTrace(("%s", __func__));
    uint64_t currentTimestamp = GetTimestampInMs();
    if (((timeStamp == 0) || ((currentTimestamp - timeStamp) > timestamp)) && (checkSum != checksum)) {
        blobData = blob;
        timeStamp = currentTimestamp;
        checkSum = checksum;
        ScheduleFileWrite(true);
    }
}

void UnknownBlobManager::ReadSavedData(void)
{
    QCC_DbgTrace(("%s", __func__));
    std::istringstream stream;
    ValidateFileAndRead(stream);
}

UnknownBlobGroupManager::UnknownBlobGroupManager(ControllerService& controllerSvc) :
    controllerService(controllerSvc)
{
    QCC_DbgTrace(("%s", __func__));
    unknownBlobsMap.clear();
}

UnknownBlobGroupManager::~UnknownBlobGroupManager()
{
    QCC_DbgTrace(("%s", __func__));

    unknownBlobsLock.Lock();
    for (UnknownBlobsMap::iterator it = unknownBlobsMap.begin(); it != unknownBlobsMap.end(); ++it) {
        if (it->second != NULL) {
            delete it->second;
            it->second = NULL;
        }
    }
    unknownBlobsMap.clear();
    unknownBlobsLock.Unlock();
}

LSFResponseCode UnknownBlobGroupManager::Reset(uint32_t& numBlobs, uint32_t& failures)
{
    QCC_DbgPrintf(("%s", __func__));
    numBlobs = 0;
    failures = 0;
    LSFResponseCode responseCode = LSF_OK;

    unknownBlobsLock.Lock();
    for (UnknownBlobsMap::iterator it = unknownBlobsMap.begin(); it != unknownBlobsMap.end(); ++it) {
        numBlobs++;
        if (it->second != NULL) {
            if (it->second->Reset() != LSF_OK) {
                failures++;
            }
        }
    }
    unknownBlobsLock.Unlock();

    if (failures) {
        responseCode = LSF_ERR_FAILURE;
    }

    return responseCode;
}

void UnknownBlobGroupManager::ReadSavedData(void)
{
    QCC_DbgTrace(("%s", __func__));

    for (uint32_t blobType = (LSF_PULSE_EFFECT_UPDATE + 1); blobType < (LSF_PULSE_EFFECT_UPDATE + 20); blobType++) {
        std::stringstream ss;
        ss << blobType;
        std::string fileName = controllerService.GetStorePath();
        fileName.append("UnknownBlob_");
        fileName.append(ss.str());
        fileName.append(".lsf");

        std::ifstream stream(fileName.c_str());

        if (stream.is_open()) {
            QCC_DbgPrintf(("Found Unknown Blob File %s\n", fileName.c_str()));
            stream.close();
            unknownBlobsLock.Lock();
            UnknownBlobManager* blobMgrPtr = new UnknownBlobManager(controllerService, fileName, blobType);
            if (blobMgrPtr != NULL) {
                unknownBlobsMap.insert(std::make_pair(blobType, blobMgrPtr));
                blobMgrPtr->ReadSavedData();
            }
            unknownBlobsLock.Unlock();
        }
    }
}

void UnknownBlobGroupManager::HandleReceivedBlob(uint32_t blobType, const std::string& blob, uint32_t checksum, uint64_t timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    /*
     * Check if we already know about this blob. If we do, replace it if required.
     * If we do not know about this blob, save it
     */
    unknownBlobsLock.Lock();
    UnknownBlobsMap::iterator it = unknownBlobsMap.find(blobType);
    if (it != unknownBlobsMap.end()) {
        QCC_DbgPrintf(("%s: Already have a store for blob type %d", __func__, blobType));
        it->second->HandleReceivedBlob(blob, checksum, timestamp);
    } else {
        QCC_DbgPrintf(("%s: Creating a new store for blob type %d", __func__, blobType));
        std::stringstream ss;
        ss << blobType;
        std::string fileName = controllerService.GetStorePath();
        fileName.append("UnknownBlob_");
        fileName.append(ss.str());
        fileName.append(".lsf");

        UnknownBlobManager* blobMgrPtr = new UnknownBlobManager(controllerService, fileName, blobType);
        if (blobMgrPtr != NULL) {
            unknownBlobsMap.insert(std::make_pair(blobType, blobMgrPtr));
            blobMgrPtr->HandleReceivedBlob(blob, checksum, timestamp);
        }
    }
    unknownBlobsLock.Unlock();
}

void UnknownBlobGroupManager::ReadWriteFile(void)
{
    QCC_DbgPrintf(("%s", __func__));
    unknownBlobsLock.Lock();
    for (UnknownBlobsMap::iterator it = unknownBlobsMap.begin(); it != unknownBlobsMap.end(); ++it) {
        if (it->second != NULL) {
            it->second->ReadWriteFile();
        }
    }
    unknownBlobsLock.Unlock();
}

void UnknownBlobGroupManager::GetBlobInfo(uint32_t blobType, uint32_t& checksum, uint64_t& timestamp)
{
    QCC_DbgPrintf(("%s", __func__));
    checksum = 0;
    timestamp = 0;
    unknownBlobsLock.Lock();
    UnknownBlobsMap::iterator it = unknownBlobsMap.find(blobType);
    if (it != unknownBlobsMap.end()) {
        QCC_DbgPrintf(("%s: Found a store for blob type %d", __func__, blobType));
        it->second->GetBlobInfoInternal(checksum, timestamp);
    }
    unknownBlobsLock.Unlock();
}

void UnknownBlobGroupManager::TriggerUpdate(uint32_t blobType)
{
    QCC_DbgPrintf(("%s", __func__));
    unknownBlobsLock.Lock();
    UnknownBlobsMap::iterator it = unknownBlobsMap.find(blobType);
    if (it != unknownBlobsMap.end()) {
        QCC_DbgPrintf(("%s: Found a store for blob type %d", __func__, blobType));
        it->second->TriggerUpdate();
    }
    unknownBlobsLock.Unlock();
}

void UnknownBlobGroupManager::ScheduleFileRead(uint32_t blobType, ajn::Message& message)
{
    QCC_DbgPrintf(("%s", __func__));
    unknownBlobsLock.Lock();
    UnknownBlobsMap::iterator it = unknownBlobsMap.find(blobType);
    if (it != unknownBlobsMap.end()) {
        QCC_DbgPrintf(("%s: Found a store for blob type %d", __func__, blobType));
        it->second->ScheduleFileRead(message);
    }
    unknownBlobsLock.Unlock();
}

void UnknownBlobGroupManager::GetUnknownBlobTypeList(std::list<uint32_t>& blobTypeList)
{
    QCC_DbgPrintf(("%s", __func__));
    blobTypeList.clear();
    unknownBlobsLock.Lock();
    for (UnknownBlobsMap::iterator it = unknownBlobsMap.begin(); it != unknownBlobsMap.end(); ++it) {
        if (it->second != NULL) {
            blobTypeList.push_back(it->first);
        }
    }
    unknownBlobsLock.Unlock();
}