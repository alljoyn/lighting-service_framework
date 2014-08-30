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
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TableRow;
import android.widget.TextView;

public abstract class DetailedItemTableFragment extends ScrollableTableFragment implements View.OnClickListener {
    protected void insertDetailedItemRow(Context context, String itemID, String sortableName, String displayName, String details) {
        insertDetailedItemRow(
            context,
            (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE),
            itemID,
            sortableName,
            displayName,
            details,
            false);
    }

    protected void insertDetailedItemRow(LayoutInflater inflater, View root, String itemID, String sortableName, String displayName, String details) {
        insertDetailedItemRow(
            root.getContext(),
            inflater,
            itemID,
            sortableName,
            displayName,
            details,
            false);
    }

    protected void insertDetailedItemRow(Context context, String itemID, String sortableName, String displayName, String details, boolean isMasterScene) {
        insertDetailedItemRow(
            context,
            (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE),
            itemID,
            sortableName,
            displayName,
            details,
            isMasterScene);
    }

    protected void insertDetailedItemRow(Context context, LayoutInflater inflater, String itemID, String sortableName, String displayName, String details) {
        insertDetailedItemRow(context,
            inflater,
            itemID,
            sortableName,
            displayName,
            details,
            false);
    }

    protected void insertDetailedItemRow(Context context, LayoutInflater inflater, String itemID, String sortableName, String displayName, String details, boolean isMasterScene) {
        Log.d(SampleAppActivity.TAG, "insertDetailedItemRow(): " + itemID + ", " + sortableName + ", " + displayName);

        TableRow tableRow = (TableRow)table.findViewWithTag(itemID);

        if (tableRow == null) {
            tableRow = new TableRow(context);

            inflater.inflate(R.layout.view_detailed_item_row, tableRow);

            ImageButton icon = (ImageButton)tableRow.findViewById(R.id.detailedItemButtonIcon);
            icon.setBackgroundResource(isMasterScene ? R.drawable.master_scene_set_icon : R.drawable.scene_set_icon);
            ((TextView)tableRow.findViewById(R.id.detailedItemRowTextHeader)).setText(displayName);
            ((TextView)tableRow.findViewById(R.id.detailedItemRowTextDetails)).setText(details);

            ImageButton infoButton = (ImageButton)tableRow.findViewById(R.id.detailedItemButtonMore);
            infoButton.setImageResource(R.drawable.nav_more_menu_icon);
            infoButton.setTag(itemID);
            infoButton.setOnClickListener(this);

            tableRow.setTag(itemID);

            TableSorter.insertSortedTableRow(table, tableRow, sortableName);
        } else {
            ((TextView)tableRow.findViewById(R.id.detailedItemRowTextHeader)).setText(displayName);
            ((TextView)tableRow.findViewById(R.id.detailedItemRowTextDetails)).setText(details);

            if (!sortableName.equals(tableRow.getTag(R.id.TAG_KEY_SORTABLE_NAME).toString())) {
                table.removeView(tableRow);

                TableSorter.insertSortedTableRow(table, tableRow, sortableName);
            }
        }
    }

    @Override
    public void onClick(View button) {
        int buttonID = button.getId();

        Log.d(SampleAppActivity.TAG, "onClick(): " + buttonID + ", " + parent);

        if (parent != null) {
            if (buttonID == R.id.detailedItemButtonMore) {
                ((SampleAppActivity)getActivity()).onItemButtonMore(parent, type, button, button.getTag().toString(), null);
            }
        }
    }

    public abstract void addElement(String id);

    public void removeElement(String id) {
        final TableRow row = (TableRow) table.findViewWithTag(id);
        if (row != null) {
            getActivity().runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    table.removeView(row);
                    table.postInvalidate();
                }
            });
        }
    }
}
