#ifndef _LAMP_RESPONSE_CODES_H_
#define _LAMP_RESPONSE_CODES_H_
/**
 * @file LSFResponseCodes.h
 * @defgroup LSFResponseCodes Lamp Response Codes
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
 * Lamp Response Codes
 * REMINDER: Update LSFResponseCodes.h in standard_core_library/common/inc if adding a new Lamp Response code.
 */
typedef enum {
    LAMP_OK                          = 0,  /**< Success status */
    LAMP_ERR_NULL                    = 1,  /**< Unexpected NULL pointer */
    LAMP_ERR_UNEXPECTED              = 2,  /**< An operation was unexpected at this time */
    LAMP_ERR_INVALID                 = 3,  /**< A value was invalid */
    LAMP_ERR_UNKNOWN                 = 4,  /**< A unknown value */
    LAMP_ERR_FAILURE                 = 5,  /**< A failure has occurred */
    LAMP_ERR_BUSY                    = 6,  /**< An operation failed and should be retried later */
    LAMP_ERR_REJECTED                = 7,  /**< The connection was rejected */
    LAMP_ERR_RANGE                   = 8,  /**< Value provided was out of range */
    LAMP_ERR_INVALID_FIELD           = 10, /**< Invalid param/state field */
    LAMP_ERR_MESSAGE                 = 11, /**< An invalid message was received */
    LAMP_ERR_INVALID_ARGS            = 12, /**< The arguments were invalid */
    LAMP_ERR_EMPTY_NAME              = 13, /**< The name was empty */
    LAMP_ERR_RESOURCES               = 14, /**< Out of memory*/
    LAMP_ERR_REPLY_WITH_INVALID_ARGS = 15, /**< The reply received for a message had invalid arguments */
    LAMP_RESPONSE_CODE_LAST                /**< The last Lamp Response code */
} LampResponseCode;

/**
 * @}
 */
#endif