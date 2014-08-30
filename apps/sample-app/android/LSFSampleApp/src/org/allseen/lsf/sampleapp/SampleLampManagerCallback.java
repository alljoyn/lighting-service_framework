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
package org.allseen.lsf.sampleapp;

import java.util.Map;

import org.alljoyn.bus.Variant;
import org.allseen.lsf.LampDetails;
import org.allseen.lsf.LampManagerCallback;
import org.allseen.lsf.LampParameters;
import org.allseen.lsf.LampState;
import org.allseen.lsf.ResponseCode;

import android.os.Handler;
import android.os.SystemClock;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.util.Log;

public class SampleLampManagerCallback extends LampManagerCallback {
    private static final int RETRY_DELAY = 500;

    protected SampleAppActivity activity;
    protected FragmentManager fragmentManager;
    protected Handler handler;

    public SampleLampManagerCallback(SampleAppActivity activity, FragmentManager fragmentManager, Handler handler) {
        super();

        this.activity = activity;
        this.fragmentManager = fragmentManager;
        this.handler = handler;
    }

    @Override
    public void getAllLampIDsReplyCB(ResponseCode responseCode, String[] lampIDs) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "getAllLampIDsReplyCB");
        }

        Log.d(SampleAppActivity.TAG, "---------------------------");
        Log.d(SampleAppActivity.TAG, "getAllLampIDsReplyCB() count:" + lampIDs.length);

        // Process lamp IDs regardless of response code
        for (String lampID : lampIDs) {
            Log.d(SampleAppActivity.TAG, "Got new lamp ID reply " + lampID);

            postUpdateLampID(lampID);
        }
    }

    @Override
    public void getLampNameReplyCB(ResponseCode responseCode, final String lampID, String language, String lampName) {
        if (responseCode.equals(ResponseCode.OK)) {
            Log.d(SampleAppActivity.TAG, "Got lamp name reply " + lampID + ": " +  lampName);
            postUpdateLampName(lampID, lampName);
        } else {
            postGetLampName(lampID, RETRY_DELAY);
            activity.showErrorResponseCode(responseCode, "getLampNameReplyCB");
       }
    }

    @Override
    public void setLampNameReplyCB(ResponseCode responseCode, String lampID, String language) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "setLampNameReplyCB");
        }

        // Read back name regardless of response code
        Log.d(SampleAppActivity.TAG, "setLampNameReplyCB(): " + lampID);
        AllJoynManager.lampManager.getLampName(lampID, SampleAppActivity.LANGUAGE);
    }

    @Override
    public void lampsNameChangedCB(String[] lampIDs) {
        for (String lampID : lampIDs) {
            Log.d(SampleAppActivity.TAG, "lampsNameChangedCB() " + lampID);
            AllJoynManager.lampManager.getLampName(lampID, SampleAppActivity.LANGUAGE);
        }
    }

    @Override
    public void getLampDetailsReplyCB(ResponseCode responseCode, String lampID, LampDetails lampDetails) {
        if (responseCode.equals(ResponseCode.OK)) {
            Log.d(SampleAppActivity.TAG, "Got lamp details reply " + lampID + ": " +  lampDetails);
            postUpdateLampDetails(lampID, lampDetails);
        } else {
            postGetLampDetails(lampID, RETRY_DELAY);
            activity.showErrorResponseCode(responseCode, "getLampDetailsReplyCB");
        }
    }

    @Override
    public void getLampParametersReplyCB(ResponseCode responseCode, String lampID, LampParameters lampParameters) {
        if (responseCode.equals(ResponseCode.OK)) {
            Log.d(SampleAppActivity.TAG, "Got lamp parameters reply " + lampID + ": " +  lampParameters);
            postUpdateLampParameters(lampID, lampParameters);
        } else {
            postGetLampParameters(lampID, RETRY_DELAY);
            activity.showErrorResponseCode(responseCode, "getLampParametersReplyCB");
        }
    }

    @Override
    public void getLampStateReplyCB(ResponseCode responseCode, String lampID, LampState lampState) {
        if (responseCode.equals(ResponseCode.OK)) {
            Log.d(SampleAppActivity.TAG, "Got lamp state reply " + lampID + ": " +  lampState);
            postUpdateLampState(lampID, lampState);
            postGetLampParameters(lampID, 0);
        } else {
            postGetLampState(lampID, RETRY_DELAY);
            activity.showErrorResponseCode(responseCode, "getLampStateReplyCB");
        }
    }

    @Override
    public void getLampStateOnOffFieldReplyCB(ResponseCode responseCode, String lampID, boolean onOff) {
        if (responseCode.equals(ResponseCode.OK)) {
            Log.d(SampleAppActivity.TAG, "Got on/off reply " + lampID + ": " +  onOff);
            postUpdateLampStateOnOff(lampID, onOff);
            postGetLampParameters(lampID, 0);
        } else {
            postGetLampStateOnOffField(lampID, RETRY_DELAY);
            activity.showErrorResponseCode(responseCode, "getLampStateOnOffFieldReplyCB");
        }
    }

    @Override
    public void getLampStateHueFieldReplyCB(ResponseCode responseCode, String lampID, long hue) {
        if (responseCode.equals(ResponseCode.OK)) {
            Log.d(SampleAppActivity.TAG, "Got hue reply " + lampID + ": " +  hue);
            postUpdateLampStateHue(lampID, hue);
        } else {
            postGetLampStateHueField(lampID, RETRY_DELAY);
            activity.showErrorResponseCode(responseCode, "getLampStateHueFieldReplyCB");
        }
    }

    @Override
    public void getLampStateSaturationFieldReplyCB(ResponseCode responseCode, String lampID, long saturation) {
        if (responseCode.equals(ResponseCode.OK)) {
            Log.d(SampleAppActivity.TAG, "Got saturation reply " + lampID + ": " +  saturation);
            postUpdateLampStateSaturation(lampID, saturation);
        } else {
            postGetLampStateSaturationField(lampID, RETRY_DELAY);
            activity.showErrorResponseCode(responseCode, "getLampStateSaturationFieldReplyCB");
        }
    }

    @Override
    public void getLampStateBrightnessFieldReplyCB(ResponseCode responseCode, String lampID, long brightness) {
        if (responseCode.equals(ResponseCode.OK)) {
            Log.d(SampleAppActivity.TAG, "Got brightness reply " + lampID + ": " +  brightness);
            postUpdateLampStateBrightness(lampID, brightness);
            postGetLampParameters(lampID, 0);
        } else {
            postGetLampStateBrightnessField(lampID, RETRY_DELAY);
            activity.showErrorResponseCode(responseCode, "getLampStateBrightnessFieldReplyCB");
        }
    }

    @Override
    public void getLampStateColorTempFieldReplyCB(ResponseCode responseCode, String lampID, long colorTemp) {
        if (responseCode.equals(ResponseCode.OK)) {
            Log.d(SampleAppActivity.TAG, "Got color temp reply " + lampID + ": " +  colorTemp);
            postUpdateLampStateColorTemp(lampID, colorTemp);
        } else {
            postGetLampStateColorTempField(lampID, RETRY_DELAY);
            activity.showErrorResponseCode(responseCode, "getLampStateColorTempFieldReplyCB");
        }
    }

    @Override
    public void lampsStateChangedCB(String[] lampIDs) {
        for (String lampID : lampIDs) {
            Log.d(SampleAppActivity.TAG, "lampsStateChangedCB()" + lampID);
            AllJoynManager.lampManager.getLampState(lampID);
        }
    }

    @Override
    public void transitionLampStateOnOffFieldReplyCB(ResponseCode responseCode, String lampID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampStateOnOffFieldReplyCB");
        }

        // Read back field value regardless of response code
        AllJoynManager.lampManager.getLampStateOnOffField(lampID);
    }

    @Override
    public void transitionLampStateHueFieldReplyCB(ResponseCode responseCode, String lampID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampStateHueFieldReplyCB");
        }

        // Read back field value regardless of response code
        AllJoynManager.lampManager.getLampStateHueField(lampID);
    }

    @Override
    public void transitionLampStateSaturationFieldReplyCB(ResponseCode responseCode, String lampID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampStateSaturationFieldReplyCB");
        }

        // Read back field value regardless of response code
        AllJoynManager.lampManager.getLampStateSaturationField(lampID);
    }

    @Override
    public void transitionLampStateBrightnessFieldReplyCB(ResponseCode responseCode, String lampID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampStateBrightnessFieldReplyCB");
        }

        // Read back field value regardless of response code
        AllJoynManager.lampManager.getLampStateBrightnessField(lampID);
    }

    @Override
    public void transitionLampStateColorTempFieldReplyCB(ResponseCode responseCode, String lampID) {
        if (!responseCode.equals(ResponseCode.OK)) {
            activity.showErrorResponseCode(responseCode, "transitionLampStateColorTempFieldReplyCB");
        }

        // Read back field value regardless of response code
        AllJoynManager.lampManager.getLampStateColorTempField(lampID);
    }

    protected void postUpdateLampID(final String lampID) {
        postUpdateLampID(lampID, null, null, 0);
    }

    public void postUpdateLampID(final String lampID, final Map<String, Variant> announceData, final Map<String, Object> aboutData, int delay) {
        Log.d(SampleAppActivity.TAG, "Updating ID " + lampID);
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = activity.lampModels.get(lampID);

                Log.d(SampleAppActivity.TAG, "lookup lamp model " + lampModel);
                if (lampModel == null) {
                    lampModel = new LampDataModel(lampID);
                    activity.lampModels.put(lampID, lampModel);
                }

                if (LampDataModel.defaultName.equals(lampModel.name)) {
                    Log.d(SampleAppActivity.TAG, "Getting data set for " + lampID);
                    AllJoynManager.lampManager.getLampName(lampID, SampleAppActivity.LANGUAGE);
                    AllJoynManager.lampManager.getLampState(lampID);
                    AllJoynManager.lampManager.getLampParameters(lampID);
                    AllJoynManager.lampManager.getLampDetails(lampID);
                }

                if (announceData != null) {
                    Log.d(SampleAppActivity.TAG, "Setting about data for lamp " + lampID);
                    lampModel.setAboutData(announceData, aboutData);
                }

                // update the timestamp
                lampModel.timestamp = SystemClock.elapsedRealtime();
            }
        }, delay);

        postUpdateDimmableItemTask(lampID);
    }

    protected void postGetLampName(final String lampID, int delay) {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!activity.garbageCollector.isLampExpired(lampID)) {
                    AllJoynManager.lampManager.getLampName(lampID, SampleAppActivity.LANGUAGE);
                }
            }
        }, delay);
    }

    protected void postUpdateLampState(final String lampID, final LampState lampState) {
        Log.d(SampleAppActivity.TAG, "Updating lamp state " + lampID + ": " +  lampState);
        handler.post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = activity.lampModels.get(lampID);

                if (lampModel != null) {
                    lampModel.state = lampState;
                }
            }
        });

        postUpdateDimmableItemTask(lampID);
    }

    protected void postGetLampState(final String lampID, int delay) {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!activity.garbageCollector.isLampExpired(lampID)) {
                    AllJoynManager.lampManager.getLampState(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampParameters(final String lampID, final LampParameters lampParams) {
        Log.d(SampleAppActivity.TAG, "Updating lamp params " + lampID + ": " +  lampParams);
        handler.post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = activity.lampModels.get(lampID);

                if (lampModel != null) {
                    lampModel.setParameters(lampParams);
                }
            }
        });

        postUpdateDimmableItemTask(lampID);
    }

    protected void postGetLampParameters(final String lampID, int delay) {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!activity.garbageCollector.isLampExpired(lampID)) {
                    AllJoynManager.lampManager.getLampParameters(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampDetails(final String lampID, final LampDetails lampDetails) {
        Log.d(SampleAppActivity.TAG, "Updating lamp details " + lampID + ": " +  lampDetails);
        handler.post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = activity.lampModels.get(lampID);

                if (lampModel != null) {
                    lampModel.setDetails(lampDetails);
                }
            }
        });

        postUpdateDimmableItemTask(lampID);
    }

    protected void postGetLampDetails(final String lampID, final int delay) {
        if (!activity.garbageCollector.isLampExpired(lampID)) {
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    AllJoynManager.lampManager.getLampDetails(lampID);
                }
            }, delay);
        }
    }

    protected void postUpdateLampName(final String lampID, final String lampName) {
        Log.d(SampleAppActivity.TAG, "Updating lamp name " + lampID + ": " +  lampName);
        handler.post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = activity.lampModels.get(lampID);

                if (lampModel != null) {
                    lampModel.name = lampName;
                }
            }
        });

        postUpdateDimmableItemTask(lampID);
    }

    protected void postUpdateLampStateOnOff(final String lampID, final boolean onOff) {
        Log.d(SampleAppActivity.TAG, "Updating on/off " + lampID + ": " +  onOff);
        handler.post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = activity.lampModels.get(lampID);

                if (lampModel != null) {
                    lampModel.state.setOnOff(onOff);
                }
            }
        });

        postUpdateDimmableItemTask(lampID);
    }

    protected void postGetLampStateOnOffField(final String lampID, int delay) {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!activity.garbageCollector.isLampExpired(lampID)) {
                    AllJoynManager.lampManager.getLampStateOnOffField(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampStateHue(final String lampID, final long hue) {
        Log.d(SampleAppActivity.TAG, "Updating hue " + lampID + ": " +  hue);
        handler.post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = activity.lampModels.get(lampID);

                if (lampModel != null) {
                    lampModel.state.setHue(hue);
                }
            }
        });

        postUpdateDimmableItemTask(lampID);
    }

    protected void postGetLampStateHueField(final String lampID, int delay) {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!activity.garbageCollector.isLampExpired(lampID)) {
                    AllJoynManager.lampManager.getLampStateHueField(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampStateSaturation(final String lampID, final long saturation) {
        Log.d(SampleAppActivity.TAG, "Updating saturation " + lampID + ": " +  saturation);
        handler.post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = activity.lampModels.get(lampID);

                if (lampModel != null) {
                    lampModel.state.setSaturation(saturation);
                }
            }
        });

        postUpdateDimmableItemTask(lampID);
    }

    protected void postGetLampStateSaturationField(final String lampID, int delay) {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!activity.garbageCollector.isLampExpired(lampID)) {
                    AllJoynManager.lampManager.getLampStateSaturationField(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampStateBrightness(final String lampID, final long brightness) {
        Log.d(SampleAppActivity.TAG, "Updating brightness " + lampID + ": " +  brightness);
        handler.post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = activity.lampModels.get(lampID);

                if (lampModel != null) {
                    lampModel.state.setBrightness(brightness);
                }
            }
        });

        postUpdateDimmableItemTask(lampID);
    }

    protected void postGetLampStateBrightnessField(final String lampID, int delay) {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!activity.garbageCollector.isLampExpired(lampID)) {
                    AllJoynManager.lampManager.getLampStateBrightnessField(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateLampStateColorTemp(final String lampID, final long colorTemp) {
        Log.d(SampleAppActivity.TAG, "Updating color temp " + lampID + ": " +  colorTemp);
        handler.post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = activity.lampModels.get(lampID);

                if (lampModel != null) {
                    lampModel.state.setColorTemp(colorTemp);
                }
            }
        });

        postUpdateDimmableItemTask(lampID);
    }

    protected void postGetLampStateColorTempField(final String lampID, int delay) {
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                if (!activity.garbageCollector.isLampExpired(lampID)) {
                    AllJoynManager.lampManager.getLampStateColorTempField(lampID);
                }
            }
        }, delay);
    }

    protected void postUpdateDimmableItemTask(final String lampID) {
        Log.d(SampleAppActivity.TAG, "Updating row " + lampID);
        handler.post(new Runnable() {
            @Override
            public void run() {
                LampDataModel lampModel = activity.lampModels.get(lampID);

                if (lampModel != null) {
                    Fragment lampsPageFragment = fragmentManager.findFragmentByTag(LampsPageFragment.TAG);

                    if (lampsPageFragment != null) {
                        LampsTableFragment tableFragment = (LampsTableFragment)lampsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_TABLE);

                        if (tableFragment != null) {
                            tableFragment.addElement(lampID);
                        }

                        LampInfoFragment infoFragment = (LampInfoFragment)lampsPageFragment.getChildFragmentManager().findFragmentByTag(PageFrameParentFragment.CHILD_TAG_INFO);

                        if (infoFragment != null) {
                            infoFragment.updateInfoFields(lampModel);
                        }
                    }

                    activity.groupManagerCB.refreshAllLampGroupIDs();
                    activity.sceneManagerCB.refreshScene(null);
                }
            }
        });
    }
}
