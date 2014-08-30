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

import java.util.Locale;

public class LightingItemDataModel implements Comparable<LightingItemDataModel> {
    public static final String defaultID = "";

    public String id;
    public String name;
    public String sort;
    public long timestamp;

    public LightingItemDataModel(String itemID, String itemName) {
        super();

        id = itemID != null ? itemID : defaultID;
        name = itemName;
        sort = itemName.toLowerCase(Locale.getDefault());

        updateTime();
    }

    public LightingItemDataModel(LightingItemDataModel other) {
        this.id = other.id;
        this.name = other.name;
        this.sort = other.sort;
        this.timestamp = other.timestamp;
    }

    public void updateTime() {
        timestamp = System.currentTimeMillis();
    }

    @Override
	public int compareTo(LightingItemDataModel other) {
        int comparison = sort.compareTo(other.sort);

        return comparison == 0 ? id.compareTo(other.id) : comparison;
	}

    @Override
    public int hashCode() {
        return id.hashCode();
    }

    public boolean equalsID(String itemID) {
        return id.equals(itemID);
    }

    @Override
    public boolean equals(Object other) {
        boolean equivalent = false;

        if ((other != null) && (other instanceof LightingItemDataModel)) {
            equivalent = (other == this) || (equalsID(((LightingItemDataModel)other).id));
        }

        return equivalent;
    }
}
