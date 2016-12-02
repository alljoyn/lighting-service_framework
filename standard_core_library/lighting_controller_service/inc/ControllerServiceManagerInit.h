#ifndef CONTROLLER_SERVICE_MANAGER_INIT_H
#define CONTROLLER_SERVICE_MANAGER_INIT_H
/**
 * \defgroup ControllerServiceManagerInit
 * ControllerServiceManagerInit code
 */
/**
 * \ingroup ControllerService
 */
/**
 * @file
 * This file provides definitions for initializing the ControllerServiceManager
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

#include "ControllerService.h"

namespace lsf {

OPTIONAL_NAMESPACE_CONTROLLER_SERVICE

/**
 * Used to initialize the ControllerServiceManager
 */
ControllerServiceManager* InitializeControllerServiceManager(
    const std::string& factoryConfigFile,
    const std::string& configFile,
    const std::string& lampGroupFile,
    const std::string& presetFile,
    const std::string& transitionEffectFile,
    const std::string& pulseEffectFile,
    const std::string& sceneElementsFile,
    const std::string& sceneFile,
    const std::string& sceneWithSceneElementsFile,
    const std::string& masterSceneFile);

OPTIONAL_NAMESPACE_CLOSE

} //lsf

#endif