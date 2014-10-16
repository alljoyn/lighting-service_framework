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
package org.allseen.lsf.helper.callback;

import java.util.Map;

import org.alljoyn.about.AboutKeys;
import org.alljoyn.bus.Variant;
import org.allseen.lsf.ControllerClientCallback;
import org.allseen.lsf.ErrorCode;
import org.allseen.lsf.helper.manager.AboutManager;
import org.allseen.lsf.helper.manager.AllJoynManager;
import org.allseen.lsf.helper.manager.ControllerMaintenance;
import org.allseen.lsf.helper.manager.LightingSystemManager;
import org.allseen.lsf.helper.model.ControllerDataModel;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class HelperControllerClientCallback extends ControllerClientCallback {
    protected LightingSystemManager director;

    public HelperControllerClientCallback(LightingSystemManager director) {
        super();

        this.director = director;
    }

    @Override
    public void connectedToControllerServiceCB(String controllerServiceDeviceID, String controllerServiceName) {
        AllJoynManager.controllerConnected = true;

        postUpdateControllerID(controllerServiceDeviceID, controllerServiceName, true, 0);
        postGetAllLampIDs();
        postGetAllLampGroupIDs();
        postGetAllPresetIDs();
        postGetAllBasicSceneIDs();
        postGetAllMasterSceneIDs();
    }

    @Override
    public void connectToControllerServiceFailedCB(String controllerServiceDeviceID, String controllerServiceName) {
        AllJoynManager.controllerConnected = false;
        postUpdateControllerID(controllerServiceDeviceID, controllerServiceName, false, 0);
    }

    @Override
    public void disconnectedFromControllerServiceCB(String controllerServiceDeviceID, String controllerServiceName) {
        AllJoynManager.controllerConnected = false;
        postUpdateControllerID(controllerServiceDeviceID, controllerServiceName, false, 0);
    }

    @Override
    public void controllerClientErrorCB(ErrorCode[] errorCodes) {
        director.getControllerManager().sendErrorEvent("controllerClientErrorCB", errorCodes);
    }

    public void postUpdateControllerID(final String controllerID, final String controllerName, final boolean connected, int delay) {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                ControllerDataModel leadModel = director.getControllerManager().getLeadControllerModel();

                if (connected || leadModel.id.equals(controllerID)) {
                    leadModel.id = controllerID;
                    leadModel.setName(controllerName);
                    leadModel.connected = connected;
                    leadModel.updateTime();

                    postSendControllerChanged();
                }
            }
        }, delay);
    }

    public void postUpdateControllerName(final String controllerID, Map<String, Variant> announceData, int delay) {
        final String controllerName = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_DEVICE_NAME, announceData, null);

        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                ControllerDataModel leadModel = director.getControllerManager().getLeadControllerModel();

                if (leadModel.id.equals(controllerID)) {
                    leadModel.setName(controllerName);
                    leadModel.updateTime();

                    postSendControllerChanged();
                }
            }
        }, delay);
    }

    protected void postGetAllLampIDs() {
        new ControllerMaintenance(director);
    }

    protected void postGetAllLampGroupIDs() {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                AllJoynManager.groupManager.getAllLampGroupIDs();
            }
        }, 100);
    }

    protected void postGetAllPresetIDs() {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                AllJoynManager.presetManager.getAllPresetIDs();
            }
        }, 200);
    }

    protected void postGetAllBasicSceneIDs() {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                AllJoynManager.sceneManager.getAllSceneIDs();
            }
        }, 300);
    }

    protected void postGetAllMasterSceneIDs() {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                AllJoynManager.masterSceneManager.getAllMasterSceneIDs();
            }
        }, 400);
    }

    protected void postSendControllerChanged() {
        // if connection status is ever changed, then prompt for updating the loading information
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                director.getControllerManager().sendLeaderStateChangeEvent();
            }
        });
    }
}
