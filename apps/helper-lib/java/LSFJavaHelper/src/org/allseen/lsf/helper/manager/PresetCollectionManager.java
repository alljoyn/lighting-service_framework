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

import java.util.Iterator;

import org.allseen.lsf.helper.facade.Preset;
import org.allseen.lsf.helper.listener.LightingItemErrorEvent;
import org.allseen.lsf.helper.listener.PresetCollectionListener;
import org.allseen.lsf.helper.model.PresetDataModel;

/**
 * <b>WARNING: This class is not intended to be used by clients, and its interface may change
 * in subsequent releases of the SDK</b>.
 */
public class PresetCollectionManager extends LightingItemCollectionManager<Preset, PresetCollectionListener, PresetDataModel> {

    public PresetCollectionManager(LightingSystemManager director) {
        super(director);
    }

    public Preset addPreset(String presetID) {
        return addPreset(presetID, new Preset(presetID));
    }

    public Preset addPreset(String presetID, Preset preset) {
        return itemAdapters.put(presetID, preset);
    }

    public Preset getPreset(String presetID) {
        return getAdapter(presetID);
    }

    public Preset[] getPresets() {
        return getAdapters().toArray(new Preset[size()]);
    }

    public Iterator<Preset> getPresetIterator() {
        return getAdapters().iterator();
    }

    public Preset removePreset(String presetID) {
        return removeAdapter(presetID);
    }

    @Override
    protected void sendChangedEvent(PresetCollectionListener listener, Iterator<Preset> presets, int count) {
        listener.onPresetsChanged(presets, count);
    }

    @Override
    protected void sendRemovedEvent(PresetCollectionListener listener, Iterator<Preset> presets, int count) {
        listener.onPresetsRemoved(presets, count);
    }

    @Override
    protected void sendErrorEvent(PresetCollectionListener listener, LightingItemErrorEvent errorEvent) {
        listener.onPresetError(errorEvent);
    }

    @Override
    public PresetDataModel getModel(String presetID) {
        Preset preset = getAdapter(presetID);

        return preset != null ? preset.getPresetDataModel() : null;
    }
}
