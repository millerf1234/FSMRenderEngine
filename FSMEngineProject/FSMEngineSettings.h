//
//  File:             FSMEngineSettings.h
//
//  Description:      Provides settings which dictate the built-in behavior of 
//                    FSMEngine-related components and functionality. 
//
//                    This file is designed to be used in the same manner as 
//                    the 'BuildSettings.h' header.
//
//  Programmer:       Forrest Miller
//


#pragma once

#ifndef FSM_ENGINE_SETTINGS_H_
#define FSM_ENGINE_SETTINGS_H_


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      FSMEngine SETTINGS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//          ```````````````````````````````
//          ````````````````````````````````
//        __`````````````````````````````````\
//      _/  \---------------------------------\_
//     {_      DEBUG                           _}=+>
//       \__/---------------------------------/
//           ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,/
//           ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//           ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,

// MACROS:  LOG_EVERY_FSM_NAMED_EXCEPTION_AS_WARNING
//          LOG_EVERY_FSM_NAMED_EXCEPTION_AS_ERROR
// Dependencies: 
//        - None     [OKAY TO HAVE BOTH MACROS DEFINED SIMULTANEOUSLY FOR LOGGING EXCEPTIONS AT BOTH LEVELS] 
//
//Define either of these macros to have every FSMNamedException thrown during runtime logged in 
//either the WARNING log or ERROR log, respectively. The LOG entry is generated from within the
//NamedFSMException's constructor and will always be recorded, even for FSMEngine's internally-handled 
//exceptions. Only FSMNamedExceptions are affected by this macro, so generating a log entry for generic 
//FSMExceptions is controlled using separate macros. 
//
#define LOG_NAMED_FSM_EXCEPTIONS_AS_WARNINGS 1
#define LOG_EVERY_FSM_NAMED_EXCEPTION_AS_ERROR 1





 
   
      
          
               
                     
                            
                                     
#endif //FSM_ENGINE_SETTINGS_H_
                                     
                            
                     
               
          
      
   
 
