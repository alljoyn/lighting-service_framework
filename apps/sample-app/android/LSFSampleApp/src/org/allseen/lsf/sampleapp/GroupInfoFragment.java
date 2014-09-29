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

public class GroupInfoFragment extends DimmableItemInfoFragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = super.onCreateView(inflater, container, savedInstanceState);
        String groupID = key;

        itemType = SampleAppActivity.Type.GROUP;
        itemModels = ((SampleAppActivity)getActivity()).groupModels;

        ((TextView)statusView.findViewById(R.id.statusLabelName)).setText(R.string.label_group_name);

        // displays members of this group
        TextView membersLabel = (TextView)(view.findViewById(R.id.groupInfoMembers).findViewById(R.id.nameValueNameText));
        membersLabel.setText(R.string.group_info_label_members);
        membersLabel.setClickable(true);
        membersLabel.setOnClickListener(this);

        TextView membersValue = (TextView)(view.findViewById(R.id.groupInfoMembers).findViewById(R.id.nameValueValueText));
        membersValue.setClickable(true);
        membersValue.setOnClickListener(this);

        updateInfoFields(((SampleAppActivity)getActivity()).groupModels.get(groupID));

        return view;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        ((SampleAppActivity)getActivity()).updateActionBar(R.string.title_group_info, false, false, false, false, true);
    }

    @Override
    public void onClick(View view) {
        int viewID = view.getId();

        if (viewID == R.id.nameValueNameText || viewID == R.id.nameValueValueText) {
            if ((parent != null) && (!AllLampsDataModel.ALL_LAMPS_GROUP_ID.equals(key))) {
                GroupDataModel groupModel = ((SampleAppActivity)getActivity()).groupModels.get(key);

                if (groupModel != null) {
                    ((SampleAppActivity)getActivity()).pendingGroupModel = new GroupDataModel(groupModel);
                    ((PageMainContainerFragment)parent).showSelectMembersChildFragment();
                }
            }
        } else {
            super.onClick(view);
        }
    }

    public void updateInfoFields(GroupDataModel groupModel) {
        if (groupModel.id.equals(key)) {
            stateAdapter.setCapability(groupModel.getCapability());
            super.updateInfoFields(groupModel);

            String details = Util.createMemberNamesString((SampleAppActivity)getActivity(), groupModel.members, ", ", R.string.group_info_members_none);
            TextView membersValue = (TextView)(view.findViewById(R.id.groupInfoMembers).findViewById(R.id.nameValueValueText));

            if (details != null && !details.isEmpty()) {
                membersValue.setText(details);
            }
        }
    }

    @Override
    protected int getLayoutID() {
        return R.layout.fragment_group_info;
    }

    @Override
    protected void onHeaderClick() {
        if (!AllLampsDataModel.ALL_LAMPS_GROUP_ID.equals(key)) {
            SampleAppActivity activity = (SampleAppActivity)getActivity();
            GroupDataModel groupModel = activity.groupModels.get(key);

            activity.showItemNameDialog(R.string.title_group_rename, new UpdateGroupNameAdapter(groupModel, (SampleAppActivity) getActivity()));
        }
    }
}
