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

public class Scene extends DefaultNativeClassWrapper {
    public Scene() {
        createNativeObject();
    }

    public Scene(Scene other) {
        this();

        this.setStateTransitionEffects(other.getStateTransitionEffects());
        this.setPresetTransitionEffects(other.getPresetTransitionEffects());
        this.setStatePulseEffects(other.getStatePulseEffects());
        this.setPresetPulseEffects(other.getPresetPulseEffects());
    }

    public native void setStateTransitionEffects(StateTransitionEffect[] effects);
    public native StateTransitionEffect[] getStateTransitionEffects();

    public native void setPresetTransitionEffects(PresetTransitionEffect[] effects);
    public native PresetTransitionEffect[] getPresetTransitionEffects();

    public native void setStatePulseEffects(StatePulseEffect[] effects);
    public native StatePulseEffect[] getStatePulseEffects();

    public native void setPresetPulseEffects(PresetPulseEffect[] effects);
    public native PresetPulseEffect[] getPresetPulseEffects();

    @Override
    public native String toString();

    @Override
    protected native void createNativeObject();

    @Override
    protected native void destroyNativeObject();
}
