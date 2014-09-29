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

public enum LampType {
    INVALID,
    A15,
    A17,
    A19,
    A20,
    A21,
    A23,
    AR70,
    AR111,
    B8,
    B10,
    B11,
    B13,
    BR25,
    BR30,
    BR38,
    BR40,
    BT15,
    BT28,
    BT37,
    BT56,
    C6,
    C7,
    C9,
    C11,
    C15,
    CA5,
    CA7,
    CA8,
    CA10,
    CA11,
    E17,
    E18,
    E23,
    E25,
    E37,
    ED17,
    ED18,
    ED23,
    ED28,
    ED37,
    F10,
    F15,
    F20,
    G9,
    G11,
    G12,
    G16,
    G19,
    G25,
    G30,
    G40,
    T2,
    T3,
    T4,
    T5,
    T6,
    T7,
    T8,
    T9,
    T10,
    T12,
    T14,
    T20,
    MR8,
    MR11,
    MR16,
    MR20,
    PAR14,
    PAR16,
    PAR20,
    PAR30,
    PAR36,
    PAR38,
    PAR46,
    PAR56,
    PAR64,
    PS25,
    PS35,
    R12,
    R14,
    R16,
    R20,
    R25,
    R30,
    R40,
    RP11,
    S6,
    S8,
    S11,
    S14,
    ST18,
    LASTVALUE;

    /** Static lookup, used by the native code */
    @SuppressWarnings("unused")
    private static LampType fromValue(int value) {
        for (LampType c : LampType.values()) {
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
