#ifndef _LSF_TYPES_H_
#define _LSF_TYPES_H_
/**
 * \ingroup Common
 */
/**
 * \file  common/inc/LSFTypes.h
 * This file provides definitions for LSF types
 */
/******************************************************************************
 *    Copyright (c) Open Connectivity Foundation (OCF), AllJoyn Open Source
 *    Project (AJOSP) Contributors and others.
 *    
 *    SPDX-License-Identifier: Apache-2.0
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0
 *    
 *    Copyright (c) Open Connectivity Foundation and Contributors to AllSeen
 *    Alliance. All rights reserved.
 *    
 *    Permission to use, copy, modify, and/or distribute this software for
 *    any purpose with or without fee is hereby granted, provided that the
 *    above copyright notice and this permission notice appear in all
 *    copies.
 *    
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *    WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *    AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *    DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *    PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *    TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *    PERFORMANCE OF THIS SOFTWARE.
******************************************************************************/
/**
 * \ingroup Common
 */
#include <memory>
#include <string>
#include <string.h>
#include <map>
#include <list>
#include <set>

#include <alljoyn/MsgArg.h>
#include <alljoyn/Session.h>
#include <qcc/StringUtil.h>

#include <LampValues.h>
#include <LSFResponseCodes.h>

using namespace qcc;
using namespace ajn;

#define LSF_CASE(_case) case _case: return # _case

namespace lsf {
/*
 * Typedef for LSFString type
 */
typedef std::string LSFString;

/**
 * Typedef for LSFStringList type
 */
typedef std::list<LSFString> LSFStringList;

/**
 * Typedef for LampFaultCodeList type
 */
typedef std::list<LampFaultCode> LampFaultCodeList;

/**
 * Typedef for LampNameMap type
 */
typedef std::map<LSFString, LSFString> LampNameMap;

/**
 * String used as an identifier for the current state Preset
 */
extern const LSFString CurrentStateIdentifier;

/**
 * String used as an identifier for the All Lamps Group
 */
extern const LSFString AllLampsGroupIdentifier;

/**
 * Controller Service Object Path
 */
extern const char* ControllerServiceObjectPath;

/**
 * Controller Service Interface Name
 */
extern const char* ControllerServiceInterfaceName;

/**
 * Controller Service Lamp Interface Name
 */
extern const char* ControllerServiceLampInterfaceName;

/**
 * Controller Service LampGroup Interface Name
 */
extern const char* ControllerServiceLampGroupInterfaceName;

/**
 * Controller Service Preset Interface Name
 */
extern const char* ControllerServicePresetInterfaceName;

/**
 * Controller Service Transition Effect Interface Name
 */
extern const char* ControllerServiceTransitionEffectInterfaceName;

/**
 * Controller Service Pulse Effect Interface Name
 */
extern const char* ControllerServicePulseEffectInterfaceName;

/**
 * Controller Service Scene Interface Name
 */
extern const char* ControllerServiceSceneInterfaceName;

/**
 * Controller Service Scene 1.1 Interface Name
 */
extern const char* ControllerServiceSceneWithSceneElementsInterfaceName;

/**
 * Controller Service Scene Element Interface Name
 */
extern const char* ControllerServiceSceneElementInterfaceName;

/**
 * Controller Service Master Scene Interface Name
 */
extern const char* ControllerServiceMasterSceneInterfaceName;

/**
 * Controller Service Data Set Interface Name
 */
extern const char* ControllerServiceDataSetInterfaceName;

/**
 * Controller Service Session Port
 */
extern ajn::SessionPort ControllerServiceSessionPort;

/**
 * Controller Service Interface Version
 */
extern const uint32_t ControllerServiceInterfaceVersion;

/**
 * Controller Service Lamp Interface Version
 */
extern const uint32_t ControllerServiceLampInterfaceVersion;

/**
 * Controller Service Lamp Group Interface Version
 */
extern const uint32_t ControllerServiceLampGroupInterfaceVersion;

/**
 * Controller Service Preset Interface Version
 */
extern const uint32_t ControllerServicePresetInterfaceVersion;

/**
 * Controller Service Transition Effect Interface Version
 */
extern const uint32_t ControllerServiceTransitionEffectInterfaceVersion;

/**
 * Controller Service Pulse Effect Interface Version
 */
extern const uint32_t ControllerServicePulseEffectInterfaceVersion;

/**
 * Controller Service Scene Interface Version
 */
extern const uint32_t ControllerServiceSceneInterfaceVersion;

/**
 * Controller Service Scene 1.1 Interface Version
 */
extern const uint32_t ControllerServiceSceneWithSceneElementsInterfaceVersion;

/**
 * Controller Service Scene Element Interface Version
 */
extern const uint32_t ControllerServiceSceneElementInterfaceVersion;

/**
 * Controller Service Master Scene Interface Version
 */
extern const uint32_t ControllerServiceMasterSceneInterfaceVersion;

/**
 * Controller Service Leader Election And State Sync Interface Version
 */
extern const uint32_t ControllerServiceLeaderElectionAndStateSyncInterfaceVersion;

/**
 * Controller Service Data Set Interface Version
 */
extern const uint32_t ControllerServiceDataSetInterfaceVersion;

/**
 * Lamp Service Object Path
 */
extern const char* LampServiceObjectPath;

/**
 * Lamp Service Interface Name
 */
extern const char* LampServiceInterfaceName;

/**
 * Lamp Service State Interface Name
 */
extern const char* LampServiceStateInterfaceName;

/**
 * Lamp Service Parameters Interface Name
 */
extern const char* LampServiceParametersInterfaceName;

/**
 * Lamp Service Details Interface Name
 */
extern const char* LampServiceDetailsInterfaceName;

/**
 * Lamp Service Session Port
 */
extern ajn::SessionPort LampServiceSessionPort;

/**
 * Config Service Object Path
 */
extern const char* ConfigServiceObjectPath;

/**
 * Config Service Interface Name
 */
extern const char* ConfigServiceInterfaceName;

/**
 * Onboarding Service Object Path
 */
extern const char* OnboardingServiceObjectPath;

/**
 * Onboarding Service Interface Name
 */
extern const char* OnboardingServiceInterfaceName;

/**
 * About Interface Name
 */
extern const char* AboutInterfaceName;

/**
 * Leader Election And State Sync Object Path
 */
extern const char* LeaderElectionAndStateSyncObjectPath;

/**
 * Leader Election And State Sync Interface Name
 */
extern const char* LeaderElectionAndStateSyncInterfaceName;

/**
 * Apply Scene Event Action Interface Name
 */
extern const char* ApplySceneEventActionInterfaceName;

/**
 * Apply Scene Event Action Object Path
 */
extern const char* ApplySceneEventActionObjectPath;

/**
 * Returns the current system timestamp in ms
 */
uint64_t GetTimestampInMs(void);

/**
 * Returns the current system timestamp in seconds
 */
uint32_t GetTimestampInSeconds(void);

/**
 * Enum defining the LSF Blob Type
 */
typedef enum _LSFBlobType {
    LSF_PRESET = 0,                        /**< Preset type */
    LSF_LAMP_GROUP = 1,                    /**< Lamp Group type */
    LSF_SCENE = 2,                         /**< Scene type */
    LSF_MASTER_SCENE = 3,                  /**< Master Scene type */
    LSF_TRANSITION_EFFECT = 4,             /**< Transition Effect type */
    LSF_PULSE_EFFECT = 5,                  /**< Pulse Effect type */
    LSF_PRESET_UPDATE = 6,                 /**< Preset Update type */
    LSF_LAMP_GROUP_UPDATE = 7,             /**< Lamp Group Update type */
    LSF_SCENE_UPDATE = 8,                  /**< Scene Update type */
    LSF_MASTER_SCENE_UPDATE = 9,           /**< Master Scene Update type */
    LSF_TRANSITION_EFFECT_UPDATE = 10,     /**< Transition Effect Update type */
    LSF_PULSE_EFFECT_UPDATE = 11,          /**< Pulse Effect Update type */
    LSF_SCENE_ELEMENT = 12,                /**< Scene Element type */
    LSF_SCENE_2 = 13,                      /**< Scene type in and after 15.04 i.e. Version 2 of the Controller Service*/
    LSF_BLOB_TYPE_LAST_VALUE = 0xFFFFFFFF  /**< Last value */
} LSFBlobType;

/**
 * Creates a unique list from the input list by removing duplicate entries
 *
 * @param uniqueList Container to return the unique list in
 * @param fromList   Input list
 *
 * @return None
 */
void CreateUniqueList(LSFStringList& uniqueList, LSFStringList& fromList);

/**
 * Creates a unique list from the input array by removing duplicate entries
 *
 * @param uniqueList Container to return the unique list in
 * @param idsArray   Array of ids
 * @param idsSize    Size of the array
 *
 * @return None
 */
void CreateUniqueList(LSFStringList& uniqueList, ajn::MsgArg* idsArray, size_t idsSize);

/**
 * Creates new duplicate string from the input string
 *
 * @param str Input string
 *
 * @return New duplicate string
 */
char* strdupnew(const char* str);

/**
 * Class defining the Lamp State \n
 * Lamp State is the state information that the Lamp persists through power cycle that includes End-User set state attributes such as Hue, Saturation, Color Temperature, and Brightness.
 */
class LampState {

