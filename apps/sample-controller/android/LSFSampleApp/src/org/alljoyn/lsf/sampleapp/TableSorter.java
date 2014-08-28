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

import android.util.Log;
import android.widget.TableLayout;
import android.widget.TableRow;

public class TableSorter {
    // Ensure groups are listed first, then lamps, then scenes
    public final static String SORTING_PREFIX_GROUP = "G:";
    public final static String SORTING_PREFIX_LAMP = "L:";
    public final static String SORTING_PREFIX_SCENE = "S:";

    protected static void insertSortedTableRow(TableLayout table, TableRow tableRow, String sortableName) {
        insertSortedTableRow("", table, tableRow, sortableName);
    }

    protected static void insertSortedTableRow(String sortPrefix, TableLayout table, TableRow tableRow, String sortableName) {
        int searchLow = 0;
        int searchHigh = table.getChildCount() - 1;

        if (sortableName.contains(SampleGroupManager.ALL_LAMPS_GROUP_ID)) {
            sortableName = SampleGroupManager.ALL_LAMPS_GROUP_ID;
        }

        while (searchLow <= searchHigh) {
            int searchNext = searchLow + ((searchHigh - searchLow) / 2);
            String nextName = table.getChildAt(searchNext).getTag(R.id.TAG_KEY_SORTABLE_NAME).toString();
            int comparison = sortableName.compareToIgnoreCase(nextName);

            Log.v(SampleAppActivity.TAG, "insertSortedTableRow(): checking [" + searchLow + ", " + searchNext + ", " + searchHigh + "]: " + sortableName + " <=> " + nextName);

            if (comparison < 0) {
                searchHigh = searchNext - 1;
            } else if (comparison > 0) {
                searchLow = searchNext + 1;
            } else {
                searchLow = searchHigh + 1;
            }
        }

        tableRow.setTag(R.id.TAG_KEY_SORTABLE_NAME, sortableName);

        table.addView(tableRow, searchLow);
    }
}
