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

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

public class BasicSceneInfoFragment extends PageFrameChildFragment implements View.OnClickListener {

    protected BasicSceneDataModel getBasicSceneDataModel() {
        return ((SampleAppActivity) getActivity()).pendingBasicSceneModel;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        SampleAppActivity activity = (SampleAppActivity) getActivity();
        BasicSceneDataModel basicSceneModel = getBasicSceneDataModel();

        view = inflater.inflate(R.layout.fragment_basic_scene_info, container, false);
        View statusView = view.findViewById(R.id.infoStatusRow);

        setImageButtonBackgroundResource(statusView, R.id.statusButtonPower, R.drawable.scene_set_icon);

        // item name
        TextView nameLabel = (TextView)statusView.findViewById(R.id.statusLabelName);
        nameLabel.setText(R.string.basic_scene_info_name);
        nameLabel.setClickable(true);
        nameLabel.setOnClickListener(this);

        TextView nameText = (TextView)statusView.findViewById(R.id.statusTextName);
        nameText.setClickable(true);
        nameText.setOnClickListener(this);

        updateInfoFields(activity, basicSceneModel);

        return view;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        ((SampleAppActivity)getActivity()).updateActionBar(R.string.title_basic_scene_info, false, true, false, true, true);
    }

    @Override
    public void onActionAdd() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        activity.pendingBasicSceneElementMembers = new LampGroup();
        activity.pendingBasicSceneElementCapability = new CapabilityData(true, true, true);
        activity.pendingNoEffectModel = null;
        activity.pendingTransitionEffectModel = null;
        activity.pendingPulseEffectModel = null;

