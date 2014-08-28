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

package org.alljoyn.lsf;

public enum ResponseCode {
    OK(0x00),
    ERR_NULL(0x01),
    ERR_UNEXPECTED(0x02),
    ERR_INVALID(0x03),
    ERR_UNKNOWN(0x04),
    ERR_FAILURE(0x05),
    ERR_BUSY(0x06),
    ERR_REJECTED(0x07),
    ERR_RANGE(0x08),
//  ERR_???(0x09),
    ERR_INVALID_FIELD(0x0A),
//  ERR_MESSAGE(0x0B),
    ERR_INVALID_ARGS(0x0C),
    ERR_PARTIAL(0x0D),
    ERR_NOT_FOUND(0x0E),
    ERR_NO_SLOT(0x0F),
    ERR_DEPENDENCY(0x10);

    /** Integer value */
    private int value;

    /** Constructor */
    private ResponseCode(int value) {
        this.value = value;
    }

    /** Static lookup, used by the native code */
    @SuppressWarnings("unused")
    private static ResponseCode fromValue(int value) {
        for (ResponseCode c : ResponseCode.values()) {
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
    public int getValue() { return value; }
}
