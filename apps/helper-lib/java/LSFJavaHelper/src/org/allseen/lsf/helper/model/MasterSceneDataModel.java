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
package org.allseen.lsf.helper.model;

import org.allseen.lsf.MasterScene;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class MasterSceneDataModel extends LightingItemDataModel {
    public static final char TAG_PREFIX_MASTER_SCENE = 'M';

    public static String defaultName = "<Loading master scene info...>";

    public MasterScene masterScene;

    public MasterSceneDataModel() {
        this("", null);
    }

    public MasterSceneDataModel(String masterSceneID) {
        this(masterSceneID, null);
    }

    public MasterSceneDataModel(String masterSceneID, String masterSceneName) {
        super(masterSceneID, TAG_PREFIX_MASTER_SCENE, masterSceneName != null ? masterSceneName : defaultName);

        masterScene = new MasterScene();
    }

    public MasterSceneDataModel(MasterSceneDataModel other) {
        super(other);

        this.masterScene = new MasterScene(other.masterScene);
    }
}
