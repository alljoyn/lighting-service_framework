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

public class EmptyLampState extends LampState {
    public static EmptyLampState instance = new EmptyLampState();

    private EmptyLampState() {}

    //TODO-FIX getters should all return null when superclass methods are defined to all return objects
    @Override
    public void setOnOff(boolean onOff)         { /* Do nothing */ }
    @Override
    public boolean getOnOff()                   { return false; }
    @Override
    public void setHue(long hue)                { /* Do nothing */ }
    @Override
    public long getHue()                        { return 0; }
    @Override
    public void setSaturation(long saturation)  { /* Do nothing */ }
    @Override
    public long getSaturation()                 { return 0; }
    @Override
    public void setColorTemp(long colorTemp)    { /* Do nothing */ }
    @Override
    public long getColorTemp()                  { return 0; }
    @Override
    public void setBrightness(long brightness)  { /* Do nothing */ }
    @Override
    public long getBrightness()                 { return 0; }
}
