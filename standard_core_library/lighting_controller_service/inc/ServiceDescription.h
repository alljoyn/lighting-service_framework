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
 *    Copyright (c) Open Connectivity Foundation (OCF) and AllJoyn Open
 *    Source Project (AJOSP) Contributors and others.
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
 *     THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 *     WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 *     DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 *     PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 *     TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 *     PERFORMANCE OF THIS SOFTWARE.
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