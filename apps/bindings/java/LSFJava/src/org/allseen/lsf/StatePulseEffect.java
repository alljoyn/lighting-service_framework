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

public class StatePulseEffect extends DefaultNativeClassWrapper implements PulseEffect {
    public StatePulseEffect() {
        createNativeObject();
    }

    public StatePulseEffect(StatePulseEffect other) {
        this();

        this.setLamps(other.getLamps());
        this.setLampGroups(other.getLampGroups());
        this.setPulsePeriod(other.getPulsePeriod());
        this.setPulseDuration(other.getPulseDuration());
        this.setPulseCount(other.getPulseCount());
        this.setFromLampState(other.getFromLampState());
        this.setToLampState(other.getToLampState());
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
    public native void setPulsePeriod(long pulsePeriod);
    @Override
    public native long getPulsePeriod();

    @Override
    public native void setPulseDuration(long pulseDuration);
    @Override
    public native long getPulseDuration();

    @Override
    public native void setPulseCount(long pulseCount);
    @Override
    public native long getPulseCount();

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
