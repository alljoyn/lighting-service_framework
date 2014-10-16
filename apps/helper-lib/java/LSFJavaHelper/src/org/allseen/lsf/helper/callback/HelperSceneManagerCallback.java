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

import org.allseen.lsf.ResponseCode;
import org.allseen.lsf.Scene;
import org.allseen.lsf.SceneManagerCallback;
import org.allseen.lsf.helper.manager.AllJoynManager;
import org.allseen.lsf.helper.manager.LightingSystemManager;
import org.allseen.lsf.helper.model.SceneDataModel;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class HelperSceneManagerCallback extends SceneManagerCallback {
    protected LightingSystemManager director;

    public HelperSceneManagerCallback(LightingSystemManager director) {
        super();

        this.director = director;
    }

    @Override
    public void getAllSceneIDsReplyCB(ResponseCode responseCode, String[] sceneIDs) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getSceneCollectionManager().sendErrorEvent("getAllSceneIDsReplyCB", responseCode, null);
        }

        for (final String sceneID : sceneIDs) {
            postProcessSceneID(sceneID);
        }
    }

    @Override
    public void getSceneNameReplyCB(ResponseCode responseCode, String sceneID, String language, String sceneName) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getSceneCollectionManager().sendErrorEvent("getSceneNameReplyCB", responseCode, sceneID);
        }

        postUpdateSceneName(sceneID, sceneName);
    }

    @Override
    public void setSceneNameReplyCB(ResponseCode responseCode, String sceneID, String language) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getSceneCollectionManager().sendErrorEvent("setSceneNameReplyCB", responseCode, sceneID);
        }

        AllJoynManager.sceneManager.getSceneName(sceneID, LightingSystemManager.LANGUAGE);
    }

    @Override
    public void scenesNameChangedCB(final String[] sceneIDs) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                boolean containsNewIDs = false;

                for (final String sceneID : sceneIDs) {
                    if (director.getSceneCollectionManager().hasID(sceneID)) {
                        AllJoynManager.sceneManager.getSceneName(sceneID, LightingSystemManager.LANGUAGE);
                    } else {
                        containsNewIDs = true;
                    }
                }

                if (containsNewIDs) {
                    AllJoynManager.sceneManager.getAllSceneIDs();
                }
            }
        });
    }

    @Override
    public void createSceneReplyCB(ResponseCode responseCode, String sceneID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getSceneCollectionManager().sendErrorEvent("createSceneReplyCB", responseCode, sceneID);
        }

        postProcessSceneID(sceneID);
    }

    @Override
    public void scenesCreatedCB(String[] sceneIDs) {
        AllJoynManager.sceneManager.getAllSceneIDs();
    }

    @Override
    public void updateSceneReplyCB(ResponseCode responseCode, String sceneID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getSceneCollectionManager().sendErrorEvent("updateSceneReplyCB", responseCode, sceneID);
        }
    }

    @Override
    public void scenesUpdatedCB(String[] sceneIDs) {
        for (String sceneID : sceneIDs) {
            AllJoynManager.sceneManager.getScene(sceneID);
        }
    }

    @Override
    public void deleteSceneReplyCB(ResponseCode responseCode, String sceneID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getSceneCollectionManager().sendErrorEvent("deleteSceneReplyCB", responseCode, sceneID);
        }
    }

    @Override
    public void scenesDeletedCB(String[] sceneIDs) {
        postDeleteScenes(sceneIDs);
    }

    @Override
    public void getSceneReplyCB(ResponseCode responseCode, String sceneID, Scene scene) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getSceneCollectionManager().sendErrorEvent("getSceneReplyCB", responseCode, sceneID);
        }

        postUpdateScene(sceneID, scene);
    }

    @Override
    public void applySceneReplyCB(ResponseCode responseCode, String sceneID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getSceneCollectionManager().sendErrorEvent("applySceneReplyCB", responseCode, sceneID);
        }

        //TODO-CHK Do we need to do anything here?
    }

    @Override
    public void scenesAppliedCB(String[] sceneIDs) {
        //TODO-CHK Do we need to do anything here?
    }

    protected void postProcessSceneID(final String sceneID) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                if (!director.getSceneCollectionManager().hasID(sceneID)) {
                    postUpdateSceneID(sceneID);
                    AllJoynManager.sceneManager.getSceneName(sceneID, LightingSystemManager.LANGUAGE);
                    AllJoynManager.sceneManager.getScene(sceneID);
                }
            }
        });
    }

    protected void postUpdateSceneID(final String sceneID) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                if (!director.getSceneCollectionManager().hasID(sceneID)) {
                    director.getSceneCollectionManager().addScene(sceneID);
                }
            }
        });

        postSendSceneChanged(sceneID);
    }

    protected void postUpdateScene(final String sceneID, final Scene scene) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                SceneDataModel basicSceneModel = director.getSceneCollectionManager().getModel(sceneID);

                if (basicSceneModel != null) {
                    basicSceneModel.fromScene(scene);
                }
            }
        });

        postSendSceneChanged(sceneID);
    }

    protected void postUpdateSceneName(final String sceneID, final String sceneName) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                SceneDataModel basicSceneModel = director.getSceneCollectionManager().getModel(sceneID);

                if (basicSceneModel != null) {
                    basicSceneModel.setName(sceneName);
                }
            }
        });

        postSendSceneChanged(sceneID);
    }

    protected void postDeleteScenes(final String[] sceneIDs) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                for (String sceneID : sceneIDs) {
                    director.getSceneCollectionManager().removeScene(sceneID);
                }
            }
        });
    }

    protected void postSendSceneChanged(final String sceneID) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                director.getSceneCollectionManager().sendChangedEvent(sceneID);
            }
        });
    }
}
