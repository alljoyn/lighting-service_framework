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

#ifdef LSF_BINDINGS
#include <lsf/controllerservice/ControllerServiceManagerInit.h>
#else
#include <ControllerServiceManagerInit.h>
#endif

namespace lsf {

OPTIONAL_NAMESPACE_CONTROLLER_SERVICE

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
    const std::string& masterSceneFile)
{
    return new ControllerServiceManager(factoryConfigFile, configFile, lampGroupFile, presetFile, transitionEffectFile, pulseEffectFile, sceneElementsFile, sceneFile, sceneWithSceneElementsFile, masterSceneFile);
}

OPTIONAL_NAMESPACE_CLOSE

} //lsf