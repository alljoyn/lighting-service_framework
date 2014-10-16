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
package org.allseen.lsf.helper.model;

import org.allseen.lsf.LampState;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class ColorItemDataModel extends LightingItemDataModel {

    public LampState state;

    protected LampCapabilities capability;

    public ColorItemDataModel(String itemID, char prefix, String itemName) {
        super(itemID, prefix, itemName);

        state = new LampState();
        capability = new LampCapabilities();

        state.setOnOff(false);
        state.setBrightness(0);
        state.setHue(0);
        state.setSaturation(0);
        state.setColorTemp(0);
    }

    public ColorItemDataModel(ColorItemDataModel other) {
        super(other);

        this.state = new LampState(other.state);
        this.capability = other.getCapability();
    }

    public boolean stateEquals(ColorItemDataModel that) {
        return that != null ? stateEquals(that.state) : false;
    }

    public boolean stateEquals(LampState thatState) {
        boolean result = false;

        if (thatState != null) {
            // See if we are comparing full color (hsv) or color temp (ct) values
            boolean modeHSV = this.state.getSaturation() != 0;

            if (modeHSV) {
                result =
                    this.state.getHue() == thatState.getHue() &&
                    this.state.getSaturation() == thatState.getSaturation() &&
                    this.state.getBrightness() == thatState.getBrightness() &&
                    this.state.getOnOff() == thatState.getOnOff();
            } else {
                result =
                    this.state.getSaturation() == thatState.getSaturation() &&
                    this.state.getBrightness() == thatState.getBrightness() &&
                    this.state.getColorTemp() == thatState.getColorTemp() &&
                    this.state.getOnOff() == thatState.getOnOff();
            }
        }

        return result;
    }

    public void setCapability(LampCapabilities capability) {
        this.capability = capability;
    }

    public LampCapabilities getCapability() {
        return capability;
    }
}
