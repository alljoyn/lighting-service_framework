/******************************************************************************
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
 ******************************************************************************/

#include <ServiceDescription.h>

#define QCC_MODULE "SERVICE_DESCRIPTION"

namespace lsf {

const std::string ControllerServiceDescription =
    "<node>"
    "  <interface name='org.allseen.LSF.ControllerService'>"
    "  <property name='Version' type='u' access='read'/>"
    "    <method name='LightingResetControllerService'>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "    </method>"
    "    <method name='GetControllerServiceVersion'>"
    "      <arg name='version' type='u' direction='out'/>"
    "    </method>"
    "    <signal name='ControllerServiceLightingReset'>"
    "    </signal>"
    "  </interface>"
    "</node>";

const std::string ControllerServiceLampDescription =
    "<node>"
    "  <interface name='org.allseen.LSF.ControllerService.Lamp'>"
    "    <property name='Version' type='u' access='read'/>"
    "    <method name='GetAllLampIDs'>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampIDs' type='as' direction='out'/>"
    "    </method>"
    "    <method name='GetLampSupportedLanguages'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='supportedLanguages' type='as' direction='out'/>"
    "    </method>"
    "    <method name='GetLampManufacturer'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='language' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='language' type='s' direction='out'/>"
    "      <arg name='manufacturer' type='s' direction='out'/>"
    "    </method>"
    "    <method name='GetLampName'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='language' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='language' type='s' direction='out'/>"
    "      <arg name='lampName' type='s' direction='out'/>"
    "    </method>"
    "    <method name='SetLampName'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='lampName' type='s' direction='in'/>"
    "      <arg name='language' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='language' type='s' direction='out'/>"
    "    </method>"
    "    <method name='GetLampDetails'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='lampDetails' type='a{sv}' direction='out'/>"
    "    </method>"
    "    <method name='GetLampParameters'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='lampParameters' type='a{sv}' direction='out'/>"
    "    </method>"
    "    <method name='GetLampParametersField'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='lampParameterFieldName' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='lampParameterFieldName' type='s' direction='out'/>"
    "      <arg name='lampParameterFieldValue' type='v' direction='out'/>"
    "    </method>"
    "    <method name='GetLampState'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='lampState' type='a{sv}' direction='out'/>"
    "    </method>"
    "    <method name='GetLampStateField'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='lampStateFieldName' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='lampStateFieldName' type='s' direction='out'/>"
    "      <arg name='lampStateFieldValue' type='v' direction='out'/>"
    "    </method>"
    "    <method name='TransitionLampState'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='lampState' type='a{sv}' direction='in'/>"
    "      <arg name='transitionPeriod' type='u' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='PulseLampWithState'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='fromLampState' type='a{sv}' direction='in'/>"
    "      <arg name='toLampState' type='a{sv}' direction='in'/>"
    "      <arg name='period' type='u' direction='in'/>"
    "      <arg name='duration' type='u' direction='in'/>"
    "      <arg name='numPulses' type='u' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='PulseLampWithPreset'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='fromPresetID' type='s' direction='in'/>"
    "      <arg name='toPresetID' type='s' direction='in'/>"
    "      <arg name='period' type='u' direction='in'/>"
    "      <arg name='duration' type='u' direction='in'/>"
    "      <arg name='numPulses' type='u' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='TransitionLampStateToPreset'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='presetID' type='s' direction='in'/>"
    "      <arg name='transitionPeriod' type='u' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='TransitionLampStateField'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='lampStateFieldName' type='s' direction='in'/>"
    "      <arg name='lampStateFieldValue' type='v' direction='in'/>"
    "      <arg name='transitionPeriod' type='u' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='lampStateFieldName' type='s' direction='out'/>"
    "    </method>"
    "    <method name='ResetLampState'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='ResetLampStateField'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='lampStateFieldName' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='lampStateFieldName' type='s' direction='out'/>"
    "    </method>"
    "    <method name='GetLampFaults'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='lampFaults' type='au' direction='out'/>"
    "    </method>"
    "    <method name='ClearLampFault'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='lampFault' type='u' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='lampFault' type='u' direction='out'/>"
    "    </method>"
    "    <method name='GetLampServiceVersion'>"
    "      <arg name='lampID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampID' type='s' direction='out'/>"
    "      <arg name='lampServiceVersion' type='u' direction='out'/>"
    "    </method>"
    "    <signal name='LampsNameChanged'>"
    "      <arg name='lampIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='LampsStateChanged'>"
    "      <arg name='lampIDs' type='as' direction='out'/>"
    "    </signal>"
    "  </interface>"
    "</node>";

const std::string ControllerServiceLampGroupDescription =
    "<node>"
    "  <interface name='org.allseen.LSF.ControllerService.LampGroup'>"
    "  <property name='Version' type='u' access='read'/>"
    "    <method name='GetAllLampGroupIDs'>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupIDs' type='as' direction='out'/>"
    "    </method>"
    "    <method name='GetLampGroupName'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='language' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "      <arg name='language' type='s' direction='out'/>"
    "      <arg name='lampGroupName' type='s' direction='out'/>"
    "    </method>"
    "    <method name='SetLampGroupName'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='lampGroupName' type='s' direction='in'/>"
    "      <arg name='language' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "      <arg name='language' type='s' direction='out'/>"
    "    </method>"
    "    <method name='CreateLampGroup'>"
    "      <arg name='lampIDs' type='as' direction='in'/>"
    "      <arg name='lampGroupIDs' type='as' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='UpdateLampGroup'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='lampIDs' type='as' direction='in'/>"
    "      <arg name='lampGroupIDs' type='as' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='DeleteLampGroup'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='GetLampGroup'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "      <arg name='lampIDs' type='as' direction='out'/>"
    "      <arg name='lampGroupIDs' type='as' direction='out'/>"
    "    </method>"
    "    <method name='TransitionLampGroupState'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='lampState' type='a{sv}' direction='in'/>"
    "      <arg name='transitionPeriod' type='u' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='PulseLampGroupWithState'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='fromLampGroupState' type='a{sv}' direction='in'/>"
    "      <arg name='toLampGroupState' type='a{sv}' direction='in'/>"
    "      <arg name='period' type='u' direction='in'/>"
    "      <arg name='duration' type='u' direction='in'/>"
    "      <arg name='numPulses' type='u' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='PulseLampGroupWithPreset'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='fromPresetID' type='s' direction='in'/>"
    "      <arg name='toPresetID' type='s' direction='in'/>"
    "      <arg name='period' type='u' direction='in'/>"
    "      <arg name='duration' type='u' direction='in'/>"
    "      <arg name='numPulses' type='u' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='TransitionLampGroupStateToPreset'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='presetID' type='s' direction='in'/>"
    "      <arg name='transitionPeriod' type='u' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='TransitionLampGroupStateField'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='lampGroupStateFieldName' type='s' direction='in'/>"
    "      <arg name='lampGroupStateFieldValue' type='v' direction='in'/>"
    "      <arg name='transitionPeriod' type='u' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "      <arg name='lampGroupStateFieldName' type='s' direction='out'/>"
    "    </method>"
    "    <method name='ResetLampGroupState'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='ResetLampGroupStateField'>"
    "      <arg name='lampGroupID' type='s' direction='in'/>"
    "      <arg name='lampGroupStateFieldName' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampGroupID' type='s' direction='out'/>"
    "      <arg name='lampGroupStateFieldName' type='s' direction='out'/>"
    "    </method>"
    "    <signal name='LampGroupsNameChanged'>"
    "      <arg name='lampGroupsIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='LampGroupsCreated'>"
    "      <arg name='lampGroupsIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='LampGroupsUpdated'>"
    "      <arg name='lampGroupsIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='LampGroupsDeleted'>"
    "      <arg name='lampGroupsIDs' type='as' direction='out'/>"
    "    </signal>"
    "  </interface>"
    "</node>";

const std::string ControllerServicePresetDescription =
    "<node>"
    "  <interface name='org.allseen.LSF.ControllerService.Preset'>"
    "  <property name='Version' type='u' access='read'/>"
    "    <method name='GetDefaultLampState'>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='lampState' type='a{sv}' direction='out'/>"
    "    </method>"
    "    <method name='SetDefaultLampState'>"
    "      <arg name='lampState' type='a{sv}' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "    </method>"
    "    <method name='GetAllPresetIDs'>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='presetIDs' type='as' direction='out'/>"
    "    </method>"
    "    <method name='GetPresetName'>"
    "      <arg name='presetID' type='s' direction='in'/>"
    "      <arg name='language' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='presetID' type='s' direction='out'/>"
    "      <arg name='language' type='s' direction='out'/>"
    "      <arg name='presetName' type='s' direction='out'/>"
    "    </method>"
    "    <method name='SetPresetName'>"
    "      <arg name='presetID' type='s' direction='in'/>"
    "      <arg name='presetName' type='s' direction='in'/>"
    "      <arg name='language' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='presetID' type='s' direction='out'/>"
    "      <arg name='language' type='s' direction='out'/>"
    "    </method>"
    "    <method name='CreatePreset'>"
    "      <arg name='lampState' type='a{sv}' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='presetID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='UpdatePreset'>"
    "      <arg name='presetID' type='s' direction='in'/>"
    "      <arg name='lampState' type='a{sv}' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='presetID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='DeletePreset'>"
    "      <arg name='presetID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='presetID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='GetPreset'>"
    "      <arg name='presetID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='presetID' type='s' direction='out'/>"
    "      <arg name='lampState' type='a{sv}' direction='out'/>"
    "    </method>"
    "    <signal name='DefaultLampStateChanged'>"
    "    </signal>"
    "    <signal name='PresetsNameChanged'>"
    "      <arg name='presetIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='PresetsCreated'>"
    "      <arg name='presetsIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='PresetsUpdated'>"
    "      <arg name='presetsIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='PresetsDeleted'>"
    "      <arg name='presetsIDs' type='as' direction='out'/>"
    "    </signal>"
    "  </interface>"
    "</node>";

const std::string ControllerServiceSceneDescription =
    "<node>"
    "  <interface name='org.allseen.LSF.ControllerService.Scene'>"
    "  <property name='Version' type='u' access='read'/>"
    "    <method name='GetAllSceneIDs'>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='sceneIDs' type='as' direction='out'/>"
    "    </method>"
    "    <method name='GetSceneName'>"
    "      <arg name='sceneID' type='s' direction='in'/>"
    "      <arg name='language' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='sceneID' type='s' direction='out'/>"
    "      <arg name='language' type='s' direction='out'/>"
    "      <arg name='sceneName' type='s' direction='out'/>"
    "    </method>"
    "    <method name='SetSceneName'>"
    "      <arg name='sceneID' type='s' direction='in'/>"
    "      <arg name='SceneName' type='s' direction='in'/>"
    "      <arg name='language' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='sceneID' type='s' direction='out'/>"
    "      <arg name='language' type='s' direction='out'/>"
    "    </method>"
    "    <method name='CreateScene'>"
    "      <arg name='transitionlampsLampGroupsToState' type='a(asasa{sv}u)' direction='in'/>"
    "      <arg name='transitionlampsLampGroupsToPreset' type='a(asassu)' direction='in'/>"
    "      <arg name='pulselampsLampGroupsWithState' type='a(asasa{sv}a{sv}uuu)' direction='in'/>"
    "      <arg name='pulselampsLampGroupsWithPreset' type='a(asasssuuu)' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='sceneID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='UpdateScene'>"
    "      <arg name='sceneID' type='s' direction='in'/>"
    "      <arg name='transitionlampsLampGroupsToState' type='a(asasa{sv}u)' direction='in'/>"
    "      <arg name='transitionlampsLampGroupsToPreset' type='a(asassu)' direction='in'/>"
    "      <arg name='pulselampsLampGroupsWithState' type='a(asasa{sv}a{sv}uuu)' direction='in'/>"
    "      <arg name='pulselampsLampGroupsWithPreset' type='a(asasssuuu)' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='sceneID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='DeleteScene'>"
    "      <arg name='sceneID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='sceneID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='GetScene'>"
    "      <arg name='sceneID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='sceneID' type='s' direction='out'/>"
    "      <arg name='transitionlampsLampGroupsToState' type='a(asasa{sv}u)' direction='out'/>"
    "      <arg name='transitionlampsLampGroupsToPreset' type='a(asassu)' direction='out'/>"
    "      <arg name='pulselampsLampGroupsWithState' type='a(asasa{sv}a{sv}uuu)' direction='out'/>"
    "      <arg name='pulselampsLampGroupsWithPreset' type='a(asasssuuu)' direction='out'/>"
    "    </method>"
    "    <method name='ApplyScene'>"
    "      <arg name='sceneID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='sceneID' type='s' direction='out'/>"
    "    </method>"
    "    <signal name='ScenesNameChanged'>"
    "      <arg name='sceneIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='ScenesCreated'>"
    "      <arg name='sceneIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='ScenesUpdated'>"
    "      <arg name='sceneIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='ScenesDeleted'>"
    "      <arg name='sceneIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='ScenesApplied'>"
    "      <arg name='sceneIDs' type='as' direction='out'/>"
    "    </signal>"
    "  </interface>"
    "</node>";

const std::string ControllerServiceMasterSceneDescription =
    "<node>"
    "  <interface name='org.allseen.LSF.ControllerService.MasterScene'>"
    "  <property name='Version' type='u' access='read'/>"
    "    <method name='GetAllMasterSceneIDs'>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='masterSceneIDs' type='as' direction='out'/>"
    "    </method>"
    "    <method name='GetMasterSceneName'>"
    "      <arg name='masterSceneID' type='s' direction='in'/>"
    "      <arg name='language' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='masterSceneID' type='s' direction='out'/>"
    "      <arg name='language' type='s' direction='out'/>"
    "      <arg name='masterSceneName' type='s' direction='out'/>"
    "    </method>"
    "    <method name='SetMasterSceneName'>"
    "      <arg name='masterSceneID' type='s' direction='in'/>"
    "      <arg name='masterSceneName' type='s' direction='in'/>"
    "      <arg name='language' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='masterSceneID' type='s' direction='out'/>"
    "      <arg name='language' type='s' direction='out'/>"
    "    </method>"
    "    <method name='CreateMasterScene'>"
    "      <arg name='scenes' type='as' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='masterSceneID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='UpdateMasterScene'>"
    "      <arg name='masterSceneID' type='s' direction='in'/>"
    "      <arg name='scenes' type='as' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='masterSceneID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='DeleteMasterScene'>"
    "      <arg name='masterSceneID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='masterSceneID' type='s' direction='out'/>"
    "    </method>"
    "    <method name='GetMasterScene'>"
    "      <arg name='masterSceneID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='masterSceneID' type='s' direction='out'/>"
    "      <arg name='scenes' type='as' direction='out'/>"
    "    </method>"
    "    <method name='ApplyMasterScene'>"
    "      <arg name='masterSceneID' type='s' direction='in'/>"
    "      <arg name='responseCode' type='u' direction='out'/>"
    "      <arg name='masterSceneID' type='s' direction='out'/>"
    "    </method>"
    "    <signal name='MasterScenesNameChanged'>"
    "      <arg name='masterSceneIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='MasterScenesCreated'>"
    "      <arg name='masterSceneIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='MasterScenesUpdated'>"
    "      <arg name='masterSceneIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='MasterScenesDeleted'>"
    "      <arg name='masterSceneIDs' type='as' direction='out'/>"
    "    </signal>"
    "    <signal name='MasterScenesApplied'>"
    "      <arg name='masterSceneIDs' type='as' direction='out'/>"
    "    </signal>"
    "  </interface>"
    "</node>";

const std::string LeaderElectionAndStateSyncDescription =
    "<node>"
    "  <interface name='org.allseen.LeaderElectionAndStateSync'>"
    "  <property name='Version' type='u' access='read'/>"
    "    <method name='GetChecksumAndModificationTimestamp'>"
    "      <arg name='checksumAndModificationTimestamp' type='a(uut)' direction='out'/>"
    "    </method>"
    "    <method name='GetBlob'>"
    "      <arg name='blobType' type='u' direction='in'/>"
    "      <arg name='blob' type='s' direction='out'/>"
    "      <arg name='checksum' type='u' direction='out'/>"
    "      <arg name='modificationTimestamp' type='t' direction='out'/>"
    "    </method>"
    "    <signal name='BlobChanged'>"
    "      <arg name='checksumAndModificationTimestamp' type='a(uut)' direction='out'/>"
    "    </signal>"
    "  </interface>"
    "</node>";
}
