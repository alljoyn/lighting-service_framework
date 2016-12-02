#ifndef LSF_RANK_H
#define LSF_RANK_H
/**
 * \defgroup Common
 * Common code
 */
/**
 * \ingroup Common
 */
/**
 * @file
 * This file provides definitions for the Rank class
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

#include <qcc/platform.h>

namespace lsf {

class Rank {
  public:
    /**
     * class constructor
     */
    Rank();

    /**
     * Parameterized Constructor
     */
    Rank(uint64_t higherBits, uint64_t lowerBits);

    /**
     * Copy Constructor
     */
    Rank(const Rank& other);

    /**
     * class destructor
     */
    ~Rank();

    /**
     * Set the parameters of the rank
     */
    void Set(uint64_t higherBits, uint64_t lowerBits);

    /**
     * Returns the higher order bits of the rank
     */
    uint64_t GetHigherOrderBits(void) { return higherOrderBits; };

    /**
     * Returns the lower order bits of the rank
     */
    uint64_t GetLowerOrderBits(void) { return lowerOrderBits; };

    /**
     * Assignment operator
     */
    Rank& operator =(const Rank& other);

    /**
     * Rank == operator
     */
    bool operator ==(const Rank& other) const;

    /**
     * Rank != operator
     */
    bool operator !=(const Rank& other) const;

    /**
     * Rank < operator
     */
    bool operator <(const Rank& other) const;

    /**
     * Rank > operator
     */
    bool operator >(const Rank& other) const;

    /**
     * Return the details of the Rank as a string
     *
     * @return string
     */
    const char* c_str(void) const;

    /**
     * Returns true if the rank has been initialized
     */
    bool IsInitialized(void) { return initialized; }

    /**
     * Set initialized to true
     */
    void SetInitialized(void) { initialized = true; };

  private:

    /**
     * 64 higher order bits in the 128-bit rank
     */
    uint64_t higherOrderBits;

    /**
     * 64 lower order bits in the 128-bit rank
     */
    uint64_t lowerOrderBits;

    /**
     * Indicates if the rank has been initialized
     */
    bool initialized;
};

}

#endif