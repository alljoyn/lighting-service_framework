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

import java.util.HashSet;
import java.util.Set;

import org.allseen.lsf.LampGroup;

public class GroupDataModel extends DimmableItemDataModel {
    public static final char TAG_PREFIX_GROUP = 'G';

    public static String defaultName = "";

    public LampGroup members;
    private Set<String> lamps;
    private Set<String> groups;
    public int duplicates;

    public GroupDataModel() {
        this("");
    }

    public GroupDataModel(String groupID) {
        this(TAG_PREFIX_GROUP, groupID);
    }

    protected GroupDataModel(char prefix, String groupID) {
        super(groupID, prefix, defaultName);

        members = new LampGroup();
    }

    public GroupDataModel(GroupDataModel other) {
        super(other);

        this.members = new LampGroup(other.members);
        this.lamps = new HashSet<String>(other.lamps);
        this.groups = new HashSet<String>(other.groups);
        this.duplicates = other.duplicates;
    }

    public void setLamps(Set<String> lamps) {
        this.lamps = lamps;
        // capability data is now dirty
        capability = new CapabilityData();
    }

    public void setGroups(Set<String> groups) {
        this.groups = groups;
        // capability data is now dirty
        capability = new CapabilityData();
    }

    public Set<String> getLamps() {
        return lamps;
    }

    public Set<String> getGroups() {
        return groups;
    }

    // Only checks immediate child groups. To see if the group is a descendent (child, grandchild,
    // great-grandchild, etc.) you can use getGroups().contains(groupID);
    public boolean containsGroup(String groupID) {
        String[] childIDs = members.getLampGroups();

        for (String childID : childIDs) {
            if (childID.equals(groupID)) {
                return true;
            }
        }

        return false;
    }
}
