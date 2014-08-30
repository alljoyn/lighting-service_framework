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

public class LampGroupManagerCallback extends DefaultNativeClassWrapper {

    public LampGroupManagerCallback() {
        createNativeObject();
    }

    public void getAllLampGroupIDsReplyCB(ResponseCode responseCode, String[] lampGroupIDs)                                     { }
    public void getLampGroupNameReplyCB(ResponseCode responseCode, String lampGroupID, String language, String lampGroupName)   { }
    public void setLampGroupNameReplyCB(ResponseCode responseCode, String lampGroupID, String language)                         { }
    public void lampGroupsNameChangedCB(String[] lampGroupIDs)                                                                  { }
    public void createLampGroupReplyCB(ResponseCode responseCode, String lampGroupID)                                           { }
    public void lampGroupsCreatedCB(String[] lampGroupIDs)                                                                      { }
    public void getLampGroupReplyCB(ResponseCode responseCode, String lampGroupID, LampGroup lampGroup)                         { }
    public void deleteLampGroupReplyCB(ResponseCode responseCode, String lampGroupID)                                           { }
    public void lampGroupsDeletedCB(String[] lampGroupIDs)                                                                      { }
    public void transitionLampGroupStateReplyCB(ResponseCode responseCode, String lampGroupID)                                  { }
    public void pulseLampGroupWithStateReplyCB(ResponseCode responseCode, String lampGroupID)                                   { }
    public void pulseLampGroupWithPresetReplyCB(ResponseCode responseCode, String lampGroupID)                                  { }
    public void transitionLampGroupStateOnOffFieldReplyCB(ResponseCode responseCode, String lampGroupID)                        { }
    public void transitionLampGroupStateHueFieldReplyCB(ResponseCode responseCode, String lampGroupID)                          { }
    public void transitionLampGroupStateSaturationFieldReplyCB(ResponseCode responseCode, String lampGroupID)                   { }
    public void transitionLampGroupStateBrightnessFieldReplyCB(ResponseCode responseCode, String lampGroupID)                   { }
    public void transitionLampGroupStateColorTempFieldReplyCB(ResponseCode responseCode, String lampGroupID)                    { }
    public void resetLampGroupStateReplyCB(ResponseCode responseCode, String lampGroupID)                                       { }
    public void resetLampGroupStateOnOffFieldReplyCB(ResponseCode responseCode, String lampGroupID)                             { }
    public void resetLampGroupStateHueFieldReplyCB(ResponseCode responseCode, String lampGroupID)                               { }
    public void resetLampGroupStateSaturationFieldReplyCB(ResponseCode responseCode, String lampGroupID)                        { }
    public void resetLampGroupStateBrightnessFieldReplyCB(ResponseCode responseCode, String lampGroupID)                        { }
    public void resetLampGroupStateColorTempFieldReplyCB(ResponseCode responseCode, String lampGroupID)                         { }
    public void updateLampGroupReplyCB(ResponseCode responseCode, String lampGroupID)                                           { }
    public void lampGroupsUpdatedCB(String[] lampGroupIDs)                                                                      { }
    public void transitionLampGroupStateToPresetReplyCB(ResponseCode responseCode, String lampGroupID)                          { }

    // @Override
    @Override
    protected native void createNativeObject();

    // @Override
    @Override
    protected native void destroyNativeObject();
}
