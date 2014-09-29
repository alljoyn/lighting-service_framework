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

public class GroupSelectMembersFragment extends SelectMembersFragment {

    public GroupSelectMembersFragment() {
        super(R.string.label_group);
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        super.onCreateOptionsMenu(menu, inflater);

        SampleAppActivity activity = (SampleAppActivity)getActivity();

        activity.updateActionBar(activity.pendingGroupModel.id.isEmpty() ? R.string.title_group_add : R.string.title_group_edit, false, false, false, true, true);
    }

    @Override
    protected String getHeaderText() {
        return getString(R.string.group_select_members);
    }

    @Override
    protected LampGroup getPendingMembers() {
        return ((SampleAppActivity)getActivity()).pendingGroupModel.members;
    }

    @Override
    protected String getPendingItemID() {
        return ((SampleAppActivity)getActivity()).pendingGroupModel.id;
    }

   @Override
    protected void processSelection(SampleAppActivity activity, List<String> lampIDs, List<String> groupIDs, List<String> sceneIDs) {
        activity.pendingGroupModel.members.setLamps(lampIDs.toArray(new String[lampIDs.size()]));
        activity.pendingGroupModel.members.setLampGroups(groupIDs.toArray(new String[groupIDs.size()]));

        if (activity.pendingGroupModel.id.isEmpty()) {
            AllJoynManager.groupManager.createLampGroup(activity.pendingGroupModel.members, activity.pendingGroupModel.getName(), SampleAppActivity.LANGUAGE);
        } else {
            AllJoynManager.groupManager.updateLampGroup(activity.pendingGroupModel.id, activity.pendingGroupModel.members);
        }
    }

@Override
protected int getMixedSelectionMessageID() {
    return R.string.mixing_lamp_types_message_group;
}

@Override
protected int getMixedSelectionPositiveButtonID() {
    return R.string.create_group;
}
}
