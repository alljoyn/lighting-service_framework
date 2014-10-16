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
package org.allseen.lsf.helper.model;

import org.allseen.lsf.LampGroup;
import org.allseen.lsf.ResponseCode;
import org.allseen.lsf.helper.manager.LightingSystemManager;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class AllLampsLampGroup extends LampGroup {
    protected LightingSystemManager director;

    public AllLampsLampGroup(LightingSystemManager director) {
        super();
        this.director = director;
    }

    @Override
    public void setLamps(String[] lampIDs) {
        //TODO-FIX Log.w(SampleAppActivity.TAG, "Invalid attempt to set lamp members of the all-lamp lamp group");
    }

    @Override
    public String[] getLamps() {
        return director.getLampCollectionManager().getIDArray();
    }

    @Override
    public void setLampGroups(String[] lampGroupIDs) {
        //TODO-FIX Log.w(SampleAppActivity.TAG, "Invalid attempt to set group members of the all-lamp lamp group");
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
