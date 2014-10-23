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

public class LampState extends DefaultNativeClassWrapper {
	public LampState() {
		createNativeObject();
	}

	public LampState(LampState other) {
		this();

		this.setOnOff(other.getOnOff());
		this.setHue(other.getHue());
		this.setSaturation(other.getSaturation());
		this.setBrightness(other.getBrightness());
		this.setColorTemp(other.getColorTemp());

		// The setNull() call currently needs to be last due to the side
		// effects of the other setXXX() calls.
		this.setNull(other.isNull());
	}

    //TODO-FIX The get*() methods returning primitives should return their
    //         Object equivalent so that we can return NULL on failure
    public native void setOnOff(boolean onOff);
    public native boolean getOnOff();

    public native void setHue(long hue);
    public native long getHue();

    public native void setSaturation(long saturation);
    public native long getSaturation();

    public native void setColorTemp(long colorTemp);
    public native long getColorTemp();

    public native void setBrightness(long brightness);
    public native long getBrightness();

    protected native void setNull(boolean isNull);
    public native boolean isNull();

	@Override
	protected native void createNativeObject();

	@Override
	protected native void destroyNativeObject();
}

