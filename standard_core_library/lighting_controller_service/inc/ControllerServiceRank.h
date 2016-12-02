#ifndef LSF_CONTROLLER_SERVICE_RANK_H
#define LSF_CONTROLLER_SERVICE_RANK_H
/**
 * \defgroup ControllerService
 * ControllerService code
 */
/**
 * \ingroup ControllerService
 */
/**
 * @file
 * This file provides definitions for the ControllerServiceRank class
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

#ifdef LSF_BINDINGS
#include <lsf/controllerservice/OEM_CS_Config.h>
#else
#include <OEM_CS_Config.h>
#endif

#include <Rank.h>
#include "LSFNamespaceSpecifier.h"

namespace lsf {

OPTIONAL_NAMESPACE_CONTROLLER_SERVICE

/**
 * Bit mask for MAC address rank parameter
 */
        #define MAC_ADDR_BIT_MASK 0x0000FFFFFFFFFFFF

/**
 * Number of bit positions to left shift for MAC Address rank parameter
 */
        #define BIT_SHIFT_FOR_MAC_ADDRESS 1

/**
 * Number of bit positions to left shift for Controller Service Version rank parameter
 */
        #define BIT_SHIFT_FOR_CONTROLLER_SERVICE_VERSION 49

/**
 * Number of bit positions to left shift for Network Config rank parameter
 */
        #define BIT_SHIFT_FOR_NODE_TYPE 57

/**
 * Number of bit positions to left shift for Power rank parameter
 */
        #define BIT_SHIFT_FOR_POWER 59

/**
 * Number of bit positions to left shift for Availability rank parameter
 */
        #define BIT_SHIFT_FOR_AVAILABILITY 61

/**
 * Max value of the Controller Service version allowed in rank encoding
 */
        #define MAX_CONTROLLER_SERVICE_VERSION 255

/**
 * Controller service version
 */
        #define CONTROLLER_SERVICE_VERSION 2

class ControllerServiceRank : public Rank {
  public:
    /**
     * Initializes the rank by reading values from the firmware
     */
    void Initialize(void);


  private:

};

OPTIONAL_NAMESPACE_CLOSE

} //lsf

#endif