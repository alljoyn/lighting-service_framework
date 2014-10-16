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

import org.allseen.lsf.helper.manager.AllJoynManager;
import org.allseen.lsf.helper.model.LightingItemDataModel;
import org.allseen.lsf.helper.model.PresetDataModel;

/**
 * A Preset object represents a predefined color state in a lighting system.
 * <p>
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class Preset extends LightingItem {

    protected PresetDataModel presetModel;

    public Preset(String presetID) {
        this(presetID, null);
    }

    public Preset(String presetID, String presetName) {
        super();

        presetModel = new PresetDataModel(presetID, presetName);
    }

    public void applyTo(Lamp lamp) {
        applyToLamp(lamp.getLampDataModel().id);
    }

    public void applyTo(Group group) {
        applyToGroup(group.getGroupDataModel().id);
    }

    protected void applyToLamp(String lampID) {
        AllJoynManager.lampManager.transitionLampStateToPreset(lampID, presetModel.id, 0);
    }

    protected void applyToGroup(String groupID) {
        AllJoynManager.groupManager.transitionLampGroupStateToPreset(groupID, presetModel.id, 0);
    }

    @Override
    protected LightingItemDataModel getItemDataModel() {
        return getPresetDataModel();
    }

    public PresetDataModel getPresetDataModel() {
        return presetModel;
    }
}
