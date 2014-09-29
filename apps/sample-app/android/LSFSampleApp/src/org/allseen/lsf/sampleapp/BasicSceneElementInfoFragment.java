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

import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.widget.SeekBar;

public abstract class BasicSceneElementInfoFragment extends DimmableItemInfoFragment {

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        super.onCreateOptionsMenu(menu, inflater);

        ((SampleAppActivity)getActivity()).updateActionBar(R.string.title_basic_scene_element_add, false, false, false, true, true);
    }

    @Override
    public void onClick(View view) {
        int viewID = view.getId();

        if (viewID == R.id.statusButtonPower) {
            onHeaderClick();
        } else {
            super.onClick(view);
        }
    }

    @Override
    protected void onHeaderClick() {
        // TODO-FIX dialog to change effect type here
    }

    // Override parent to update the pending lamp state rather than call the activity
    @Override
    public void setField(SeekBar seekBar) {
        int seekBarID = seekBar.getId();
        int seekBarProgress = seekBar.getProgress();
        Object seekBarTag = seekBar.getTag();
        LampState pendingState = getPendingSceneElementState(seekBarTag);

        if (seekBarID == R.id.stateSliderBrightness) {
            pendingState.setBrightness(DimmableItemScaleConverter.convertBrightnessViewToModel(seekBarProgress));
        } else if (seekBarID == R.id.stateSliderHue) {
            pendingState.setHue(DimmableItemScaleConverter.convertHueViewToModel(seekBarProgress));
        } else if (seekBarID == R.id.stateSliderSaturation) {
            pendingState.setSaturation(DimmableItemScaleConverter.convertSaturationViewToModel(seekBarProgress));
        } else if (seekBarID == R.id.stateSliderColorTemp) {
            pendingState.setColorTemp(DimmableItemScaleConverter.convertColorTempViewToModel(seekBarProgress + DimmableItemScaleConverter.VIEW_COLORTEMP_MIN));
        }
        updatePresetFields(pendingState, getLampStateViewAdapter(seekBarTag));
        setColorIndicator(getLampStateViewAdapter(seekBarTag).stateView, getPendingSceneElementState(seekBarTag));
    }

    @Override
    public void updatePresetFields() {
        updatePresetFields(getPendingSceneElementDataModel());
    }

    protected LampState getPendingSceneElementState(Object viewTag) {
        return getPendingSceneElementDataModel().state;
    }

    protected LampStateViewAdapter getLampStateViewAdapter(Object viewTag) {
        return stateAdapter;
    }

    @Override
    public void onActionDone() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        BasicSceneElementDataModel elementModel = getPendingSceneElementDataModel();

        elementModel.members = activity.pendingBasicSceneElementMembers;
        elementModel.capability = activity.pendingBasicSceneElementCapability;

        updatePendingSceneElement();

        activity.pendingBasicSceneElementMembers = null;
        activity.pendingBasicSceneElementCapability = null;

        activity.pendingNoEffectModel = null;
        activity.pendingTransitionEffectModel = null;
        activity.pendingPulseEffectModel = null;

        parent.popBackStack(PageFrameParentFragment.CHILD_TAG_INFO);
    }

    protected abstract BasicSceneElementDataModel getPendingSceneElementDataModel();
    protected abstract void updatePendingSceneElement();
}
