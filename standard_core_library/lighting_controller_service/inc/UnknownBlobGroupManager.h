#ifndef _UNKNOWN_BLOB_MANAGER_H_
#define _UNKNOWN_BLOB_MANAGER_H_
/**
 * \ingroup ControllerService
 */
/**
 * @file
 * This file provides definitions for unknown blob manager
 */
/******************************************************************************
 * Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
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

#include <Manager.h>

#include <Mutex.h>
#include <LSFTypes.h>

#include <string>
#include <map>

namespace lsf {

/**
 * class manages a Unknown Blob. \n
 */
class UnknownBlobManager : public Manager {
  public:
    /**
     * class constructor. \n
     * @param controllerSvc - reference to controller service instance
     */
    UnknownBlobManager(ControllerService& controllerSvc, const std::string& unknownBlobFile, uint32_t blobTypeNum);
    /**
     * Reset
     */
    LSFResponseCode Reset(void);
    /**
     * Get the data string
     */
    bool GetString(std::string& output, uint32_t& checksum, uint64_t& timestamp);
    /**
     * Write to cache persistent data
     */
    void ReadWriteFile(void);
    /**
     * Handle received blob. \n
     * Getting the blob string and writing it to the file. \n
     */
    void HandleReceivedBlob(const std::string& blob, uint32_t checksum, uint64_t timestamp);

    /**
     * Read saved persistent data to cache
     */
    void ReadSavedData(void);

  private:
    /**
     * Blob Data
     */
    std::string blobData;
    /**
     * Blob Type Identifier
     */
    uint32_t blobType;
};

/**
 * class manages the Unknown Blobs. \n
 */
class UnknownBlobGroupManager {
  public:
    /**
     * class constructor. \n
     * @param controllerSvc - reference to controller service instance
     */
    UnknownBlobGroupManager(ControllerService& controllerSvc);
    /**
     * class destructor. \n
     */
    ~UnknownBlobGroupManager();
    /**
     * Reset
     */
    LSFResponseCode Reset(uint32_t& numBlobs, uint32_t& failures);
    /**
     * Read saved persistent data to cache
     */
    void ReadSavedData(void);
    /**
     * Write to cache persistent data
     */
    void ReadWriteFile(void);
    /**
     * Handle received blob. \n
     * Getting the blob string and writing it to the file. \n
     */
    void HandleReceivedBlob(uint32_t blobType, const std::string& blob, uint32_t checksum, uint64_t timestamp);
    /**
     * Get blob information about checksum and time stamp.
     */
    void GetBlobInfo(uint32_t blobType, uint32_t& checksum, uint64_t& timestamp);

    /**
     * Trigger update for persistent data
     */
    void TriggerUpdate(uint32_t blobType);

    /**
     * Schedule File Read
     */
    void ScheduleFileRead(uint32_t blobType, ajn::Message& message);

    /**
     * Get a list of blobTypes corresponding to the unknown blobs
     */
    void GetUnknownBlobTypeList(std::list<uint32_t>& blobTypeList);

  private:

    typedef std::map<uint32_t, UnknownBlobManager*> UnknownBlobsMap;

    Mutex unknownBlobsLock;
    /**
     * Map of all the unknown blobs
     */
    UnknownBlobsMap unknownBlobsMap;
    /**
     * Reference to the Controller Service
     */
    ControllerService& controllerService;
};

}


#endif