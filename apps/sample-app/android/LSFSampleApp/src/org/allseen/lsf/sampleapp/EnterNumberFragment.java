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

import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.Toast;

public abstract class EnterNumberFragment extends PageFrameChildFragment {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_enter_number, container, false);

        if (view != null) {
            setTextViewValue(view, R.id.enterNumberLabel, getString(getLabelID()), 0);
        }

        ((EditText)view.findViewById(R.id.enterNumberText)).setInputType(getInputType());

        updateNumberField();

        return view;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        ((SampleAppActivity)getActivity()).updateActionBar(getTitleID(), false, false, false, true);
    }

    @Override
    public void onActionDone() {
        final String stringValue = ((EditText)view.findViewById(R.id.enterNumberText)).getText().toString();

        if ((stringValue != null) && (!stringValue.isEmpty())) {
            try {
                long longValue = Math.round(Double.valueOf(stringValue) * getScale());
                long maxValue = (long)Integer.MAX_VALUE - Integer.MIN_VALUE;

                if (longValue >= 0 && longValue <= maxValue) {

                    if (setNumberValue(longValue)) {
                        // hide soft keyboard
                        InputMethodManager inputManager = (InputMethodManager) getActivity().getSystemService(Context.INPUT_METHOD_SERVICE);
                        inputManager.hideSoftInputFromWindow(view.getWindowToken(), 0);
                    }
                } else {
                    Toast.makeText(getActivity(), String.format(getString(R.string.toast_number_value_invalid), maxValue / getScale()), Toast.LENGTH_LONG).show();
                }
            } catch (NumberFormatException e) {
                Toast.makeText(getActivity(), R.string.toast_number_format_invalid, Toast.LENGTH_LONG).show();
            }
        } else {
            Toast.makeText(getActivity(), String.format(getString(R.string.toast_number_missing), getString(getLabelID())), Toast.LENGTH_LONG).show();
        }
    }

    protected double getScale() {
        return 1000.0;
    }

    public void updateNumberField() {
        String number = getNumberString();

        ((EditText)view.findViewById(R.id.enterNumberText)).setText(number);
    }

    protected abstract int getTitleID();
    protected abstract int getLabelID();
    protected abstract int getInputType();

    protected abstract String getNumberString();
    protected abstract boolean setNumberValue(long longValue);
}

