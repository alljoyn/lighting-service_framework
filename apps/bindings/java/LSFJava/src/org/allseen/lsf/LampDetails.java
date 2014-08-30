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

public class LampDetails extends DefaultNativeClassWrapper {
    public LampDetails() {
        createNativeObject();
    }

    public LampDetails(LampDetails other) {
        //TODO-IMPL
    }

    //TODO-FIX The methods returning primitives should return their
    //         Object equivalent so that we can return NULL on failure
    public native LampMake getMake();
    public native LampModel getModel();
    public native DeviceType getType();
    public native LampType getLampType();
    public native BaseType getLampBaseType();
    public native int getLampBeamAngle();
    public native boolean isDimmable();
    public native boolean hasColor();
    public native boolean hasVariableColorTemp();
    public native boolean hasEffects();
    public native int getMinVoltage();
    public native int getMaxVoltage();
    public native int getWattage();
    public native int getIncandescentEquivalent();
    public native int getMaxLumens();
    public native int getMinTemperature();
    public native int getMaxTemperature();
    public native int getColorRenderingIndex();
    public native String getLampID();

    @Override
    protected native void createNativeObject();

    @Override
    protected native void destroyNativeObject();
}
