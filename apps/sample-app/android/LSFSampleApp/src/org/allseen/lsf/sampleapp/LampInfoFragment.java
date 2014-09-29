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

import org.allseen.lsf.LampParameters;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class LampInfoFragment extends DimmableItemInfoFragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = super.onCreateView(inflater, container, savedInstanceState);
        String lampID = key;

        itemType = SampleAppActivity.Type.LAMP;
        itemModels = ((SampleAppActivity)getActivity()).lampModels;

        ((TextView)statusView.findViewById(R.id.statusLabelName)).setText(R.string.label_lamp_name);

        // details
        view.findViewById(R.id.lampInfoTableRow5).setOnClickListener(this);

        updateInfoFields(((SampleAppActivity)getActivity()).lampModels.get(lampID));

        return view;
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        ((SampleAppActivity)getActivity()).updateActionBar(R.string.title_lamp_info, false, false, false, false, true);
    }

    @Override
    public void onClick(View view) {
        int viewID = view.getId();

        if (viewID == R.id.lampInfoTableRow5) {
            ((SampleAppActivity)getActivity()).showLampDetailsFragment((LampsPageFragment)parent, key);
        } else {
            super.onClick(view);
        }
    }

    public void updateInfoFields(LampDataModel lampModel) {
        if (lampModel.id.equals(key)) {
            stateAdapter.setCapability(lampModel.getCapability());
            super.updateInfoFields(lampModel);

            LampParameters lampParams = lampModel.getParameters() != null ? lampModel.getParameters() : EmptyLampParamaters.instance;
            setTextViewValue(view, R.id.lampInfoTextLumens, lampParams.getLumens(), 0);
            setTextViewValue(view, R.id.lampInfoTextEnergy, lampParams.getEnergyUsageMilliwatts(), R.string.units_mw);
        }
    }

    @Override
    protected int getLayoutID() {
        return R.layout.fragment_lamp_info;
    }

    @Override
    protected void onHeaderClick() {
        SampleAppActivity activity = (SampleAppActivity)getActivity();
        LampDataModel lampModel = activity.lampModels.get(key);

        activity.showItemNameDialog(R.string.title_lamp_rename, new UpdateLampNameAdapter(lampModel, (SampleAppActivity) getActivity()));
    }
}
