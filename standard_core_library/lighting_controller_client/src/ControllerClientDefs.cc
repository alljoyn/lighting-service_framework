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

#include <ControllerClientDefs.h>
#include <LSFTypes.h>

namespace lsf {

const char* ControllerClientErrorText(ErrorCode errorCode)
{
    switch (errorCode) {
        LSF_CASE(ERROR_NONE);
        LSF_CASE(ERROR_REGISTERING_SIGNAL_HANDLERS);
        LSF_CASE(ERROR_NO_ACTIVE_CONTROLLER_SERVICE_FOUND);
        LSF_CASE(ERROR_ALLJOYN_METHOD_CALL_TIMEOUT);
        LSF_CASE(ERROR_IRRECOVERABLE);
        LSF_CASE(ERROR_DISCONNECTED_FROM_BUS);
        LSF_CASE(ERROR_CONTROLLER_CLIENT_EXITING);
        LSF_CASE(ERROR_MESSAGE_WITH_INVALID_ARGS);
        LSF_CASE(ERROR_LAST_VALUE);

    default:
        return "<unknown>";
    }
}

const char* ControllerClientStatusText(ControllerClientStatus status)
{
    switch (status) {
        LSF_CASE(CONTROLLER_CLIENT_OK);
        LSF_CASE(CONTROLLER_CLIENT_ERR_NOT_CONNECTED);
        LSF_CASE(CONTROLLER_CLIENT_ERR_FAILURE);
        LSF_CASE(CONTROLLER_CLIENT_ERR_RETRY);
        LSF_CASE(CONTROLLER_CLIENT_LAST_VALUE);

    default:
        return "<unknown>";
    }
}

}