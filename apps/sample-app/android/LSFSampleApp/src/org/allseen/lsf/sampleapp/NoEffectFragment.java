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

public class NoEffectFragment extends BasicSceneElementInfoFragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = super.onCreateView(inflater, container, savedInstanceState);

        setImageButtonBackgroundResource(statusView, R.id.statusButtonPower, R.drawable.list_constant_icon);

        ((TextView)statusView.findViewById(R.id.statusLabelName)).setText(R.string.label_effect_name);

        updateInfoFields(((SampleAppActivity)getActivity()).pendingNoEffectModel);

        return view;
    }

    @Override
    public void updateInfoFields(DimmableItemDataModel itemModel) {
        SampleAppActivity activity = (SampleAppActivity)getActivity();

        // Capabilities can change if the member set is edited
        itemModel.setCapability(activity.pendingBasicSceneElementCapability);

        stateAdapter.setCapability(itemModel.getCapability());

        super.updateInfoFields(itemModel);

        updateNoEffectInfoFields(activity, (NoEffectDataModel)itemModel);
    }

    protected void updateNoEffectInfoFields(SampleAppActivity activity, NoEffectDataModel elementModel) {
        String members = MemberNamesString.format(activity, activity.pendingBasicSceneElementMembers, MemberNamesOptions.en, 3, R.string.effect_info_help_no_members);
        setTextViewValue(view.findViewById(R.id.infoHelpRow), R.id.helpText, String.format(getString(R.string.effect_info_help_none), members), 0);

        // Superclass updates the icon, so we have to re-override
        setImageButtonBackgroundResource(statusView, R.id.statusButtonPower, R.drawable.list_constant_icon);
    }

    @Override
    protected int getLayoutID() {
        return R.layout.fragment_effect_constant;
    }

    @Override
    protected BasicSceneElementDataModel getPendingSceneElementDataModel() {
        return ((SampleAppActivity)getActivity()).pendingNoEffectModel;
    }

    @Override
    public void updatePendingSceneElement() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        activity.pendingBasicSceneModel.updateNoEffect(activity.pendingNoEffectModel);
    }
}
