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

import java.util.ArrayList;
import java.util.List;
import org.allseen.lsf.PresetPulseEffect;
import org.allseen.lsf.PresetTransitionEffect;
import org.allseen.lsf.Scene;
import org.allseen.lsf.StatePulseEffect;
import org.allseen.lsf.StateTransitionEffect;

public class BasicSceneDataModel extends ItemDataModel {
    public static final char TAG_PREFIX_SCENE = 'S';

    public static String defaultName = "";

    public List<NoEffectDataModel> noEffects;
    public List<TransitionEffectDataModel> transitionEffects;
    public List<PulseEffectDataModel> pulseEffects;

    public BasicSceneDataModel() {
        this("");
    }

    public BasicSceneDataModel(String sceneID) {
        this(sceneID, defaultName);
    }

    public BasicSceneDataModel(String sceneID, String sceneName) {
        super(sceneID, TAG_PREFIX_SCENE, sceneName);

        noEffects = new ArrayList<NoEffectDataModel>();
        transitionEffects = new ArrayList<TransitionEffectDataModel>();
        pulseEffects = new ArrayList<PulseEffectDataModel>();
    }

    public BasicSceneDataModel(BasicSceneDataModel other) {
        super(other);

        this.noEffects = new ArrayList<NoEffectDataModel>(other.noEffects);
        this.transitionEffects = new ArrayList<TransitionEffectDataModel>(other.transitionEffects);
        this.pulseEffects = new ArrayList<PulseEffectDataModel>(other.pulseEffects);
    }

    public boolean containsGroup(String groupID) {
        for (NoEffectDataModel noEffect : noEffects) {
            if (noEffect.containsGroup(groupID)) {
                return true;
            }
        }

        for (TransitionEffectDataModel transitionEffect : transitionEffects) {
            if (transitionEffect.containsGroup(groupID)) {
                return true;
            }
        }

        for (PulseEffectDataModel pulseEffect : pulseEffects) {
            if (pulseEffect.containsGroup(groupID)) {
                return true;
            }
        }

        return false;
    }

    public boolean containsPreset(String presetID) {
        for (NoEffectDataModel noEffect : noEffects) {
            if (noEffect.containsPreset(presetID)) {
                return true;
            }
        }

        for (TransitionEffectDataModel transitionEffect : transitionEffects) {
            if (transitionEffect.containsPreset(presetID)) {
                return true;
            }
        }

        for (PulseEffectDataModel pulseEffect : pulseEffects) {
            if (pulseEffect.containsPreset(presetID)) {
                return true;
            }
        }

        return false;
    }

    public void updateNoEffect(NoEffectDataModel elementModel) {
        updateElement(noEffects, elementModel);
    }

    public void updateTransitionEffect(TransitionEffectDataModel elementModel) {
        updateElement(transitionEffects, elementModel);
    }

    public void updatePulseEffect(PulseEffectDataModel elementModel) {
        updateElement(pulseEffects, elementModel);
    }

    public boolean removeElement(String elementID) {
        return removeElement(noEffects, elementID) || removeElement(transitionEffects, elementID) || removeElement(pulseEffects, elementID);
    }

    protected <T extends BasicSceneElementDataModel> void updateElement(List<T> elementModels, T elementModel) {
        boolean updated = false;

        for (int i = 0; !updated && i < elementModels.size(); i++) {
            if (elementModels.get(i).id.equals(elementModel.id)){
                elementModels.set(i, elementModel);
                updated = true;
            }
        }

        if (!updated) {
            elementModels.add(elementModel);
        }
   }

    protected <T extends BasicSceneElementDataModel> boolean removeElement(List<T> elementModels, String elementID) {
        boolean removed = false;

        for (int i = 0; !removed && i < elementModels.size(); i++) {
            if (elementModels.get(i).id.equals(elementID)){
                elementModels.remove(i);
                removed = true;
            }
        }

        return removed;
   }