  public:
    /**
     * Contructor
     */
    LampState();

    /**
     * Parameterized Contructor
     *
     * @param onOff      ON/OFF field
     * @param hue        Hue
     * @param saturation Saturation
     * @param colorTemp  Color Temperature
     * @param brightness Brightness
     */
    LampState(bool onOff, uint32_t hue, uint32_t saturation, uint32_t colorTemp, uint32_t brightness);

    /**
     * Parameterized Contructor
     *
     * @param arg Msgarg holding a dictionary
     */
    LampState(const ajn::MsgArg& arg);

    /**
     * Copy Contructor
     *
     * @param other Lamp State to copy from
     */
    LampState(const LampState& other);

    /**
     * Assignment operator
     *
     * @param other Lamp State to assign from
     */
    LampState& operator=(const LampState& other);

    /**
     * Return the details of the Lamp State as a string
     *
     * @return string
     */
    const char* c_str(void) const;

    /**
     * Set the Lamp State
     *
     * @param arg Msgarg holding a dictionary
     */
    void Set(const ajn::MsgArg& arg);

    /**
     * Get the Lamp State as a MsgArg
     *
     * @param arg*       Msgarg holding a dictionary
     * @param ownership  Set ownership flag in the MsgArg
     */
    void Get(ajn::MsgArg* arg, bool ownership = false) const;

    /**
     * ON/OFF
     */
    bool onOff;

    /**
     * Hue
     */
    uint32_t hue;

    /**
     * Saturation
     */
    uint32_t saturation;

    /**
     * Color Temperature
     */
    uint32_t colorTemp;

