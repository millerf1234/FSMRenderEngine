//  File:               FSMEngine_GlobalConfigurationMacrosForEasyLoggingPP.h
//
//  Description:        This file configures the file 'easylogging++.h', it should be included only 
//                      in easylogging++.h and must be included at the top of the file. 
//
//                      Options are divided into 3 groups:
//
//                        The first group consists of options that can be modified freely without 
//                            potentially breaking the code.
//                       
//                        The second group consists of options that are not considered project invariants
//                            yet may cause the code to break due to their nature and are best left unmodified.
//                           
//                        The third group of options consists of options that are considered to be
//                            project invariants and are not to be modified without serious reworking of the 
//                            entire codebase. It is highly recommended to not make any changes to any options in
//                            this group.
//
//  Notice:             ELPP's documentation states that all configuration macros must be written at  
//                      the top of the "easylogging++.h" file or else they won't be compiled correctly.
//                      This is of course pure hogwash, since it is of course possible to do this. However
//                      having these macros in their own separate file does raise the risk that they will 
//                      not be 
//                      
//
//  ELPP Reference:     Complete documentation on all available configuration macros can
//                       be found at: https://github.com/zuhd-org/easyloggingpp#configuration-macros
//                                    
//  Programmer:         Forrest Miller
//  Date:               January 2019
//

#pragma once

#ifndef FSM_ENGINE_GLOBAL_CONFIGURATION_MACROS_FOR_EASY_LOGGING_PP_H_
#define FSM_ENGINE_GLOBAL_CONFIGURATION_MACROS_FOR_EASY_LOGGING_PP_H_

//First a FSMEngine test macro to make sure that these configuration macros
//are being taken into account during compilation. (If this macro is defined,
//then it known that every macro in this file is being taken into account).
#define FSMENGINE_CONFIRM_ELPP_CONFIGUATION_MACROS_ARE_DEFINED 1


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
//                                                     FSMEngine                                                      //   
//                                            Logging Configuration Options                                           //   
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  


// ----------------------------------------------------------------------------------------------------------------------  
//          Ungrouped  --  Configuration Flags   [These may one day get moved to being set in a config file]
// ----------------------------------------------------------------------------------------------------------------------

//Logging flag values (This does not list all of them):
//   NewLineForContainer -------------------------->      1
//   DisableApplicationAbortOnFatalLog------------->      8
//   ImmediateFlush ------------------------------->     16
//   ColoredTerminalOutput ------------------------>     64      (only has effect if OS supports color terminal [Windows does not support])
//   MultiLoggerSupport --------------------------->    128 
//   HierarchicalLogging -------------------------->   2048      (Not able to work simultaneously with Verbose logging)
//   CreateLoggerAutomatically -------------------->   4096
//   AutoSpacing----------------------------------->   8192
//   FixedTimeFormat------------------------------->  16384
#define ELPP_DEFAULT_LOGGING_FLAGS  1 | 8 | 128 | 2048 | 4096 | 8192  | 16384 


// ----------------------------------------------------------------------------------------------------------------------  
//         Option Group 1  -- Freely Modifiable Options
// ----------------------------------------------------------------------------------------------------------------------

//Disable logging [Not Recommended]
///#define ELPP_DISABLE_LOGS 1

//Disable logging to file [Not Recommended]
///#define ELPP_NO_LOG_TO_FILE 1


// ----------------------------------------------------------------------------------------------------------------------  
//          Option Group 2  -- Unstable Modifiable Options
// ----------------------------------------------------------------------------------------------------------------------

//Define this setting to try out experimental Asynchronous Logging. The EasyLogger++ documentation claims this
//should work for any environment already supporting std::thread and std::this_thread::sleep_for. This feature
//is still under testing and may change in the future. 
//WARNING!: Currently enabling this setting leads to program crashing instantly on launch
///#define ELPP_EXPERIMENTAL_ASYNC 1

//Not supported on Windows, requires header file <syslog.h> (which I think is a UNIX/POSIX thing)
///#define ELPP_SYSLOG  

//#define ELPP_THREAD_SAFE 1 



// ----------------------------------------------------------------------------------------------------------------------  
//          Option Group 3  -- PROJECT INVARIANTS  [DO NOT MODIFY!]
// ----------------------------------------------------------------------------------------------------------------------
//Disable verbose logging
#define ELPP_DISABLE_VERBOSE_LOGS 1


//For C++11 and newer projects it is possible to call loggers
//using variadic arguments a la 'printf()' style
//  See: https://github.com/zuhd-org/easyloggingpp#printf-like-logging
//#define ELPP_STL_LOGGING 1
//#define ELPP_LOG_STD_ARRAY 1  //Must have ELPP_STL_LOGGING defined



//Enable performance-tracking logging
#define ELPP_FEATURE_PERFORMANCE_TRACKING 1



///////////////////////////////////////////////////////////////
//////    End of FSMEngine settings configuration
///////////////////////////////////////////////////////////////


#endif //FSM_ENGINE_GLOBAL_CONFIGURATION_MACROS_FOR_EASY_LOGGING_PP_H_