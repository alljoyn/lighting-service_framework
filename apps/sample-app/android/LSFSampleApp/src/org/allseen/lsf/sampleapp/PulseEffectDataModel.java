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

import org.allseen.lsf.LampState;

public class PulseEffectDataModel extends BasicSceneElementDataModel {
    public static String defaultName = "";

    public boolean startWithCurrent;

    public String endPresetID;
    public LampState endState;

    public long period;
    public long duration;
    public long count;

    public PulseEffectDataModel() {
        super(EffectType.Pulse, defaultName);

        this.startWithCurrent = false;

        this.endPresetID = null;
        this.endState = new LampState();

        // State is always set to "on". To turn the lamp off as part of an effect,
        // you have to set the brightness to zero
        this.endState.setOnOff(true);

        this.period = 1000;
        this.duration = 500;
        this.count = 10;
    }

    public PulseEffectDataModel(PulseEffectDataModel other) {
        super(other);

        this.startWithCurrent = other.startWithCurrent;

        this.endPresetID = other.endPresetID;
        this.endState = new LampState(other.endState);

        this.period = other.period;
        this.duration = other.duration;
        this.count = other.count;
    }
}
