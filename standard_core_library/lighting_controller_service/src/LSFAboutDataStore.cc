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

#ifdef LSF_BINDINGS
#include "lsf/controllerservice/LSFAboutDataStore.h"
#include "lsf/controllerservice/ControllerService.h"
#else
#include "LSFAboutDataStore.h"
#include "ControllerService.h"
#endif

#include <alljoyn/config/AboutDataStoreInterface.h>
#include <alljoyn/AboutData.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <qcc/Debug.h>

#if !defined(LSF_OS_DARWIN)
#include <alljoyn/services_common/GuidUtil.h>
#endif

using namespace ajn;
using namespace lsf;
using namespace services;

#ifdef LSF_BINDINGS
using namespace controllerservice;
#endif

#define QCC_MODULE "LSF_ABOUT_DATA_STORE"

LSFAboutDataStore::LSFAboutDataStore(ControllerService* controllerService, const char* factoryConfigFile, const char* configFile) :
    AboutDataStoreInterface(factoryConfigFile, configFile), m_controllerService(controllerService), m_IsInitialized(false)
{
    QCC_DbgTrace(("%s", __func__));
    Initialize(factoryConfigFile, configFile);
}

LSFAboutDataStore::LSFAboutDataStore(const char* factoryConfigFile, const char* configFile, const char* deviceId) :
    AboutDataStoreInterface(factoryConfigFile, configFile), m_controllerService(NULL), m_IsInitialized(false)
{
    QCC_DbgTrace(("%s", __func__));
    Initialize(factoryConfigFile, configFile, deviceId);
}

QStatus LSFAboutDataStore::Initialize(const char* factoryConfigFile, const char* configFile, const char* deviceId)
{
    QCC_DbgTrace(("%s", __func__));
    m_configFileName.assign(configFile);
    m_factoryConfigFileName.assign(factoryConfigFile);

    QStatus status = InitializeFactorySettings(deviceId);

    if (status == ER_OK) {
        status = InitializeConfigSettings();

        if (status != ER_OK) {
            QCC_LogError(status, ("%s: InitializeConfigSettings failed", __func__));
        }
    } else {
        QCC_LogError(status, ("%s: InitializeFactorySettings failed", __func__));
    }

    return status;
}

LSFAboutDataStore::~LSFAboutDataStore()
{

}

QStatus LSFAboutDataStore::InitializeFactorySettings(const char* passedInDeviceId)
{
    QCC_DbgTrace(("%s", __func__));

    QStatus status = ER_FAIL;

    editLock.Lock();

    AboutData factorySettings;

    std::ifstream factoryFile(m_factoryConfigFileName.c_str(), std::ios::binary);
    if (!factoryFile) {
        /* A factory config file does not exist. So create one using hard-coded
           defaults */
        OEM_CS_PopulateDefaultProperties(&factorySettings);

        if (passedInDeviceId == NULL) {
            /*
             * Deliberately set the Device ID after
             * reading the default properties from the OEM as we do not
             * want the OEMs to override this values because this
             * values - if not set appropriately can cause issues in the
             * Controller Service operation
             */
            qcc::String deviceId;
    #if !defined(LSF_OS_DARWIN)
            GuidUtil::GetInstance()->GetDeviceIdString(&deviceId);
    #else
            /*
             * Anybody trying to build the Controller Service code on IOS would need
             * to generate a new random device ID and initialize "deviceId".
             * This is required because GuidUtil is not available in the services_common
             * IOS library.
             */
    #endif
            factorySettings.SetDeviceId(deviceId.c_str());
        } else {
            factorySettings.SetDeviceId(passedInDeviceId);
            QCC_DbgTrace(("%s: Set DeviceID to the passed in value %s", __func__, passedInDeviceId));
        }

        if (factorySettings.IsValid()) {
            status = ER_OK;
        } else {
            QCC_LogError(status, ("%s:OEM_CS_PopulateDefaultProperties passed back invalid data", __func__));
        }

        if (status == ER_OK) {
            writeFactorySettings(&factorySettings);
        }
    } else {
        /* A factory config file exists. Check that the contents are correct */
        std::string str((std::istreambuf_iterator<char>(factoryFile)),
                        std::istreambuf_iterator<char>());
        QCC_DbgTrace(("%s: Contains: %s", __func__, str.c_str()));
        if (factorySettings.CreateFromXml(qcc::String(str.c_str())) == ER_OK) {
            if (factorySettings.IsValid()) {
                status = ER_OK;
            } else {
                QCC_LogError(status, ("%s: Invalid aboutData", __func__));
            }
        } else {
            QCC_LogError(status, ("%s:Error initializing factory settings", __func__));
        }
        factoryFile.close();
    }

    editLock.Unlock();

    return status;
}

