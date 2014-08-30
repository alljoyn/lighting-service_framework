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
package org.allseen.lsf.sampleapp;

public class CapabilityData {

    public static final int UNSET = -1;
    public static final int NONE = 0;
    public static final int SOME = 1;
    public static final int ALL = 2;

    public int dimmable;
    public int color;
    public int temp;

    // creates a dirty, unset CapabilityData
    public CapabilityData() {
        dimmable = UNSET;
        color = UNSET;
        temp = UNSET;
    }

    public CapabilityData(CapabilityData other) {
        dimmable = other.dimmable;
        color = other.color;
        temp = other.temp;
    }

    public CapabilityData(boolean dimmable, boolean color, boolean temp) {
        this.dimmable = dimmable ? ALL : NONE;
        this.color = color ? ALL : NONE;
        this.temp = temp ? ALL : NONE;
    }

    public void includeData(CapabilityData data) {
        if (data != null) {
            dimmable = combine(dimmable, data.dimmable);
            color = combine(color, data.color);
            temp = combine(temp, data.temp);
        } else {
            // there is nothing to do with a null CapabilityData except to treat it as UNSET
            // that scenario reduces to doing nothing
            return;
        }
    }

    // determines if this capability is mixed
    public boolean isMixed() {
        // all lamps of the same type implies that there will be no SOME capability state
        // mixed lamps implies at least 1 mismatch in capabilities implies at least 1 SOME capability state
        return (dimmable == SOME) || (color == SOME) || (temp == SOME);
    }

    // determines if the combination of a and b is unset, none, some, or all
    private int combine(int a, int b) {
        switch (a) {
            case UNSET:
                // there's no real data in a, use b
                return b;
            case NONE:
                // none if b is none or unset, else some
                return (b == NONE || b == UNSET) ? NONE : SOME;
            case SOME:
                // some will always be some
                return SOME;
            case ALL:
                // all if b is all or unset, else some
                return (b == ALL || b == UNSET) ? ALL : SOME;
            default:
                return UNSET;
        }
    }

    @Override
    public String toString() {
        return "[dimmable: " + dimmable + " color: " + color + " temp: " + temp + " ]";
    }
}