    /**
     * Brightness
     */
    uint32_t brightness;

    /**
     * Indicates if the state is a NULL state
     */
    bool nullState;

};

/**
 * Typedef for PresetMap type. \n
 * The key of the map is the preset id which is auto generated. \n
 * The value of the map is a pair that contains: \n
 *      Key - user defined name of the preset
 *      Value - The lamp state as a preset value
 */
typedef std::map<LSFString, std::pair<LSFString, LampState> > PresetMap;

/**
 * Class defining the Lamp Parameters \n
 * Lamp parameters are read-only volatile parameters that are read from the Lamp hardware. This consists of parameters like Lamp Output and Power Draw.
 */
class LampParameters {

  public:
    /**
     * Constructor
     */
    LampParameters();

    /**
     * Parameterized Constructor
     *
     * @param arg MsgArg with a dictionary
     */
    LampParameters(const ajn::MsgArg& arg);

    /**
     * Copy Constructor
     *
     * @param other Lamp Parameters to copy from
     */
    LampParameters(const LampParameters& other);

    /**
     * Assignment operator
     *
     * @param other Lamp Parameters to assign from
     */
    LampParameters& operator=(const LampParameters& other);

    /**
     * Return the details of the Lamp Parameters as a string
     *
     * @return string
     */
    const char* c_str(void) const;

    /**
     * Set the Lamp Parameters
     *
     * @param arg Msgarg holding a dictionary
     */
    void Set(const ajn::MsgArg& arg);

    /**
     * Get the Lamp Parameters as a MsgArg
     *
     * @param arg*       Msgarg holding a dictionary
     * @param ownership  Set ownership flag in the MsgArg
     */
    void Get(ajn::MsgArg* arg, bool ownership = false) const;

    /**
     * Energy Usage in  Milliwatts
     */
    uint32_t energyUsageMilliwatts;

    /**
     * Brightness in Lumens
     */
    uint32_t lumens;
};

/**
 * Class defining the Lamp Details \n
 * Lamp Details are the LSF-specific data that the Lamp exposes via a Lamp Service interface such that information about the Lamp can be introspected via a Lighting Controller Service. \n
 * Lamp Details are read-only and set at the time of manufacturing.
 */
class LampDetails {

  public:

    /**
     * Constructor
     */
    LampDetails();

    /**
     * Parameterized Constructor
     *
     * @param arg MsgArg with a dictionary
     */
    LampDetails(const ajn::MsgArg& arg);

    /**
     * Copy Constructor
     *
     * @param other Lamp Details to copy from
     */
    LampDetails(const LampDetails& other);

    /**
     * Assignment operator
     *
     * @param other Lamp Details to assign from
     */
    LampDetails& operator=(const LampDetails& other);

    /**
     * Return the details of the Lamp Details as a string
     *
     * @return string
     */
    const char* c_str(void) const;

    /**
     * Set the Lamp Details
     *
     * @param arg Msgarg holding a dictionary
     */
    void Set(const ajn::MsgArg& arg);

    /**
     * Get the Lamp Details as a MsgArg
     *
     * @param arg*       Msgarg holding a dictionary
     * @param ownership  Set ownership flag in the MsgArg
     */
    void Get(ajn::MsgArg* arg, bool ownership = false) const;

    /**
     * Lamp Make
     */
    LampMake make;

    /**
     * Lamp Model
     */
    LampModel model;

    /**
     * Device Type
     */
    DeviceType type;

    /**
     * Lamp Type
     */
    LampType lampType;

    /**
     * Lamp Base Type
     */
    BaseType lampBaseType;

    /**
     * Lamp Beam Angle
     */
    uint32_t lampBeamAngle;

    /**
     * Denotes if the lamp is dimmable
     */
    bool dimmable;

    /**
     * Denotes if the lamp supports color
     */
    bool color;

    /**
     * Denotes if the lamp supports variable color temperature
     */
    bool variableColorTemp;

    /**
     * Denotes if the lamp supports effects
     */
    bool hasEffects;

    /**
     * Max Voltage
     */
    uint32_t maxVoltage;

    /**
     * Min Voltage
     */
    uint32_t minVoltage;

    /**
     * Wattage
     */
    uint32_t wattage;

    /**
     * Incandescent Equivalent
     */
    uint32_t incandescentEquivalent;

    /**
     * Max Lumens
     */
    uint32_t maxLumens;

    /**
     * Min Temperature
     */
    uint32_t minTemperature;

    /**
     * Max Temperature
     */
    uint32_t maxTemperature;

    /**
     * Color Rendering Index
     */
    uint32_t colorRenderingIndex;

    /**
     * Lamp ID
     */
    LSFString lampID;
};

/**
 * Class defining a Lamp Group \n
 * A Lamp Group is a logical grouping of Lamps allowing them to be controlled simultaneously as if they are a single Lamp. \n
 * A Lamp Group can be a part of another Lamp Group. \n
 * Lamp Groups are persisted through power cycles in the Lighting Controller Service.
 */
class LampGroup {
  public:

    /**
     * Constructor
     */
    LampGroup();

    /**
     * Parameterized Constructor
     *
     * @param lampList      MsgArg with a array of Lamp IDs
     * @param lampGroupList MsgArg with a array of Lamp Group IDs
     */
    LampGroup(const ajn::MsgArg& lampList, const ajn::MsgArg& lampGroupList);

    /**
     * Parameterized Constructor
     *
     * @param lampList      List of Lamp IDs
     * @param lampGroupList List of Lamp Group IDs
     */
    LampGroup(LSFStringList lampList, LSFStringList lampGroupList);

