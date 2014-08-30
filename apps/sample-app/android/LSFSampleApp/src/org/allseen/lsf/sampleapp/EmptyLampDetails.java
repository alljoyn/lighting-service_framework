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

import org.allseen.lsf.BaseType;
import org.allseen.lsf.DeviceType;
import org.allseen.lsf.LampDetails;
import org.allseen.lsf.LampMake;
import org.allseen.lsf.LampModel;
import org.allseen.lsf.LampType;

public class EmptyLampDetails extends LampDetails {
    public static EmptyLampDetails instance = new EmptyLampDetails();

    private EmptyLampDetails()              { }

    //TODO-FIX these should all return null when superclass methods are defined to all return objects
    @Override
    public LampMake getMake()               { return null; }
    @Override
    public LampModel getModel()             { return null; }
    @Override
    public DeviceType getType()             { return null; }
    @Override
    public LampType getLampType()           { return null; }
    @Override
    public BaseType getLampBaseType()       { return null; }
    @Override
    public int getLampBeamAngle()           { return 0; }
    @Override
    public boolean isDimmable()             { return false; }
    @Override
    public boolean hasColor()               { return false; }
    @Override
    public boolean hasVariableColorTemp()   { return false; }
    @Override
    public boolean hasEffects()             { return false; }
    @Override
    public int getMinVoltage()              { return 0; }
    @Override
    public int getMaxVoltage()              { return 0; }
    @Override
    public int getWattage()                 { return 0; }
    @Override
    public int getIncandescentEquivalent()  { return 0; }
    @Override
    public int getMaxLumens()               { return 0; }
    @Override
    public int getMinTemperature()          { return 0; }
    @Override
    public int getMaxTemperature()          { return 0; }
    @Override
    public int getColorRenderingIndex()     { return 0; }
    @Override
    public String getLampID()               { return null; }
}
