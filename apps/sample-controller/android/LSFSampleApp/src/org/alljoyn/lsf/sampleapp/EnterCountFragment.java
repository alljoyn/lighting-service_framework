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

import java.util.Locale;

import android.text.InputType;

public class EnterCountFragment extends EnterNumberFragment {

    @Override
    protected int getTitleID() {
        return R.string.title_effect_pulse_count;
    }

    @Override
    protected int getLabelID() {
        return R.string.label_enter_count;
    }

    @Override
    protected int getInputType() {
        return InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL;
    }

    @Override
    protected String getNumberString() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        int count = activity.pendingPulseEffectModel.count;

        return String.valueOf(count);
    }

    @Override
    protected void setNumberString(String number) {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        int count = Integer.valueOf(number);

        activity.pendingPulseEffectModel.count = count;
        activity.pendingBasicSceneModel.updatePulseEffect(activity.pendingPulseEffectModel);

        // Go back to the effect info display
        activity.onBackPressed();
    }
}

