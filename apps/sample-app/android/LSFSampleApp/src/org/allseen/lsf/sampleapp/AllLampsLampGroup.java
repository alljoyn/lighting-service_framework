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
import org.allseen.lsf.ResponseCode;

import android.util.Log;

public class AllLampsLampGroup extends LampGroup {
    protected SampleAppActivity activity;

    public AllLampsLampGroup(SampleAppActivity activity) {
        super();
        this.activity = activity;
    }

    @Override
    public void setLamps(String[] lampIDs) {
        Log.w(SampleAppActivity.TAG, "Invalid attempt to set lamp members of the all-lamp lamp group");
    }

    @Override
    public String[] getLamps() {
        return activity.lampModels.keySet().toArray(new String[activity.lampModels.size()]);
    }

    @Override
    public void setLampGroups(String[] lampGroupIDs) {
        Log.w(SampleAppActivity.TAG, "Invalid attempt to set group members of the all-lamp lamp group");
    }

    @Override
    public String[] getLampGroups() {
        return new String[] {};
    }

    @Override
    public ResponseCode isDependentLampGroup(String lampGroupID) {
        //TODO-FIX Not sure what this should be
        return ResponseCode.OK;
    }

}
