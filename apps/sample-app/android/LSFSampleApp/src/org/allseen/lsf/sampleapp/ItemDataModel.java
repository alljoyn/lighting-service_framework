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

import android.os.SystemClock;

public class ItemDataModel {

    public String id;
    private final char prefix;
    private String name;
    public long timestamp;
    public LightingItemSortableTag tag;

    public ItemDataModel(String id, char prefix, String name) {
        super();

        this.id = id;
        this.prefix = prefix;
        this.name = name;
        this.timestamp = SystemClock.elapsedRealtime();
        this.tag = new LightingItemSortableTag(this.id, this.prefix, this.name);
    }

    public ItemDataModel(ItemDataModel other) {
        this.id = other.id;
        this.prefix = other.prefix;
        this.name = other.name;
        this.timestamp = other.timestamp;
        this.tag = new LightingItemSortableTag(other.tag);
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

        if ((other != null) && (other instanceof ItemDataModel)) {
            equivalent = (other == this) || (equalsID(((ItemDataModel)other).id));
        }

        return equivalent;
    }
}
