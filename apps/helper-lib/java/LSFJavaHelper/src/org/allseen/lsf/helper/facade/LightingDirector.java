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
package org.allseen.lsf.helper.facade;

import org.allseen.lsf.helper.facade.Group;
import org.allseen.lsf.helper.facade.Lamp;
import org.allseen.lsf.helper.facade.Scene;
import org.allseen.lsf.helper.manager.LightingSystemManager;
import android.os.Handler;
import android.support.v4.app.FragmentManager;

public class LightingDirector {
    private final LightingSystemManager lightingManager;

    public LightingDirector(Handler handler) {
        super();
        lightingManager = new LightingSystemManager(handler);
    }

    public void start(FragmentManager fragmentManager) {
        lightingManager.start(fragmentManager);
    }

    public void stop(FragmentManager fragmentManager) {
        lightingManager.stop(fragmentManager);
    }

    public Lamp[] getLamps() {
        return lightingManager.getLampCollectionManager().getLamps();
    }

    public Group[] getGroups() {
        return lightingManager.getGroupCollectionManager().getGroups();
    }

    public Scene[] getScenes() {
        return lightingManager.getSceneCollectionManager().getScenes();
    }

    public void postOnNextControllerConnection(Runnable task, int delay) {
        lightingManager.postOnNextControllerConnection(task, delay);
    }

//TODO Untested -- temporarily hidden until after the hackathon
//    public void addLampCollectionListener(LampCollectionListener listener) {
//        getLampCollectionManager().addListener(listener);
//    }
//
//    public LampCollectionManager getLampCollectionManager() {
//        return lightingManager.getLampCollectionManager();
//    }
//
//    public void addGroupCollectionListener(GroupCollectionListener listener) {
//        getGroupCollectionManager().addListener(listener);
//    }
//
//    public GroupCollectionManager getGroupCollectionManager() {
//        return lightingManager.getGroupCollectionManager();
//    }
//
//    public Preset[] getPresets() {
//        return getPresetCollectionManager().getPresets();
//    }
//
//    public void addPresetCollectionListener(PresetCollectionListener listener) {
//        getPresetCollectionManager().addListener(listener);
//    }
//
//    public PresetCollectionManager getPresetCollectionManager() {
//        return lightingManager.getPresetCollectionManager();
//    }
//
//    public void addSceneCollectionListener(SceneCollectionListener listener) {
//        getSceneCollectionManager().addListener(listener);
//    }
//
//    public SceneCollectionManager getSceneCollectionManager() {
//        return lightingManager.getSceneCollectionManager();
//    }
//
//    public MasterScene[] getMasterScenes() {
//        return getMasterSceneCollectionManager().getMasterScenes();
//    }
//
//    public void addMasterSceneCollectionListener(MasterSceneCollectionListener listener) {
//        getMasterSceneCollectionManager().addListener(listener);
//    }
//
//    public MasterSceneCollectionManager getMasterSceneCollectionManager() {
//        return lightingManager.getMasterSceneCollectionManager();
//    }
//
//    public void addControllerListener(ControllerListener listener) {
//        getControllerManager().addListener(listener);
//    }
//
//    public ControllerManager getControllerManager() {
//        return lightingManager.getControllerManager();
//    }
//
//    public LampManager getLampManager() {
//        return AllJoynManager.lampManager;
//    }
//
//    public SceneManager getSceneManager() {
//        return AllJoynManager.sceneManager;
//    }
//
//    public boolean isLampExpired(String lampID) {
//        //TODO-IMPL
//        return false;
//    }
}
