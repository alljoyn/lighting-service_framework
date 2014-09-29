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

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.allseen.lsf.LampGroup;
import org.allseen.lsf.LampState;
import org.allseen.lsf.MasterScene;

public class Util {

    // Creates a details string, containing a list of all lamps and groups in a basic scene
    public static String createMemberNamesString(SampleAppActivity activity, BasicSceneDataModel basicSceneModel, String separator) {
        String details = null;

        if (basicSceneModel.noEffects != null) {
            for (NoEffectDataModel elementModel : basicSceneModel.noEffects) {
                details = createMemberNamesString(activity, details, elementModel.members, separator, R.string.basic_scene_members_none);
            }
        }

        if (basicSceneModel.transitionEffects != null) {
            for (TransitionEffectDataModel elementModel : basicSceneModel.transitionEffects) {
                details = createMemberNamesString(activity, details, elementModel.members, separator, R.string.basic_scene_members_none);
            }
        }

        if (basicSceneModel.pulseEffects != null) {
            for (PulseEffectDataModel elementModel : basicSceneModel.pulseEffects) {
                details = createMemberNamesString(activity, details, elementModel.members, separator, R.string.basic_scene_members_none);
            }
        }

        return details;
    }

    // Creates a details string, appending a list of all lamps and subgroups in a lamp group to a previously created detail string
    protected static String createMemberNamesString(SampleAppActivity activity, String previous, LampGroup members, String separator, int noMembersStringID) {
        String current = createMemberNamesString(activity, members, separator, noMembersStringID);

        if (previous != null && !previous.isEmpty()) {
            current = previous + separator + current;
        }

        return current;
    }

    // Creates a details string, containing a list of all lamps and subgroups in a lamp group
    public static String createMemberNamesString(SampleAppActivity activity, LampGroup members, String separator, int noMembersStringID) {
        List<String> groupNames = new ArrayList<String>();
        List<String> lampNames = new ArrayList<String>();

        for (String groupID : members.getLampGroups()) {
            GroupDataModel groupModel = activity.groupModels.get(groupID);
            groupNames.add(groupModel != null ? groupModel.getName() : String.format(activity.getString(R.string.member_group_not_found), groupID));
        }

        for (String lampID : members.getLamps()) {
            LampDataModel lampModel = activity.lampModels.get(lampID);
            lampNames.add(lampModel != null ? lampModel.getName() : String.format(activity.getString(R.string.member_lamp_not_found), lampID));
        }

        Collections.sort(groupNames);
        Collections.sort(lampNames);

        StringBuilder sb = new StringBuilder();

        for (String groupName : groupNames) {
            sb.append(groupName + separator);
        }

        for (String lampName : lampNames) {
            sb.append(lampName + separator);
        }

        String details = sb.toString();

        if (details.length() > separator.length()) {
            // drop the last comma and space
            details = details.substring(0, details.length() - separator.length());
        } else if (noMembersStringID > 0) {
            details = activity.getString(noMembersStringID);
        } else {
            details = "";
        }

        return details;
    }

    // Creates a string containing a sorted comma-separated list of preset names that match the specified state
    public static String createPresetNamesString(SampleAppActivity activity, LampState itemState) {
        List<String> presetNames = new ArrayList<String>();

        for (PresetDataModel presetModel : activity.presetModels.values()) {
            if (presetModel.state != null) {
                if (presetModel.stateEquals(itemState)) {
                    presetNames.add(presetModel.getName());
                }
            }
        }

        String flattenedPresetNames = sortAndFlattenNameList(presetNames);

        return !flattenedPresetNames.isEmpty() ? flattenedPresetNames : activity.getString(R.string.title_presets_save_new);
    }

    // Creates a details string, containing a list of all scenes
    public static String createSceneNamesString(SampleAppActivity activity, MasterScene masterScene) {
        List<String> basicSceneNames = new ArrayList<String>();

        for (String basicSceneID : masterScene.getScenes()) {
            BasicSceneDataModel basicSceneModel = activity.basicSceneModels.get(basicSceneID);
            basicSceneNames.add(basicSceneModel != null ? basicSceneModel.getName() : String.format(activity.getString(R.string.member_scene_not_found), basicSceneID));
        }

        String flattenedSceneNames = sortAndFlattenNameList(basicSceneNames);

        return !flattenedSceneNames.isEmpty() ? flattenedSceneNames : activity.getString(R.string.master_scene_members_none);
    }

    public static String sortAndFlattenNameList(List<String> names) {
        Collections.sort(names);

        StringBuilder sb = new StringBuilder();

        for (String name : names) {
            sb.append(name + ", ");
        }

        String flattenedNames = sb.toString();

        if (flattenedNames.length() > 2) {
            // drop the last comma and space
            flattenedNames = flattenedNames.substring(0, flattenedNames.length() - 2);
        }

        return flattenedNames;
    }
}
