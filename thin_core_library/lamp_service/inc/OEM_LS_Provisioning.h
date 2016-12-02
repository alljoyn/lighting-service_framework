#ifndef _OEM_LS_PROVISIONING_H_
#define _OEM_LS_PROVISIONING_H_
/**
 * @file OEM_LS_Provisioning.h
 * @defgroup property_store The OEM-defined properties used by About/Config Service
 * @{
 */
/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

/**
 * Initialize the property store.  This will initialize our propertystore to
 * the default property values and then attempt to overwrite them with values
 * stored in NVRAM.  This function only needs to be changed if the OEM chooses
 * to add config properties.
 *
 * @param  None
 * @return Status of the operation
 */
AJ_Status PropertyStore_Init(void);

/**
 * Persist device ID.
 * Device ID is a unique identifier for this device.
 * The Lamp Service will generate the DeviceID the first time it boots up.
 * Since the ID must persist across calls to FactoryReset, the Lamp Service must persist it separately from the Config data.
 * This function is called to persist the ID.
 *
 * @param  None
 * @return None
 */
void SavePersistentDeviceId(void);

/**
 * The max length of the default language name
 */
#define LANG_VALUE_LENGTH 7

/**
 * The max length of the realm name
 */
#define KEY_VALUE_LENGTH 10

/**
 * The max length of the machine ID
 */
#define MACHINE_ID_LENGTH (UUID_LENGTH * 2)

/**
 * The max length of the device name
 */
#define DEVICE_NAME_VALUE_LENGTH LSF_MAX_NAME_LENGTH

/**
 * The max length of the password
 */
#define PASSWORD_VALUE_LENGTH (AJ_ADHOC_LEN * 2)

#endif