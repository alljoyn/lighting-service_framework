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

public class StateTransitionEffect extends DefaultNativeClassWrapper implements TransitionEffect {
    public StateTransitionEffect() {
        createNativeObject();
    }

    public StateTransitionEffect(StateTransitionEffect other) {
        this();

        this.setLamps(other.getLamps());
        this.setLampGroups(other.getLampGroups());
        this.setTransitionPeriod(other.getTransitionPeriod());
        this.setLampState(other.getLampState());
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
    public native void setTransitionPeriod(long transitionPeriod);
    @Override
    public native long getTransitionPeriod();

    public native void setLampState(LampState lampState);
    public native LampState getLampState();

    @Override
    public native String toString();

    @Override
    protected native void createNativeObject();

    @Override
    protected native void destroyNativeObject();
}
