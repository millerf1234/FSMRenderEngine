//  File:               FSMEngine_GlobalConfigurationMacrosForEasyLoggingPP.h
//
//  Description:        This file configures the file 'easylogging++.h', it should be included only 
//                      in easylogging++.h and must be included at the top of the file
//
//  Programmer:         Forrest Miller
//  Date:               January 2019

#pragma once

#ifndef FSM_ENGINE_GLOBAL_CONFIGURATION_MACROS_FOR_EASY_LOGGING_PP_H_
#define FSM_ENGINE_GLOBAL_CONFIGURATION_MACROS_FOR_EASY_LOGGING_PP_H_


//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//  FSMEngine 
//  Additional Logging Configuration Options
//////////////////////////////////////////////////////////////

//#define ELPP_NO_LOG_TO_FILE	//Disable logging to file 


//Disable verbose logging
#define ELPP_DISABLE_VERBOSE_LOGS 1

#define ELPP_STL_LOGGING 1
#define ELPP_LOG_STD_ARRAY 1  //Must have ELPP_STL_LOGGING defined


//#define ELPP_SYSLOG  //Not supported on Windows, requires header file <syslog.h>

//Logging flag values (I only wrote down a few of them):
//   NewLineForContainer -------------------------->      1
//   ImmediateFlush ------------------------------->     16
//   ColoredTerminalOutput ------------------------>     64      (only has effect if OS supports color terminal [Windows does not support])
//   MultiLoggerSupport --------------------------->    128 
//   HierarchicalLogging -------------------------->   2048      (Not able to work simultaneously with Verbose logging)
//   CreateLoggerAutomatically -------------------->   4096
//   AutoSpacing----------------------------------->   8192
//   FixedTimeFormat------------------------------->  16384
#define ELPP_DEFAULT_LOGGING_FLAGS  (1 | 128 | 2048 | 4096 | 8192 | 16384) 

#define ELPP_FEATURE_PERFORMANCE_TRACKING  //Enable performance-tracking logging

///////////////////////////////////////////////////////////////
//////    End of FSMEngine settings configuration
///////////////////////////////////////////////////////////////


#endif //FSM_ENGINE_GLOBAL_CONFIGURATION_MACROS_FOR_EASY_LOGGING_PP_H_