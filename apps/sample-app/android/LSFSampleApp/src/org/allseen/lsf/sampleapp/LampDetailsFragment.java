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

import org.allseen.lsf.LampDetails;
import org.allseen.lsf.LampMake;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.view.ViewGroup;

public class LampDetailsFragment extends PageFrameChildFragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_lamp_details, container, false);

        LampDataModel lampModel = ((SampleAppActivity)getActivity()).lampModels.get(key);
        LampAbout lampAbout = lampModel.getAbout();

        if (!lampAbout.aboutQuery && lampAbout.aboutPeer != null) {
            AllJoynManager.aboutManager.getLampQueriedAboutData(key, lampAbout.aboutPeer, lampAbout.aboutPort);
        }

        if (view != null) {
            updateDetailFields(lampModel);
        }

        return view;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        ((SampleAppActivity)getActivity()).updateActionBar(R.string.title_lamp_details, false, false, false, false, true);
    }

    public void updateDetailFields(LampDataModel lampModel) {
        LampDetails lampDetails = (lampModel != null) && (lampModel.getDetails() != null) ? lampModel.getDetails() : EmptyLampDetails.instance;
        LampMake lampMake = lampDetails.getMake();
        String lampMakeName = null;

        if (lampMake != null) {
            lampMakeName = lampMake.name();

            if (lampMakeName.equals("OEM1")) {
                lampMakeName = "Qualcomm Technologies, Inc.";
            }
        }

        setTextViewValue(view, R.id.lampDetailsTextMake, lampMakeName, 0);
        setTextViewValue(view, R.id.lampDetailsTextModel, lampDetails.getModel(), 0);
        setTextViewValue(view, R.id.lampDetailsTextDevice, lampDetails.getType(), 0);
        setTextViewValue(view, R.id.lampDetailsTextLamp, lampDetails.getLampType(), 0);
        setTextViewValue(view, R.id.lampDetailsTextBase, lampDetails.getLampBaseType(), 0);
        setTextViewValue(view, R.id.lampDetailsTextBeam, lampDetails.getLampBeamAngle(), 0);

        setTextViewValue(view, R.id.lampDetailsTextDimmable, lampDetails.isDimmable(), 0);
        setTextViewValue(view, R.id.lampDetailsTextHasColor, lampDetails.hasColor(), 0);
        setTextViewValue(view, R.id.lampDetailsTextHasTemp, lampDetails.hasVariableColorTemp(), 0);
        setTextViewValue(view, R.id.lampDetailsTextHasEffects, lampDetails.hasEffects(), 0);

        setTextViewValue(view, R.id.lampDetailsTextVoltageMin, lampDetails.getMinVoltage(), R.string.units_volts);
        setTextViewValue(view, R.id.lampDetailsTextVoltageMax, lampDetails.getMaxVoltage(), R.string.units_volts);
        setTextViewValue(view, R.id.lampDetailsTextWattageActual, lampDetails.getWattage(), R.string.units_watts);
        setTextViewValue(view, R.id.lampDetailsTextWattageEquiv, lampDetails.getIncandescentEquivalent(), R.string.units_watts);
        setTextViewValue(view, R.id.lampDetailsTextLumens, lampDetails.getMaxLumens(), 0);
        setTextViewValue(view, R.id.lampDetailsTextTempMin, lampDetails.getMinTemperature(), R.string.units_kelvin);
        setTextViewValue(view, R.id.lampDetailsTextTempMax, lampDetails.getMaxTemperature(), R.string.units_kelvin);

        if (lampModel != null) {
            LampAbout lampAbout = lampModel.getAbout();

	        setTextViewValue(view, R.id.lampAboutTextDeviceID, lampAbout.aboutDeviceID, 0);
	        setTextViewValue(view, R.id.lampAboutTextAppID, lampAbout.aboutAppID, 0);
	        setTextViewValue(view, R.id.lampAboutTextDeviceName, lampAbout.aboutDeviceName, 0);
	        setTextViewValue(view, R.id.lampAboutTextDefaultLang, lampAbout.aboutDefaultLanguage, 0);
	        setTextViewValue(view, R.id.lampAboutTextAppName, lampAbout.aboutAppName, 0);
	        setTextViewValue(view, R.id.lampAboutTextDescription, lampAbout.aboutDescription, 0);
	        setTextViewValue(view, R.id.lampAboutTextManufacturer, lampAbout.aboutManufacturer, 0);
	        setTextViewValue(view, R.id.lampAboutTextModel, lampAbout.aboutModelNumber, 0);
	        setTextViewValue(view, R.id.lampAboutTextDate, lampAbout.aboutDateOfManufacture, 0);
	        setTextViewValue(view, R.id.lampAboutTextSoftVer, lampAbout.aboutSoftwareVersion, 0);
	        setTextViewValue(view, R.id.lampAboutTextHardVer, lampAbout.aboutHardwareVersion, 0);
	        setTextViewValue(view, R.id.lampAboutTextSuppoerUrl, lampAbout.aboutSupportUrl, 0);
        }
    }
}
