#ifndef _LAMP_STATE_H_
#define _LAMP_STATE_H_
/**
 * @file LampState.h
 * @defgroup lamp_state The Lamp State related APIs used by the Lamp Service
 * @{
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

#include <alljoyn.h>

#include <LampResponseCodes.h>

/**
 * Struct LampState.
 * This is used to store the various lamp state fields.
 */
typedef struct _LampState {
    uint32_t hue;           /**< The lamp hue (in degree). 0 means 0. uint32_max-1 means 360. */
    uint32_t saturation;    /**< The lamp saturation (in percentage). 0 means 0. uint32_max-1 means 100. */
    uint32_t colorTemp;     /**< The lamp color temperature (in Kevin). 0 means 1000K. uint32_max-1 means 20000K. OEM implementation will need to take care of the scaling. */
    uint32_t brightness;    /**< The lamp brightness (in percentage). 0 means 0. uint32_max-1 means 100. */
    uint8_t onOff;          /**< Is the lamp on or off */
} LampState;

/**
 * Unmarshal the LampState fields from a message
 *
 * @param[out] state The lamp state container to read into
 * @param[in]  msg   The message to read from
 * @return Status of the operation
 */
LampResponseCode LAMP_UnmarshalState(LampState* state, AJ_Message* msg);

/**
 * Serialize the Lamp's current state
 *
 * @param state The state to marshal
 * @param msg   The msg to serialize data into
 * @return Status of the operation
 */
LampResponseCode LAMP_MarshalState(LampState* state, AJ_Message* msg);

/**
 * Initialize the LampState by reading it from NVRAM
 *
 * @param None
 * @param None
 */
void LAMP_InitializeState(void);

/**
 * Get the current LampState
 *
 * @param[out] state    A pointer to a LampState object
 */
void LAMP_GetState(LampState* state);

/**
 * Set the lamp state and save it in NVRAM.
 *
 * @param[in] state The new state
 */
void LAMP_SetState(const LampState* state);

/**
 * Clear the lamp state
 *
 * @param None
 * @param None
 */
void LAMP_ClearState(void);

/**
 * @}
 */
#endif