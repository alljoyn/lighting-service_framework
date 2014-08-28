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

import org.alljoyn.bus.BusAttachment;
import org.alljoyn.lsf.ControllerClient;
import org.alljoyn.lsf.ControllerClientCallback;
import org.alljoyn.lsf.LampGroupManager;
import org.alljoyn.lsf.LampManager;
import org.alljoyn.lsf.LampManagerCallback;
import org.alljoyn.lsf.PresetManager;
import org.alljoyn.lsf.PresetManagerCallback;
import org.alljoyn.lsf.SceneManager;
import org.alljoyn.lsf.SceneManagerCallback;
import org.alljoyn.lsf.MasterSceneManager;
import org.alljoyn.lsf.MasterSceneManagerCallback;

import android.content.Context;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.util.Log;

public class AllJoynManager {
    private static final String TAG = "TagAlljoynManager";

    public static BusAttachment bus;
    public static ControllerClient controllerClient;
    public static LampManager lampManager;
    public static LampGroupManager groupManager;
    public static PresetManager presetManager;
    public static SceneManager sceneManager;
    public static MasterSceneManager masterSceneManager;
    public static SampleAppActivity activity;
    public static AboutManager aboutManager;

    public static boolean controllerConnected;

    public static void init(
            FragmentManager fragmentManager,
            ControllerClientCallback controllerClientCallback,
            LampManagerCallback lampManagerCallback,
            SampleGroupManagerCallback groupManagerCallback,
            PresetManagerCallback presetManagerCallback,
            SceneManagerCallback sceneManagerCallback,
            MasterSceneManagerCallback masterSceneManagerCallback,
            AboutManager aboutManager,
            SampleAppActivity activity) {
        AllJoynManager.activity = activity;
        AllJoynManager.aboutManager = aboutManager;

        controllerConnected = false;

        AllJoynManagerFragment alljoynManagerFragment = (AllJoynManagerFragment)fragmentManager.findFragmentByTag(AllJoynManager.TAG);

        if (alljoynManagerFragment == null) {
            alljoynManagerFragment = new AllJoynManagerFragment();
            alljoynManagerFragment.controllerClientCallback = controllerClientCallback;
            alljoynManagerFragment.lampManagerCallback = lampManagerCallback;
            alljoynManagerFragment.groupManagerCallback = groupManagerCallback;
            alljoynManagerFragment.presetManagerCallback = presetManagerCallback;
            alljoynManagerFragment.sceneManagerCallback = sceneManagerCallback;
            alljoynManagerFragment.masterSceneManagerCallback = masterSceneManagerCallback;

            fragmentManager.beginTransaction().add(alljoynManagerFragment, AllJoynManager.TAG).commit();
        }
    }

    public static void destroy(FragmentManager fragmentManager) {
        AllJoynManagerFragment alljoynManagerFragment = (AllJoynManagerFragment)fragmentManager.findFragmentByTag(AllJoynManager.TAG);

        if (alljoynManagerFragment != null) {
            fragmentManager.beginTransaction().remove(alljoynManagerFragment).commitAllowingStateLoss();
        }
    }

    public static class AllJoynManagerFragment extends Fragment {
        private AllJoynManagerAsyncTask alljoynManagerTask;

        public ControllerClientCallback controllerClientCallback;
        public LampManagerCallback lampManagerCallback;
        public SampleGroupManagerCallback groupManagerCallback;
        public PresetManagerCallback presetManagerCallback;
        public SceneManagerCallback sceneManagerCallback;
        public MasterSceneManagerCallback masterSceneManagerCallback;

        public BusAttachment bus;
        public ControllerClient controllerClient;
        public LampManager lampManager;
        public LampGroupManager groupManager;
        public PresetManager presetManager;
        public SceneManager sceneManager;
        public MasterSceneManager masterSceneManager;

        @Override
        public void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);

            setRetainInstance(true);

            Context context = getActivity().getApplicationContext();

            org.alljoyn.bus.alljoyn.DaemonInit.PrepareDaemon(context);

            alljoynManagerTask = new AllJoynManagerAsyncTask(this);
            alljoynManagerTask.execute(context.getPackageName());
        }

        @Override
        public void onDestroy() {
            super.onDestroy();

            Log.d(SampleAppActivity.TAG, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
            Log.d(SampleAppActivity.TAG, "onDestroy()");
            aboutManager.destroy();

            masterSceneManager.destroy();
            sceneManager.destroy();
            presetManager.destroy();
            groupManager.destroy();
            lampManager.destroy();

            controllerClient.destroy();

            bus.disconnect();
            bus.release();

            AllJoynManager.masterSceneManager = null;
            AllJoynManager.sceneManager = null;
            AllJoynManager.presetManager = null;
            AllJoynManager.groupManager = null;
            AllJoynManager.lampManager = null;
            AllJoynManager.controllerClient = null;
            AllJoynManager.bus = null;
        }
    };

    private static class AllJoynManagerAsyncTask extends AsyncTask<String, Void, Void> {
        private final AllJoynManagerFragment alljoynManagerFragment;

        private AllJoynManagerAsyncTask(AllJoynManagerFragment alljoynManagerFragment) {
            this.alljoynManagerFragment = alljoynManagerFragment;
        }

        @Override
        protected Void doInBackground(String... params) {
            Log.d(SampleAppActivity.TAG, "Creating BusAttachment");
            alljoynManagerFragment.bus = new BusAttachment(params[0], BusAttachment.RemoteMessage.Receive);
            alljoynManagerFragment.bus.connect();
//TODO-TMP            alljoynManagerFragment.bus.setDebugLevel("ALL", 7);
//TODO-TMP            alljoynManagerFragment.bus.setDaemonDebug("ALL", 7);

            alljoynManagerFragment.controllerClient = new ControllerClient(alljoynManagerFragment.bus, alljoynManagerFragment.controllerClientCallback);
            alljoynManagerFragment.lampManager = new LampManager(alljoynManagerFragment.controllerClient, alljoynManagerFragment.lampManagerCallback);
            alljoynManagerFragment.groupManager = new SampleGroupManager(alljoynManagerFragment.controllerClient, alljoynManagerFragment.groupManagerCallback);
            alljoynManagerFragment.presetManager = new PresetManager(alljoynManagerFragment.controllerClient, alljoynManagerFragment.presetManagerCallback);
            alljoynManagerFragment.sceneManager = new SceneManager(alljoynManagerFragment.controllerClient, alljoynManagerFragment.sceneManagerCallback);
            alljoynManagerFragment.masterSceneManager = new MasterSceneManager(alljoynManagerFragment.controllerClient, alljoynManagerFragment.masterSceneManagerCallback);

            return null;
        }

        @Override
        protected void onPostExecute(Void response) {
            super.onPostExecute(response);

            AllJoynManager.bus = alljoynManagerFragment.bus;
            AllJoynManager.controllerClient = alljoynManagerFragment.controllerClient;
            AllJoynManager.lampManager = alljoynManagerFragment.lampManager;
            AllJoynManager.groupManager = alljoynManagerFragment.groupManager;
            AllJoynManager.presetManager = alljoynManagerFragment.presetManager;
            AllJoynManager.sceneManager = alljoynManagerFragment.sceneManager;
            AllJoynManager.masterSceneManager = alljoynManagerFragment.masterSceneManager;

            aboutManager.initializeClient(AllJoynManager.bus);
       }
    }
}