QStatus LSFAboutDataStore::InitializeConfigSettings(void)
{
    QCC_DbgTrace(("%s", __func__));

    QStatus status = ER_FAIL;

    editLock.Lock();

    std::ifstream configFile(m_configFileName.c_str(), std::ios::binary);
    if (configFile) {
        std::string str((std::istreambuf_iterator<char>(configFile)),
                        std::istreambuf_iterator<char>());
        QCC_DbgTrace(("%s: Contains: %s", __func__, str.c_str()));

        qcc::String config = str.c_str();

        if (!config.empty()) {
            if (CreateFromXml(config) == ER_OK) {
                if (IsValid()) {
                    status = ER_OK;
                } else {
                    QCC_LogError(status, ("%s: Invalid aboutData", __func__));
                }
            } else {
                QCC_LogError(status, ("%s:Error initializing config settings", __func__));
            }
        }
        configFile.close();
    } else {
        /* There is no config file. We need to create one using factory settings */
        std::ifstream factoryFile(m_factoryConfigFileName.c_str(), std::ios::binary);
        std::string str((std::istreambuf_iterator<char>(factoryFile)),
                        std::istreambuf_iterator<char>());
        QCC_DbgTrace(("%s: Contains: %s", __func__, str.c_str()));
        if (CreateFromXml(qcc::String(str.c_str())) == ER_OK) {
            if (IsValid()) {
                status = ER_OK;
            } else {
                QCC_LogError(status, ("%s: Invalid aboutData", __func__));
            }
        } else {
            QCC_LogError(status, ("%s:Error initializing config settings", __func__));
        }
        factoryFile.close();

        if (status == ER_OK) {
            writeConfigSettings();
        }
    }

    status = SetNewFieldDetails("IsLeader", ANNOUNCED, "b");
    if (status == ER_OK) {
        status = SetNewFieldDetails("RankHigherBits", ANNOUNCED, "t");
        if (status == ER_OK) {
            status = SetNewFieldDetails("RankLowerBits", ANNOUNCED, "t");
        }
    }

    m_IsInitialized = true;
    QCC_DbgTrace(("%s:Config Settings initialization successful", __func__));

    editLock.Unlock();

    return status;
}

void LSFAboutDataStore::FactoryReset()
{
    QCC_DbgTrace(("%s", __func__));

    editLock.Lock();

    m_IsInitialized = false;

    std::ifstream factoryConfigFile(m_factoryConfigFileName.c_str(), std::ios::binary);
    std::string str((std::istreambuf_iterator<char>(factoryConfigFile)),
                    std::istreambuf_iterator<char>());
    factoryConfigFile.close();

    std::ofstream configFileWrite(m_configFileName.c_str(), std::ofstream::out | std::ofstream::trunc);
    configFileWrite.write(str.c_str(), str.length());
    configFileWrite.close();

    editLock.Unlock();
}

QStatus LSFAboutDataStore::ReadAll(const char* languageTag, DataPermission::Filter filter, ajn::MsgArg& all)
{
    QCC_DbgTrace(("%s", __func__));
    editLock.Lock();
    QStatus status = GetAboutData(&all, languageTag);
    editLock.Unlock();
    QCC_DbgTrace(("%s: status = %s", __func__, QCC_StatusText(status)));
    return status;
}

