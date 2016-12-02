#ifndef LSF_DEVICE_ICON_H
#define LSF_DEVICE_ICON_H
/**
 * \ingroup ControllerService
 */
/**
 * @file
 * This file provides definitions for device icon
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

#include <qcc/String.h>

namespace lsf {
/**
 * Device Icon URL
 */
extern const qcc::String DeviceIconURL;
/**
 * Device Icon Mime Type
 */
extern const qcc::String DeviceIconMimeType;
/**
 * Device Icon embedded data
 */
extern uint8_t DeviceIcon[];
/**
 * Device icon size
 */
extern const size_t DeviceIconSize;

}

#endif