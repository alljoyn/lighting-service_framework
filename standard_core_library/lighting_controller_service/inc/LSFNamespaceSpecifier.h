#ifndef _LSF_NamespaceSpecifier_h
#define _LSF_NamespaceSpecifier_h

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
#define OPTIONAL_NAMESPACE_CONTROLLER_CLIENT namespace controllerclient {
#define OPTIONAL_NAMESPACE_CONTROLLER_SERVICE namespace controllerservice {
#define OPTIONAL_NAMESPACE_CLOSE }
#else
#define OPTIONAL_NAMESPACE_CONTROLLER_CLIENT
#define OPTIONAL_NAMESPACE_CONTROLLER_SERVICE
#define OPTIONAL_NAMESPACE_CLOSE
#endif

#endif