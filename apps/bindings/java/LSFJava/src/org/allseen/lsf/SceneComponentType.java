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

//TODO-FIX Bring up to latest library version
public enum SceneComponentType {
    UNKNOWN(0x0),
    LAMPS_AND_STATE(0x1),
    LAMPS_AND_SAVED_STATE(0x2),
    LAMP_GROUPS_AND_STATE(0x3),
    LAMP_GROUPS_AND_SAVED_STATE(0x4);

    /** Integer value */
    private int value;

    /** Constructor */
    private SceneComponentType(int value) {
        this.value = value;
    }

    /** Static lookup, used by the native code */
    @SuppressWarnings("unused")
    private static SceneComponentType fromValue(int value) {
        for (SceneComponentType c : SceneComponentType.values()) {
            if (c.getValue() == value) {
                return c;
            }
        }

        return UNKNOWN;
    }

    /**
     * Gets the integer value.
     *
     * @return the integer value
     */
    public int getValue() { return value; }
}
