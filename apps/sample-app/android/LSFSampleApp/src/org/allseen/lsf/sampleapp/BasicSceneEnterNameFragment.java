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

import org.allseen.lsf.LampGroup;

import android.util.Log;

public class BasicSceneEnterNameFragment extends EnterNameFragment {

    public BasicSceneEnterNameFragment() {
        super(R.string.label_basic_scene);
    }

    @Override
    protected int getTitleID() {
        return R.string.title_basic_scene_add;
    }

    @Override
    protected void setName(String name) {
        BasicSceneDataModel sceneModel = new BasicSceneDataModel();

        sceneModel.setName(name);

        SampleAppActivity activity = (SampleAppActivity)getActivity();
        activity.pendingBasicSceneModel = sceneModel;
        activity.pendingBasicSceneElementMembers = new LampGroup();
        activity.pendingBasicSceneElementCapability = new CapabilityData(true, true, true);

        Log.d(SampleAppActivity.TAG, "Pending basic scene name: " + activity.pendingBasicSceneModel.getName());
    }

    @Override
    protected String getDuplicateNameMessage() {
        return this.getString(R.string.duplicate_name_message_scene);
    }

    @Override
    protected boolean duplicateName(String name) {
        for (BasicSceneDataModel data : ((SampleAppActivity) this.getActivity()).basicSceneModels.values()) {
            if (data.getName().equals(name)) {
                return true;
            }
        }
        return false;
    }
}
