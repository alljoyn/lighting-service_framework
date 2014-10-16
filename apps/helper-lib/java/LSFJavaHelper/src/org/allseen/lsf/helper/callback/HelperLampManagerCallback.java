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

import org.alljoyn.bus.Variant;
import org.allseen.lsf.LampDetails;
import org.allseen.lsf.LampManager;
import org.allseen.lsf.LampManagerCallback;
import org.allseen.lsf.LampParameters;
import org.allseen.lsf.LampState;
import org.allseen.lsf.ResponseCode;
import org.allseen.lsf.helper.facade.Lamp;
import org.allseen.lsf.helper.manager.LightingSystemManager;
import org.allseen.lsf.helper.model.LampAbout;
import org.allseen.lsf.helper.model.LampDataModel;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class HelperLampManagerCallback extends LampManagerCallback {
    private static final int RETRY_DELAY = 500;

    protected LightingSystemManager director;

    public HelperLampManagerCallback(LightingSystemManager director) {
        super();

        this.director = director;
    }

    @Override
    public void getAllLampIDsReplyCB(ResponseCode responseCode, String[] lampIDs) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getLampCollectionManager().sendErrorEvent("getAllLampIDsReplyCB", responseCode);
        }

        // Process lamp IDs regardless of response code
        for (String lampID : lampIDs) {
            postUpdateLampID(lampID);
        }
    }

    @Override
    public void getLampNameReplyCB(ResponseCode responseCode, String lampID, String language, String lampName) {
        if (responseCode.equals(ResponseCode.OK)) {
            postUpdateLampName(lampID, lampName);
        } else {
            postGetLampName(lampID, RETRY_DELAY);
            director.getLampCollectionManager().sendErrorEvent("getLampNameReplyCB", responseCode, lampID);
        }
    }

    @Override
    public void setLampNameReplyCB(ResponseCode responseCode, String lampID, String language) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getLampCollectionManager().sendErrorEvent("setLampNameReplyCB", responseCode, lampID);
        }

        // Read back name regardless of response code
        director.getLampManager().getLampName(lampID, LightingSystemManager.LANGUAGE);
    }

    @Override
    public void lampNameChangedCB(String lampID, String lampName) {
        postUpdateLampName(lampID, lampName);
    }

    @Override
    public void lampsFoundCB(String[] lampIDs) {
        for (String lampID : lampIDs) {
            postUpdateLampID(lampID);
        }
    }

    @Override
    public void lampsLostCB(String[] lampIDs) {
        //TODO-IMPL
    }

    @Override
    public void getLampDetailsReplyCB(ResponseCode responseCode, String lampID, LampDetails lampDetails) {
        if (responseCode.equals(ResponseCode.OK)) {
            postUpdateLampDetails(lampID, lampDetails);
        } else {
            postGetLampDetails(lampID, RETRY_DELAY);
            director.getLampCollectionManager().sendErrorEvent("getLampDetailsReplyCB", responseCode, lampID);
        }
    }

    @Override
    public void getLampParametersReplyCB(ResponseCode responseCode, String lampID, LampParameters lampParameters) {
        if (responseCode.equals(ResponseCode.OK)) {
            postUpdateLampParameters(lampID, lampParameters);
        } else {
            postGetLampParameters(lampID, RETRY_DELAY);
            director.getLampCollectionManager().sendErrorEvent("getLampParametersReplyCB", responseCode, lampID);
        }
    }

    @Override
    public void getLampStateReplyCB(ResponseCode responseCode, String lampID, LampState lampState) {
        if (responseCode.equals(ResponseCode.OK)) {
            postUpdateLampState(lampID, lampState);
            postGetLampParameters(lampID, 0);
        } else {
            postGetLampState(lampID, RETRY_DELAY);
            director.getLampCollectionManager().sendErrorEvent("getLampStateReplyCB", responseCode, lampID);
        }
    }

    @Override
    public void getLampStateOnOffFieldReplyCB(ResponseCode responseCode, String lampID, boolean onOff) {
        if (responseCode.equals(ResponseCode.OK)) {
            postUpdateLampStateOnOff(lampID, onOff);
            postGetLampParameters(lampID, 0);
        } else {
            postGetLampStateOnOffField(lampID, RETRY_DELAY);
            director.getLampCollectionManager().sendErrorEvent("getLampStateOnOffFieldReplyCB", responseCode, lampID);
        }
    }

    @Override
    public void getLampStateHueFieldReplyCB(ResponseCode responseCode, String lampID, long hue) {
        if (responseCode.equals(ResponseCode.OK)) {
            postUpdateLampStateHue(lampID, hue);
        } else {
            postGetLampStateHueField(lampID, RETRY_DELAY);
            director.getLampCollectionManager().sendErrorEvent("getLampStateHueFieldReplyCB", responseCode, lampID);
        }
    }

    @Override
    public void getLampStateSaturationFieldReplyCB(ResponseCode responseCode, String lampID, long saturation) {
        if (responseCode.equals(ResponseCode.OK)) {
            postUpdateLampStateSaturation(lampID, saturation);
        } else {
            postGetLampStateSaturationField(lampID, RETRY_DELAY);
            director.getLampCollectionManager().sendErrorEvent("getLampStateSaturationFieldReplyCB", responseCode, lampID);
        }
    }

    @Override
    public void getLampStateBrightnessFieldReplyCB(ResponseCode responseCode, String lampID, long brightness) {
        if (responseCode.equals(ResponseCode.OK)) {
            postUpdateLampStateBrightness(lampID, brightness);
            postGetLampParameters(lampID, 0);
        } else {
            postGetLampStateBrightnessField(lampID, RETRY_DELAY);
            director.getLampCollectionManager().sendErrorEvent("getLampStateBrightnessFieldReplyCB", responseCode, lampID);
        }
    }

    @Override
    public void getLampStateColorTempFieldReplyCB(ResponseCode responseCode, String lampID, long colorTemp) {
        if (responseCode.equals(ResponseCode.OK)) {
            postUpdateLampStateColorTemp(lampID, colorTemp);
        } else {
            postGetLampStateColorTempField(lampID, RETRY_DELAY);
            director.getLampCollectionManager().sendErrorEvent("getLampStateColorTempFieldReplyCB", responseCode, lampID);
        }
    }

    @Override
    public void lampStateChangedCB(String lampID, LampState lampState) {
        postUpdateLampState(lampID, lampState);
        postGetLampParameters(lampID, 0);
    }

    @Override
    public void transitionLampStateOnOffFieldReplyCB(ResponseCode responseCode, String lampID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getLampCollectionManager().sendErrorEvent("transitionLampStateOnOffFieldReplyCB", responseCode, lampID);
        }

        // Read back field value regardless of response code
        director.getLampManager().getLampStateOnOffField(lampID);
    }

    @Override
    public void transitionLampStateHueFieldReplyCB(ResponseCode responseCode, String lampID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getLampCollectionManager().sendErrorEvent("transitionLampStateHueFieldReplyCB", responseCode, lampID);
        }

        // Read back field value regardless of response code
        director.getLampManager().getLampStateHueField(lampID);
    }

    @Override
    public void transitionLampStateSaturationFieldReplyCB(ResponseCode responseCode, String lampID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getLampCollectionManager().sendErrorEvent("transitionLampStateSaturationFieldReplyCB", responseCode, lampID);
        }

        // Read back field value regardless of response code
        director.getLampManager().getLampStateSaturationField(lampID);
    }

    @Override
    public void transitionLampStateBrightnessFieldReplyCB(ResponseCode responseCode, String lampID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getLampCollectionManager().sendErrorEvent("transitionLampStateBrightnessFieldReplyCB", responseCode, lampID);
        }

        // Read back field value regardless of response code
        director.getLampManager().getLampStateBrightnessField(lampID);
    }

    @Override
    public void transitionLampStateColorTempFieldReplyCB(ResponseCode responseCode, String lampID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            director.getLampCollectionManager().sendErrorEvent("transitionLampStateColorTempFieldReplyCB", responseCode, lampID);
        }

        // Read back field value regardless of response code
        director.getLampManager().getLampStateColorTempField(lampID);
    }

    protected void postUpdateLampID(String lampID) {
        postUpdateLampID(lampID, null, null, 0);
    }

    public void postUpdateLampID(final String lampID, final Map<String, Variant> announcedData, final Map<String, Object> queriedData, int delay) {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                Lamp lamp = director.getLampCollectionManager().getLamp(lampID);

                if (lamp == null) {
                    lamp = director.getLampCollectionManager().addLamp(lampID);
                }

                LampDataModel lampModel = lamp.getLampDataModel();

                if (LampDataModel.defaultName.equals(lampModel.getName())) {
                    LampManager lampManager = director.getLampManager();

                    lampManager.getLampName(lampID, LightingSystemManager.LANGUAGE);
                    lampManager.getLampState(lampID);
                    lampManager.getLampParameters(lampID);
                    lampManager.getLampDetails(lampID);
                }

                LampAbout lampAbout = lampModel.getAbout();

                lampAbout.setAnnouncedData(announcedData);
                lampAbout.setQueriedData(queriedData);

                // update the timestamp
                lampModel.updateTime();
            }
        }, delay);

        postSendLampChanged(lampID);
    }

    protected void postGetLampName(final String lampID, int delay) {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!director.isLampExpired(lampID)) {
                    director.getLampManager().getLampName(lampID, LightingSystemManager.LANGUAGE);
                }
            }
        }, delay);
    }

    protected void postUpdateLampState(final String lampID, final LampState lampState) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = director.getLampCollectionManager().getModel(lampID);

                if (lampModel != null) {
                    lampModel.state = lampState;
                }
            }
        });

        postSendLampChanged(lampID);
    }

    protected void postGetLampState(final String lampID, int delay) {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!director.isLampExpired(lampID)) {
                    director.getLampManager().getLampState(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampParameters(final String lampID, final LampParameters lampParams) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = director.getLampCollectionManager().getModel(lampID);

                if (lampModel != null) {
                    lampModel.setParameters(lampParams);
                }
            }
        });

        postSendLampChanged(lampID);
    }

    protected void postGetLampParameters(final String lampID, int delay) {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!director.isLampExpired(lampID)) {
                    director.getLampManager().getLampParameters(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampDetails(final String lampID, final LampDetails lampDetails) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = director.getLampCollectionManager().getModel(lampID);

                if (lampModel != null) {
                    lampModel.setDetails(lampDetails);
                }
            }
        });

        postSendLampChanged(lampID);
    }

    protected void postGetLampDetails(final String lampID, final int delay) {
        if (!director.isLampExpired(lampID)) {
            director.getHandler().postDelayed(new Runnable() {
                @Override
                public void run() {
                    director.getLampManager().getLampDetails(lampID);
                }
            }, delay);
        }
    }

    protected void postUpdateLampName(final String lampID, final String lampName) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = director.getLampCollectionManager().getModel(lampID);

                if (lampModel != null) {
                    lampModel.setName(lampName);
                }
            }
        });

        postSendLampChanged(lampID);
    }

    protected void postUpdateLampStateOnOff(final String lampID, final boolean onOff) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = director.getLampCollectionManager().getModel(lampID);

                if (lampModel != null) {
                    lampModel.state.setOnOff(onOff);
                }
            }
        });

        postSendLampChanged(lampID);
    }

    protected void postGetLampStateOnOffField(final String lampID, int delay) {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!director.isLampExpired(lampID)) {
                    director.getLampManager().getLampStateOnOffField(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampStateHue(final String lampID, final long hue) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = director.getLampCollectionManager().getModel(lampID);

                if (lampModel != null) {
                    lampModel.state.setHue(hue);
                }
            }
        });

        postSendLampChanged(lampID);
    }

    protected void postGetLampStateHueField(final String lampID, int delay) {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!director.isLampExpired(lampID)) {
                    director.getLampManager().getLampStateHueField(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampStateSaturation(final String lampID, final long saturation) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = director.getLampCollectionManager().getModel(lampID);

                if (lampModel != null) {
                    lampModel.state.setSaturation(saturation);
                }
            }
        });

        postSendLampChanged(lampID);
    }

    protected void postGetLampStateSaturationField(final String lampID, int delay) {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!director.isLampExpired(lampID)) {
                    director.getLampManager().getLampStateSaturationField(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampStateBrightness(final String lampID, final long brightness) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = director.getLampCollectionManager().getModel(lampID);

                if (lampModel != null) {
                    lampModel.state.setBrightness(brightness);
                }
            }
        });

        postSendLampChanged(lampID);
    }

    protected void postGetLampStateBrightnessField(final String lampID, int delay) {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!director.isLampExpired(lampID)) {
                    director.getLampManager().getLampStateBrightnessField(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampStateColorTemp(final String lampID, final long colorTemp) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = director.getLampCollectionManager().getModel(lampID);

                if (lampModel != null) {
                    lampModel.state.setColorTemp(colorTemp);
                }
            }
        });

        postSendLampChanged(lampID);
    }

    protected void postGetLampStateColorTempField(final String lampID, int delay) {
        director.getHandler().postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!director.isLampExpired(lampID)) {
                    director.getLampManager().getLampStateColorTempField(lampID);
                }
            }
        }, delay);
    }

    protected void postSendLampChanged(final String lampID) {
        director.getHandler().post(new Runnable() {
            @Override
            public void run() {
                director.getLampCollectionManager().sendChangedEvent(lampID);
            }
        });
    }
}
