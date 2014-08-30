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

import org.alljoyn.about.AboutKeys;
import org.alljoyn.bus.Variant;
import org.allseen.lsf.LampDetails;
import org.allseen.lsf.LampParameters;

public class LampDataModel extends DimmableItemDataModel {
    private LampDetails details;
    private LampParameters parameters;

    public String aboutDeviceID;
    public String aboutAppID;
    public String aboutDeviceName;
    public String aboutDefaultLanguage;
    public String aboutAppName;
    public String aboutDescription;
    public String aboutManufacturer;
    public String aboutModelNumber;
    public String aboutDateOfManufacture;
    public String aboutSoftwareVersion;
    public String aboutHardwareVersion;
    public String aboutSupportUrl;

    public static String defaultName;
    public static String dataNotFound;

    public LampDataModel(String lampID) {
        super(lampID, defaultName);

        details = new LampDetails();
        parameters = new LampParameters();

        aboutDeviceID = dataNotFound;
        aboutAppID = dataNotFound;
        aboutDeviceName = dataNotFound;
        aboutDefaultLanguage = dataNotFound;
        aboutAppName = dataNotFound;
        aboutDescription = dataNotFound;
        aboutManufacturer = dataNotFound;
        aboutModelNumber = dataNotFound;
        aboutDateOfManufacture = dataNotFound;
        aboutSoftwareVersion = dataNotFound;
        aboutHardwareVersion = dataNotFound;
        aboutSupportUrl = dataNotFound;
    }

    public void setAboutData(Map<String, Variant> announceData, Map<String, Object> aboutData) {
        aboutDeviceID = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_DEVICE_ID, announceData, dataNotFound);
        aboutAppID = AboutManager.getByteArrayHexStringFromAnnounceData(AboutKeys.ABOUT_APP_ID, announceData, dataNotFound);
        aboutDeviceName = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_DEVICE_NAME, announceData, dataNotFound);
        aboutDefaultLanguage = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_DEFAULT_LANGUAGE, announceData, dataNotFound);
        aboutAppName = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_APP_NAME, announceData, dataNotFound);
        aboutManufacturer = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_MANUFACTURER, announceData, dataNotFound);
        aboutModelNumber = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_MODEL_NUMBER, announceData, dataNotFound);

        aboutDescription = AboutManager.getStringFromAboutData(AboutKeys.ABOUT_DESCRIPTION, aboutData, dataNotFound);
        aboutDateOfManufacture = AboutManager.getStringFromAboutData(AboutKeys.ABOUT_DATE_OF_MANUFACTURE, aboutData, dataNotFound);
        aboutSoftwareVersion = AboutManager.getStringFromAboutData(AboutKeys.ABOUT_SOFTWARE_VERSION, aboutData, dataNotFound);
        aboutHardwareVersion = AboutManager.getStringFromAboutData(AboutKeys.ABOUT_HARDWARE_VERSION, aboutData, dataNotFound);
        aboutSupportUrl = AboutManager.getStringFromAboutData(AboutKeys.ABOUT_SUPPORT_URL, aboutData, dataNotFound);
    }

    public void setDetails(LampDetails details) {
        this.details = details;
        setCapability(new CapabilityData(details.isDimmable(), details.hasColor(), details.hasVariableColorTemp()));
    }

    public void setParameters(LampParameters parameters) {
        this.parameters = parameters;
    }

    public LampDetails getDetails() {
        return details;
    }

    public LampParameters getParameters() {
        return parameters;
    }
}
