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

#include <LSFKeyListener.h>
#include <qcc/Debug.h>

#define QCC_MODULE "LSF_KEY_LISTENER"
using namespace lsf;

LSFKeyListener::LSFKeyListener() : passCode("000000"), GetPassCode(0)
{
    QCC_DbgTrace(("%s", __func__));
}

LSFKeyListener::~LSFKeyListener()
{
    QCC_DbgTrace(("%s", __func__));
    passCode.secure_clear();
}

void LSFKeyListener::SetPassCode(qcc::String const& code)
{
    QCC_DbgTrace(("%s", __func__));
    passCode = code;
}

void LSFKeyListener::SetGetPassCodeFunc(const char* (*GetPassCodeFunc)())
{
    QCC_DbgTrace(("%s", __func__));
    GetPassCode = GetPassCodeFunc;
}

bool LSFKeyListener::RequestCredentials(const char* authMechanism, const char* authPeer,
                                        uint16_t authCount, const char* userId, uint16_t credMask, Credentials& creds)
{
    QCC_DbgTrace(("%s", __func__));
    if (strcmp(authMechanism, "ALLJOYN_ECDHE_PSK") == 0 || strcmp(authMechanism, "ALLJOYN_PIN_KEYX") == 0) {
        if (credMask & AuthListener::CRED_PASSWORD) {
            if (authCount <= 3) {
                const char* passCodeFromGet = 0;
                if (GetPassCode) {
                    passCodeFromGet = GetPassCode();
                }
                creds.SetPassword(passCodeFromGet ? passCodeFromGet : passCode.c_str());
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

void LSFKeyListener::AuthenticationComplete(const char* authMechanism, const char* authPeer, bool success)
{
    QCC_DbgTrace(("%s", __func__));
}