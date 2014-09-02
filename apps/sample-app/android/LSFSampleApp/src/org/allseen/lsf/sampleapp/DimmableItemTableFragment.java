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

import android.content.Context;
import android.graphics.Color;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;

public abstract class DimmableItemTableFragment
    extends ScrollableTableFragment
    implements
        View.OnClickListener,
        SeekBar.OnSeekBarChangeListener {

    protected abstract int getInfoButtonImageID();
    protected abstract Fragment getInfoFragment();

    public TableRow insertDimmableItemRow(Context context, String itemID, String sortableName, boolean powerOn, String displayName, long brightness, int infoBG) {
        return insertDimmableItemRow(
            context,
            (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE),
            itemID,
            sortableName,
            powerOn,
            displayName,
            brightness,
            infoBG);
    }

    public TableRow insertDimmableItemRow(Context context, String itemID, String sortableName, boolean powerOn, String displayName, long brightness, int infoBG, boolean enabled) {
        return insertDimmableItemRow(
            context,
            (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE),
            itemID,
            sortableName,
            powerOn,
            displayName,
            brightness,
            infoBG,
            enabled);
    }

    public TableRow insertDimmableItemRow(LayoutInflater inflater, View root, String itemID, String sortableName, boolean powerOn, String displayName, long brightness, int infoBG) {
        return insertDimmableItemRow(
            root.getContext(),
            inflater,
            itemID,
            sortableName,
            powerOn,
            displayName,
            brightness,
            infoBG);
    }

    public TableRow insertDimmableItemRow(Context context, LayoutInflater inflater, String itemID, String sortableName, boolean powerOn, String displayName, long modelBrightness, int infoBG) {
        return insertDimmableItemRow(
                context,
                inflater,
                itemID,
                sortableName,
                powerOn,
                displayName,
                modelBrightness,
                infoBG,
                true);
    }

    public TableRow insertDimmableItemRow(Context context, LayoutInflater inflater, String itemID, String sortableName, boolean powerOn, String displayName, long modelBrightness, int infoBG, boolean enabled) {
        Log.d(SampleAppActivity.TAG, "insertDimmableItemRow(): " + itemID + ", " + sortableName + ", " + displayName);

        final boolean isEnabled = enabled;

        TableRow tableRow = (TableRow)table.findViewWithTag(itemID);

        if (tableRow == null) {
            tableRow = new TableRow(context);

            inflater.inflate(R.layout.view_dimmable_item_row, tableRow);

            ImageButton powerButton = (ImageButton)tableRow.findViewById(R.id.dimmableItemButtonPower);
            powerButton.setTag(itemID);
            powerButton.setBackgroundResource(powerOn ? R.drawable.power_button_on : R.drawable.power_button_off);
            powerButton.setOnClickListener(this);

            ((TextView)tableRow.findViewById(R.id.dimmableItemRowText)).setText(displayName);

            SeekBar seekBar = (SeekBar)tableRow.findViewById(R.id.dimmableItemRowSlider);
            seekBar.setProgress(DimmableItemScaleConverter.convertBrightnessModelToView(modelBrightness));
            seekBar.setTag(itemID);
            seekBar.setSaveEnabled(false);
            seekBar.setOnSeekBarChangeListener(this);
            seekBar.setEnabled(isEnabled);


            ImageButton infoButton = (ImageButton)tableRow.findViewById(R.id.dimmableItemButtonMore);
            infoButton.setImageResource(getInfoButtonImageID());
            infoButton.setTag(itemID);
            infoButton.setOnClickListener(this);
            if (infoBG != 0) {
                infoButton.setBackgroundColor(powerOn ? infoBG : Color.BLACK);
            }

            tableRow.setTag(itemID);
            TableSorter.insertSortedTableRow(table, tableRow, sortableName);
        } else {
            ((ImageButton)tableRow.findViewById(R.id.dimmableItemButtonPower)).setBackgroundResource(powerOn ? R.drawable.power_button_on : R.drawable.power_button_off);
            ((TextView)tableRow.findViewById(R.id.dimmableItemRowText)).setText(displayName);
            ((SeekBar)tableRow.findViewById(R.id.dimmableItemRowSlider)).setProgress(DimmableItemScaleConverter.convertBrightnessModelToView(modelBrightness));
            ((SeekBar)tableRow.findViewById(R.id.dimmableItemRowSlider)).setEnabled(isEnabled);
            if (infoBG != 0) {
                ((ImageButton)tableRow.findViewById(R.id.dimmableItemButtonMore)).setBackgroundColor(powerOn ? infoBG : Color.BLACK);
            }

            if (!sortableName.equals(tableRow.getTag(R.id.TAG_KEY_SORTABLE_NAME).toString())) {
                table.removeView(tableRow);
                TableSorter.insertSortedTableRow(table, tableRow, sortableName);
            }
        }

        tableRow.setClickable(true);
        tableRow.setOnClickListener(this);
        return tableRow;
    }

    @Override
    public void onClick(View button) {
        Log.d(SampleAppActivity.TAG, "onClick()");

        int buttonID = button.getId();

        if (parent != null) {
            if (buttonID == R.id.dimmableItemButtonPower) {
                ((SampleAppActivity)getActivity()).togglePower(type, button.getTag().toString());
            } else if (buttonID == R.id.dimmableItemButtonMore) {
                ((SampleAppActivity)getActivity()).onItemButtonMore(parent, type ,button, button.getTag().toString(), null);
            } else if (!((SeekBar)button.findViewById(R.id.dimmableItemRowSlider)).isEnabled()) {
                Toast.makeText(getActivity(), R.string.no_support_dimmable, Toast.LENGTH_LONG).show();
            }
        }
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int brightness, boolean fromUser) {
        //AJSI-291: UI Slider behaviour change (from continuous updating to updating when finger is lifted)
        /*if (parent != null && fromUser) {
            ((SampleAppActivity)getActivity()).setBrightness(type, seekBar.getTag().toString(), seekBar.getProgress());
        }*/
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {
        // Currently nothing to do
    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        if (parent != null) {
            ((SampleAppActivity)getActivity()).setBrightness(type, seekBar.getTag().toString(), seekBar.getProgress());
        }
    }

    public abstract void addElement(String id);
}
