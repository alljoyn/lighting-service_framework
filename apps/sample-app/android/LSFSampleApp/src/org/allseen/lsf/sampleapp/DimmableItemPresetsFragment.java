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

import java.util.Map;

import org.allseen.lsf.LampState;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.TextView;

public abstract class DimmableItemPresetsFragment extends SelectableItemTableFragment implements ItemNameAdapter{

    protected boolean allowApply = false;
    protected String currentName = null;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View root = super.onCreateView(inflater, container, savedInstanceState);

        TextView headerView = (TextView)view.findViewById(R.id.selectHeader);
        headerView.setClickable(true);
        headerView.setOnClickListener(this);

        onUpdateView(inflater, root);

        return root;
    }

    public void onUpdateView() {
        onUpdateView(getActivity().getLayoutInflater(), view);
    }

    public void onUpdateView(LayoutInflater inflater, View root) {
        SampleAppActivity activity = (SampleAppActivity)getActivity();

        table.removeAllViews();

        for (PresetDataModel presetModel : activity.presetModels.values()) {
            updateSelectableItemRow(inflater, root, presetModel.id, presetModel.tag, R.drawable.nav_more_menu_icon, presetModel.getName(), false);
        }
    }

    @Override
    protected void updateTableRows() {
        allowApply = false;
        super.updateTableRows();
        allowApply = true;
    }

    @Override
    protected String getHeaderText() {
        return getString(R.string.title_presets_save_new);
    }

    @Override
    protected boolean isItemSelected(String presetID) {
        return isItemSelected(((SampleAppActivity)getActivity()).presetModels.get(presetID));
    }

    protected boolean isItemSelected(PresetDataModel presetModel) {
        return presetModel.stateEquals(getDimmableItemDataModel().state);
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        super.onCreateOptionsMenu(menu, inflater);

        ((SampleAppActivity)getActivity()).updateActionBar(R.string.title_presets, false, false, false, false, true);
    }

    @Override
    protected int getTableRowLayout() {
        return R.layout.view_selectable_preset_row;
    }

    @Override
    protected int getSelectButtonDrawableID() {
        return R.drawable.checkbox;
    }

    @Override
    protected boolean isExclusive() {
        return true;
    }

    @Override
    public void onClick(View view) {
        final ItemNameAdapter adapter = this;

        if (view.getId() == R.id.selectHeader) {
            currentName = null;
            ((SampleAppActivity)getActivity()).showItemNameDialog(R.string.title_presets_save_new, adapter);
        } else if (view.getId() == R.id.selectableItemButtonIcon) {
            ((SampleAppActivity)getActivity()).showPresetMorePopup(view, view.getTag().toString());
        } else {
            super.onClick(view);
        }
    }

    @Override
    public String getCurrentName() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();

        // Default new preset name suffix is next available index (number of preset models + 1 for now)
        return currentName == null ? String.format(activity.getString(R.string.presets_new_name), activity.presetModels.size() + 1) : currentName;
    }

    @Override
    public void onClick(DialogInterface dialog, int id) {
        EditText nameText = (EditText) (((AlertDialog) dialog).findViewById(R.id.itemNameText));
        final ItemNameAdapter adapter = this;

        currentName = nameText.getText().toString();

        if (duplicateName(currentName)) {
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            builder.setTitle(R.string.duplicate_name);
            builder.setMessage(String.format(getString(R.string.duplicate_name_message_preset), currentName));
            builder.setPositiveButton(R.string.dialog_ok, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    doSavePreset(currentName);
                }
            });
            builder.setNegativeButton(R.string.dialog_cancel, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    dialog.cancel();
                    ((SampleAppActivity)getActivity()).showItemNameDialog(R.string.title_presets_save_new, adapter);
                }
            });
            builder.create().show();
        } else {
            doSavePreset(currentName);
        }
    }

    private boolean duplicateName(String presetName) {
        boolean isDuplicate = false;
        Map<String, PresetDataModel> currentPresets = ((SampleAppActivity) getActivity()).presetModels;
        for (String currentPresetName : currentPresets.keySet()) {
            if (currentPresets.get(currentPresetName).getName().equals(presetName)) {
                isDuplicate = true;
            }
        }
        return isDuplicate;
    }

    @Override
    public void onCheckedChanged(CompoundButton selectButton, boolean checked) {
        super.onCheckedChanged(selectButton, checked);

        if (checked && allowApply) {
            doApplyPreset(selectButton.getTag().toString());
        }
    }

    protected void doSavePreset(String presetName) {
        DimmableItemDataModel itemModel = getDimmableItemDataModel();

        if ((presetName != null) && (!presetName.isEmpty()) && (itemModel != null)) {
            doSavePreset(presetName, itemModel.state);
        }
    }

    protected void doSavePreset(String presetName, LampState presetState) {
        if ((presetName != null) && (!presetName.isEmpty()) && (presetState != null)) {
            AllJoynManager.presetManager.createPreset(presetState, presetName, SampleAppActivity.LANGUAGE);
        }
        if (getActivity() != null) {
            ((SampleAppActivity)getActivity()).onBackPressed();
        }
    }

    protected void doApplyPreset(String presetID) {
        doApplyPreset(((SampleAppActivity)getActivity()).presetModels.get(presetID));

        ((SampleAppActivity)getActivity()).onBackPressed();
    }

    protected abstract void doApplyPreset(PresetDataModel presetModel);
    protected abstract DimmableItemDataModel getDimmableItemDataModel();
}
