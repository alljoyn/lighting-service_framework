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

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.alljoyn.about.AboutKeys;
import org.alljoyn.about.AboutService;
import org.alljoyn.about.AboutServiceImpl;
import org.alljoyn.about.client.AboutClient;
import org.alljoyn.bus.BusAttachment;
import org.alljoyn.bus.BusException;
import org.alljoyn.bus.Variant;
import org.alljoyn.services.common.AnnouncementHandler;
import org.alljoyn.services.common.BusObjectDescription;

import android.os.Handler;
import android.util.Log;

public class AboutManager implements AnnouncementHandler {
    private AboutService aboutService;
    private final Handler handler;
    private final SampleAppActivity activity;

    private static final String[] LAMP_SERVICE_INTERFACE_NAMES = {
        "org.allseen.LSF.LampService",
        "org.allseen.LSF.LampState",
        "org.allseen.LSF.LampParameters",
        "org.allseen.LSF.LampDetails" };
    private static final String[] CONTROLLER_SERVICE_INTERFACE_NAMES = {
        "org.allseen.LSF.ControllerService.Lamp",
        "org.allseen.LSF.ControllerService.LampGroup",
        "org.allseen.LSF.ControllerService.Preset",
        "org.allseen.LSF.ControllerService.Scene",
        "org.allseen.LSF.ControllerService.MasterScene" };

    public AboutManager(SampleAppActivity activity, Handler handler) {
        this.activity = activity;
        this.handler = handler;
    }

    public void initializeClient(BusAttachment bus) {
        // Add match to receive sessionless signals
        bus.addMatch("sessionless='t',type='error'");

        aboutService = AboutServiceImpl.getInstance();

        try {
            aboutService.startAboutClient(bus);
            Log.d(SampleAppActivity.TAG, "AboutClient started");
        } catch (Exception e) {
            Log.e(SampleAppActivity.TAG, "Exception caught when starting AboutService client: " + e.getMessage());
        }

        aboutService.addAnnouncementHandler(this, AboutManager.LAMP_SERVICE_INTERFACE_NAMES);
        aboutService.addAnnouncementHandler(this, AboutManager.CONTROLLER_SERVICE_INTERFACE_NAMES);
    }

    public void destroy() {
        if (aboutService != null) {
            aboutService.removeAnnouncementHandler(this, AboutManager.LAMP_SERVICE_INTERFACE_NAMES);
            aboutService.removeAnnouncementHandler(this, AboutManager.CONTROLLER_SERVICE_INTERFACE_NAMES);

            try {
                aboutService.stopAboutClient();
                Log.d(SampleAppActivity.TAG, "AboutClient destroyed");
            } catch (Exception e) {
                Log.e(SampleAppActivity.TAG, "Exception caught trying to stop AboutService client: " + e.getMessage());
            }

            aboutService = null;
        }
    }

    @Override
    public void onAnnouncement(String peerName, short port, BusObjectDescription[] busObjects, Map<String, Variant> announcedData) {
        Log.d(SampleAppActivity.TAG, "Announcement received from AboutService");

        // Flatten the interfaces
        List<String> allInterfaces = new ArrayList<String>();
        for (BusObjectDescription busObject : busObjects) {
            for (String iface : busObject.getInterfaces()) {
                allInterfaces.add(iface);
            }
        }

        if (containsLampInterfaces(allInterfaces)) {
            Log.d(SampleAppActivity.TAG, "announcement: lamp ifaces found");
            addLampAnnouncedAboutData(peerName, port, announcedData);
        } else if (containsControllerInterfaces(allInterfaces)) {
            Log.d(SampleAppActivity.TAG, "announcement: controller ifaces found");
            addControllerAnnouncedAboutData(announcedData);
        }
    }

    protected boolean containsLampInterfaces(List<String> allInterfaces) {
        for (String iface : AboutManager.LAMP_SERVICE_INTERFACE_NAMES) {
            if (!allInterfaces.contains(iface)) {
                // this does not have a necessary lamp interface
                return false;
            }
        }

        return true;
    }

