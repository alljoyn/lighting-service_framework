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

import java.util.Iterator;

import org.allseen.lsf.helper.facade.Scene;
import org.allseen.lsf.helper.listener.LightingItemErrorEvent;
import org.allseen.lsf.helper.listener.SceneCollectionListener;
import org.allseen.lsf.helper.model.SceneDataModel;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class SceneCollectionManager extends LightingItemCollectionManager<Scene, SceneCollectionListener, SceneDataModel> {

    public SceneCollectionManager(LightingSystemManager director) {
        super(director);
    }

    public Scene addScene(String sceneID) {
        return addScene(sceneID, new Scene(sceneID));
    }

    public Scene addScene(String sceneID, Scene scene) {
        return itemAdapters.put(sceneID, scene);
    }

    public Scene getScene(String sceneID) {
        return getAdapter(sceneID);
    }

    public Scene[] getScenes() {
        return getAdapters().toArray(new Scene[size()]);
    }

    public Iterator<Scene> getPresetIterator() {
        return getAdapters().iterator();
    }

    public Scene removeScene(String sceneID) {
        return removeAdapter(sceneID);
    }

    @Override
    protected void sendChangedEvent(SceneCollectionListener listener, Iterator<Scene> scenes, int count) {
        listener.onScenesChanged(scenes, count);
    }

    @Override
    protected void sendRemovedEvent(SceneCollectionListener listener, Iterator<Scene> scenes, int count) {
        listener.onScenesRemoved(scenes, count);
    }

    @Override
    protected void sendErrorEvent(SceneCollectionListener listener, LightingItemErrorEvent errorEvent) {
        listener.onSceneError(errorEvent);
    }

    @Override
    public SceneDataModel getModel(String sceneID) {
        Scene scene = getAdapter(sceneID);

        return scene != null ? scene.getSceneDataModel() : null;
    }
}
