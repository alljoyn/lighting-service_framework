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

import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.SeekBar;
import android.widget.TextView;

public class NoEffectFragment extends DimmableItemInfoFragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = super.onCreateView(inflater, container, savedInstanceState);

        setImageButtonBackgroundResource(statusView, R.id.statusButtonPower, R.drawable.list_constant_icon);

        ((TextView)statusView.findViewById(R.id.statusLabelName)).setText(R.string.label_effect_name);

        updateInfoFields(((SampleAppActivity)getActivity()).pendingNoEffectModel);

        return view;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        super.onCreateOptionsMenu(menu, inflater);

        ((SampleAppActivity)getActivity()).updateActionBar(R.string.title_basic_scene_element_add, false, false, false, true);
    }

    // Override parent to initiate a effect type change on button press
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
    public void updateInfoFields(DimmableItemDataModel itemModel) {
        // Capabilities can change if the member set is edited
        itemModel.setCapability(((SampleAppActivity)getActivity()).pendingBasicSceneElementCapability);

        stateAdapter.setCapability(itemModel.getCapability());

        super.updateInfoFields(itemModel);

        // Superclass updates the icon, so we have to re-override
        setImageButtonBackgroundResource(statusView, R.id.statusButtonPower, R.drawable.list_constant_icon);
    }

    // Override parent to update the pending lamp state rather than call the activity
    @Override
    public void setField(SeekBar seekBar) {
        int seekBarID = seekBar.getId();
        int seekBarProgress = seekBar.getProgress();
        LampState pendingState = ((SampleAppActivity)getActivity()).pendingNoEffectModel.state;

        if (seekBarID == R.id.stateSliderBrightness) {
            pendingState.setBrightness(DimmableItemScaleConverter.convertBrightnessViewToModel(seekBarProgress));
        } else if (seekBarID == R.id.stateSliderHue) {
            pendingState.setHue(DimmableItemScaleConverter.convertHueViewToModel(seekBarProgress));
        } else if (seekBarID == R.id.stateSliderSaturation) {
            pendingState.setSaturation(DimmableItemScaleConverter.convertSaturationViewToModel(seekBarProgress));
        } else if (seekBarID == R.id.stateSliderColorTemp) {
            pendingState.setColorTemp(DimmableItemScaleConverter.convertColorTempViewToModel(seekBarProgress + DimmableItemScaleConverter.VIEW_COLORTEMP_MIN));
        }
    }

    @Override
    protected int getLayoutID() {
        return R.layout.fragment_effect_constant;
    }

    @Override
    protected void onHeaderClick() {
        // TODO-FIX dialog to change effect type here
    }

    @Override
    protected PresetsDialogAdapter createPresetsDialogAdapter(String presetID) {
        //TODO-FIX handle preset effects properly
        return new PresetsDialogEffectAdapter(this, ((SampleAppActivity)getActivity()).pendingNoEffectModel, presetID);
    }

    @Override
    public void onActionDone() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();

        activity.pendingNoEffectModel.members = activity.pendingBasicSceneElementMembers;
        activity.pendingNoEffectModel.capability = activity.pendingBasicSceneElementCapability;

        activity.pendingBasicSceneModel.updateNoEffect(activity.pendingNoEffectModel);

        activity.pendingBasicSceneElementMembers = null;
        activity.pendingBasicSceneElementCapability = null;
        activity.pendingNoEffectModel = null;

        if (activity.pendingBasicSceneModel.id != null && !activity.pendingBasicSceneModel.id.isEmpty()) {
            AllJoynManager.sceneManager.updateScene(activity.pendingBasicSceneModel.id, activity.pendingBasicSceneModel.toScene());
        } else {
            AllJoynManager.sceneManager.createScene(activity.pendingBasicSceneModel.toScene(), activity.pendingBasicSceneModel.name, SampleAppActivity.LANGUAGE);
        }

        parent.clearBackStack();
    }
}
