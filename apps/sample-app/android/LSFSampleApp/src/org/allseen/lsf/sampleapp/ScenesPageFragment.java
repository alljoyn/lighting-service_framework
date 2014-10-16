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

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class ScenesPageFragment extends PageMainContainerFragment {
    public static final String CHILD_TAG_SELECT_EFFECT = "SELECT_EFFECT";
    public static final String CHILD_TAG_CONSTANT_EFFECT = "CONSTANT_EFFECT";
    public static final String CHILD_TAG_TRANSITION_EFFECT = "TRANSITION_EFFECT";
    public static final String CHILD_TAG_PULSE_EFFECT = "PULSE_EFFECT";

    public static final String CHILD_TAG_EFFECT_DURATION = "EFFECT_DURATION";
    public static final String CHILD_TAG_EFFECT_PERIOD = "EFFECT_PERIOD";
    public static final String CHILD_TAG_EFFECT_COUNT = "EFFECT_COUNT";

    public static String TAG;

    protected boolean isMasterMode;

    public void setMasterMode(boolean isMasterMode) {
        this.isMasterMode = isMasterMode;
    }

    public boolean isMasterMode() {
        return isMasterMode;
    }

    public void showSelectEffectChildFragment() {
        showChildFragment(CHILD_TAG_SELECT_EFFECT, null);
    }

    public void showNoEffectChildFragment() {
        showChildFragment(CHILD_TAG_CONSTANT_EFFECT, ((SampleAppActivity)getActivity()).pendingNoEffectModel.id);
    }

    public void showTransitionEffectChildFragment() {
        showChildFragment(CHILD_TAG_TRANSITION_EFFECT, ((SampleAppActivity)getActivity()).pendingTransitionEffectModel.id);
    }

    public void showPulseEffectChildFragment() {
        showChildFragment(CHILD_TAG_PULSE_EFFECT, ((SampleAppActivity)getActivity()).pendingPulseEffectModel.id);
    }

    public void showEnterDurationChildFragment() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        String tag = activity.pendingTransitionEffectModel != null ? activity.pendingTransitionEffectModel.id : activity.pendingPulseEffectModel.id;
        showChildFragment(CHILD_TAG_EFFECT_DURATION, tag);
    }

    public void showEnterPeriodChildFragment() {
        showChildFragment(CHILD_TAG_EFFECT_PERIOD, ((SampleAppActivity)getActivity()).pendingPulseEffectModel.id);
    }

    public void showEnterCountChildFragment() {
        showChildFragment(CHILD_TAG_EFFECT_COUNT, ((SampleAppActivity)getActivity()).pendingPulseEffectModel.id);
    }

    @Override
    protected PageFrameChildFragment createChildFragment(String tag)
    {
        if (tag == CHILD_TAG_SELECT_EFFECT) {
            return createSelectEffectChildFragment();
        } else if (tag == CHILD_TAG_CONSTANT_EFFECT) {
            return createConstantEffectChildFragment();
        } else if (tag == CHILD_TAG_TRANSITION_EFFECT) {
            return createTransitionEffectChildFragment();
        } else if (tag == CHILD_TAG_PULSE_EFFECT) {
            return createPulseEffectChildFragment();
        } else if (tag == CHILD_TAG_EFFECT_DURATION) {
            return createEnterDurationChildFragment();
        } else if (tag == CHILD_TAG_EFFECT_PERIOD) {
            return createEnterPeriodChildFragment();
        } else if (tag == CHILD_TAG_EFFECT_COUNT) {
            return createEnterCountChildFragment();
        } else {
            return super.createChildFragment(tag);
        }
    }

    @Override
    public PageFrameChildFragment createTableChildFragment() {
        return new ScenesTableFragment();
    }

    @Override
    public PageFrameChildFragment createInfoChildFragment() {
        return isMasterMode ? new MasterSceneInfoFragment() : new BasicSceneInfoFragment();
    }

    @Override
    public PageFrameChildFragment createPresetsChildFragment() {
        return new BasicSceneElementPresetsFragment();
    }

    @Override
    public PageFrameChildFragment createEnterNameChildFragment() {
        return isMasterMode ? new MasterSceneEnterNameFragment() : new BasicSceneEnterNameFragment();
    }

    @Override
    public PageFrameChildFragment createSelectMembersChildFragment() {
        return isMasterMode ? new MasterSceneSelectMembersFragment() : new BasicSceneSelectMembersFragment();
    }

    public PageFrameChildFragment createSelectEffectChildFragment() {
        return isMasterMode ? null : new BasicSceneSelectEffectFragment();
    }

    public PageFrameChildFragment createConstantEffectChildFragment() {
        return isMasterMode ? null : new NoEffectFragment();
    }

    public PageFrameChildFragment createTransitionEffectChildFragment() {
        return isMasterMode ? null : new TransitionEffectFragment();
    }

    public PageFrameChildFragment createPulseEffectChildFragment() {
        return isMasterMode ? null : new PulseEffectFragment();
    }

    public PageFrameChildFragment createEnterDurationChildFragment() {
        return new EnterDurationFragment();
    }

    public PageFrameChildFragment createEnterPeriodChildFragment() {
        return new EnterPeriodFragment();
    }

    public PageFrameChildFragment createEnterCountChildFragment() {
        return new EnterCountFragment();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View root = super.onCreateView(inflater, container, savedInstanceState);

        ScenesPageFragment.TAG = getTag();

        return root;
    }

    @Override
    public int onBackPressed() {
        String startingChildTag = child.getTag();
        int backStackCount = super.onBackPressed();

        if (!isMasterMode && CHILD_TAG_ENTER_NAME.equals(startingChildTag)) {
            // To support the basic scene creation workflow, when going backwards
            // from the enter name fragment we have to skip over the dummy scene
            // info fragment (see SampleAppActivity.doAddScene()). So we queue up
            // a second back press here.
            ((SampleAppActivity)getActivity()).postOnBackPressed();
        }

        return backStackCount;
    }
}
