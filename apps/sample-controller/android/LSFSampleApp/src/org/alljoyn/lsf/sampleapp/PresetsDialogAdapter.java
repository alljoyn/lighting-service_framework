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
package org.alljoyn.lsf.sampleapp;

import java.util.Comparator;
import java.util.Map;
import java.util.TreeMap;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.util.Log;
import android.widget.EditText;

public abstract class PresetsDialogAdapter implements ItemNameAdapter {

    protected DimmableItemDataModel itemModel;
    protected String presetID;

    protected SampleAppActivity activity;
    protected AlertDialog presetsDialog;

    public PresetsDialogAdapter(DimmableItemDataModel itemModel, String presetID) {
        super();

        this.itemModel = itemModel;
        this.presetID = presetID;
    }

    public void create(final SampleAppActivity activity) {
        this.activity = activity;

        AlertDialog.Builder builder = new AlertDialog.Builder(activity);
        builder.setTitle(R.string.label_preset_select);

        final Map<String, String> presets = new TreeMap<String, String>(new Comparator<String>() {
            @Override
            public int compare(String lhs, String rhs) {
                // sort without respect to case
                return lhs.compareToIgnoreCase(rhs);
            }
        });

        for (PresetDataModel data : activity.presetModels.values()) {
            presets.put(data.name, data.id);
        }

        final String[] presetsNameArray = presets.keySet().toArray(new String[presets.size()]);
        Log.d(SampleAppActivity.TAG, "Presets - " + presetsNameArray.length + " presets found");

        // either display a list of presets, or say that there are none
        if (presetsNameArray.length > 0) {
            builder.setItems(presetsNameArray, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    doApplyPreset(presets.get(presetsNameArray[which]));
                }
            });
        } else {
            builder.setMessage(R.string.presets_message_none);
        }

        final ItemNameAdapter adapter = this;
        builder.setPositiveButton(R.string.title_presets_save_new, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                activity.showItemNameDialog(R.string.title_presets_save_new, adapter);
            }
        });

        presetsDialog = builder.create();
        presetsDialog.show();
    }

    @Override
    public String getCurrentName() {
        return String.format(activity.getResources().getString(R.string.presets_new_name), activity.presetModels.size() + 1);
    }

    @Override
    public void onClick(DialogInterface dialog, int id) {
        EditText nameText = (EditText) (((AlertDialog) dialog).findViewById(R.id.itemNameText));
        String presetName = nameText.getText().toString();

        Log.d(SampleAppActivity.TAG, "Preset name: " + presetName);

        if (presetName != null && !presetName.isEmpty()) {
            doSavePreset(presetName);
        }
    }

    protected void doSavePreset(String presetName) {
        if ((presetName != null) && (!presetName.isEmpty()) && (itemModel != null) && (itemModel.state != null)) {
            AllJoynManager.presetManager.createPreset(itemModel.state, presetName, SampleAppActivity.LANGUAGE);
        }
    }

    protected abstract void doApplyPreset(String presetID);
}
