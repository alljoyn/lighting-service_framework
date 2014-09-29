/*
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
 */

package org.allseen.lsf;

public enum ErrorCode {
    NONE,
    ERR_REGISTERING_SIGNAL_HANDLERS,
    ERR_NO_ACTIVE_CONTROLLER_SERVICE_FOUND,
    ERR_ALLJOYN_METHOD_CALL_TIMEOUT,
    ERR_IRRECOVERABLE,
    ERR_DISCONNECTED_FROM_BUS,
    ERR_CONTROLLER_CLIENT_EXITING,
    ERROR_CODE_LAST;

    /** Static lookup, used by the native code */
    @SuppressWarnings("unused")
    private static ErrorCode fromValue(int value) {
        for (ErrorCode c : ErrorCode.values()) {
            if (c.getValue() == value) {
                return c;
            }
        }

        return null;
    }

    /**
     * Gets the integer value.
     *
     * @return the integer value
     */
    public int getValue() { return ordinal(); }
}