    public Scene toScene() {
        Scene scene = new Scene();

        List<StateTransitionEffect> stateTransitionEffects = new ArrayList<StateTransitionEffect>();
        List<PresetTransitionEffect> presetTransitionEffects = new ArrayList<PresetTransitionEffect>();
        List<StatePulseEffect> statePulseEffects = new ArrayList<StatePulseEffect>();
        List<PresetPulseEffect> presetPulseEffects = new ArrayList<PresetPulseEffect>();

        for (int i = 0; i < noEffects.size(); i++) {
            NoEffectDataModel elementModel = noEffects.get(i);

            if (elementModel.presetID == null) {
                StateTransitionEffect stateTransitionEffect = new StateTransitionEffect();

                stateTransitionEffect.setLamps(elementModel.members.getLamps());
                stateTransitionEffect.setLampGroups(elementModel.members.getLampGroups());
                stateTransitionEffect.setLampState(elementModel.state);
                stateTransitionEffect.setTransitionPeriod(0);

                stateTransitionEffects.add(stateTransitionEffect);
            } else {
                PresetTransitionEffect presetTransitionEffect = new PresetTransitionEffect();

                presetTransitionEffect.setLamps(elementModel.members.getLamps());
                presetTransitionEffect.setLampGroups(elementModel.members.getLampGroups());
                presetTransitionEffect.setPresetID(elementModel.presetID);
                presetTransitionEffect.setTransitionPeriod(0);

                presetTransitionEffects.add(presetTransitionEffect);
            }
        }

        for (int i = 0; i < transitionEffects.size(); i++) {
            TransitionEffectDataModel elementModel = transitionEffects.get(i);

            if (elementModel.presetID == null) {
                StateTransitionEffect stateTransitionEffect = new StateTransitionEffect();

                stateTransitionEffect.setLamps(elementModel.members.getLamps());
                stateTransitionEffect.setLampGroups(elementModel.members.getLampGroups());
                stateTransitionEffect.setLampState(elementModel.state);
                stateTransitionEffect.setTransitionPeriod(elementModel.duration);

                stateTransitionEffects.add(stateTransitionEffect);
            } else {
                PresetTransitionEffect presetTransitionEffect = new PresetTransitionEffect();

                presetTransitionEffect.setLamps(elementModel.members.getLamps());
                presetTransitionEffect.setLampGroups(elementModel.members.getLampGroups());
                presetTransitionEffect.setPresetID(elementModel.presetID);

                presetTransitionEffects.add(presetTransitionEffect);
            }
        }

        for (int i = 0; i < pulseEffects.size(); i++) {
            PulseEffectDataModel elementModel = pulseEffects.get(i);

            if (elementModel.presetID == null || elementModel.endPresetID == null) {
                StatePulseEffect statePulseEffect = new StatePulseEffect();

                statePulseEffect.setLamps(elementModel.members.getLamps());
                statePulseEffect.setLampGroups(elementModel.members.getLampGroups());
                statePulseEffect.setFromLampState(elementModel.startWithCurrent ? EmptyLampState.instance : elementModel.state);
                statePulseEffect.setToLampState(elementModel.endState);
                statePulseEffect.setPulseDuration(elementModel.duration);
                statePulseEffect.setPulsePeriod(elementModel.period);
                statePulseEffect.setPulseCount(elementModel.count);

                statePulseEffects.add(statePulseEffect);
            } else {
                PresetPulseEffect presetPulseEffect = new PresetPulseEffect();

                presetPulseEffect.setLamps(elementModel.members.getLamps());
                presetPulseEffect.setLampGroups(elementModel.members.getLampGroups());
                presetPulseEffect.setFromPreset(elementModel.startWithCurrent ? PresetPulseEffect.PRESET_ID_CURRENT_STATE : elementModel.presetID);
                presetPulseEffect.setToPreset(elementModel.endPresetID);
                presetPulseEffect.setPulseDuration(elementModel.duration);
                presetPulseEffect.setPulsePeriod(elementModel.period);
                presetPulseEffect.setPulseCount(elementModel.count);

                presetPulseEffects.add(presetPulseEffect);
            }
        }

        scene.setStateTransitionEffects(stateTransitionEffects.toArray(new StateTransitionEffect[stateTransitionEffects.size()]));
        scene.setPresetTransitionEffects(presetTransitionEffects.toArray(new PresetTransitionEffect[presetTransitionEffects.size()]));
        scene.setStatePulseEffects(statePulseEffects.toArray(new StatePulseEffect[statePulseEffects.size()]));
        scene.setPresetPulseEffects(presetPulseEffects.toArray(new PresetPulseEffect[presetPulseEffects.size()]));

        return scene;
    }

