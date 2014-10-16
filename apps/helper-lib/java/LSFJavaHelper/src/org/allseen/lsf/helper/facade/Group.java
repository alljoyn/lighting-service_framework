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

/**
 * A Group object represents a set of lamps in a lighting system, and can be used to send the
 * same command to all of them.
 *
 * Groups can contain lamps and other groups.
 */
public class Group extends ColorItem {

    protected GroupDataModel groupModel;

    /**
     * Constructs a Group using the specified ID.
     * <p>
     * <b>WARNING: This method is intended to be used internally. Client software should not instantiate
     * Groups directly, but should instead get them from the {@link LightingDirector} using the
     * {@link LightingDirector#getGroups()} method.</b>
     *
     * @param groupID The ID of the group
     */
    public Group(String groupID) {
        this(groupID, null);
    }

    /**
     * Constructs a Group using the specified ID and name.
     * <p>
     * <b>WARNING: This method is intended to be used internally. Client software should not instantiate
     * Groups directly, but should instead get them from the {@link LightingDirector} using the
     * {@link LightingDirector#getGroups()} method.</b>
     *
     * @param groupID The ID of the group
     * @param groupName The name of the group
     */
    public Group(String groupID, String groupName) {
        super();

        groupModel = new GroupDataModel(groupID, groupName);
    }

    /**
     * Sends a command to turn all constituent lamps in the Group on or off.
     *
     * @param powerOn Pass in true for on, false for off
     */
    @Override
    public void setPowerOn(boolean powerOn) {
        AllJoynManager.groupManager.transitionLampGroupStateOnOffField(groupModel.id, powerOn);
    }

    /**
     * Sends a command to change the color of all constituent lamps in the Group.
     *
     * @param hueDegrees The hue component of the desired color, in degrees (0-360)
     * @param saturationPercent The saturation component of the desired color, in percent (0-100)
     * @param brightnessPercent The brightness component of the desired color, in percent (0-100)
     * @param colorTempDegrees The color temperature component of the desired color, in degrees Kelvin (2700-9000)
     */
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

    /**
     * <b>WARNING: This method is not intended to be used by clients, and may change or be
     * removed in subsequent releases of the SDK.</b>
     */
    public GroupDataModel getGroupDataModel() {
        return groupModel;
    }
}
