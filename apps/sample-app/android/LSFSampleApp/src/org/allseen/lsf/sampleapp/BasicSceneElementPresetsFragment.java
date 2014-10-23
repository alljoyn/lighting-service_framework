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

public class BasicSceneElementPresetsFragment extends DimmableItemPresetsFragment {

    @Override
    protected DimmableItemDataModel getDimmableItemDataModel() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        DimmableItemDataModel itemModel;

        if (activity.pendingNoEffectModel != null) {
            itemModel = activity.pendingNoEffectModel;
        } else if (activity.pendingTransitionEffectModel != null) {
            itemModel = activity.pendingTransitionEffectModel;
        } else {
            itemModel = activity.pendingPulseEffectModel;
        }

        return itemModel;
    }

    @Override
    protected boolean isItemSelected(PresetDataModel presetModel) {
        if (key2 == PulseEffectFragment.STATE2_ITEM_TAG) {
            PulseEffectDataModel pulseModel = (PulseEffectDataModel)getDimmableItemDataModel();
            return presetModel.stateEquals(pulseModel.endState);
        } else {
            return super.isItemSelected(presetModel);
        }
    }

    @Override
    protected void doSavePreset(String presetName) {
        if (key2 == PulseEffectFragment.STATE2_ITEM_TAG) {
            PulseEffectDataModel pulseModel = (PulseEffectDataModel)getDimmableItemDataModel();
            doSavePreset(presetName, pulseModel.endState);
        } else {
            super.doSavePreset(presetName);
        }
    }

    @Override
    protected void doApplyPreset(PresetDataModel presetModel) {
        if (key2 == PulseEffectFragment.STATE2_ITEM_TAG) {
            PulseEffectDataModel pulseModel = (PulseEffectDataModel)getDimmableItemDataModel();
            pulseModel.endPresetID = presetModel.id;
            pulseModel.endState = new LampState(presetModel.state);
        } else {
            BasicSceneElementDataModel elementModel = (BasicSceneElementDataModel)getDimmableItemDataModel();
            elementModel.presetID = presetModel.id;
            elementModel.state = new LampState(presetModel.state);
        }
    }
}
