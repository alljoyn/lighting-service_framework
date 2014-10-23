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

import org.allseen.lsf.LampGroup;

public class BasicSceneElementDataModel extends DimmableItemDataModel {
    public static final char TAG_PREFIX_SCENE_ELEMENT = 's';

    protected static int nextID = 1;

    public final EffectType type;
    public LampGroup members;
    public String presetID;

    public BasicSceneElementDataModel() {
        this(null, "");
    }

    public BasicSceneElementDataModel(EffectType type, String name) {
        super(String.valueOf(nextID++), TAG_PREFIX_SCENE_ELEMENT, name);

        this.type = type;
        this.members = new LampGroup();

        // State is always set to "on". To turn the lamp off as part of an effect,
        // you have to set the brightness to zero
        this.state.setOnOff(true);

        this.capability.dimmable = CapabilityData.ALL;
        this.capability.color = CapabilityData.ALL;
        this.capability.temp = CapabilityData.ALL;
    }

    public BasicSceneElementDataModel(BasicSceneElementDataModel other) {
        super(other);

        this.type = other.type;
        this.members = new LampGroup(other.members);
        this.presetID = other.presetID;
    }

    public boolean containsGroup(String groupID) {
        String[] childIDs = members.getLampGroups();

        for (String childID : childIDs) {
            if (childID.equals(groupID)) {
                return true;
            }
        }

        return false;
    }

    public boolean containsPreset(String presetID) {
        //TODO-FIX
        return false;
    }
}
