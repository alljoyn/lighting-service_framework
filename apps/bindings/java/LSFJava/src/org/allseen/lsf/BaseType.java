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

public enum BaseType {
    INVALID,
    E5,
    E10,
    E11,
    E12,
    E14,
    E17,
    E26,
    E27,
    E29,
    E39,
    LASTVALUE;

    /** Static lookup, used by the native code */
    @SuppressWarnings("unused")
    private static BaseType fromValue(int value) {
        for (BaseType c : BaseType.values()) {
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
