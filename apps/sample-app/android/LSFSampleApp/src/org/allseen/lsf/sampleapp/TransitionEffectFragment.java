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
import android.widget.TextView;

public class TransitionEffectFragment extends BasicSceneElementInfoFragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = super.onCreateView(inflater, container, savedInstanceState);

        setTextViewValue(statusView, R.id.statusLabelName, getString(R.string.label_effect_name), 0);
        setTextViewValue(view.findViewById(R.id.infoDurationRow), R.id.nameValueNameText, getString(R.string.effect_info_transition_duration), 0);

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

    // Override parent to initiate an effect type change on button press
    @Override
    public void onClick(View view) {
        int viewID = view.getId();

        if (viewID == R.id.nameValueNameText || viewID == R.id.nameValueValueText) {
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

        updateTransitionEffectInfoFields(activity, (TransitionEffectDataModel)itemModel);
    }

    protected void updateTransitionEffectInfoFields(SampleAppActivity activity, TransitionEffectDataModel elementModel) {
        String members = MemberNamesString.format(activity, activity.pendingBasicSceneElementMembers, MemberNamesOptions.en, 3, R.string.effect_info_help_no_members);
        setTextViewValue(view.findViewById(R.id.infoHelpRow), R.id.helpText, String.format(getString(R.string.effect_info_help_transition), members), 0);

        // Superclass updates the icon, so we have to re-override
        setImageButtonBackgroundResource(statusView, R.id.statusButtonPower, R.drawable.list_transition_icon);

        String durationValue = String.format(getString(R.string.effect_info_duration_format), activity.pendingTransitionEffectModel.duration / 1000.0);
        setTextViewValue(view.findViewById(R.id.infoDurationRow), R.id.nameValueValueText, durationValue, R.string.units_seconds);
    }

    @Override
    protected int getLayoutID() {
        return R.layout.fragment_effect_transition;
    }

    protected void onDurationClick() {
        ((ScenesPageFragment)parent).showEnterDurationChildFragment();
    }

    @Override
    protected BasicSceneElementDataModel getPendingSceneElementDataModel() {
        return ((SampleAppActivity)getActivity()).pendingTransitionEffectModel;
    }

    @Override
    public void updatePendingSceneElement() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        activity.pendingBasicSceneModel.updateTransitionEffect(activity.pendingTransitionEffectModel);
    }
}
