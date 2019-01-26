//File:        glad_includer.h
//
//Description:   This file is a wrapper for including the OpenGL loader 'glad.h'. 
//
//
//   References:
//            glad generator links  (Note that additional OpenGL extensions are made available by
//                                   the inclusion of the file "glcorearb.h" to the same folder as
//                                   the system's "gl.h" folder was located. Thus it is not necessary
//                                   to try to have glad load any extensions)
//
//                  GLAD DEBUG:
//                     http://glad.dav1d.de/#profile=core&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&language=c-debug&specification=gl&localfiles=on&loader=on
//                  GLAD RELEASE:
//                     http://glad.dav1d.de/#profile=core&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&language=c&specification=gl&localfiles=on&loader=on
//       
//
//
//
//  Programmer:   Forrest Miller
//  Date:         January 25, 2019
//


#pragma once

#include "..\..\BuildSettings.h"

#ifndef GLAD_INCLUDER_H_
#define GLAD_INCLUDER_H_


#if defined FORCE_GLAD_VERSION_DEBUG_
  #if defined FORCE_GLAD_VERSION_RELEASE_
    #error "Error: Unable to have both FORCE_GLAD_VERSION_DEBUG_ and FORCE_GLAD_VERSION_RELEASE_ defined at\
the same time! Check header file "BuildSettings.h" "
  #endif //FORCE_GLAD_VERSION_RELEASE_
  #define _FSM_ENGINE_BUILD_SETTING___GLAD_USE_DEBUG___

#elif defined FORCE_GLAD_VERSION_RELEASE_
  #define _FSM_ENGINE_BUILD_SETTING___GLAD_USE_RELEASE___

#elif defined USE_DEBUG_
  #define _FSM_ENGINE_BUILD_SETTING___GLAD_USE_DEBUG___ 
  
#else
  #define _FSM_ENGINE_BUILD_SETTING___GLAD_USE_RELEASE___
#endif //FORCE_GLAD_VERSION_DEBUG_



//Load glad based off the specified version
#if defined _FSM_ENGINE_BUILD_SETTING___GLAD_USE_DEBUG___
  //Do setup for loading debug glad
  #include "..\glad\debug\glad_debug.h"
  #undef _FSM_ENGINE_BUILD_SETTING___GLAD_USE_DEBUG___

#elif defined _FSM_ENGINE_BUILD_SETTING___GLAD_USE_RELEASE___ 
  #include "..\glad\release\glad.h"
  
  #ifdef ENABLE_GLFUNCTION_PROFILING_
    #error "Error: It is illegal to have profiling for OpenGL functions enabled when glad is building in RELEASE. \
Either make sure the setting ENABLE_GLFUNCTION_PROFILING_ is undefined or switch glad version to DEBUG (see "BuildSettings.h") "
  #endif //ENABLE_GLFUNCTION_PROFILING_
  
  #ifdef ENABLE_GLAD_PRE_CALLBACK_LOGGING_MESSAGES_
    #pragma message ("--------------------------------------------WARNING!-----------------------------------------------------------\n" \
    "There was a non-fatal conflict detected within the current build configuration: \n" \
    "  {issue}:       The logging of OpenGL Pre-Function-Call Callback events is enabled while glad is configured to operate under its RELEASE configuration!\n" \
    "  {cause}:         Unfortunately, this is a feature that is available only when using glad in its DEBUG configuration.\n" \
    "  {consequences}:  Logging of OpenGL Function calls will be disabled for this current build!\n\n" \
    "  {how to fix}:       [To disable this warning, either:\n" \
    "                             (i) disable Pre-Function-Call Callback logging; or\n" \
    "                            (ii) set glad to build in its DEBUG configuration to use this feature]\n" \
    "                      (both of these settings are located in the header \"BuildSettings.h\")\n" \
    "---------------------------------------------------------------------------------------------------------------\n" )
    #undef ENABLE_GLAD_PRE_CALLBACK_LOGGING_MESSAGES_
  #endif
  
  #ifdef ENABLE_GLAD_POST_CALLBACK_LOGGING_MESSAGES_
    #pragma message ("--------------------------------------------WARNING!-----------------------------------------------------------\n" \
    "There was a non-fatal conflict detected within the current build configuration for this project: \n" \
    "     The logging of OpenGL Post-Function-Call Callback events is enabled while glad is configured to operate under its RELEASE configuration!\n" \
    "         Unfortunately, this is a feature that is available only when using the DEBUG configuration of glad.\n" \
    "         No logging of OpenGL Function calls will be performed!\n\n" \
    "               [To disable this warning, either:\n" \
    "                     (i) disable Post-Function-Call Callback logging [by not defining the macro 'ENABLE_GLAD_POST_CALLBACK_LOGGING_MESSAGES_'] ; or\n" \
    "                    (ii) set glad to build in its DEBUG configuration to use this feature]\n" \
    "   (both of these settings are found in \"BuildSettings.h\")\n" \
    "---------------------------------------------------------------------------------------------------------------\n" )
    /*#pragma message ("WARNING! The logging of OpenGL Post-Function-Call Callback events is enabled while glad is configured to operate under its RELEASE configuration!\n" \
"         The logging of OpenGL function call events is a feature that only is available when using the DEBUG configuration of glad.\n" \
"         No logging of OpenGL Function calls will be performed!\n" \
"                  [To disable this warning, either:\n" \
"                       (i) disable Post-Function-Call Callback logging; or\n" \
"                      (ii) set glad to build in its DEBUG configuration to use this feature]\n" \
"             (both of these settings are found in the header \"BuildSettings.h\")\n") */
    #undef ENABLE_GLAD_POST_CALLBACK_LOGGING_MESSAGES_
  #endif

  #undef _FSM_ENGINE_BUILD_SETTING___GLAD_USE_RELEASE___


#else 
  #error "ERROR: Must have either debug or release version of glad [Right now there is a bug causing neither option to be available]!!!"
#endif //_GLAD_USE_DEBUG__







#endif //GLAD_INCLUDER_H_