    /**
     * Destructor
     */
    ~LampGroup() {
        lamps.clear();
        lampGroups.clear();
    }

    /**
     * Return the details of the Lamp Group as a string
     *
     * @return string
     */
    const char* c_str(void) const;

    /**
     * Copy Constructor
     *
     * @param other Lamp Group to copy from
     */
    LampGroup(const LampGroup& other);

    /**
     * Assignment operator
     *
     * @param other Lamp Group to assign from
     */
    LampGroup& operator=(const LampGroup& other);

    /**
     * Set the Lamp Group
     *
     * @param lampList      Msgarg with an array of Lamp IDs
     * @param lampGroupList Msgarg with an array of Lamp Group IDs
     */
    void Set(const ajn::MsgArg& lampList, const ajn::MsgArg& lampGroupList);

    /**
     * Get the Lamp Group as a MsgArg
     *
     * @param lampList       Msgarg holding an array of Lamp IDs
     * @param lampGroupList  Msgarg holding an array of Lamp Group IDs
     */
    void Get(ajn::MsgArg* lampList, ajn::MsgArg* lampGroupList) const;

    /**
     * Return true if the Lamp Group contains the specified lampGroupID
     *
     * @param  lampGroupID    Lamp Group ID
     * @return LSF_ERR_DEPENDENCY if the Lamp Group contains lampGroupID, LSF_OK otherwise
     */
    LSFResponseCode IsDependentLampGroup(LSFString& lampGroupID);

    /**
     * List of Lamp IDs
     */
    LSFStringList lamps;

    /**
     * List of Lamp Group IDs
     */
    LSFStringList lampGroups;
};

/**
 * Typedef for LampGroupMap type
 */
typedef std::map<LSFString, std::pair<LSFString, LampGroup> > LampGroupMap;

/**
 * Class defining a Effect ID component of a Scene
 */
class SceneElement {
  public:
    /**
     * Default Constructor
     */
    SceneElement() :
        invalidArgs(true)
    {
        lamps.clear();
        lampGroups.clear();
        effectID.clear();
    }
    /**
     * Parameterized Constructor
     *
     * @param lampList      List of Lamp IDs
     * @param lampGroupList List of Lamp Group IDs
     * @param effectID      Effect ID
     */
    SceneElement(const LSFStringList& lampList, const LSFStringList& lampGroupList, const LSFString& effectId);

    /**
     * Parameterized Constructor
     *
     * @param lampList      List of Lamp IDs
     * @param lampGroupList List of Lamp Group IDs
     * @param effectID      Effect ID
     */
    SceneElement(const ajn::MsgArg& lampList, const ajn::MsgArg& lampGroupList, const ajn::MsgArg& effectId);

    /**
     * Destructor
     */
    ~SceneElement() {
        lamps.clear();
        lampGroups.clear();
        effectID.clear();
    }

    /**
     * Return the details of the SceneElement as a string
     *
     * @return string
     */
    const char* c_str(void) const;

    /**
     * Copy Constructor
     *
     * @param other Component to copy from
     */
    SceneElement(const SceneElement& other);

    /**
     * Assignment operator
     *
     * @param other Component to assign from
     */
    SceneElement& operator=(const SceneElement& other);

    /**
     * Set the SceneElement
     *
     * @param lampList      List of Lamp IDs
     * @param lampGroupList List of Lamp Group IDs
     * @param effectID      Effect ID
     */
    void Set(const ajn::MsgArg& lampList, const ajn::MsgArg& lampGroupList, const ajn::MsgArg& effectId);

    /**
     * Get the component as a MsgArg
     *
     * @param lampList      List of Lamp IDs
     * @param lampGroupList List of Lamp Group IDs
     * @param effectID      Effect ID
     */
    void Get(ajn::MsgArg* lampList, ajn::MsgArg* lampGroupList, ajn::MsgArg* effectId) const;

    /**
     * Check if there a scene element that depends on specific lamp group
     * @param lampGroupID - the lamp group id
     * @return LSF_OK if there is not dependency \n
     *         LSF_ERR_DEPENDENCY if there is dependency
     */
    LSFResponseCode IsDependentOnLampGroup(LSFString& lampGroupID);

    /**
     * Check if there a scene element that depends on specific effect
     * @param effectId - the effect id
     * @return LSF_OK if there is not dependency \n
     *         LSF_ERR_DEPENDENCY if there is dependency
     */
    LSFResponseCode IsDependentOnEffect(LSFString& effectId);

    /**
     * List of Lamps
     */
    LSFStringList lamps;

    /**
     * List of Lamp Groups
     */
    LSFStringList lampGroups;

    /**
     * Effect ID
     */
    LSFString effectID;

    /**
     * Indicated invalid arguments
     */
    bool invalidArgs;
};

/*
 * std::map<sceneElementID, std::pair<sceneElementName, SceneElement>>
 */
typedef std::map<LSFString, std::pair<LSFString, SceneElement> > SceneElementMap;

/**
 * Class defining a Transition Effect
 */
class TransitionEffect {
  public:
    /**
     * Default Constructor
     *
     * @param lampState     Lamp State to transition to
     * @param transPeriod   Transition Period to transition over
     */
    TransitionEffect();

    /**
     * Parameterized Constructor
     *
     * @param lampState     Lamp State to transition to
     * @param transPeriod   Transition Period to transition over
     */
    TransitionEffect(LampState& lampState, uint32_t& transPeriod);

