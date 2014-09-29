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
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class MasterSceneInfoFragment extends PageFrameChildFragment implements View.OnClickListener {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        SampleAppActivity activity = (SampleAppActivity) getActivity();
        MasterSceneDataModel masterSceneModel = activity.masterSceneModels.get(key);

        view = inflater.inflate(R.layout.fragment_master_scene_info, container, false);

        View statusView = view.findViewById(R.id.infoStatusRow);

        setImageButtonBackgroundResource(statusView, R.id.statusButtonPower, R.drawable.master_scene_set_icon);

        // Name
        TextView nameLabel = (TextView)statusView.findViewById(R.id.statusLabelName);
        nameLabel.setText(R.string.master_scene_info_name);
        nameLabel.setClickable(true);
        nameLabel.setOnClickListener(this);

        TextView nameText = (TextView)statusView.findViewById(R.id.statusTextName);
        nameText.setClickable(true);
        nameText.setOnClickListener(this);

        // Members
        View rowMembers = view.findViewById(R.id.sceneInfoRowMembers);
        rowMembers.setClickable(true);
        rowMembers.setOnClickListener(this);

        setTextViewValue(rowMembers, R.id.sceneMembersRowLabel, getString(R.string.master_scene_info_members), 0);

        updateInfoFields(activity, masterSceneModel);

        return view;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        ((SampleAppActivity)getActivity()).updateActionBar(R.string.title_master_scene_info, false, false, false, false, true);
    }

    @Override
    public void onClick(View view) {
        int viewID = view.getId();

        if (viewID == R.id.statusLabelName || viewID == R.id.statusTextName) {
            onHeaderClick();
        } else if (viewID == R.id.sceneInfoRowMembers){
            onMembersClick();
        }
    }

    protected void onHeaderClick() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        MasterSceneDataModel masterSceneModel = activity.masterSceneModels.get(key);

        activity.showItemNameDialog(R.string.title_master_scene_rename, new UpdateMasterSceneNameAdapter(masterSceneModel, (SampleAppActivity) getActivity()));
    }

    protected void onMembersClick() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();

        activity.pendingMasterSceneModel = new MasterSceneDataModel(activity.masterSceneModels.get(key));

        ((ScenesPageFragment)parent).showSelectMembersChildFragment();
    }

    public void updateInfoFields() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        updateInfoFields(activity, activity.masterSceneModels.get(key));
    }

    public void updateInfoFields(SampleAppActivity activity, MasterSceneDataModel masterSceneModel) {
        // Update name and members
        setTextViewValue(view, R.id.statusTextName, masterSceneModel.getName(), 0);
        setTextViewValue(view.findViewById(R.id.sceneInfoRowMembers), R.id.sceneMembersRowText, Util.createSceneNamesString(activity, masterSceneModel.masterScene), 0);
    }
}