    protected boolean containsControllerInterfaces(List<String> allInterfaces) {
        for (String iface : AboutManager.CONTROLLER_SERVICE_INTERFACE_NAMES) {
            if (!allInterfaces.contains(iface)) {
                // this does not have a necessary controller interface
                return false;
            }
        }

        return true;
    }

    @Override
    public void onDeviceLost(String arg0) {
        Log.d(SampleAppActivity.TAG, "Device Lost: " + arg0);
    }

    protected void addLampAnnouncedAboutData(String peerName, short port, final Map<String, Variant> announcedData) {
        String lampID = getStringFromAnnouncedData(AboutKeys.ABOUT_DEVICE_ID, announcedData, null);

        if (lampID != null) {
            Log.d(SampleAppActivity.TAG, "Announce received: " + lampID);

            activity.lampManagerCB.postOnLampAboutAnnouncedData(lampID, peerName, port, announcedData, 0);
        } else {
            Log.e(SampleAppActivity.TAG, "Announcement lacks device ID");
        }
    }

    public void getLampQueriedAboutData(String lampID, String peer, short port) {
        AboutClient aboutClient = null;
        Map<String, Object> aboutData = null;

        try {
            aboutClient = aboutService.createAboutClient(peer, null, port);
        } catch (Exception e) {
            Log.e(SampleAppActivity.TAG, "About client creation failed: " + e.getMessage());
        }

        try {
            if (aboutClient != null) {
                aboutClient.connect();

                aboutData = aboutClient.getAbout(SampleAppActivity.LANGUAGE);

                activity.lampManagerCB.postOnLampQueriedAboutData(lampID, aboutData, 0);
            }
        } catch (BusException e) {
            Log.e(SampleAppActivity.TAG, "About data retrieval failed: " + e.getMessage());
        } finally {
            if (aboutClient != null) {
                aboutClient.disconnect();
            }
        }
    }

    protected void addControllerAnnouncedAboutData(final Map<String, Variant> announceData) {
        String controllerID = getStringFromAnnouncedData(AboutKeys.ABOUT_DEVICE_ID, announceData, null);
        String controllerName = getStringFromAnnouncedData(AboutKeys.ABOUT_DEVICE_NAME, announceData, null);

        activity.controllerClientCB.postOnControllerAnnouncedAboutData(controllerID, controllerName, 200);
    }

    public static String getStringFromAnnouncedData(String key, Map<String, Variant> announceData, String defaultValue) {
        String value = defaultValue;

        if (announceData != null) {
            try {
                Variant variant = announceData.get(key);

                if (variant != null) {
                    value = variant.getObject(String.class);
                }
            } catch (BusException e) {
                Log.e(SampleAppActivity.TAG, "Announce parsing failed: key: " + key + ": " + e.getMessage());
            }
        }

        return value;
    }

    public static String getByteArrayHexStringFromAnnouncedData(String key, Map<String, Variant> announcedData, String defaultValue) {
        String value = defaultValue;

        if (announcedData != null) {
            try {
                Variant variant = announcedData.get(key);

                if (variant != null) {
                    byte[] bytes = variant.getObject(byte[].class);

                    StringBuilder sb = new StringBuilder("0x");

                    for (int i = 0; i < bytes.length; i++) {
                        sb.append(String.format("%02X", bytes[i]));

                        // group the bytes into groups of 4
                        if((i + 1) % 4 == 0) {
                            sb.append(" ");
                        }
                    }

                    value = sb.toString();
                }
            } catch (BusException e) {
                Log.e(SampleAppActivity.TAG, "Announce parsing failed: key: " + key + ": " + e.getMessage());
            }
        }

       return value;
    }

    public static String getStringFromQueriedData(String key, Map<String, Object> queriedData, String defaultValue) {
        String value = defaultValue;

        if (queriedData != null) {
            Object object = queriedData.get(key);

            if (object instanceof String) {
                value = (String)object;
            }
        }

        return value;
    }
}
