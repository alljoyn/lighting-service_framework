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

public class TransitionEffectFragment extends DimmableItemInfoFragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = super.onCreateView(inflater, container, savedInstanceState);

        setTextViewValue(statusView, R.id.statusLabelName, getString(R.string.label_effect_name), 0);
        setTextViewValue(view.findViewById(R.id.infoDurationRow), R.id.nameValueNameText, getString(R.string.effect_info_duration_name), 0);

        TextView effectName = (TextView)statusView.findViewById(R.id.statusTextName);
        effectName.setClickable(true);
        effectName.setOnClickListener(this);

        TextView durationName = (TextView)view.findViewById(R.id.infoDurationRow).findViewById(R.id.nameValueNameText);
        durationName.setClickable(true);
        durationName.setOnClickListener(this);

        TextView durationValue = (TextView)view.findViewById(R.id.infoDurationRow).findViewById(R.id.nameValueValueText);
        durationValue.setClickable(true);
        durationValue.setOnClickListener(this);

        updateInfoFields(((SampleAppActivity)getActivity()).pendingTransitionEffectModel);

        return view;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        super.onCreateOptionsMenu(menu, inflater);

        //TODO-FIX next should be true here also to allow another effect?
        ((SampleAppActivity)getActivity()).updateActionBar(R.string.title_basic_scene_element_add, false, false, false, true);
    }

    // Override parent to initiate an effect type change on button press
    @Override
    public void onClick(View view) {
        int viewID = view.getId();

        if (viewID == R.id.statusButtonPower) {
            onHeaderClick();
        } else if (viewID == R.id.nameValueNameText || viewID == R.id.nameValueValueText) {
            onDurationClick();
        } else {
            super.onClick(view);
        }
    }

    @Override
    public void updateInfoFields(DimmableItemDataModel itemModel) {
        SampleAppActivity activity = (SampleAppActivity)getActivity();

        // Capabilities can change if the member set is edited
        itemModel.setCapability(activity.pendingBasicSceneElementCapability);
        stateAdapter.setCapability(itemModel.getCapability());

        super.updateInfoFields(itemModel);

        // Superclass updates the icon, so we have to re-override
        setImageButtonBackgroundResource(statusView, R.id.statusButtonPower, R.drawable.list_transition_icon);

        String durationValue = String.format(getString(R.string.effect_info_duration_format), activity.pendingTransitionEffectModel.duration / 1000.0);
        setTextViewValue(view.findViewById(R.id.infoDurationRow), R.id.nameValueValueText, durationValue, R.string.units_seconds);
    }

    @Override
    // Override parent to update the pending lamp state rather than call the activity
    public void setField(SeekBar seekBar) {
        int seekBarID = seekBar.getId();
        int seekBarProgress = seekBar.getProgress();
        LampState pendingState = ((SampleAppActivity)getActivity()).pendingTransitionEffectModel.state;

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
        return R.layout.fragment_effect_transition;
    }

    @Override
    protected void onHeaderClick() {
        // TODO-FIX dialog to change effect type here
    }

    protected void onDurationClick() {
        ((ScenesPageFragment)parent).showEnterDurationChildFragment();
    }

    @Override
    protected PresetsDialogAdapter createPresetsDialogAdapter(String presetID) {
        //TODO-FIX handle preset effects properly
        return new PresetsDialogEffectAdapter(this, ((SampleAppActivity)getActivity()).pendingTransitionEffectModel, presetID);
    }

    @Override
    public void onActionDone() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();

        activity.pendingTransitionEffectModel.members = activity.pendingBasicSceneElementMembers;
        activity.pendingTransitionEffectModel.capability = activity.pendingBasicSceneElementCapability;

        activity.pendingBasicSceneModel.updateTransitionEffect(activity.pendingTransitionEffectModel);

        activity.pendingBasicSceneElementMembers = null;
        activity.pendingBasicSceneElementCapability = null;
        activity.pendingTransitionEffectModel = null;

        if (activity.pendingBasicSceneModel.id != null && !activity.pendingBasicSceneModel.id.isEmpty()) {
            AllJoynManager.sceneManager.updateScene(activity.pendingBasicSceneModel.id, activity.pendingBasicSceneModel.toScene());
        } else {
            AllJoynManager.sceneManager.createScene(activity.pendingBasicSceneModel.toScene(), activity.pendingBasicSceneModel.name, SampleAppActivity.LANGUAGE);
        }

        parent.clearBackStack();
    }
}
