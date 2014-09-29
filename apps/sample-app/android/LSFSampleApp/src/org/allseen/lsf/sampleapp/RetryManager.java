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

import android.util.Log;

public class RetryManager implements Runnable {

    private final SampleAppActivity activity;
    private final int interval;

    public RetryManager(SampleAppActivity activity, int interval) {
        super();

        this.activity = activity;
        this.interval = interval;
    }

    public void start() {
        activity.handler.post(this);
    }

    public void post(final Runnable command) {
        activity.handler.post(new Runnable() {
            @Override
            public void run() {
                activity.commands.offer(command);
            }
         });
    }

    @Override
    public void run() {
        if (AllJoynManager.controllerConnected) {
            Runnable command = activity.commands.poll();

            if (command != null) {
                Log.d(SampleAppActivity.TAG, "Retrying next command");
                command.run();
            }
        }

        activity.handler.postDelayed(this, interval);
    }
}
