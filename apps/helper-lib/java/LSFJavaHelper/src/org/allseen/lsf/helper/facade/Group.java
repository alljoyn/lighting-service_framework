/* Copyright (c) 2014, AllSeen Alliance. All rights reserved.
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
package org.allseen.lsf.helper.facade;

import org.allseen.lsf.LampState;
import org.allseen.lsf.helper.manager.AllJoynManager;
import org.allseen.lsf.helper.model.ColorItemDataModel;
import org.allseen.lsf.helper.model.ColorStateConverter;
import org.allseen.lsf.helper.model.GroupDataModel;

public class Group extends ColorItem {

    protected GroupDataModel groupModel;

    public Group(String groupID) {
        this(groupID, null);
    }

    public Group(String groupID, String groupName) {
        super();

        groupModel = new GroupDataModel(groupID, groupName);
    }

    @Override
    public void setPowerOn(boolean powerOn) {
        AllJoynManager.groupManager.transitionLampGroupStateOnOffField(groupModel.id, powerOn);
    }

    @Override
    public void setColor(int hueDegrees, int saturationPercent, int brightnessPercent, int colorTempDegrees) {
        LampState lampState = new LampState();

        lampState.setOnOff(true);

        ColorStateConverter.convertViewToModel(hueDegrees, saturationPercent, brightnessPercent, colorTempDegrees, lampState);

        AllJoynManager.groupManager.transitionLampGroupState(groupModel.id, lampState, 0);
    }

    @Override
    protected ColorItemDataModel getColorDataModel() {
        return getGroupDataModel();
    }

    public GroupDataModel getGroupDataModel() {
        return groupModel;
    }
}
