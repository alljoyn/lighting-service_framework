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

import org.allseen.lsf.ControllerServiceManagerCallback;
import org.allseen.lsf.ResponseCode;

import android.os.Handler;
import android.support.v4.app.FragmentManager;
import android.util.Log;

public class SampleControllerServiceManagerCallback extends ControllerServiceManagerCallback {
    protected SampleAppActivity activity;
    protected FragmentManager fragmentManager;
    protected Handler handler;

    public SampleControllerServiceManagerCallback(SampleAppActivity activity, FragmentManager fragmentManager, Handler handler) {
        super();

        this.activity = activity;
        this.fragmentManager = fragmentManager;
        this.handler = handler;
    }

    @Override
    public void getControllerServiceVersionReplyCB(long version) {
        Log.d(SampleAppActivity.TAG, "getControllerServiceVersionReplyCB() :" + version);
    }

    @Override
    public void lightingResetControllerServiceReplyCB(ResponseCode responseCode) {
        Log.d(SampleAppActivity.TAG, "lightingResetControllerServiceReplyCB() :" + responseCode);
    }

    @Override
    public void controllerServiceLightingResetCB() {
        Log.d(SampleAppActivity.TAG, "controllerServiceLightingResetCB()");
    }

    @Override
    public void controllerServiceNameChangedCB(String controllerServiceDeviceID, String controllerServiceName) {
        // This is currently handled by the AboutManager
        Log.d(SampleAppActivity.TAG, "controllerServiceNameChangedCB(): " + controllerServiceDeviceID + ", " + controllerServiceName);
    }
}
