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

public class LampStateUniformity {

    public boolean power;
    public boolean hue;
    public boolean saturation;
    public boolean brightness;
    public boolean colorTemp;

    public LampStateUniformity() {
        this(true, true, true, true, true);
    }

    public LampStateUniformity(boolean power, boolean hue, boolean saturation, boolean brightness, boolean colorTemp) {
        this.power = power;
        this.hue = hue;
        this.saturation = saturation;
        this.brightness = brightness;
        this.colorTemp = colorTemp;
    }

    public LampStateUniformity(LampStateUniformity other) {
        this.power = other.power;
        this.hue = other.hue;
        this.saturation = other.saturation;
        this.brightness = other.brightness;
        this.colorTemp = other.colorTemp;
    }
}
