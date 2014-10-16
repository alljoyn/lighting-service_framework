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
public class ColorStateConverter {
    public static final long UINT32_MAX = 0xffffffffL;
    public static final int VIEW_HUE_MIN = 0;
    public static final int VIEW_HUE_SPAN = 360;
    public static final int VIEW_SATURATION_MIN = 0;
    public static final int VIEW_SATURATION_SPAN = 100;
    public static final int VIEW_BRIGHTNESS_MIN = 0;
    public static final int VIEW_BRIGHTNESS_SPAN = 100;
    public static final int VIEW_COLORTEMP_MIN = 2700;
    public static final int VIEW_COLORTEMP_SPAN = 9000 - VIEW_COLORTEMP_MIN;

    public static int convertHueModelToView(long modelHue) {
        return convertModelToView(modelHue, VIEW_HUE_MIN, VIEW_HUE_SPAN);
    }

    public static long convertHueViewToModel(int viewHue) {
        return convertViewToModel(viewHue, VIEW_HUE_MIN, VIEW_HUE_SPAN);
    }

    public static int convertSaturationModelToView(long modelSaturation) {
        return convertModelToView(modelSaturation, VIEW_SATURATION_MIN, VIEW_SATURATION_SPAN);
    }

    public static long convertSaturationViewToModel(int viewSaturation) {
        return convertViewToModel(viewSaturation, VIEW_SATURATION_MIN, VIEW_SATURATION_SPAN);
    }

    public static int convertBrightnessModelToView(LampState lampState) {
        return convertBrightnessModelToView(lampState.getBrightness());
    }

    public static int convertBrightnessModelToView(long modelBrightness) {
        return convertModelToView(modelBrightness, VIEW_BRIGHTNESS_MIN, VIEW_BRIGHTNESS_SPAN);
    }

    public static void convertBrightnessViewToModel(int viewBrightness, LampState lampState) {
        lampState.setBrightness(convertBrightnessViewToModel(viewBrightness));
    }

    public static long convertBrightnessViewToModel(int viewBrightness) {
        return convertViewToModel(viewBrightness, VIEW_BRIGHTNESS_MIN, VIEW_BRIGHTNESS_SPAN);
    }

    public static int convertColorTempModelToView(long modelColorTemp) {
        return convertModelToView(modelColorTemp, VIEW_COLORTEMP_MIN, VIEW_COLORTEMP_SPAN);
    }

    public static long convertColorTempViewToModel(int viewColorTemp) {
        return convertViewToModel(viewColorTemp, VIEW_COLORTEMP_MIN, VIEW_COLORTEMP_SPAN);
    }

    public static int[] convertModelToView(LampState lampState) {
        int viewHue = convertHueModelToView(lampState.getHue());
        int viewSaturation = convertSaturationModelToView(lampState.getSaturation());
        int viewBrightness = convertBrightnessModelToView(lampState.getBrightness());
        int viewColorTemp = convertColorTempModelToView(lampState.getColorTemp());

        return new int[] {viewHue, viewSaturation, viewBrightness, viewColorTemp};
    }

    public static void convertViewToModel(int viewHue, int viewSaturation, int viewBrightness, int viewColorTemp, LampState lampState) {
        lampState.setHue(convertHueViewToModel(viewHue));
        lampState.setSaturation(convertSaturationViewToModel(viewSaturation));
        lampState.setBrightness(convertBrightnessViewToModel(viewBrightness));
        lampState.setColorTemp(convertColorTempViewToModel(viewColorTemp));
    }

    public static float[] convertModelToHSV(LampState lampState) {
        int viewHue = convertHueModelToView(lampState.getHue());
        int viewSaturation = convertSaturationModelToView(lampState.getSaturation());
        int viewBrightness = convertBrightnessModelToView(lampState.getBrightness());

        return new float[] {viewHue, (float) (viewSaturation / 100.0), (float) (viewBrightness / 100.0)};
    }

    protected static int convertModelToView(long modelValue, int min, int span) {
        return min + (int)Math.round((modelValue / (double)UINT32_MAX) * span);
    }

    protected static long convertViewToModel(int viewValue, int min, int span) {
        return Math.round(((double)(viewValue - min) / (double)span) * UINT32_MAX);
    }
}
