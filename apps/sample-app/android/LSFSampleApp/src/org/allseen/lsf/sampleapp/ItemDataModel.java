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

public class ItemDataModel implements Comparable<ItemDataModel> {

    public String id;
    public String name;
    public long timestamp;

    public ItemDataModel(String itemID, String itemName) {
        super();

        id = itemID;
        name = itemName;
        timestamp = SystemClock.elapsedRealtime();
    }

    public ItemDataModel(ItemDataModel other) {
        this.id = other.id;
        this.name = other.name;
        this.timestamp = other.timestamp;
    }

    @Override
	public int compareTo(ItemDataModel other) {
		ItemDataModel otherDataModel = (ItemDataModel) other;
		String otherDataModelStr = otherDataModel.name + otherDataModel.id;
		String thisDataModelStr = this.name + this.id;

		return thisDataModelStr.compareTo(otherDataModelStr);
	}

	@Override
    public int hashCode() {
        return id.hashCode();
    }

    @Override
    public boolean equals(Object that) {
        boolean result = false;

        if ((that != null) && (that instanceof ItemDataModel)) {
            result = (that == this) || (id.equals(((ItemDataModel)that).id));
        }

        return result;
    }
}
