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
package org.allseen.lsf.helper.manager;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.allseen.lsf.ResponseCode;
import org.allseen.lsf.helper.listener.LightingItemErrorEvent;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public abstract class LightingItemCollectionManager<ADAPTER, LISTENER, MODEL> extends LightingItemListenerManager<LISTENER> {

    protected final Map<String, ADAPTER> itemAdapters = new HashMap<String, ADAPTER>();
    protected final List<LISTENER> itemListeners = new ArrayList<LISTENER>();

    public LightingItemCollectionManager(LightingSystemManager director) {
        super(director);
    }

    public boolean hasID(String itemID) {
        return itemAdapters.containsKey(itemID);
    }

    protected Collection<String> getIDCollection() {
        return itemAdapters.keySet();
    }

    public Iterator<String> getIDIterator() {
        return getIDCollection().iterator();
    }

    public String[] getIDArray() {
        return getIDArray(new String[size()]);
    }

    public String[] getIDArray(String[] itemIDs) {
        return getIDCollection().toArray(itemIDs);
    }

    public int size() {
        return itemAdapters.size();
    }

    protected ADAPTER getAdapter(String itemID) {
        return itemAdapters.get(itemID);
    }

    protected ADAPTER removeAdapter(String itemID) {
        ADAPTER item = itemAdapters.remove(itemID);

        sendRemovedEvent(item);

        return item;
    }

    protected Collection<ADAPTER> getAdapters() {
        return itemAdapters.values();
    }

    @Override
    public void addListener(LISTENER listener) {
        super.addListener(listener);

        sendChangedEvent(listener, itemAdapters.values().iterator(), size());
    }

    public void sendChangedEvent(String itemID) {
        sendChangedEvent(Arrays.asList(getAdapter(itemID)).iterator(), 1);
    }

    protected void sendChangedEvent(Iterator<ADAPTER> items, int count) {
        for (LISTENER listener : itemListeners) {
            sendChangedEvent(listener, items, count);
        }
    }

    public void sendRemovedEvent(ADAPTER item) {
        sendRemovedEvent(Arrays.asList(item).iterator(), 1);
    }

    protected void sendRemovedEvent(Iterator<ADAPTER> items, int count) {
        for (LISTENER listener : itemListeners) {
            sendChangedEvent(listener, items, count);
        }
    }

    public void sendErrorEvent(String name, ResponseCode responseCode) {
        sendErrorEvent(name, responseCode, null);
    }

    public void sendErrorEvent(String name, ResponseCode responseCode, String itemID) {
        sendErrorEvent(new LightingItemErrorEvent(name, responseCode, itemID));
    }

    public void sendErrorEvent(LightingItemErrorEvent errorEvent) {
        for (LISTENER listener : itemListeners) {
            sendErrorEvent(listener, errorEvent);
        }
    }

    protected abstract void sendChangedEvent(LISTENER listener, Iterator<ADAPTER> items, int count);
    protected abstract void sendRemovedEvent(LISTENER listener, Iterator<ADAPTER> items, int count);
    protected abstract void sendErrorEvent(LISTENER listener, LightingItemErrorEvent errorEvent);

    public abstract MODEL getModel(String itemID);
}