QStatus LSFAboutDataStore::Update(const char* name, const char* languageTag, const ajn::MsgArg* value)
{
    QCC_DbgTrace(("%s: name(%s), languageTag(%s), value(%s)", __func__, name, languageTag, value->ToString().c_str()));

    editLock.Lock();
    QStatus status = ER_INVALID_VALUE;
    if (strcmp(name, AboutData::APP_ID) == 0) {
        uint8_t* appId = NULL;
        size_t* num = NULL;
        status = value->Get("ay", num, &appId);
        if (status == ER_OK) {
            status = SetAppId(appId, *num);
        }
    } else if (strcmp(name, AboutData::DEFAULT_LANGUAGE) == 0) {
        char* defaultLanguage;
        status = value->Get("s", &defaultLanguage);
        if (status == ER_OK) {
            if (0 == strcmp(defaultLanguage, "")) {
                status = ER_LANGUAGE_NOT_SUPPORTED;
            } else {
                status = IsLanguageSupported(defaultLanguage);
                if (status == ER_OK) {
                    status = SetDefaultLanguage(defaultLanguage);
                }
            }
        }
    } else if (strcmp(name, AboutData::DEVICE_NAME) == 0) {
        QCC_DbgTrace(("%s", __func__));
        char* deviceName = NULL;
        status = value->Get("s", &deviceName);
        status = IsLanguageSupported(languageTag);
        if (status == ER_OK) {
            status = SetDeviceName(deviceName, languageTag);
        }
    } else if (strcmp(name, AboutData::DEVICE_ID) == 0) {
        char* deviceId = NULL;
        status = value->Get("s", &deviceId);
        if (status == ER_OK) {
            status = SetDeviceId(deviceId);
        }
    } else if (strcmp(name, AboutData::APP_NAME) == 0) {
        char* appName = NULL;
        status = value->Get("s", &appName);
        if (status == ER_OK) {
            status = SetAppName(appName, languageTag);
        }
    } else if (strcmp(name, AboutData::MANUFACTURER) == 0) {
        char* chval = NULL;
        status = value->Get("s", &chval);
        if (status == ER_OK) {
            status = SetManufacturer(chval);
        }
    } else if (strcmp(name, AboutData::MODEL_NUMBER) == 0) {
        char* chval = NULL;
        status = value->Get("s", chval);
        if (status == ER_OK) {
            status = SetModelNumber(chval);
        }
    } else if (strcmp(name, AboutData::SUPPORTED_LANGUAGES) == 0) {
        //Added automatically when adding value
        QCC_DbgTrace(("%s:Supported languages will be added automatically when adding value", __func__));
    } else if (strcmp(name, AboutData::DESCRIPTION) == 0) {
        char* chval = NULL;
        status = value->Get("s", &chval);
        if (status == ER_OK) {
            status = SetDescription(chval);
        }
    } else if (strcmp(name, AboutData::DATE_OF_MANUFACTURE) == 0) {
        char* chval = NULL;
        status = value->Get("s", &chval);
        if (status == ER_OK) {
            status = SetDateOfManufacture(chval);
        }
    } else if (strcmp(name, AboutData::SOFTWARE_VERSION) == 0) {
        char* chval = NULL;
        status = value->Get("s", &chval);
        if (status == ER_OK) {
            status = SetSoftwareVersion(chval);
        }
    } else if (strcmp(name, AboutData::HARDWARE_VERSION) == 0) {
        char* chval = NULL;
        status = value->Get("s", &chval);
        if (status == ER_OK) {
            status = SetHardwareVersion(chval);
        }
    } else if (strcmp(name, AboutData::SUPPORT_URL) == 0) {
        char* chval = NULL;
        status = value->Get("s", &chval);
        if (status == ER_OK) {
            status = SetSupportUrl(chval);
        }
    }

    if (status == ER_OK) {
        status = write();
    }
    editLock.Unlock();

    return status;
}

