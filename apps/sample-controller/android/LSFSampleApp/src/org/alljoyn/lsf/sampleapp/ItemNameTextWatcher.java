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
package org.alljoyn.lsf.sampleapp;

import android.app.AlertDialog;
import android.text.Editable;
import android.text.TextWatcher;
import android.widget.Button;
import android.widget.EditText;

public class ItemNameTextWatcher implements TextWatcher {

    protected AlertDialog alertDialog;
    protected EditText nameText;

    public ItemNameTextWatcher(AlertDialog alertDialog, EditText nameText) {
        super();

        this.alertDialog = alertDialog;
        this.nameText = nameText;
    }

    @Override
    public void afterTextChanged(Editable s) {
        Button button = alertDialog.getButton(AlertDialog.BUTTON_POSITIVE);
        Editable newName = nameText.getText();

        if ((button != null) && (newName != null)) {
            button.setEnabled((newName.length() > 0) && (newName.charAt(0) != ' '));
        }
    }

    @Override
    public void beforeTextChanged(CharSequence s, int start, int count, int after) {
        // Currently nothing to do
    }

    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count) {
        // Currently nothing to do
    }
}
