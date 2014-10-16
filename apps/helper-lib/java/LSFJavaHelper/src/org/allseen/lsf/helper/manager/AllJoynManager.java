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
package org.allseen.lsf.helper.manager;

import java.util.concurrent.Semaphore;

import org.alljoyn.bus.BusAttachment;
import org.allseen.lsf.ControllerClient;
import org.allseen.lsf.ControllerClientCallback;
import org.allseen.lsf.LampGroupManager;
import org.allseen.lsf.LampManager;
import org.allseen.lsf.LampManagerCallback;
import org.allseen.lsf.MasterSceneManager;
import org.allseen.lsf.MasterSceneManagerCallback;
import org.allseen.lsf.PresetManager;
import org.allseen.lsf.PresetManagerCallback;
import org.allseen.lsf.SceneManager;
import org.allseen.lsf.SceneManagerCallback;
import org.allseen.lsf.helper.callback.HelperGroupManagerCallback;

import android.content.Context;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.util.Log;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class AllJoynManager {
    private static final String TAG = "TagAlljoynManager";

    public static BusAttachment bus;
    public static ControllerClient controllerClient;
    public static LampManager lampManager;
    public static LampGroupManager groupManager;
    public static PresetManager presetManager;
    public static SceneManager sceneManager;
    public static MasterSceneManager masterSceneManager;
    public static AboutManager aboutManager;

    public static boolean controllerConnected;
    public static final Semaphore alljoynSemaphore = new Semaphore(1);

    public static void init(
            final FragmentManager fragmentManager,
            final ControllerClientCallback controllerClientCallback,
            final LampManagerCallback lampManagerCallback,
            final HelperGroupManagerCallback groupManagerCallback,
            final PresetManagerCallback presetManagerCallback,
            final SceneManagerCallback sceneManagerCallback,
            final MasterSceneManagerCallback masterSceneManagerCallback,
            final AboutManager aboutManager ) {

//TODO-FIX
//        // acquire lock on new thread to prevent deadlock
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//                Log.d(AllJoynManager.TAG, "AllJoynManager.init() - acquiring lock");
//                alljoynSemaphore.acquireUninterruptibly();
//                Log.d(AllJoynManager.TAG, "AllJoynManager.init() - locked");
//
//                // run business logic back in main UI thread
//                activity.handler.post(new Runnable() {
//                    @Override
//                    public void run() {
                        Log.d(AllJoynManager.TAG, "AllJoynManager.init() - running");

                        AllJoynManager.aboutManager = aboutManager;

                        controllerConnected = false;

                        AllJoynManagerFragment alljoynManagerFragment = (AllJoynManagerFragment) fragmentManager.findFragmentByTag(AllJoynManager.TAG);

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
//TODO-FIX
//                    }
//                });
//            }
//        }).start();
//        Log.d(AllJoynManager.TAG, "AllJoynManager.init() - thread started, resuming");
    }

    public static void destroy(final FragmentManager fragmentManager) {

//TODO-FIX
//        // acquire lock on new thread to prevent deadlock
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//                Log.d(AllJoynManager.TAG, "AllJoynManager.destroy() - acquiring lock");
//                alljoynSemaphore.acquireUninterruptibly();
//                Log.d(AllJoynManager.TAG, "AllJoynManager.destroy() - locked");
//
//                // run business logic back in main UI thread
//                activity.handler.post(new Runnable() {
//                    @Override
//                    public void run() {
                        Log.d(AllJoynManager.TAG, "AllJoynManager.destroy() - running");

                        AllJoynManagerFragment alljoynManagerFragment = (AllJoynManagerFragment) fragmentManager.findFragmentByTag(AllJoynManager.TAG);

                        if (alljoynManagerFragment != null) {
                            fragmentManager.beginTransaction().remove(alljoynManagerFragment).commitAllowingStateLoss();
                        }
//TODO-FIX
//                    }
//                });
//            }
//        }).start();
//        Log.d(AllJoynManager.TAG, "AllJoynManager.destroy() - thread started, resuming");
    }

    /**
     * <b>WARNING: This class is not intended to be used by clients, and its interface may change
     * in subsequent releases of the SDK</b>.
     */
    public static class AllJoynManagerFragment extends Fragment {
        private AllJoynManagerAsyncTask alljoynManagerTask;

        public ControllerClientCallback controllerClientCallback;
        public LampManagerCallback lampManagerCallback;
        public HelperGroupManagerCallback groupManagerCallback;
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

            Log.d(AllJoynManager.TAG, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
            Log.d(AllJoynManager.TAG, "onDestroy()");
            aboutManager.destroy();

            masterSceneManager.destroy();
            sceneManager.destroy();
            presetManager.destroy();
            groupManager.destroy();
            lampManager.destroy();

            controllerClient.destroy();

            bus.disconnect();
            bus.release();

//TODO-DEL?
//            AllJoynManager.masterSceneManager = null;
//            AllJoynManager.sceneManager = null;
//            AllJoynManager.presetManager = null;
//            AllJoynManager.groupManager = null;
//            AllJoynManager.lampManager = null;
//            AllJoynManager.controllerClient = null;
//            AllJoynManager.bus = null;

            AllJoynManager.alljoynSemaphore.release();
            Log.d(AllJoynManager.TAG, "AllJoynManager.destroy() - unlocked");
        }
    };

    private static class AllJoynManagerAsyncTask extends AsyncTask<String, Void, Void> {
        private final AllJoynManagerFragment alljoynManagerFragment;

        private AllJoynManagerAsyncTask(AllJoynManagerFragment alljoynManagerFragment) {
            this.alljoynManagerFragment = alljoynManagerFragment;
        }

        @Override
        protected Void doInBackground(String... params) {
            Log.d(AllJoynManager.TAG, "Creating BusAttachment");
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

            AllJoynManager.alljoynSemaphore.release();
            Log.d(AllJoynManager.TAG, "AllJoynManager.init() - unlocked");
       }
    }
}