    /**
     * Parameterized Constructor
     *
     * @param presetID     Preset ID
     * @param transPeriod  Transition Period to transition over
     */
    TransitionEffect(LSFString& presetId, uint32_t& transPeriod);

    /**
     * Parameterized Constructor
     *
     * @param lampState     Lamp State to transition to
     */
    TransitionEffect(LampState& lampState);

    /**
     * Parameterized Constructor
     *
     * @param presetID     Preset ID
     */
    TransitionEffect(LSFString& presetId);

    /**
     * Parameterized Constructor
     *
     * @param lampState      MsgArg with Lamp State info
     * @param presetId       MsgArg with Preset ID info
     * @param transPeriod    MsgArg with transition period info
     */
    TransitionEffect(const ajn::MsgArg& lampState, const ajn::MsgArg& presetId, const ajn::MsgArg& transPeriod);

    /**
     * Return the details of the Transition Effect as a string
     *
     * @return string
     */
    const char* c_str(void) const;

    /**
     * Copy Constructor
     *
     * @param other Component to copy from
     */
    TransitionEffect(const TransitionEffect& other);

    /**
     * Assignment operator
     *
     * @param other Component to assign from
     */
    TransitionEffect& operator=(const TransitionEffect& other);

    /**
     * Set the Lamp Group
     *
     * @param lampState      MsgArg with Lamp State info
     * @param presetId       MsgArg with Preset ID info
     * @param transPeriod    MsgArg with transition period info
     */
    void Set(const ajn::MsgArg& lampState, const ajn::MsgArg& presetId, const ajn::MsgArg& transPeriod);

    /**
     * Get the component as a MsgArg
     *
     * @param lampState      MsgArg with Lamp State info
     * @param presetId       MsgArg with Preset ID info
     * @param transPeriod    MsgArg with transition period info
     */
    void Get(ajn::MsgArg* lampState, ajn::MsgArg* presetId, ajn::MsgArg* transPeriod) const;

    /**
     * Lamp State to transition to
     */
    LampState state;

    /**
     * Transition period to transition over
     */
    uint32_t transitionPeriod;

    /**
     * Preset ID associated with the Transition Effect
     */
    LSFString presetID;

    /**
     * Indicated invalid arguments
     */
    bool invalidArgs;
};

/**
 * Typedef for TransitionEffectMap type. \n
 * The key of the map is the transition effect id which is auto generated. \n
 * The value of the map is a pair that contains: \n
 *      Key - user defined name of the transition effect
 *      Value - The transition effect
 */
typedef std::map<LSFString, std::pair<LSFString, TransitionEffect> > TransitionEffectMap;

/**
 * Class defining a Transition To State component of a Scene
 */
class TransitionLampsLampGroupsToState {
  public:
    /**
     * Default Constructor
     */
    TransitionLampsLampGroupsToState() :
        transitionPeriod(0), invalidArgs(true)
    {
        lamps.clear();
        lampGroups.clear();
        state = LampState();
    }
    /**
     * Parameterized Constructor
     *
     * @param lampList      List of Lamp IDs
     * @param lampGroupList List of Lamp Group IDs
     * @param lampState     Lamp State to transition to
     * @param transPeriod   Transition Period to transition over
     */
    TransitionLampsLampGroupsToState(LSFStringList& lampList, LSFStringList& lampGroupList, LampState& lampState, uint32_t& transPeriod);

    /**
     * Parameterized Constructor
     *
     * @param lampList      List of Lamp IDs
     * @param lampGroupList List of Lamp Group IDs
     * @param lampState     Lamp State to transition to
     */
    TransitionLampsLampGroupsToState(LSFStringList& lampList, LSFStringList& lampGroupList, LampState& lampState);

    /**
     * Parameterized Constructor
     *
     * @param component      MsgArg with an array of Lamp IDs,
     *                       array of Lamp Group IDs and a
     *                       dictionary with Lamp State
     */
    TransitionLampsLampGroupsToState(const ajn::MsgArg& component);

    /**
     * Return the details of the Transition To State component as a string
     *
     * @return string
     */
    const char* c_str(void) const;

    /**
     * Copy Constructor
     *
     * @param other Component to copy from
     */
    TransitionLampsLampGroupsToState(const TransitionLampsLampGroupsToState& other);

    /**
     * Assignment operator
     *
     * @param other Component to assign from
     */
    TransitionLampsLampGroupsToState& operator=(const TransitionLampsLampGroupsToState& other);

    /**
     * Set the Lamp Group
     *
     * @param component      MsgArg with an array of Lamp IDs,
     *                       array of Lamp Group IDs and a
     *                       dictionary with Lamp State
     */
    void Set(const ajn::MsgArg& component);

    /**
     * Get the component as a MsgArg
     *
     * @param component       Msgarg holding the component
     */
    void Get(ajn::MsgArg* component) const;

    /**
     * List of Lamps
     */
    LSFStringList lamps;

    /**
     * List of Lamp Groups
     */
    LSFStringList lampGroups;

    /**
     * Lamp State to transition to
     */
    LampState state;

    /**
     * Transition period to transition over
     */
    uint32_t transitionPeriod;

