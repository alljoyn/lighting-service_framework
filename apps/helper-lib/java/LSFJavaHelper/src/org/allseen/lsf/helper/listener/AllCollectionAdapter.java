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
package org.allseen.lsf.helper.listener;

import java.util.Iterator;

import org.allseen.lsf.helper.facade.Group;
import org.allseen.lsf.helper.facade.Lamp;
import org.allseen.lsf.helper.facade.MasterScene;
import org.allseen.lsf.helper.facade.Preset;
import org.allseen.lsf.helper.facade.Scene;
import org.allseen.lsf.helper.model.ControllerDataModel;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class AllCollectionAdapter implements AllCollectionListener {

    @Override
    public void onLampsChanged(Iterator<Lamp> lamps, int count)                         { }

    @Override
    public void onLampsRemoved(Iterator<Lamp> lamps, int count)                         { }

    @Override
    public void onLampError(LightingItemErrorEvent error)                               { }

    @Override
    public void onGroupsChanged(Iterator<Group> groups, int count)                      { }

    @Override
    public void onGroupsRemoved(Iterator<Group> groups, int count)                      { }

    @Override
    public void onGroupError(LightingItemErrorEvent error)                              { }

    @Override
    public void onPresetsChanged(Iterator<Preset> presets, int count)                   { }

    @Override
    public void onPresetsRemoved(Iterator<Preset> presets, int count)                   { }

    @Override
    public void onPresetError(LightingItemErrorEvent error)                             { }

    @Override
    public void onScenesChanged(Iterator<Scene> scenes, int count)                      { }

    @Override
    public void onScenesRemoved(Iterator<Scene> scenes, int count)                      { }

    @Override
    public void onSceneError(LightingItemErrorEvent error)                              { }

    @Override
    public void onMasterScenesChanged(Iterator<MasterScene> masterScenes, int count)    { }

    @Override
    public void onMasterScenesRemoved(Iterator<MasterScene> masterScenes, int count)    { }

    @Override
    public void onMasterSceneError(LightingItemErrorEvent error)                        { }

    @Override
    public void onLeaderModelChange(ControllerDataModel leadModel) { }

    @Override
    public void onControllerErrors(ControllerErrorEvent errorEvent) { }

    @Override
    public void onWifiConnected(WifiEvent wifiEvent) { }

    @Override
    public void onWifiDisconnected(WifiEvent wifiEvent) { }
}
