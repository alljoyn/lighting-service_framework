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

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

public class PulseEffectFragment extends BasicSceneElementInfoFragment {
    public static final String STATE2_ITEM_TAG = "STATE2";

    protected LampStateViewAdapter stateAdapter2;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = super.onCreateView(inflater, container, savedInstanceState);

        setTextViewValue(view.findViewById(R.id.infoStatusRow), R.id.statusLabelName, getString(R.string.label_effect_name), 0);

        setTextViewValue(view.findViewById(R.id.infoStateRow), R.id.stateMainLabel, getString(R.string.state_label_header_start), 0);
        setTextViewValue(view.findViewById(R.id.infoStateRow2), R.id.stateMainLabel, getString(R.string.state_label_header_end), 0);

        setTextViewValue(view.findViewById(R.id.infoPeriodRow), R.id.nameValueNameText, getString(R.string.effect_info_period_name), 0);
        setTextViewValue(view.findViewById(R.id.infoDurationRow), R.id.nameValueNameText, getString(R.string.effect_info_pulse_duration), 0);
        setTextViewValue(view.findViewById(R.id.infoCountRow), R.id.nameValueNameText, getString(R.string.effect_info_count_name), 0);

        // second presets button
        Button presetsButton2 = (Button)view.findViewById(R.id.infoStateRow2).findViewById(R.id.stateButton);
        presetsButton2.setTag(STATE2_ITEM_TAG);
        presetsButton2.setClickable(true);
        presetsButton2.setOnClickListener(this);

        TextView periodName = (TextView)view.findViewById(R.id.infoPeriodRow).findViewById(R.id.nameValueNameText);
        periodName.setTag(R.id.infoPeriodRow);
        periodName.setClickable(true);
        periodName.setOnClickListener(this);

        TextView periodValue = (TextView)view.findViewById(R.id.infoPeriodRow).findViewById(R.id.nameValueValueText);
        periodValue.setTag(R.id.infoPeriodRow);
        periodValue.setClickable(true);
        periodValue.setOnClickListener(this);

        TextView durationName = (TextView)view.findViewById(R.id.infoDurationRow).findViewById(R.id.nameValueNameText);
        durationName.setTag(R.id.infoDurationRow);
        durationName.setClickable(true);
        durationName.setOnClickListener(this);

        TextView durationValue = (TextView)view.findViewById(R.id.infoDurationRow).findViewById(R.id.nameValueValueText);
        durationValue.setTag(R.id.infoDurationRow);
        durationValue.setClickable(true);
        durationValue.setOnClickListener(this);

        TextView countName = (TextView)view.findViewById(R.id.infoCountRow).findViewById(R.id.nameValueNameText);
        countName.setTag(R.id.infoCountRow);
        countName.setClickable(true);
        countName.setOnClickListener(this);

        TextView countValue = (TextView)view.findViewById(R.id.infoCountRow).findViewById(R.id.nameValueValueText);
        countValue.setTag(R.id.infoCountRow);
        countValue.setClickable(true);
        countValue.setOnClickListener(this);

        // state adapter
        stateAdapter2 = new LampStateViewAdapter(view.findViewById(R.id.infoStateRow2), STATE2_ITEM_TAG, this);

        updateInfoFields(((SampleAppActivity)getActivity()).pendingPulseEffectModel);

        return view;
    }

    // Override parent to initiate an effect type change on button press
    @Override
    public void onClick(View view) {
        int viewID = view.getId();

        if (viewID == R.id.nameValueNameText || viewID == R.id.nameValueValueText) {
            int viewSubID = ((Integer)view.getTag()).intValue();

            if (viewSubID == R.id.infoPeriodRow) {
                onPeriodClick();
            } else if (viewSubID == R.id.infoDurationRow) {
                onDurationClick();
            } else if (viewSubID == R.id.infoCountRow) {
                onCountClick();
            }
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
        stateAdapter2.setCapability(itemModel.getCapability());

        super.updateInfoFields(itemModel);

        updatePulseEffectInfoFields((PulseEffectDataModel)itemModel);
    }

    @Override
    public void updatePresetFields(DimmableItemDataModel itemModel) {
        super.updatePresetFields(itemModel);
        updatePresetFields(((PulseEffectDataModel)itemModel).endState, stateAdapter2);
    }

    protected void updatePulseEffectInfoFields(PulseEffectDataModel elementModel) {
        // Superclass updates the icon, so we have to re-override
        setImageButtonBackgroundResource(statusView, R.id.statusButtonPower, R.drawable.list_pulse_icon);

        stateAdapter2.setBrightness(elementModel.endState.getBrightness());
        stateAdapter2.setHue(elementModel.endState.getHue());
        stateAdapter2.setSaturation(elementModel.endState.getSaturation());
        stateAdapter2.setColorTemp(elementModel.endState.getColorTemp());

        String periodValue = String.format(getString(R.string.effect_info_period_format), elementModel.period / 1000.0);
        setTextViewValue(view.findViewById(R.id.infoPeriodRow), R.id.nameValueValueText, periodValue, R.string.units_seconds);

        String durationValue = String.format(getString(R.string.effect_info_period_format), elementModel.duration / 1000.0);
        setTextViewValue(view.findViewById(R.id.infoDurationRow), R.id.nameValueValueText, durationValue, R.string.units_seconds);

        setTextViewValue(view.findViewById(R.id.infoCountRow), R.id.nameValueValueText, elementModel.count, 0);
    }

    @Override
    protected int getLayoutID() {
        return R.layout.fragment_effect_pulse;
    }

    protected void onPeriodClick() {
        ((ScenesPageFragment)parent).showEnterPeriodChildFragment();
    }

    protected void onDurationClick() {
        ((ScenesPageFragment)parent).showEnterDurationChildFragment();
    }

    protected void onCountClick() {
        ((ScenesPageFragment)parent).showEnterCountChildFragment();
    }

    @Override
    protected LampState getPendingSceneElementState(Object viewTag) {
        PulseEffectDataModel pendingModel = ((SampleAppActivity)getActivity()).pendingPulseEffectModel;
        return viewTag != STATE2_ITEM_TAG ? super.getPendingSceneElementState(viewTag) : pendingModel.endState;
    }

    @Override
    protected LampStateViewAdapter getLampStateViewAdapter(Object viewTag) {
        return viewTag != STATE2_ITEM_TAG ? super.getLampStateViewAdapter(viewTag) : stateAdapter2;
    }

    @Override
    protected BasicSceneElementDataModel getPendingSceneElementDataModel() {
        return ((SampleAppActivity)getActivity()).pendingPulseEffectModel;
    }

    @Override
    public void updatePendingSceneElement() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        activity.pendingBasicSceneModel.updatePulseEffect(activity.pendingPulseEffectModel);
    }
}