    /**
     * Indicated invalid arguments
     */
    bool invalidArgs;
};

/**
 * class holds transition details of lamp groups and individual lamps to requested present
 */
class TransitionLampsLampGroupsToPreset {
  public:
    /**
     * Default Constructor
     */
    TransitionLampsLampGroupsToPreset() :
        transitionPeriod(0), invalidArgs(true)
    {
        lamps.clear();
        lampGroups.clear();
        presetID.clear();
    }
    /**
     * TransitionLampsLampGroupsToPreset CTOR
     *
     * @param lampList
     * @param lampGroupList
     * @param presetID
     * @param transPeriod
     */
    TransitionLampsLampGroupsToPreset(LSFStringList& lampList, LSFStringList& lampGroupList, LSFString& presetID, uint32_t& transPeriod);

    /**
     * TransitionLampsLampGroupsToPreset CTOR
     *
     * @param lampList
     * @param lampGroupList
     * @param presetID
     */
    TransitionLampsLampGroupsToPreset(LSFStringList& lampList, LSFStringList& lampGroupList, LSFString& presetID);

    /**
     * TransitionLampsLampGroupsToPreset CTOR
     *
     * @param component
     */
    TransitionLampsLampGroupsToPreset(const ajn::MsgArg& component);

    /**
     * c_str - method return textual representation of the class
     *
     * @return text
     */
    const char* c_str(void) const;

    /**
     * copy constructor
     */
    TransitionLampsLampGroupsToPreset(const TransitionLampsLampGroupsToPreset& other);

    /**
     * operator ==
     */
    TransitionLampsLampGroupsToPreset& operator=(const TransitionLampsLampGroupsToPreset& other);

    /**
     * set lamps and lamp groups
     * @param component
     */
    void Set(const ajn::MsgArg& component);
    /**
     * Get lamps
     * @param component
     */
    void Get(ajn::MsgArg* component) const;

    /**
     * lamp unique names
     */
    LSFStringList lamps;
    /**
     * lamp group names
     */
    LSFStringList lampGroups;
    /**
     * present id
     */
    LSFString presetID;
    /**
     * transition period
     */
    uint32_t transitionPeriod;
    /**
     * invalid arguments
     */
    bool invalidArgs;
};

/**
 * Class defines pulse effect details
 */
class PulseEffect {
  public:
    /**
     * Default Constructor
     */
    PulseEffect();
    /**
     * PulseEffect CTOR
     * @param fromLampState
     * @param toLampState
     * @param period
     * @param duration
     * @param numOfPulses
     */
    PulseEffect(LampState& toLampState, uint32_t& period, uint32_t& duration, uint32_t& numOfPulses, const LampState& fromLampState = LampState());
    /**
     * PulseEffect CTOR
     * @param fromLampPreset
     * @param toLampPreset
     * @param period
     * @param duration
     * @param numOfPulses
     */
    PulseEffect(LSFString& toLampPreset, uint32_t& period, uint32_t& duration, uint32_t& numOfPulses, const LSFString& fromLampPreset = CurrentStateIdentifier);
    /**
     * PulseEffect CTOR
     * @param fromLampState
     * @param toLampState
     * @param period
     * @param duration
     * @param numOfPulses
     * @param fromLampPreset
     * @param toLampPreset
     */
    PulseEffect(const ajn::MsgArg& toLampState, const ajn::MsgArg& period, const ajn::MsgArg& duration, const ajn::MsgArg& numOfPulses, const ajn::MsgArg& fromLampState,
                const ajn::MsgArg& toLampPreset, const ajn::MsgArg& fromLampPreset);
    /**
     * string representation of the object
     */
    const char* c_str(void) const;
    /**
     * copy constructor
     */
    PulseEffect(const PulseEffect& other);
    /**
     * operator==
     */
    PulseEffect& operator=(const PulseEffect& other);
    /**
     * set
     * @param fromLampState
     * @param toLampState
     * @param period
     * @param duration
     * @param numOfPulses
     * @param fromLampPreset
     * @param toLampPreset
     */
    void Set(const ajn::MsgArg& toLampState, const ajn::MsgArg& period, const ajn::MsgArg& duration, const ajn::MsgArg& numOfPulses, const ajn::MsgArg& fromLampState,
             const ajn::MsgArg& toLampPreset, const ajn::MsgArg& fromLampPreset);
    /**
     * get lamps
     * @param fromLampState
     * @param toLampState
     * @param period
     * @param duration
     * @param numOfPulses
     * @param fromLampPreset
     * @param toLampPreset
     */
    void Get(ajn::MsgArg* toLampState, ajn::MsgArg* period, ajn::MsgArg* duration, ajn::MsgArg* numOfPulses, ajn::MsgArg* fromLampState,
             ajn::MsgArg* toLampPreset, ajn::MsgArg* fromLampPreset) const;

    /**
     * to state
     */
    LampState toState;
    /**
     * pulse period
     */
    uint32_t pulsePeriod;
    /**
     * pulse duration
     */
    uint32_t pulseDuration;
    /**
     * number of pulses
     */
    uint32_t numPulses;
    /**
     * from state
     */
    LampState fromState;
    /**
     * to preset
     */
    LSFString toPreset;
    /**
     * from preset
     */
    LSFString fromPreset;

