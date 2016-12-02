#ifndef _SERVICE_DESCRIPTION_H_
#define _SERVICE_DESCRIPTION_H_
/**
 * \ingroup ControllerService
 */
/**
 * @file
 * This file provides definitions for service description
 */
/******************************************************************************
 *  * 
 *    Copyright (c) 2016 Open Connectivity Foundation and AllJoyn Open
 *    Source Project Contributors and others.
 *    
 *    All rights reserved. This program and the accompanying materials are
 *    made available under the terms of the Apache License, Version 2.0
 *    which accompanies this distribution, and is available at
 *    http://www.apache.org/licenses/LICENSE-2.0

 ******************************************************************************/

#include <string>
#include "LSFNamespaceSpecifier.h"

namespace lsf {

OPTIONAL_NAMESPACE_CONTROLLER_SERVICE

extern const std::string ControllerServiceDescription;
extern const std::string ControllerServiceLampDescription;
extern const std::string ControllerServiceLampGroupDescription;
extern const std::string ControllerServicePresetDescription;
extern const std::string ControllerServiceTransitionEffectDescription;
extern const std::string ControllerServicePulseEffectDescription;
extern const std::string ControllerServiceSceneDescription;
extern const std::string ControllerServiceSceneWithSceneElementsDescription;
extern const std::string ControllerServiceSceneElementDescription;
extern const std::string ControllerServiceMasterSceneDescription;
extern const std::string LeaderElectionAndStateSyncDescription;
extern const std::string ControllerServiceDataSetDescription;

OPTIONAL_NAMESPACE_CLOSE

} //lsf

#endif