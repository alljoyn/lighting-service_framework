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

#include <LSFResponseCodes.h>
#include <LSFTypes.h>

namespace lsf {

const char* LSFResponseCodeText(LSFResponseCode responseCode)
{
    switch (responseCode) {
        LSF_CASE(LSF_OK);
        LSF_CASE(LSF_ERR_NULL);
        LSF_CASE(LSF_ERR_UNEXPECTED);
        LSF_CASE(LSF_ERR_INVALID);
        LSF_CASE(LSF_ERR_UNKNOWN);
        LSF_CASE(LSF_ERR_FAILURE);
        LSF_CASE(LSF_ERR_BUSY);
        LSF_CASE(LSF_ERR_REJECTED);
        LSF_CASE(LSF_ERR_RANGE);
        LSF_CASE(LSF_ERR_INVALID_FIELD);
        LSF_CASE(LSF_ERR_MESSAGE);
        LSF_CASE(LSF_ERR_INVALID_ARGS);
        LSF_CASE(LSF_ERR_EMPTY_NAME);
        LSF_CASE(LSF_ERR_RESOURCES);
        LSF_CASE(LSF_ERR_REPLY_WITH_INVALID_ARGS);
        LSF_CASE(LSF_ERR_PARTIAL);
        LSF_CASE(LSF_ERR_NOT_FOUND);
        LSF_CASE(LSF_ERR_NO_SLOT);
        LSF_CASE(LSF_ERR_DEPENDENCY);
        LSF_CASE(LSF_RESPONSE_CODE_LAST);

    default:
        return "<unknown>";
    }
}

}