    public void fromScene(Scene scene) {
        StateTransitionEffect[] stateTransitionEffects = scene.getStateTransitionEffects();
        PresetTransitionEffect[] presetTransitionEffects = scene.getPresetTransitionEffects();
        StatePulseEffect[] statePulseEffects = scene.getStatePulseEffects();
        PresetPulseEffect[] presetPulseEffects = scene.getPresetPulseEffects();

        noEffects.clear();
        transitionEffects.clear();
        pulseEffects.clear();

        for (int i = 0; i < stateTransitionEffects.length; i++) {
            StateTransitionEffect stateTransitionEffect = stateTransitionEffects[i];

            if (stateTransitionEffect.getTransitionPeriod() == 0) {
                NoEffectDataModel elementModel = new NoEffectDataModel();

                // The elementModel.presetID field is left at the default value
                // set by the NoEffectDataModel constructor.
                elementModel.members.setLamps(stateTransitionEffect.getLamps());
                elementModel.members.setLampGroups(stateTransitionEffect.getLampGroups());
                elementModel.state = stateTransitionEffect.getLampState();

                noEffects.add(elementModel);
            } else {
                TransitionEffectDataModel elementModel = new TransitionEffectDataModel();

                // The elementModel.presetID field is left at the default value
                // set by the TransitionEffectDataModel constructor.
                elementModel.members.setLamps(stateTransitionEffect.getLamps());
                elementModel.members.setLampGroups(stateTransitionEffect.getLampGroups());
                elementModel.state = stateTransitionEffect.getLampState();
                elementModel.duration = stateTransitionEffect.getTransitionPeriod();

                transitionEffects.add(elementModel);
            }
        }

        for (int i = 0; i < presetTransitionEffects.length; i++) {
            PresetTransitionEffect presetTransitionEffect = presetTransitionEffects[i];

            if (presetTransitionEffect.getTransitionPeriod() == 0) {
                NoEffectDataModel elementModel = new NoEffectDataModel();

                // The elementModel.state field is left at the default value
                // set by the NoEffectDataModel constructor.
                //
                // We delay copying the state because the preset may not have been
                // received yet. See BasicSceneElementInfoFragment.onCreateView().
                elementModel.members.setLamps(presetTransitionEffect.getLamps());
                elementModel.members.setLampGroups(presetTransitionEffect.getLampGroups());
                elementModel.presetID = presetTransitionEffect.getPresetID();

                noEffects.add(elementModel);
            } else {
                TransitionEffectDataModel elementModel = new TransitionEffectDataModel();

                // The elementModel.state field is left at the default value
                // set by the TransitionEffectDataModel constructor.
                //
                // We delay copying the state because the preset may not have been
                // received yet. See BasicSceneElementInfoFragment.onCreateView().
                elementModel.members.setLamps(presetTransitionEffect.getLamps());
                elementModel.members.setLampGroups(presetTransitionEffect.getLampGroups());
                elementModel.presetID = presetTransitionEffect.getPresetID();
                elementModel.duration = presetTransitionEffect.getTransitionPeriod();

                transitionEffects.add(elementModel);
            }
        }

        for (int i = 0; i < statePulseEffects.length; i++) {
            StatePulseEffect statePulseEffect = statePulseEffects[i];
            PulseEffectDataModel elementModel = new PulseEffectDataModel();

            // The elementModel.presetID and elementModel.endPresetID fields are
            // left at the default values set by the PulseEffectDataModel constructor.
            elementModel.members.setLamps(statePulseEffect.getLamps());
            elementModel.members.setLampGroups(statePulseEffect.getLampGroups());
            elementModel.state = statePulseEffect.getFromLampState();
            elementModel.endState = statePulseEffect.getToLampState();
            elementModel.startWithCurrent = elementModel.state.isNull();
            elementModel.period = statePulseEffect.getPulsePeriod();
            elementModel.duration = statePulseEffect.getPulseDuration();
            elementModel.count = statePulseEffect.getPulseCount();

            pulseEffects.add(elementModel);
        }

        for (int i = 0; i < presetPulseEffects.length; i++) {
            PresetPulseEffect presetPulseEffect = presetPulseEffects[i];
            PulseEffectDataModel elementModel = new PulseEffectDataModel();

            // The elementModel.state and elementModel.endState fields are
            // left at the default values set by the PulseEffectDataModel
            // constructor.
            //
            // We delay copying the state because the preset may not have been
            // received yet. See BasicSceneElementInfoFragment.onCreateView()
            // and PulseEffectFragment.onCreateView().
            elementModel.members.setLamps(presetPulseEffect.getLamps());
            elementModel.members.setLampGroups(presetPulseEffect.getLampGroups());
            elementModel.presetID = presetPulseEffect.getFromPreset();
            elementModel.endPresetID = presetPulseEffect.getToPreset();
            elementModel.startWithCurrent = PresetPulseEffect.PRESET_ID_CURRENT_STATE.equals(elementModel.presetID);
            elementModel.period = presetPulseEffect.getPulsePeriod();
            elementModel.duration = presetPulseEffect.getPulseDuration();
            elementModel.count = presetPulseEffect.getPulseCount();

            pulseEffects.add(elementModel);
        }
    }
}
