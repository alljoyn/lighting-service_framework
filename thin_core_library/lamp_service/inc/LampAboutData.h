#ifndef _LAMP_ABOUT_DATA_H_
#define _LAMP_ABOUT_DATA_H_
/**
 * @file LampAboutData.h
 * @defgroup lamp_about About/Config PropertyStore used by the Lamp Service
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

#include <aj_status.h>
#include <aj_msg.h>
#include <aj_debug.h>

/**
 * Set up the About and Config Service data structures
 *
 * @param   None
 * @return  None
 */
void LAMP_SetupAboutConfigData(void);

/**
 * Callback to set the AllJoyn password
 *
 * @param buffer    The password buffer
 * @param bufLen    The buffer length
 * @return          The length of the password written to buffer
 */
uint32_t LAMP_PasswordCallback(uint8_t* buffer, uint32_t bufLen);


/**
 * Get the Lamp ID.  If the ID does not yet exist,
 * generate and persist it before returning it.
 *
 * @param   None
 * @return  The Lamp ID
 */
const char* LAMP_GetID(void);

/**
 * Get the Lamp Name
 *
 * @param   None
 * @return  The Lamp name
 */
const char* LAMP_GetName(void);

/**
 * Set the Lamp Name
 *
 * @param name  The Lamp name
 * @return      None
 */
void LAMP_SetName(const char* name);

/**
 * @}
 */
#endif