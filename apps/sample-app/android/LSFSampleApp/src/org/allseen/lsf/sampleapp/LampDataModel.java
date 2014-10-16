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
import org.allseen.lsf.LampParameters;

public class LampDataModel extends DimmableItemDataModel {
    public static final char TAG_PREFIX_LAMP = 'L';

    private LampDetails details;
    private LampParameters parameters;
    private LampAbout about;

    public static String defaultName;
    public static String dataNotFound;

    public LampDataModel(String lampID) {
        super(lampID, TAG_PREFIX_LAMP, defaultName);

        details = new LampDetails();
        parameters = new LampParameters();
        about = new LampAbout();
    }

    public void setDetails(LampDetails details) {
        this.details = details;
        setCapability(new CapabilityData(details.isDimmable(), details.hasColor(), details.hasVariableColorTemp()));
    }

    public void setParameters(LampParameters parameters) {
        this.parameters = parameters;
    }

    public void setAbout(LampAbout about) {
        this.about = about;
    }

    public LampDetails getDetails() {
        return details;
    }

    public LampParameters getParameters() {
        return parameters;
    }

    public LampAbout getAbout() {
        return about;
    }
}
