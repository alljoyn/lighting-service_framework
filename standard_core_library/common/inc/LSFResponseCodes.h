#ifndef _LSF_RESPONSE_CODES_H_
#define _LSF_RESPONSE_CODES_H_
/**
 * \ingroup Common
 */
/**
 * \file  common/inc/LSFResponseCode.h
 * This file provides definitions for LSF response codes
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
 * \ingroup Common
 */
#include <LampResponseCodes.h>

namespace lsf {
/**
 * Response code types
 */
typedef enum {
    LSF_OK                          = LAMP_OK,                          /**< Success status */
    LSF_ERR_NULL                    = LAMP_ERR_NULL,                    /**< Unexpected NULL pointer */
    LSF_ERR_UNEXPECTED              = LAMP_ERR_UNEXPECTED,              /**< An operation was unexpected at this time */
    LSF_ERR_INVALID                 = LAMP_ERR_INVALID,                 /**< A value was invalid */
    LSF_ERR_UNKNOWN                 = LAMP_ERR_UNKNOWN,                 /**< A unknown value */
    LSF_ERR_FAILURE                 = LAMP_ERR_FAILURE,                 /**< A failure has occurred */
    LSF_ERR_BUSY                    = LAMP_ERR_BUSY,                    /**< An operation failed and should be retried later */
    LSF_ERR_REJECTED                = LAMP_ERR_REJECTED,                /**< The request was rejected */
    LSF_ERR_RANGE                   = LAMP_ERR_RANGE,                   /**< Value provided was out of range */
    LSF_ERR_INVALID_FIELD           = LAMP_ERR_INVALID_FIELD,           /**< Invalid param/state field */
    LSF_ERR_MESSAGE                 = LAMP_ERR_MESSAGE,                 /**< Invalid message */
    LSF_ERR_INVALID_ARGS            = LAMP_ERR_INVALID_ARGS,            /**< The arguments were invalid */
    LSF_ERR_EMPTY_NAME              = LAMP_ERR_EMPTY_NAME,              /**< The name is empty */
    LSF_ERR_RESOURCES               = LAMP_ERR_RESOURCES,               /**< not enough resources */
    LSF_ERR_REPLY_WITH_INVALID_ARGS = LAMP_ERR_REPLY_WITH_INVALID_ARGS, /**< The reply received for a message had invalid arguments */
    LSF_ERR_PARTIAL                 = LAMP_RESPONSE_CODE_LAST,          /**< The requested operation was only partially successful */
    LSF_ERR_NOT_FOUND               = LAMP_RESPONSE_CODE_LAST + 1,      /**< The entity of interest was not found */
    LSF_ERR_NO_SLOT                 = LAMP_RESPONSE_CODE_LAST + 2,      /**< There is no slot for new entry */
    LSF_ERR_DEPENDENCY              = LAMP_RESPONSE_CODE_LAST + 3,      /**< There is a dependency of the entity for which a delete request was received */
    LSF_RESPONSE_CODE_LAST          = LAMP_RESPONSE_CODE_LAST + 4       /**< The last LSF response code */
} LSFResponseCode;

/**
 * Return a descriptive text corresponding to the responseCode
 *
 * @param  responseCode Response Code
 * @return Descriptive text corresponding to responseCode
 */
const char* LSFResponseCodeText(LSFResponseCode responseCode);

}

#endif