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

import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Toast;

public class LampStateViewAdapter implements OnSeekBarChangeListener, OnClickListener {

    public final View stateView;
    public final DimmableItemInfoFragment parentFragment;

    public final Button presetsButton;
    public final SeekBar brightnessSeekBar;
    public final SeekBar hueSeekBar;
    public final SeekBar saturationSeekBar;
    public final SeekBar tempSeekBar;

    private CapabilityData capability;

    public LampStateViewAdapter(View stateView, String tag, DimmableItemInfoFragment parentFragment) {
        this.stateView = stateView;
        this.parentFragment = parentFragment;

        presetsButton = (Button) stateView.findViewById(R.id.stateButton);

        brightnessSeekBar = (SeekBar) stateView.findViewById(R.id.stateSliderBrightness);
        brightnessSeekBar.setTag(tag);
        brightnessSeekBar.setSaveEnabled(false);
        brightnessSeekBar.setOnSeekBarChangeListener(this);
        stateView.findViewById(R.id.stateControlBrightness).setOnClickListener(this);

        hueSeekBar = (SeekBar) stateView.findViewById(R.id.stateSliderHue);
        hueSeekBar.setMax(360);
        hueSeekBar.setTag(tag);
        hueSeekBar.setSaveEnabled(false);
        hueSeekBar.setOnSeekBarChangeListener(this);
        stateView.findViewById(R.id.stateControlHue).setOnClickListener(this);

        saturationSeekBar = (SeekBar) stateView.findViewById(R.id.stateSliderSaturation);
        saturationSeekBar.setTag(tag);
        saturationSeekBar.setSaveEnabled(false);
        saturationSeekBar.setOnSeekBarChangeListener(this);
        stateView.findViewById(R.id.stateControlSaturation).setOnClickListener(this);

        tempSeekBar = (SeekBar) stateView.findViewById(R.id.stateSliderColorTemp);
        tempSeekBar.setMax(DimmableItemScaleConverter.VIEW_COLORTEMP_SPAN);
        tempSeekBar.setTag(tag);
        tempSeekBar.setSaveEnabled(false);
        tempSeekBar.setOnSeekBarChangeListener(this);
        stateView.findViewById(R.id.stateControlColorTemp).setOnClickListener(this);

        capability = new CapabilityData();
    }

    public void setCapability(CapabilityData capability) {
        this.capability = capability;
        boolean displayStarFooter = false;

        // dimmable
        if (capability.dimmable >= CapabilityData.SOME) {
            brightnessSeekBar.setEnabled(true);
            presetsButton.setEnabled(true);
        } else {
            brightnessSeekBar.setEnabled(false);
            presetsButton.setEnabled(false);
            parentFragment.setTextViewValue(stateView, R.id.stateTextBrightness, parentFragment.getResources().getString(R.string.na), 0);
        }
        if (capability.dimmable == CapabilityData.SOME) {
            stateView.findViewById(R.id.stateLabelBrightnessStar).setVisibility(View.VISIBLE);
            displayStarFooter = true;
        } else {
            stateView.findViewById(R.id.stateLabelBrightnessStar).setVisibility(View.INVISIBLE);
        }

        // color support
        if (capability.color >= CapabilityData.SOME) {
            hueSeekBar.setEnabled(true);
            saturationSeekBar.setEnabled(true);
        } else {
            hueSeekBar.setEnabled(false);
            saturationSeekBar.setEnabled(false);
            parentFragment.setTextViewValue(stateView, R.id.stateTextHue, parentFragment.getResources().getString(R.string.na), 0);
            parentFragment.setTextViewValue(stateView, R.id.stateTextSaturation, parentFragment.getResources().getString(R.string.na), 0);
        }
        if (capability.color == CapabilityData.SOME) {
            stateView.findViewById(R.id.stateLabelHueStar).setVisibility(View.VISIBLE);
            stateView.findViewById(R.id.stateLabelSaturationStar).setVisibility(View.VISIBLE);
            displayStarFooter = true;
        } else {
            stateView.findViewById(R.id.stateLabelHueStar).setVisibility(View.INVISIBLE);
            stateView.findViewById(R.id.stateLabelSaturationStar).setVisibility(View.INVISIBLE);
        }

        // temperature support
        if (capability.temp >= CapabilityData.SOME) {
            tempSeekBar.setEnabled(true);
        } else {
            tempSeekBar.setEnabled(false);
            parentFragment.setTextViewValue(stateView, R.id.stateTextColorTemp, parentFragment.getResources().getString(R.string.na), 0);
        }
        if (capability.temp == CapabilityData.SOME) {
            stateView.findViewById(R.id.stateLabelColorTempStar).setVisibility(View.VISIBLE);
            displayStarFooter = true;
        } else {
            stateView.findViewById(R.id.stateLabelColorTempStar).setVisibility(View.INVISIBLE);
        }

        if (displayStarFooter) {
            stateView.findViewById(R.id.stateTextNotSupportedByAll).setVisibility(View.VISIBLE);
        } else {
            stateView.findViewById(R.id.stateTextNotSupportedByAll).setVisibility(View.GONE);
        }

        saturationCheck();
    }

    public void setPreset(String presetName) {
        presetsButton.setText(presetName);
    }

