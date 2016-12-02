#ifndef LSF_FILE_PARSER_H
#define LSF_FILE_PARSER_H
/**
 * \ingroup ControllerService
 */
/**
 * @file
 * This file provides definitions for file parser
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
#include <iostream>
#include <fstream>
#include <LSFTypes.h>
#include "LSFNamespaceSpecifier.h"

namespace lsf {

OPTIONAL_NAMESPACE_CONTROLLER_SERVICE

extern const std::string resetID;

extern const std::string initialStateID;

void ParseLampState(std::istream& stream, LampState& state);

/**
 * Read a string from the stream.  Spaces will be included between double-quotes
 *
 * @param stream    The stream
 * @return          The next token in the stream
 */
std::string ParseString(std::istream& stream);

template <typename T>
T ParseValue(std::istream& stream)
{
    T t;
    stream >> t;
    return t;
}

std::ostream& WriteValue(std::ostream& stream, const std::string& name);

std::ostream& WriteString(std::ostream& stream, const std::string& name);

OPTIONAL_NAMESPACE_CLOSE

} //lsf

#endif