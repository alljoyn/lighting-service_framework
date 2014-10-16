/* Copyright (c) 2014, AllSeen Alliance. All rights reserved.
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

import org.allseen.lsf.helper.manager.AllJoynManager;
import org.allseen.lsf.helper.model.LightingItemDataModel;
import org.allseen.lsf.helper.model.SceneDataModel;

/**
 * A Scene object represents a set of lamps and associated states in a lighting system, and can be
 * used to apply the states to the lamps.
 */
public final class Scene extends LightingItem {

    protected SceneDataModel sceneModel;

    /**
     * Constructs a Scene using the specified ID.
     * <p>
     * <b>WARNING: This method is intended to be used internally. Client software should not instantiate
     * Scenes directly, but should instead get them from the {@link LightingDirector} using the
     * {@link LightingDirector#getScenes()} method.</b>
     *
     * @param sceneID The ID of the scene
     */
    public Scene(String sceneID) {
        super();

        sceneModel = new SceneDataModel(sceneID);
    }

    /**
     * Sends a command to apply this Scene, which sets the appropriate state for all
     * constituent lamps.
     */
    public void apply() {
        AllJoynManager.sceneManager.applyScene(sceneModel.id);
    }

    @Override
    protected LightingItemDataModel getItemDataModel() {
        return getSceneDataModel();
    }

    /**
     * <b>WARNING: This method is not intended to be used by clients, and may change or be
     * removed in subsequent releases of the SDK.</b>
     */
    public SceneDataModel getSceneDataModel() {
        return sceneModel;
    }
}
