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

import org.allseen.lsf.ErrorCode;
import org.allseen.lsf.helper.listener.ControllerErrorEvent;
import org.allseen.lsf.helper.listener.ControllerListener;
import org.allseen.lsf.helper.model.ControllerDataModel;

public class ControllerManager extends LightingItemListenerManager<ControllerListener> {

    protected ControllerDataModel leadModel = new ControllerDataModel();

    public ControllerManager(LightingSystemManager director) {
        super(director);
    }

    public ControllerDataModel getLeadControllerModel() {
        return leadModel;
    }

    public void sendLeaderStateChangeEvent() {
        for (ControllerListener listener : itemListeners) {
            listener.onLeaderModelChange(leadModel);
        }
    }

    public void sendErrorEvent(String name, ErrorCode[] errorCodes) {
        sendErrorEvent(new ControllerErrorEvent(name, errorCodes));
    }

    public void sendErrorEvent(ControllerErrorEvent errorEvent) {
        for (ControllerListener listener : itemListeners) {
            sendErrorEvent(listener, errorEvent);
        }
    }

    protected void sendErrorEvent(ControllerListener listener, ControllerErrorEvent errorEvent) {
        listener.onControllerErrors(errorEvent);
    }
}
