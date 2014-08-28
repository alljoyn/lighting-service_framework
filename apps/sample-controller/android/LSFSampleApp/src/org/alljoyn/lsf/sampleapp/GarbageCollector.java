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

import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import android.os.SystemClock;
import android.support.v4.app.Fragment;
import android.util.Log;

public class GarbageCollector implements Runnable {

    private final SampleAppActivity activity;
    private final int delay;
    private final long timeout;

    public GarbageCollector(SampleAppActivity activity, int delay, long timeout) {
        super();

        this.activity = activity;
        this.delay = delay;
        this.timeout = timeout;
    }

    public void start() {
        activity.handler.post(this);
    }

    public boolean isLampExpired(String lampID) {
        return isExpired(activity.lampModels.get(lampID));
    }

    public boolean isExpired(ItemDataModel itemModel) {
        return (itemModel == null) || ((itemModel.timestamp + timeout) < SystemClock.elapsedRealtime());
    }

    @Override
    public void run() {
        Iterator<Entry<String, LampDataModel>> lampModelsIterator = activity.lampModels.entrySet().iterator();

        while (lampModelsIterator.hasNext()) {
            Map.Entry<String, LampDataModel> entry = lampModelsIterator.next();
            LampDataModel lampModel = entry.getValue();

            // prune old records
            if (isExpired(lampModel)) {
                Log.d(SampleAppActivity.TAG, "Pruned lamp " + lampModel.id);

                lampModelsIterator.remove();

                Fragment pageFragment = activity.getSupportFragmentManager().findFragmentByTag(LampsPageFragment.TAG);

                if (pageFragment != null) {
                    LampsTableFragment tableFragment = (LampsTableFragment) pageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_TABLE);

                    if (tableFragment != null) {
                        tableFragment.removeElement(lampModel.id);

                        if (activity.lampModels.size() == 0) {
                            tableFragment.updateLoading();
                        }
                    }
                }
            }
        }

        activity.handler.postDelayed(this, delay);
    }
}