        ((PageMainContainerFragment)parent).showSelectMembersChildFragment();
    }

    @Override
    public void onActionDone() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();

        if (activity.pendingBasicSceneModel.id != null && !activity.pendingBasicSceneModel.id.isEmpty()) {
            AllJoynManager.sceneManager.updateScene(activity.pendingBasicSceneModel.id, activity.pendingBasicSceneModel.toScene());
            activity.basicSceneModels.put(activity.pendingBasicSceneModel.id, activity.pendingBasicSceneModel);
        } else {
            AllJoynManager.sceneManager.createScene(activity.pendingBasicSceneModel.toScene(), activity.pendingBasicSceneModel.getName(), SampleAppActivity.LANGUAGE);
        }

        parent.clearBackStack();
    }

    @Override
    public void onClick(View view) {
        int viewID = view.getId();

        if (viewID == R.id.statusLabelName || viewID == R.id.statusTextName) {
            onHeaderClick();
        } else if (viewID == R.id.detailedItemRowTextHeader || viewID == R.id.detailedItemRowTextDetails) {
            onElementTextClick(view.getTag().toString());
        } else if (viewID == R.id.detailedItemButtonMore) {
            onElementMoreClick(view, view.getTag().toString());
        }
    }

    protected void onHeaderClick() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        BasicSceneDataModel sceneModel = getBasicSceneDataModel();

        activity.showItemNameDialog(R.string.title_basic_scene_rename, new UpdateBasicSceneNameAdapter(sceneModel, (SampleAppActivity) getActivity()));
    }

    protected void onElementTextClick(String elementID) {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        BasicSceneDataModel basicSceneModel = activity.pendingBasicSceneModel;

        if (basicSceneModel.noEffects != null) {
            for (NoEffectDataModel elementModel : basicSceneModel.noEffects) {
                if (elementID.equals(elementModel.id)) {
                    activity.pendingBasicSceneElementMembers = new LampGroup(elementModel.members);
                    activity.pendingBasicSceneElementCapability = new CapabilityData(elementModel.capability);
                    activity.pendingNoEffectModel = new NoEffectDataModel(elementModel);
                    activity.pendingTransitionEffectModel = null;
                    activity.pendingPulseEffectModel = null;

                    ((ScenesPageFragment)parent).showSelectMembersChildFragment();
                    return;
                }
            }
        }

        if (basicSceneModel.transitionEffects != null) {
            for (TransitionEffectDataModel elementModel : basicSceneModel.transitionEffects) {
                if (elementID.equals(elementModel.id)) {
                    activity.pendingBasicSceneElementMembers = new LampGroup(elementModel.members);
                    activity.pendingBasicSceneElementCapability = new CapabilityData(elementModel.capability);
                    activity.pendingNoEffectModel = null;
                    activity.pendingTransitionEffectModel = new TransitionEffectDataModel(elementModel);
                    activity.pendingPulseEffectModel = null;

                    ((ScenesPageFragment)parent).showSelectMembersChildFragment();
                    return;
                }
            }
        }

        if (basicSceneModel.pulseEffects != null) {
            for (PulseEffectDataModel elementModel : basicSceneModel.pulseEffects) {
                if (elementID.equals(elementModel.id)) {
                    activity.pendingBasicSceneElementMembers = new LampGroup(elementModel.members);
                    activity.pendingBasicSceneElementCapability = new CapabilityData(elementModel.capability);
                    activity.pendingNoEffectModel = null;
                    activity.pendingTransitionEffectModel = null;
                    activity.pendingPulseEffectModel = new PulseEffectDataModel(elementModel);

                    ((ScenesPageFragment)parent).showSelectMembersChildFragment();
                    return;
                }
            }
        }
    }

    protected void onElementMoreClick(View anchor, String elementID) {
        ((SampleAppActivity)getActivity()).onItemButtonMore(parent, SampleAppActivity.Type.ELEMENT, anchor, key, elementID);
    }

    public void updateInfoFields() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        updateInfoFields(activity, getBasicSceneDataModel());
    }

    public void updateInfoFields(SampleAppActivity activity, BasicSceneDataModel basicSceneModel) {
        // Update name and members
        setTextViewValue(view, R.id.statusTextName, basicSceneModel.getName(), 0);

        // Displays list of effects in this scene
        TableLayout elementsTable = (TableLayout) view.findViewById(R.id.sceneInfoElementTable);
        elementsTable.removeAllViews();

        if (basicSceneModel.noEffects != null) {
            for (NoEffectDataModel elementModel : basicSceneModel.noEffects) {
                addElementRow(activity, elementsTable, R.drawable.list_constant_icon, elementModel.id, elementModel.members, R.string.effect_name_none);
            }
        }

        if (basicSceneModel.transitionEffects != null) {
            for (TransitionEffectDataModel elementModel : basicSceneModel.transitionEffects) {
                addElementRow(activity, elementsTable, R.drawable.list_transition_icon, elementModel.id, elementModel.members, R.string.effect_name_transition);
            }
        }

        if (basicSceneModel.pulseEffects != null) {
            for (PulseEffectDataModel elementModel : basicSceneModel.pulseEffects) {
                addElementRow(activity, elementsTable, R.drawable.list_pulse_icon, elementModel.id, elementModel.members, R.string.effect_name_pulse);
            }
        }

        if (elementsTable.getChildCount() == 0) {
            //TODO-CHK Do we show "no effects" here?
        }
    }

    protected void addElementRow(SampleAppActivity activity, TableLayout elementTable, int iconID, String elementID, LampGroup members, int detailsID) {
        addElementRow(activity, elementTable, iconID, elementID, members, getString(detailsID));
    }

    protected void addElementRow(SampleAppActivity activity, TableLayout elementTable, int iconID, String elementID, LampGroup members, String details) {
        TableRow tableRow = new TableRow(view.getContext());
        activity.getLayoutInflater().inflate(R.layout.view_scene_element_row, tableRow);

        ((ImageButton)tableRow.findViewById(R.id.detailedItemButtonIcon)).setImageResource(iconID);

        TextView textHeader = (TextView)tableRow.findViewById(R.id.detailedItemRowTextHeader);
        textHeader.setText(Util.createMemberNamesString(activity, members, System.getProperty("line.separator"), R.string.basic_scene_members_none));
        textHeader.setTag(elementID);
        textHeader.setClickable(true);
        textHeader.setOnClickListener(this);

        TextView textDetails = (TextView)tableRow.findViewById(R.id.detailedItemRowTextDetails);
        textDetails.setText(details);
        textDetails.setTag(elementID);
        textDetails.setClickable(true);
        textDetails.setOnClickListener(this);

        ImageButton moreButton = (ImageButton)tableRow.findViewById(R.id.detailedItemButtonMore);
        moreButton.setImageResource(R.drawable.group_more_menu_icon);
        moreButton.setTag(elementID);
        moreButton.setOnClickListener(this);

        elementTable.addView(tableRow);
    }
}
