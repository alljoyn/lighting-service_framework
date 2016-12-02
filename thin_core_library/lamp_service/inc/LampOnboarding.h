#ifndef _LAMP_ONBOARDING_H_
#define _LAMP_ONBOARDING_H_
/**
 * @file LampOnboarding.h
 * @defgroup lamp_onb On-boarding service APIs used by the Lamp Service
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

#ifdef ONBOARDING_SERVICE

#include <alljoyn/onboarding/OnboardingManager.h>

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