QStatus LSFAboutDataStore::Delete(const char* name, const char* languageTag)
{
    QCC_DbgTrace(("%s: name(%s) languageTag(%s)", __func__, name, languageTag));

    editLock.Lock();
    QStatus status = ER_INVALID_VALUE;

    AboutData factorySettings;
    std::ifstream factoryFile(m_factoryConfigFileName.c_str(), std::ios::binary);
    std::string str((std::istreambuf_iterator<char>(factoryFile)),
                    std::istreambuf_iterator<char>());
    QCC_DbgTrace(("%s: Factory Config: %s", __func__, str.c_str()));
    factorySettings.CreateFromXml(qcc::String(str.c_str()));

    if (strcmp(name, AboutData::APP_ID) == 0) {
        uint8_t* appId;
        size_t num;
        status = factorySettings.GetAppId(&appId, &num);
        if (status == ER_OK) {
            status = SetAppId(appId, num);
        }
    } else if (strcmp(name, AboutData::DEFAULT_LANGUAGE) == 0) {
        char* defaultLanguage;
        status = factorySettings.GetDefaultLanguage(&defaultLanguage);
        if (status == ER_OK) {
            status = SetDefaultLanguage(defaultLanguage);
        }
    } else if (strcmp(name, AboutData::DEVICE_NAME) == 0) {
        status = IsLanguageSupported(languageTag);
        if (status == ER_OK) {
            char* deviceName = NULL;
            status = factorySettings.GetDeviceName(&deviceName, languageTag);
            QCC_DbgTrace(("%s:GetDeviceName status %s", __func__, QCC_StatusText(status)));
            if (status == ER_OK) {
                status = SetDeviceName(deviceName, languageTag);
                QCC_DbgTrace(("%s:SetDeviceName status %s", __func__, QCC_StatusText(status)));
            }
        }
    } else if (strcmp(name, AboutData::DEVICE_ID) == 0) {
        char* deviceId = NULL;
        status = factorySettings.GetDeviceId(&deviceId);
        if (status == ER_OK) {
            status = SetDeviceId(deviceId);
        }
    } else if (strcmp(name, AboutData::APP_NAME) == 0) {
        char* appName;
        status = factorySettings.GetAppName(&appName);
        if (status == ER_OK) {
            status = SetAppName(appName, languageTag);
        }
    } else if (strcmp(name, AboutData::MANUFACTURER) == 0) {
        char* manufacturer = NULL;
        status = factorySettings.GetManufacturer(&manufacturer, languageTag);
        if (status == ER_OK) {
            status = SetManufacturer(manufacturer, languageTag);
        }
    } else if (strcmp(name, AboutData::MODEL_NUMBER) == 0) {
        char* modelNumber = NULL;
        status = factorySettings.GetModelNumber(&modelNumber);
        if (status == ER_OK) {
            status = SetModelNumber(modelNumber);
        }
    } else if (strcmp(name, AboutData::SUPPORTED_LANGUAGES) == 0) {
        size_t langNum;
        langNum = factorySettings.GetSupportedLanguages();
        QCC_DbgTrace(("%s:Number of supported languages %u", __func__, langNum));
        if (langNum > 0) {
            const char** langs = new const char*[langNum];
            factorySettings.GetSupportedLanguages(langs, langNum);
            for (size_t i = 0; i < langNum; ++i) {
                SetSupportedLanguage(langs[i]);
            }
            delete [] langs;
        }
    } else if (strcmp(name, AboutData::DESCRIPTION) == 0) {
        char* description = NULL;
        status = factorySettings.GetDescription(&description, languageTag);
        if (status == ER_OK) {
            status = SetDescription(description, languageTag);
        }
    } else if (strcmp(name, AboutData::DATE_OF_MANUFACTURE) == 0) {
        char* date = NULL;
        status = factorySettings.GetDateOfManufacture(&date);
        if (status == ER_OK) {
            status = SetDateOfManufacture(date);
        }
    } else if (strcmp(name, AboutData::SOFTWARE_VERSION) == 0) {
        char* version = NULL;
        status = factorySettings.GetSoftwareVersion(&version);
        if (status == ER_OK) {
            status = SetSoftwareVersion(version);
        }
    } else if (strcmp(name, AboutData::HARDWARE_VERSION) == 0) {
        char* version = NULL;
        status = factorySettings.GetHardwareVersion(&version);
        if (status == ER_OK) {
            status = SetHardwareVersion(version);
        }
    } else if (strcmp(name, AboutData::SUPPORT_URL) == 0) {
        char* url = NULL;
        status = factorySettings.GetSupportUrl(&url);
        if (status == ER_OK) {
            status = SetSupportUrl(url);
        }
    }

    factoryFile.close();

    if (status == ER_OK) {
        status = write();
    }
    editLock.Unlock();

    return status;
}

const qcc::String& LSFAboutDataStore::GetConfigFileName()
{
    QCC_DbgTrace(("%s", __func__));
    return m_configFileName;
}

QStatus LSFAboutDataStore::write()
{
    editLock.Lock();
    writeConfigSettings();
    QStatus status = ER_OK;

    if (m_controllerService != NULL) {
        status = m_controllerService->Announce();
        QCC_DbgTrace(("%s:Announce status %s", __func__, QCC_StatusText(status)));
    } else {
        status = ER_FAIL;
        QCC_LogError(status, ("%s: m_controllerService pointer is NULL", __func__));
    }
    editLock.Unlock();

    return status;
}

void LSFAboutDataStore::writeFactorySettings(AboutData* aboutData)
{
    editLock.Lock();
    //Generate xml
    qcc::String str = ToXml(aboutData);
    //write to config file
    std::ofstream iniFileWrite(m_factoryConfigFileName.c_str(), std::ofstream::out | std::ofstream::trunc);
    //write to config file
    iniFileWrite.write(str.c_str(), str.length());
    iniFileWrite.close();
    editLock.Unlock();
}

void LSFAboutDataStore::writeConfigSettings(void)
{
    editLock.Lock();
    //Generate xml
    qcc::String str = ToXml(this);
    //write to config file
    std::ofstream iniFileWrite(m_configFileName.c_str(), std::ofstream::out | std::ofstream::trunc);
    //write to config file
    iniFileWrite.write(str.c_str(), str.length());
    iniFileWrite.close();
    editLock.Unlock();
}