    /**
     * invalid arguments
     */
    bool invalidArgs;
};

/**
 * Typedef for PulseEffectMap type. \n
 * The key of the map is the pulse effect id which is auto generated. \n
 * The value of the map is a pair that contains: \n
 *      Key - user defined name of the pulse effect
 *      Value - The pulse effect
 */
typedef std::map<LSFString, std::pair<LSFString, PulseEffect> > PulseEffectMap;

/**
 * class defines pulse details and lamps the pulse is relevant for them
 */
class PulseLampsLampGroupsWithState {
  public:
    /**
     * Default Constructor
     */
    PulseLampsLampGroupsWithState() :
        pulsePeriod(0), pulseDuration(0), numPulses(0), invalidArgs(true)
    {
        lamps.clear();
        lampGroups.clear();
        fromState = LampState();
        toState = LampState();
    }
    /**
     * PulseLampsLampGroupsWithState CTOR
     * @param lampList
     * @param lampGroupList
     * @param fromLampState
     * @param toLampState
     * @param period
     * @param duration
     * @param numOfPulses
     */
    PulseLampsLampGroupsWithState(LSFStringList& lampList, LSFStringList& lampGroupList, LampState& fromLampState, LampState& toLampState, uint32_t& period, uint32_t& duration, uint32_t& numOfPulses);
    /**
     * PulseLampsLampGroupsWithState CTOR
     * @param lampList
     * @param lampGroupList
     * @param toLampState
     * @param period
     * @param duration
     * @param numOfPulses
     */
    PulseLampsLampGroupsWithState(LSFStringList& lampList, LSFStringList& lampGroupList, LampState& toLampState, uint32_t& period, uint32_t& duration, uint32_t& numOfPulses);
    /**
     * PulseLampsLampGroupsWithState CTOR
     * @param component
     */
    PulseLampsLampGroupsWithState(const ajn::MsgArg& component);
    /**
     * string representation of the object
     */
    const char* c_str(void) const;
    /**
     * copy constructor
     */
    PulseLampsLampGroupsWithState(const PulseLampsLampGroupsWithState& other);
    /**
     * operator==
     */
    PulseLampsLampGroupsWithState& operator=(const PulseLampsLampGroupsWithState& other);
    /**
     * set lamps
     * @param component
     */
    void Set(const ajn::MsgArg& component);
    /**
     * get lamps
     * @param component
     */
    void Get(ajn::MsgArg* component) const;
    /**
     * lamps names
     */
    LSFStringList lamps;
    /**
     * lamp groups
     */
    LSFStringList lampGroups;
    /**
     * from state
     */
    LampState fromState;
    /**
     * to state
     */
    LampState toState;
    /**
     * pulse period
     */
    uint32_t pulsePeriod;
    /**
     * pulse duration
     */
    uint32_t pulseDuration;
    /**
     * number of pulses
     */
    uint32_t numPulses;
    /**
     * invalid arguments
     */
    bool invalidArgs;
};

/**
 * class contains detais about pulse lamps and lamp groups with present
 */
class PulseLampsLampGroupsWithPreset {
  public:
    /**
     * Default Constructor
     */
    PulseLampsLampGroupsWithPreset() :
        pulsePeriod(0), pulseDuration(0), numPulses(0), invalidArgs(true)
    {
        lamps.clear();
        lampGroups.clear();
        fromPreset.clear();
        toPreset.clear();
    }
    /**
     * PulseLampsLampGroupsWithPreset CTOR
     */
    PulseLampsLampGroupsWithPreset(LSFStringList& lampList, LSFStringList& lampGroupList, LSFString& fromPreset, LSFString& toPreset, uint32_t& period, uint32_t& duration, uint32_t& numOfPulses);
    /**
     * PulseLampsLampGroupsWithPreset CTOR
     */
    PulseLampsLampGroupsWithPreset(LSFStringList& lampList, LSFStringList& lampGroupList, LSFString& toPreset, uint32_t& period, uint32_t& duration, uint32_t& numOfPulses);
    /**
     * PulseLampsLampGroupsWithPreset CTOR
     */
    PulseLampsLampGroupsWithPreset(const ajn::MsgArg& component);
    /**
     * a string representation to the object
     */
    const char* c_str(void) const;
    /**
     * copy constructor
     */
    PulseLampsLampGroupsWithPreset(const PulseLampsLampGroupsWithPreset& other);
    /**
     * operator==
     */
    PulseLampsLampGroupsWithPreset& operator=(const PulseLampsLampGroupsWithPreset& other);
    /**
     * set lamps
     */
    void Set(const ajn::MsgArg& component);
    /**
     * get lamps
     */
    void Get(ajn::MsgArg* component) const;
    /**
     * lamps
     */
    LSFStringList lamps;
    /**
     * lamp groups
     */
    LSFStringList lampGroups;
    /**
     * from present
     */
    LSFString fromPreset;
    /**
     * to present
     */
    LSFString toPreset;
    /**
     * pulse period
     */
    uint32_t pulsePeriod;
    /**
     * pulse duration
     */
    uint32_t pulseDuration;
    /**
     * number of pulses
     */
    uint32_t numPulses;
    /**
     * invalid arguments
     */
    bool invalidArgs;
};

typedef std::list<TransitionLampsLampGroupsToState> TransitionLampsLampGroupsToStateList;
typedef std::list<TransitionLampsLampGroupsToPreset> TransitionLampsLampGroupsToPresetList;
typedef std::list<PulseLampsLampGroupsWithState> PulseLampsLampGroupsWithStateList;
typedef std::list<PulseLampsLampGroupsWithPreset> PulseLampsLampGroupsWithPresetList;

/**
 * Scene class holds scene details. \n
 * Scenes are preferences saved by the End User to set a particular mood or to simply store a setting for convenience and future recall. \n
 * Scenes are persisted in the Lighting Controller Service and are made up of a Saved State and/or Effect applied to a Lamp or Lamp Group.
 *
 */
class Scene {
  public:
    /**
     * class CTOR
     */
    Scene();
    /**
     * class CTOR
     */
    Scene(const ajn::MsgArg& transitionToStateComponentList, const ajn::MsgArg& transitionToPresetComponentList,
          const ajn::MsgArg& pulseWithStateComponentList, const ajn::MsgArg& pulseWithPresetComponentList);
    /**
     * class CTOR
     */
    Scene(TransitionLampsLampGroupsToStateList& transitionToStateComponentList, TransitionLampsLampGroupsToPresetList& transitionToPresetComponentList,
          PulseLampsLampGroupsWithStateList& pulseWithStateComponentList, PulseLampsLampGroupsWithPresetList& pulseWithPresetComponentList);
    /**
     * class copy constructor
     */
    Scene(const Scene& other);
    /**
     * operator=
     */
    Scene& operator=(const Scene& other);
    /**
     * a string representation of the object
     */
    const char* c_str(void) const;
    /**
     * set scene details
     */
    void Set(const ajn::MsgArg& transitionToStateComponentList, const ajn::MsgArg& transitionToPresetComponentList,
             const ajn::MsgArg& pulseWithStateComponentList, const ajn::MsgArg& pulseWithPresetComponentList);
    /**
     * get scene details
     */
    void Get(ajn::MsgArg* transitionToStateComponentList, ajn::MsgArg* transitionToPresetComponentList,
             ajn::MsgArg* pulseWithStateComponentList, ajn::MsgArg* pulseWithPresetComponentList) const;

