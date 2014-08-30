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

import java.util.Collections;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class GroupsFlattener {
    public Set<String> groupIDSet;
    public Set<String> lampIDSet;
    int duplicates;

    public GroupsFlattener() {
        super();
    }

    public void flattenGroups(Map<String, GroupDataModel> groupModels) {
        for (GroupDataModel groupModel : groupModels.values()) {
            groupIDSet = new HashSet<String>();
            lampIDSet = new HashSet<String>();
            duplicates = 0;

            flattenGroups(groupModels, groupModel);
            flattenLamps(groupModels);

            groupModel.setGroups(groupIDSet);
            groupModel.setLamps(lampIDSet);
            groupModel.duplicates = duplicates;
        }
    }

    public void flattenGroups(Map<String, GroupDataModel> groupModels, GroupDataModel parentModel) {
        if (!groupIDSet.contains(parentModel.id)) {
            groupIDSet.add(parentModel.id);

            for (String childGroupID : parentModel.members.getLampGroups()) {
                GroupDataModel childModel = groupModels.get(childGroupID);

                if (childModel != null) {
                    flattenGroups(groupModels, childModel);
                }
            }
        } else {
            duplicates++;
        }
    }

    public void flattenLamps(Map<String, GroupDataModel> groupModels) {
        for (String groupID : groupIDSet) {
            GroupDataModel groupModel = groupModels.get(groupID);

            if (groupModel != null) {
                Collections.addAll(lampIDSet, groupModel.members.getLamps());
            }
        }
    }
}
