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

import android.util.Log;

public class MasterSceneEnterNameFragment extends EnterNameFragment {

    public MasterSceneEnterNameFragment() {
        super(R.string.label_master_scene);
    }

    @Override
    protected int getTitleID() {
        return R.string.title_master_scene_add;
    }

    @Override
    protected void setName(String name) {
        MasterSceneDataModel masterSceneModel = new MasterSceneDataModel();
        masterSceneModel.setName(name);

        SampleAppActivity activity = (SampleAppActivity)getActivity();
        activity.pendingMasterSceneModel = masterSceneModel;

        Log.d(SampleAppActivity.TAG, "Pending master scene name: " + activity.pendingMasterSceneModel.getName());
    }

    @Override
    protected String getDuplicateNameMessage() {
        return this.getString(R.string.duplicate_name_message_scene);
    }

    @Override
    protected boolean duplicateName(String name) {
        for (MasterSceneDataModel data : ((SampleAppActivity) this.getActivity()).masterSceneModels.values()) {
            if (data.getName().equals(name)) {
                return true;
            }
        }
        return false;
    }
}
