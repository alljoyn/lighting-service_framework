#ifndef _LAMP_ONBOARDING_H_
#define _LAMP_ONBOARDING_H_
/**
 * @file LampOnboarding.h
 * @defgroup lamp_onb On-boarding service APIs used by the Lamp Service
 * @{
 */
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

#ifdef ONBOARDING_SERVICE

#include <alljoyn/onboarding/OnboardingManager.h>

// these are not explicitly declared in the onboarding manager header

/**
 * Check if the device is connected to a WIFI network
 *
 * @param  None
 * @return TRUE if connected to wifi
 */
int8_t AJOBS_ControllerAPI_IsWiFiClient(void);

/**
 * Check if the device is acting as a Soft AP
 *
 * @param  None
 * @return TRUE if a soft AP
 */
int8_t AJOBS_ControllerAPI_IsWiFiSoftAP(void);

/**
 * Initialize the Onboarding service
 *
 * @param  None
 * @return Status of the operation
 */
AJ_Status LAMP_InitOnboarding(void);

#endif

/**
 * @}
 */
#endif
