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
package org.allseen.lsf;

public class MasterSceneManager extends BaseNativeClassWrapper {
    public MasterSceneManager(ControllerClient controller, MasterSceneManagerCallback callback) {
         createNativeObject(controller, callback);
    }

    public native ControllerClientStatus getAllMasterSceneIDs();
    public native ControllerClientStatus getMasterSceneName(String masterSceneID, String language);
    public native ControllerClientStatus setMasterSceneName(String masterSceneID, String masterSceneName, String language);
    public native ControllerClientStatus createMasterScene(MasterScene masterScene, String masterSceneName, String language);
    public native ControllerClientStatus updateMasterScene(String masterSceneID, MasterScene masterScene);
    public native ControllerClientStatus getMasterScene(String masterSceneID);
    public native ControllerClientStatus deleteMasterScene(String masterSceneID);
    public native ControllerClientStatus applyMasterScene(String masterSceneID);

    protected native void createNativeObject(ControllerClient controller, MasterSceneManagerCallback callback);

    @Override
    protected native void destroyNativeObject();
}
