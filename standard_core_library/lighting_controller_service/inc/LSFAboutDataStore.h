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
#ifndef LSF_ABOUT_DATA_STORE_H_
#define LSF_ABOUT_DATA_STORE_H_

#include <stdio.h>
#include <iostream>
#include <alljoyn/config/AboutDataStoreInterface.h>
#include <alljoyn/AboutData.h>
#include <Rank.h>
#include <Mutex.h>

namespace lsf {

/*
 * Pre-Declarations
 */
class ControllerService;

/**
 * class LSFAboutDataStore
 * Property store implementation
 */
class LSFAboutDataStore : public AboutDataStoreInterface {

  public:
    /**
     * LSFAboutDataStore - constructor
     * @param controllerService
     * @param factoryConfigFile
     * @param configFile
     */
    LSFAboutDataStore(ControllerService* controllerService, const char* factoryConfigFile, const char* configFile);

    /**
     * LSFAboutDataStore - constructor
     * @param factoryConfigFile
     * @param configFile
     */
    LSFAboutDataStore(const char* factoryConfigFile, const char* configFile);

    /**
     * LSFAboutDataStore - destructor
     */
    ~LSFAboutDataStore();

    void SetControllerServicePtr(ControllerService* controllerService) {
        m_controllerService = controllerService;
    }

    /**
     * FactoryReset
     */
    void FactoryReset();

    /**
     * GetConfigFileName
     * @return qcc::String&
     */
    const qcc::String& GetConfigFileName();

    /**
     * virtual method ReadAll
     * @param languageTag
     * @param filter
     * @param all
     * @return QStatus
     */
    virtual QStatus ReadAll(const char* languageTag, DataPermission::Filter filter, ajn::MsgArg& all);

    /**
     * virtual method Update
     * @param name
     * @param languageTag
     * @param value
     * @return QStatus
     */
    virtual QStatus Update(const char* name, const char* languageTag, const ajn::MsgArg* value);

    /**
     * virtual method Delete
     * @param name
     * @param languageTag
     * @return QStatus
     */
    virtual QStatus Delete(const char* name, const char* languageTag);

    /**
     * Write about data store as an xml config file
     */
    QStatus write();

    /**
     * Used to retrieve the OEM default factory settings
     */
    friend void OEM_CS_PopulateDefaultProperties(AboutData* aboutData);

    /**
     * Is this controller is a leader?
     * @return boolean
     */
    bool IsLeader(void);

    /**
     * Set true if that controller is a leader
     * @param leader - true or false
     */
    void SetIsLeader(bool leader);

    /**
     * Set the rank of the current controller
     */
    void SetRank(Rank& rank);

  private:

    ControllerService* m_controllerService;
    bool m_IsInitialized;
    qcc::String m_configFileName;
    qcc::String m_factoryConfigFileName;

    Mutex editLock;

    QStatus Initialize(const char* factoryConfigFile, const char* configFile);

    QStatus IsLanguageSupported(const char* languageTag);

    /**
     * Initialize About Data
     */
    QStatus InitializeConfigSettings(void);

    /**
     * Initialize Factory Settings
     */
    QStatus InitializeFactorySettings(void);

    /**
     * Write about data store as an xml config file
     */
    void writeFactorySettings(AboutData* aboutData);

    /**
     * Write about data store as an xml config file
     */
    void writeConfigSettings(void);

    /**
     * Convert About Data to XML
     */
    qcc::String ToXml(AboutData* aboutData);
};

}

#endif /* LSF_ABOUT_DATA_STORE_H_ */
