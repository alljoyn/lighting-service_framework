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
package org.alljoyn.lsf.sampleapp;

import org.alljoyn.lsf.LampState;

public class DimmableItemDataModel extends ItemDataModel {

    public LampState state;

    protected CapabilityData capability;

    public DimmableItemDataModel() {
        this("", "");
    }

    public DimmableItemDataModel(String itemID, String itemName) {
        super(itemID, itemName);

        state = new LampState();
        capability = new CapabilityData();

        state.setOnOff(false);
        state.setBrightness(0);
        state.setHue(0);
        state.setSaturation(0);
        state.setColorTemp(0);
    }

    public DimmableItemDataModel(DimmableItemDataModel other) {
        super(other);

        this.state = new LampState(other.state);
        this.capability = other.getCapability();
    }

    public boolean stateEquals(DimmableItemDataModel that) {
        boolean result = false;

        if (that != null) {
            // See if we are comparing full color (hsv) or color temp (ct) values
            boolean modeHSV = this.state.getSaturation() != 0;

            if (modeHSV) {
                result =
                    this.state.getHue() == that.state.getHue() &&
                    this.state.getSaturation() == that.state.getSaturation() &&
                    this.state.getBrightness() == that.state.getBrightness();
            } else {
                result =
                    this.state.getSaturation() == that.state.getSaturation() &&
                    this.state.getBrightness() == that.state.getBrightness() &&
                    this.state.getColorTemp() == that.state.getColorTemp();
            }
        }

        return result;
    }

    public void setCapability(CapabilityData capability) {
        this.capability = capability;
    }

    public CapabilityData getCapability() {
        return capability;
    }
}