    /**
     * Check if there a scene that depends on specific present
     * @param presetID - the lamp present id
     * @return LSF_OK if there is not dependency \n
     *         LSF_ERR_DEPENDENCY if there is dependency
     */
    LSFResponseCode IsDependentOnPreset(LSFString& presetID);
    /**
     * Check if there a scene that depends on specific lamp group
     * @param lampGroupID - the lamp group id
     * @return LSF_OK if there is not dependency \n
     *         LSF_ERR_DEPENDENCY if there is dependency
     */
    LSFResponseCode IsDependentOnLampGroup(LSFString& lampGroupID);
    /**
     * list of transition lamps and lamp groups to state list
     */
    TransitionLampsLampGroupsToStateList transitionToStateComponent;
    /**
     * list of transition lamps and lamp groups to present
     */
    TransitionLampsLampGroupsToPresetList transitionToPresetComponent;
    /**
     * list of pulse lamps and lamp groups with state
     */
    PulseLampsLampGroupsWithStateList pulseWithStateComponent;
    /**
     * list of pulse lamps and lamp groups with present
     */
    PulseLampsLampGroupsWithPresetList pulseWithPresetComponent;
    /**
     * invalid args
     */
    bool invalidArgs;
};

/*
 * std::map<sceneID, std::pair<sceneName, Scene>>
 */
typedef std::map<LSFString, std::pair<LSFString, Scene> > SceneMap;

/**
 * class that manages the master scene
 */
class MasterScene {
  public:
    /**
     * class CTOR
     */
    MasterScene();
    /**
     * class CTOR
     */
    MasterScene(const ajn::MsgArg& sceneList);
    /**
     * class CTOR
     */
    MasterScene(LSFStringList sceneList);
    /**
     * class DTOR
     */
    ~MasterScene() { scenes.clear(); }
    /**
     * a string representation of the instance
     */
    const char* c_str(void) const;
    /**
     * copy constructor
     */
    MasterScene(const MasterScene& other);
    /**
     * operator=
     */
    MasterScene& operator=(const MasterScene& other);
    /**
     * set scene list
     */
    void Set(const ajn::MsgArg& sceneList);
    /**
     * get scene list
     */
    void Get(ajn::MsgArg* sceneList) const;
    /**
     * is master scene dependent of scene
     */
    LSFResponseCode IsDependentOnScene(LSFString& sceneID);

    /**
     * list of scenes
     */
    LSFStringList scenes;
};

/*
 * std::map<masterSceneID, std::pair<masterSceneName, masterScene>>
 */
typedef std::map<LSFString, std::pair<LSFString, MasterScene> > MasterSceneMap;

/**
 * The second type of scene object that consists solely of scene elements
 * (referenced by their IDs).
 */
class SceneWithSceneElements {
  public:
    /**
     * class CTOR
     */
    SceneWithSceneElements();
    /**
     * class CTOR
     */
    SceneWithSceneElements(const ajn::MsgArg& sceneElementsList);
    /**
     * class CTOR
     */
    SceneWithSceneElements(LSFStringList sceneElementsList);
    /**
     * class DTOR
     */
    ~SceneWithSceneElements() { sceneElements.clear(); }
    /**
     * a string representation of the instance
     */
    const char* c_str(void) const;
    /**
     * copy constructor
     */
    SceneWithSceneElements(const SceneWithSceneElements& other);
    /**
     * operator=
     */
    SceneWithSceneElements& operator=(const SceneWithSceneElements& other);
    /**
     * set scene list
     */
    void Set(const ajn::MsgArg& sceneElementsList);
    /**
     * get scene list
     */
    void Get(ajn::MsgArg* sceneElementsList) const;
    /**
     * is scene element dependent of scene
     */
    LSFResponseCode IsDependentOnSceneElement(LSFString& sceneElementID);

    /**
     * list of scenes
     */
    LSFStringList sceneElements;
};

/*
 * std::map<sceneID, std::pair<sceneName, SceneWithSceneElements>>
 */
typedef std::map<LSFString, std::pair<LSFString, SceneWithSceneElements> > SceneWithSceneElementsMap;

}

#endif