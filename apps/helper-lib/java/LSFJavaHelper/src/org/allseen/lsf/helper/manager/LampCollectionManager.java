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

import java.util.ArrayDeque;
import java.util.Collection;
import java.util.Iterator;
import java.util.Queue;

import org.allseen.lsf.helper.facade.Lamp;
import org.allseen.lsf.helper.listener.LampCollectionListener;
import org.allseen.lsf.helper.listener.LightingItemErrorEvent;
import org.allseen.lsf.helper.model.LampDataModel;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class LampCollectionManager extends LightingItemCollectionManager<Lamp, LampCollectionListener, LampDataModel> {

    protected final Queue<String> lampIDs = new ArrayDeque<String>();

    public LampCollectionManager(LightingSystemManager director) {
        super(director);
    }

    @Override
    protected Collection<String> getIDCollection() {
        // Overridden for efficiency
        return lampIDs;
    }

    public Lamp addLamp(String lampID) {
        return addLamp(lampID, new Lamp(lampID));
    }

    public Lamp addLamp(String lampID, Lamp lamp) {
        itemAdapters.put(lampID, lamp);
        lampIDs.add(lampID);

        return lamp;
    }

    public Lamp getLamp(String lampID) {
        return getAdapter(lampID);
    }

    public Lamp[] getLamps() {
        return getAdapters().toArray(new Lamp[size()]);
    }

    public Iterator<Lamp> getLampIterator() {
        return getAdapters().iterator();
    }

    public Lamp removeLamp(String lampID) {
        lampIDs.remove(lampID);

        return removeAdapter(lampID);
    }

    @Override
    protected void sendChangedEvent(LampCollectionListener listener, Iterator<Lamp> lamps, int count) {
        listener.onLampsChanged(lamps, count);
    }

    @Override
    protected void sendRemovedEvent(LampCollectionListener listener, Iterator<Lamp> lamps, int count) {
        listener.onLampsRemoved(lamps, count);
    }

    @Override
    protected void sendErrorEvent(LampCollectionListener listener, LightingItemErrorEvent errorEvent) {
        listener.onLampError(errorEvent);
    }

    @Override
    public LampDataModel getModel(String lampID) {
        Lamp lamp = getAdapter(lampID);

        return lamp != null ? lamp.getLampDataModel() : null;
    }
}
