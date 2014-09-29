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

import java.util.Locale;

import android.text.InputType;

public class EnterDurationFragment extends EnterNumberFragment {

    @Override
    protected int getTitleID() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();

        return activity.pendingTransitionEffectModel != null ? R.string.title_effect_transition_duration : R.string.title_effect_pulse_duration;
    }

    @Override
    protected int getLabelID() {
        return R.string.label_enter_duration;
    }

    @Override
    protected int getInputType() {
        return InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL;
    }

    @Override
    protected String getNumberString() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        long duration = activity.pendingTransitionEffectModel != null ? activity.pendingTransitionEffectModel.duration : activity.pendingPulseEffectModel.duration;

        return String.format(Locale.ENGLISH, getString(R.string.effect_info_duration_format), duration / 1000.0);
    }

    @Override
    protected boolean setNumberValue(long numberValue) {
        SampleAppActivity activity = (SampleAppActivity)getActivity();

        if (activity.pendingTransitionEffectModel != null) {
            activity.pendingTransitionEffectModel.duration = numberValue;
        } else {
            activity.pendingPulseEffectModel.duration = numberValue;
        }

        // Go back to the effect info display
        activity.onBackPressed();

        return true;
    }
}

