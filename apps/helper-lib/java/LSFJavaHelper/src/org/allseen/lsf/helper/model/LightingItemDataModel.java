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

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class LightingItemDataModel {
    public static final String defaultID = "!!default_ID!!";

    public String id;
    private final char prefix;
    private String name;
    public LightingItemSortableTag tag;
    public long timestamp;

    public LightingItemDataModel(String itemID, char itemPrefix, String itemName) {
        super();

        this.id = itemID != null ? itemID : defaultID;
        this.prefix = itemPrefix;
        this.name = itemName;
        this.tag = new LightingItemSortableTag(this.id, this.prefix, this.name);

        updateTime();
    }

    public LightingItemDataModel(LightingItemDataModel other) {
        this.id = other.id;
        this.prefix = other.prefix;
        this.name = other.name;
        this.timestamp = other.timestamp;
        this.tag = new LightingItemSortableTag(other.tag);
    }

    public void updateTime() {
        timestamp = System.currentTimeMillis();
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
        this.tag = new LightingItemSortableTag(this.id, this.prefix, this.name);
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