QStatus LSFAboutDataStore::IsLanguageSupported(const char* languageTag)
{
    editLock.Lock();
    QStatus status = ER_LANGUAGE_NOT_SUPPORTED;
    QCC_DbgTrace(("%s: languageTag = %s", __func__, languageTag));
    size_t langNum;
    langNum = GetSupportedLanguages();
    QCC_DbgTrace(("%s: Number of supported languages %u", __func__, langNum));
    if (langNum > 0) {
        const char** langs = new const char*[langNum];
        GetSupportedLanguages(langs, langNum);
        for (size_t i = 0; i < langNum; ++i) {
            if (0 == strcmp(languageTag, langs[i])) {
                status = ER_OK;
                break;
            }
        }
        delete [] langs;
    }

    QCC_DbgTrace(("%s: Returning %s", __func__, QCC_StatusText(status)));
    editLock.Unlock();
    return status;
}

bool LSFAboutDataStore::IsLeader(void)
{
    QCC_DbgTrace(("%s", __func__));
    editLock.Lock();
    bool leader = 0;
    MsgArg* tmp;
    GetField("IsLeader", tmp);
    leader = tmp->v_bool;
    QCC_DbgTrace(("%s: return %d\n", __func__, leader));
    editLock.Unlock();
    return leader;
}

void LSFAboutDataStore::SetIsLeader(bool leader)
{
    QCC_DbgTrace(("%s: leader = %u", __func__, leader));

    editLock.Lock();
    MsgArg temp("b", leader);
    QStatus status = SetField("IsLeader", temp);

    if ((status == ER_OK) && (m_controllerService != NULL)) {
        status = m_controllerService->Announce();
        QCC_DbgTrace(("%s:Announce status %s", __func__, QCC_StatusText(status)));
    }
    editLock.Unlock();
}

void LSFAboutDataStore::SetRank(Rank& rank)
{
    QCC_DbgTrace(("%s", __func__));

    editLock.Lock();
    MsgArg rankHigh("t", rank.GetHigherOrderBits());
    SetField("RankHigherBits", rankHigh);

    MsgArg rankLow("t", rank.GetLowerOrderBits());
    SetField("RankLowerBits", rankLow);
    editLock.Unlock();
}

qcc::String LSFAboutDataStore::ToXml(AboutData* aboutData)
{
    QCC_DbgTrace(("%s", __func__));

    QStatus status = ER_OK;
    size_t numFields = aboutData->GetFields();
    if (0 == numFields) {
        QCC_LogError(ER_FAIL, ("%s: numFields is 0", __func__));
        return "";
    }
    const char* fieldNames[512];
    aboutData->GetFields(fieldNames, numFields);
    char* defaultLanguage;
    status = aboutData->GetDefaultLanguage(&defaultLanguage);
    if (ER_OK != status) {
        QCC_LogError(status, ("%s: GetDefaultLanguage failed", __func__));
        return "";
    }
    size_t numLangs = aboutData->GetSupportedLanguages();
    const char** langs = new const char*[numLangs];
    aboutData->GetSupportedLanguages(langs, numLangs);
    qcc::String res;
    res += "<AboutData>\n";
    for (size_t i = 0; i < numFields; i++) {
        ajn::MsgArg* arg;
        char* val;
        aboutData->GetField(fieldNames[i], arg);
        if (!strcmp(fieldNames[i], "AppId")) {
            res += "  <" + qcc::String(fieldNames[i]) + ">";
            size_t lay;
            uint8_t* pay;
            arg->Get("ay", &lay, &pay);
            std::stringstream ss;
            for (size_t j = 0; j < lay; ++j) {
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(pay[j]);
            }
            res += ss.str().c_str();
            res += "</" + qcc::String(fieldNames[i]) + ">\n";
            continue;
        }

        if (arg->Signature() != "s") {
            continue;
        }

        arg->Get("s", &val);
        res += "  <" + qcc::String(fieldNames[i]) + ">";
        res += val;
        res += "</" + qcc::String(fieldNames[i]) + ">\n";
        if (!aboutData->IsFieldLocalized(fieldNames[i])) {
            continue;
        }

        for (size_t j = 0; j < numLangs; j++) {
            if (langs[j] == defaultLanguage) {
                continue;
            }

            res += "  <" + qcc::String(fieldNames[i]) + " lang=\"" + langs[j] + "\">";
            aboutData->GetField(fieldNames[i], arg, langs[j]);
            arg->Get("s", &val);
            res += val;
            res += "</" + qcc::String(fieldNames[i]) + ">\n";
        }
    }
    res += "</AboutData>";

    delete [] langs;
    QCC_DbgTrace(("%s", __func__));
    return res;
}