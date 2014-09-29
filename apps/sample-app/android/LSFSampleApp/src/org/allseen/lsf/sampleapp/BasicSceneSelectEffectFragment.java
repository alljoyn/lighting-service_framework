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

import java.util.List;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.view.ViewGroup;

public class BasicSceneSelectEffectFragment extends SelectableItemTableFragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View root = super.onCreateView(inflater, container, savedInstanceState);

        updateSelectableItemRow(inflater, root, EffectType.None.toString(), getResources().getString(R.string.effect_sort_constant), R.drawable.list_constant_icon, getResources().getString(R.string.effect_name_none), true);
        updateSelectableItemRow(inflater, root, EffectType.Transition.toString(), getResources().getString(R.string.effect_sort_transition), R.drawable.list_transition_icon, getResources().getString(R.string.effect_name_transition), false);
        updateSelectableItemRow(inflater, root, EffectType.Pulse.toString(), getResources().getString(R.string.effect_sort_pulse), R.drawable.list_pulse_icon, getResources().getString(R.string.effect_name_pulse), false);

        return root;
    }

    @Override
    protected String getHeaderText() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        String members = MemberNamesString.format(activity, activity.pendingBasicSceneElementMembers, MemberNamesOptions.en, 3, activity.pendingBasicSceneModel.getName());

        return String.format(getString(R.string.basic_scene_select_effect), members);
    }

    @Override
    protected boolean isItemSelected(String itemID) {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        EffectType effectType = EffectType.None;

        if (activity.pendingNoEffectModel != null) {
            effectType = EffectType.None;
        } else if (activity.pendingTransitionEffectModel != null) {
            effectType = EffectType.Transition;
        } else if (activity.pendingPulseEffectModel != null) {
            effectType = EffectType.Pulse;
        }

        return effectType.toString().equals(itemID);
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        super.onCreateOptionsMenu(menu, inflater);

        ((SampleAppActivity)getActivity()).updateActionBar(R.string.title_basic_scene_add, false, false, true, false, true);
    }

    @Override
    protected boolean isExclusive() {
        return true;
    }

    @Override
    public void onActionNext() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        List<String> selectedIDs = getSelectedIDs();
        String effectID = selectedIDs != null && selectedIDs.size() > 0 ? selectedIDs.get(0) : EffectType.None.toString();

        if (effectID.equals(EffectType.None.toString())) {
            activity.pendingNoEffectModel = new NoEffectDataModel();
            activity.pendingTransitionEffectModel = null;
            activity.pendingPulseEffectModel = null;
            ((ScenesPageFragment)parent).showNoEffectChildFragment();
        } else if (effectID.equals(EffectType.Transition.toString())) {
            activity.pendingNoEffectModel = null;
            activity.pendingTransitionEffectModel = new TransitionEffectDataModel();
            activity.pendingPulseEffectModel = null;
            ((ScenesPageFragment)parent).showTransitionEffectChildFragment();
        } else if (effectID.equals(EffectType.Pulse.toString())) {
            activity.pendingNoEffectModel = null;
            activity.pendingTransitionEffectModel = null;
            activity.pendingPulseEffectModel = new PulseEffectDataModel();
            ((ScenesPageFragment)parent).showPulseEffectChildFragment();
        }
    }
}
