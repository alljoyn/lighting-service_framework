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
package org.alljoyn.lsf;

public class SceneManager extends BaseNativeClassWrapper {
    public SceneManager(ControllerClient controller, SceneManagerCallback callback) {
         createNativeObject(controller, callback);
    }

    public native ControllerClientStatus getAllSceneIDs();
    public native ControllerClientStatus getSceneName(String sceneID, String language);
    public native ControllerClientStatus setSceneName(String sceneID, String sceneName, String language);
    public native ControllerClientStatus createScene(Scene scene, String sceneName, String language);
    public native ControllerClientStatus updateScene(String sceneID, Scene scene);
    public native ControllerClientStatus deleteScene(String sceneID);
    public native ControllerClientStatus getScene(String sceneID);
    public native ControllerClientStatus applyScene(String sceneID);

    protected native void createNativeObject(ControllerClient controller, SceneManagerCallback callback);

    @Override
    protected native void destroyNativeObject();
}
