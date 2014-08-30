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

import org.allseen.lsf.LampGroup;

import android.view.Menu;
import android.view.MenuInflater;

public class BasicSceneSelectMembersFragment extends SelectMembersFragment {

    public BasicSceneSelectMembersFragment() {
        super(R.string.label_basic_scene);
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        super.onCreateOptionsMenu(menu, inflater);

        ((SampleAppActivity)getActivity()).updateActionBar(R.string.title_basic_scene_element_add, false, false, true, false);
    }

    @Override
    protected String getHeaderText() {
        return getString(R.string.basic_scene_select_members);
    }

    @Override
    protected LampGroup getPendingMembers() {
        return ((SampleAppActivity)getActivity()).pendingBasicSceneElementMembers;
    }

    @Override
    protected String getPendingItemID() {
        return ((SampleAppActivity)getActivity()).pendingBasicSceneModel.id;
    }

    @Override
    protected void processSelection(SampleAppActivity activity, List<String> lampIDs, List<String> groupIDs, List<String> sceneIDs, CapabilityData capability) {
        activity.pendingBasicSceneElementCapability = capability;
        super.processSelection(activity, lampIDs, groupIDs, sceneIDs, capability);
    }

    @Override
    protected void processSelection(SampleAppActivity activity, List<String> lampIDs, List<String> groupIDs, List<String> sceneIDs) {
        activity.pendingBasicSceneElementMembers.setLamps(lampIDs.toArray(new String[lampIDs.size()]));
        activity.pendingBasicSceneElementMembers.setLampGroups(groupIDs.toArray(new String[groupIDs.size()]));
    }

    @Override
    public void onActionNext() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();

        if (processSelection()) {
            if (!effectsSupported(activity)) {
                activity.pendingNoEffectModel = new NoEffectDataModel();
                activity.pendingTransitionEffectModel = null;
                activity.pendingPulseEffectModel = null;
            }
            if (activity.pendingNoEffectModel != null) {
                ((ScenesPageFragment)parent).showNoEffectChildFragment();
            } else if (activity.pendingTransitionEffectModel != null) {
                ((ScenesPageFragment)parent).showTransitionEffectChildFragment();
            } else if (activity.pendingPulseEffectModel != null) {
                ((ScenesPageFragment)parent).showPulseEffectChildFragment();
            } else {
                ((ScenesPageFragment)parent).showSelectEffectChildFragment();
            }
        }
    }

    @Override
    protected int getMixedSelectionMessageID() {
        return R.string.mixing_lamp_types_message_scene;
    }

    @Override
    protected int getMixedSelectionPositiveButtonID() {
        return R.string.create_scene;
    }

    public boolean effectsSupported(SampleAppActivity activity) {
        boolean effectsSupported = false;
        String[] pendingSceneMembers = activity.pendingBasicSceneElementMembers.getLamps();
        for (String ID : pendingSceneMembers) {
            effectsSupported = effectsSupported || activity.lampModels.get(ID).getDetails().hasEffects();
        }
        return effectsSupported;
    }
}
