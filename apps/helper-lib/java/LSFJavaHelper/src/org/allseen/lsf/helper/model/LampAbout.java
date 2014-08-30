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

import java.util.Map;

import org.alljoyn.about.AboutKeys;
import org.alljoyn.bus.Variant;
import org.allseen.lsf.helper.manager.AboutManager;

public class LampAbout {
    public static String dataNotFound = "-";

    // Announced data
    public String aboutDeviceID;
    public String aboutAppID;
    public String aboutDeviceName;
    public String aboutDefaultLanguage;
    public String aboutAppName;
    public String aboutManufacturer;
    public String aboutModelNumber;

    // Queried data
    public String aboutDescription;
    public String aboutDateOfManufacture;
    public String aboutSoftwareVersion;
    public String aboutHardwareVersion;
    public String aboutSupportUrl;

    public LampAbout() {
        aboutDeviceID = dataNotFound;
        aboutAppID = dataNotFound;
        aboutDeviceName = dataNotFound;
        aboutDefaultLanguage = dataNotFound;
        aboutAppName = dataNotFound;
        aboutManufacturer = dataNotFound;
        aboutModelNumber = dataNotFound;

        aboutDescription = dataNotFound;
        aboutDateOfManufacture = dataNotFound;
        aboutSoftwareVersion = dataNotFound;
        aboutHardwareVersion = dataNotFound;
        aboutSupportUrl = dataNotFound;
    }

    public void setAnnouncedData(Map<String, Variant> announcedData) {
        if (announcedData != null) {
            aboutDeviceID = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_DEVICE_ID, announcedData, dataNotFound);
            aboutAppID = AboutManager.getByteArrayHexStringFromAnnounceData(AboutKeys.ABOUT_APP_ID, announcedData, dataNotFound);
            aboutDeviceName = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_DEVICE_NAME, announcedData, dataNotFound);
            aboutDefaultLanguage = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_DEFAULT_LANGUAGE, announcedData, dataNotFound);
            aboutAppName = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_APP_NAME, announcedData, dataNotFound);
            aboutManufacturer = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_MANUFACTURER, announcedData, dataNotFound);
            aboutModelNumber = AboutManager.getStringFromAnnounceData(AboutKeys.ABOUT_MODEL_NUMBER, announcedData, dataNotFound);
        }
    }

    public void setQueriedData(Map<String, Object> queriedData) {
        if (queriedData != null) {
            aboutDescription = AboutManager.getStringFromAboutData(AboutKeys.ABOUT_DESCRIPTION, queriedData, dataNotFound);
            aboutDateOfManufacture = AboutManager.getStringFromAboutData(AboutKeys.ABOUT_DATE_OF_MANUFACTURE, queriedData, dataNotFound);
            aboutSoftwareVersion = AboutManager.getStringFromAboutData(AboutKeys.ABOUT_SOFTWARE_VERSION, queriedData, dataNotFound);
            aboutHardwareVersion = AboutManager.getStringFromAboutData(AboutKeys.ABOUT_HARDWARE_VERSION, queriedData, dataNotFound);
            aboutSupportUrl = AboutManager.getStringFromAboutData(AboutKeys.ABOUT_SUPPORT_URL, queriedData, dataNotFound);
        }
    }
}