    public void setBrightness(long modelBrightness) {
        if (capability.dimmable >= CapabilityData.SOME) {
            int viewBrightness = DimmableItemScaleConverter.convertBrightnessModelToView(modelBrightness);
            brightnessSeekBar.setProgress(viewBrightness);
            parentFragment.setTextViewValue(stateView, R.id.stateTextBrightness, viewBrightness, R.string.units_percent);
        }
    }

    public void setHue(long modelHue) {
        if (capability.color >= CapabilityData.SOME) {
            int viewHue = DimmableItemScaleConverter.convertHueModelToView(modelHue);
            hueSeekBar.setProgress(viewHue);
            parentFragment.setTextViewValue(stateView, R.id.stateTextHue, viewHue, R.string.units_degrees);
        }
    }

    public void setSaturation(long modelSaturation) {
        if (capability.color >= CapabilityData.SOME) {
            int viewSaturation = DimmableItemScaleConverter.convertSaturationModelToView(modelSaturation);
            saturationSeekBar.setProgress(viewSaturation);
            parentFragment.setTextViewValue(stateView, R.id.stateTextSaturation, viewSaturation, R.string.units_percent);

            saturationCheck();
        }
    }

    public void setColorTemp(long modelColorTemp) {
        if (capability.temp >= CapabilityData.SOME) {
            int viewColorTemp = DimmableItemScaleConverter.convertColorTempModelToView(modelColorTemp);
            tempSeekBar.setProgress(viewColorTemp - DimmableItemScaleConverter.VIEW_COLORTEMP_MIN);
            parentFragment.setTextViewValue(stateView, R.id.stateTextColorTemp, viewColorTemp, R.string.units_kelvin);
        }
    }

    private void saturationCheck() {
        if (saturationSeekBar.getProgress() == 0) {
            hueSeekBar.setEnabled(false);
        } else if (capability.color >= CapabilityData.SOME) {
            hueSeekBar.setEnabled(true);
        }

        if (saturationSeekBar.getProgress() == saturationSeekBar.getMax()) {
            tempSeekBar.setEnabled(false);
        } else if (capability.temp >= CapabilityData.SOME) {
            tempSeekBar.setEnabled(true);
        }
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        //AJSI-291: UI Slider behaviour change (from continuous updating to updating when finger is lifted)
        /*if (parentFragment != null) {
            setTextViewValues(seekBar);
            if (fromUser) {
                parentFragment.setField(seekBar);
            }
        }*/
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {
        // Currently nothing to do
    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        if (parentFragment.parent != null) {
            parentFragment.setField(seekBar);

            if (seekBar.getId() == R.id.stateSliderSaturation) {
                saturationCheck();
            }

            setTextViewValues(seekBar);
        }
    }

    private void setTextViewValues(SeekBar seekBar) {
        if (parentFragment.parent != null) {
            int seekBarID = seekBar.getId();
            int progress = seekBar.getProgress();

            // if it is a status slider, update indicator text
            if (seekBarID == R.id.stateSliderBrightness) {
                parentFragment.setTextViewValue(stateView, R.id.stateTextBrightness, progress, R.string.units_percent);
            } else if (seekBarID == R.id.stateSliderHue) {
                parentFragment.setTextViewValue(stateView, R.id.stateTextHue, progress, R.string.units_degrees);
            } else if (seekBarID == R.id.stateSliderSaturation) {
                parentFragment.setTextViewValue(stateView, R.id.stateTextSaturation, progress, R.string.units_percent);
            } else if (seekBarID == R.id.stateSliderColorTemp) {
                parentFragment.setTextViewValue(stateView, R.id.stateTextColorTemp, progress + DimmableItemScaleConverter.VIEW_COLORTEMP_MIN, R.string.units_kelvin);
            }

        }
    }

    @Override
    public void onClick(View v) {
        int viewID = v.getId();

        if (viewID == R.id.stateControlBrightness) {
            if (capability.dimmable <= CapabilityData.NONE) {
                Toast.makeText(parentFragment.getActivity(), R.string.no_support_dimmable, Toast.LENGTH_LONG).show();
            }
        } else if (viewID == R.id.stateControlHue) {
            if (capability.color <= CapabilityData.NONE) {
                Toast.makeText(parentFragment.getActivity(), R.string.no_support_color, Toast.LENGTH_SHORT).show();
            } else if (saturationSeekBar.getProgress() == 0) {
                Toast.makeText(parentFragment.getActivity(), R.string.saturation_disable_hue, Toast.LENGTH_LONG).show();
            }
        } else if (viewID == R.id.stateControlSaturation) {
            if (capability.color <= CapabilityData.NONE) {
                Toast.makeText(parentFragment.getActivity(), R.string.no_support_color, Toast.LENGTH_LONG).show();
            }
        } else if (viewID == R.id.stateControlColorTemp) {
            if (capability.temp <= CapabilityData.NONE) {
                Toast.makeText(parentFragment.getActivity(), R.string.no_support_temp, Toast.LENGTH_LONG).show();
            } else if (saturationSeekBar.getProgress() == saturationSeekBar.getMax()) {
                Toast.makeText(parentFragment.getActivity(), R.string.saturation_disable_temp, Toast.LENGTH_LONG).show();
            }
        }
    }

}
