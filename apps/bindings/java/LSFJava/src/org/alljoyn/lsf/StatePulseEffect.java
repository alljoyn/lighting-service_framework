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

public class StatePulseEffect extends DefaultNativeClassWrapper implements PulseEffect {
    public StatePulseEffect() {
        createNativeObject();
    }

    public StatePulseEffect(StatePulseEffect other) {
        //TODO-IMPL
    }

    @Override
    public native void setLamps(String[] lampIDs);
    @Override
    public native String[] getLamps();

    @Override
    public native void setLampGroups(String[] lampGroupIDs);
    @Override
    public native String[] getLampGroups();

    @Override
    public native void setPulsePeriod(int pulsePeriod);
    @Override
    public native int getPulsePeriod();

    @Override
    public native void setPulseDuration(int pulseDuration);
    @Override
    public native int getPulseDuration();

    @Override
    public native void setPulseCount(int pulseCount);
    @Override
    public native int getPulseCount();

    public native void setFromLampState(LampState lampState);
    public native LampState getFromLampState();

    public native void setToLampState(LampState lampState);
    public native LampState getToLampState();

    @Override
    public native String toString();

    @Override
    protected native void createNativeObject();

    @Override
    protected native void destroyNativeObject();
}
