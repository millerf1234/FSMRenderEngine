// File:                 BuildSettings.h
//
//
//  NOTE TO SELF:         STOP WRITING DOCUMENTATION. Most of this will be wrong anyways.
//                        Wait until things have been proven to work and aren't in mid development
//
//
// Quick Description:    Defines macros that can either be set manually (recommended) or rigged to be
//                           set automatically based off configuration that was set using a build-tool
//                           (Not-Recommended). 
//                       
// Background:   
//                           or really any build tool). Also explain why just setting this stuff manually is better for now...
//
//                 TODO  Rewrite Background to explain what build settings are, what impact they have on the
//                           code, and the various options for setting them (i.e. MSBuild in MSVC, cmake for cross-platform,
// (Old Background section):
//                This file is an attempt at getting some level of guaranteed control over how the 
//                     project gets built* rather than relying upon IDE-specific settings and compiler
//                     provided macros (such as 'NDEBUG' in C compilers and '_DEBUG' in MSCV).             
//                     There was a time before this file existed when I tried to depend on 
//                     results as to the reliability of these in both Internet research and through my
//                     own (limited) experience with compiling code.  
//
//                                                                           *  ('built' meaning which versions of libraries are included/linked-against,
//                                                                                   how much non-essential code [e.g. debug log messages] gets
//                                                                                   included/excluded during compilation, and how the underlying OpenGL
//                                                                                   context(s) behave)
//                 
//                 For example, see how the following StackOverflow posts have conflicting/inconsistent
//                     responses to questions on the subject:           
//                        (i) https://stackoverflow.com/questions/18818223/how-to-make-debug-macros-portable
//                       (ii) https://stackoverflow.com/questions/34302265/does-cmake-build-type-release-imply-dndebug
//                 
//
//
//
//
// Description:          This file defines macros that provide clear unambiguous control over how the code is to be built.
//                           The intent here is to provide a verifiable way to achieve explicit control over the resulting
//                           executable's behavior. These are intended to replace reliance on both compiler behavior (i.e. 
//                           expecting the compiler to define a macro) and development-environment-specific tools/features
//                           (see reference 'ii')  
//
//                       The goal of controlling the build through macros in this header is to simultaneously achieve a higher
//                           level of both portability and predictability across build systems than would be possible through 
//                           other means. 
//                     
//                 
// Usage notes:  - This project's Build Settings are organized together under groups called 'sections'. Each 'section' contains
//                    one or more settings which share a similar theme and/or impact the project by a comparable scope. Thus this
//                    organization scheme allows for the appearance of settings to be organized by their overall level of impact. 
//                    Settings that have a broader impact thus appear earlier in the file. This allows for settings which may 
//                    be dependent upon or which can undo/override specific effects of the more broadly-reaching settings to 
//                    appear behave appropriately.  
//                               
//               - Section 0 is different from the other sections in that each of its members are invariant project settings
//                    rather than modifiable build specifications. These section 0 invariants play a central roll in how the
//                    project is to be built and operate, however, 
//
//               - For each setting, read the comments to see which dependencies and what effects that setting will have. 
//                    To activate a setting, simply make sure its corresponding macro is defined. Likewise, to disable
//                    a setting all that is needed is to prevent its 'define' statement from being run by the 
//                    preprocessor (the preferable method of doing this is to simply comment said macro out).
//                    
//               - Some build settings are dependent on other build settings. Any dependent build setting will be 
//                    clearly marked as such.
//
//               - Don't be afraid to comment out every macro, this will still result in a valid build! 
//
//               - { TODO  Rewrite this} Settings are grouped by type, so for mutually exclusive 
//                    settings there is a good chance they will appear next to each other. A clear separator* of         * ( mutually exclusive settings  
//                    demarcating members of a set of mutually exclusive settings has been inserted to allow for                are separated by '^V^V^V^' ) 
//                    quick identification.
//
// Programmer:    Forrest Miller
// Date:          January 25, 2019
//
//
//               TODO   Add more settings
//

#pragma once

#ifndef BUILD_SETTINGS_H_
#define BUILD_SETTINGS_H_



// ------------------------------------------------------------------------------------------------------------
//  ( SECTION 0 )   PROJECT BUILD INVARIANTS                  
// ------------------------------------------------------------------------------------------------------------
//  - PROJECT MUST TARGET x64
//  - PROJECT MUST TARGET SAME OPENGL VERSION AS GLAD                 (Will be OpenGL 4.6 most likely)
//  - PROJECT MUST BE BUILT IN ENVIRONMENT WHICH SUPPORTS SSE2        (This is implied by requiring 'x64' though since 'x64' has SSE2 as part of its standard)
//  - Expected Runtime Environment of Windows 8.1 or newer            (Project currently has not been tested on platforms which aren't Windows 10, but 8.1 should work fine in theory)
//  -   ...  [More to be added as development progresses]
// 
// ------------------------------------------------------------------------------------------------------------
//  ( END SECTION 0 )
// ------------------------------------------------------------------------------------------------------------






// ============================================================================================================
//  ( SECTION 1 )   Global Project Build Behavior
// ============================================================================================================

// MACRO:  USE_DEBUG_
// Dependencies: 
//    - None
//
//This macro is used to enable/disable debugging across the entire project. Turning this
//macro on will cause the OpenGL context to operate in a 'debug' mode. Also, if the 
//macro 'USE_DEBUG_' is defined, then many places within the code will have extra debug
//logic included that will report unexpected or unusual occurrences (e.g. if a switch statement 
//results in the 'default' condition being executed). Leaving 'USE_DEBUG_' undefined (by commenting
//it out) will prevent many unexpected events from being reported but should also result in better 
//performance.
//For some of the third party libraries, there are both debug and release versions available. By
//defining this macro, the debug versions of these libraries will be selected if available. Having
//this macro be undefined will cause the release version of these libraries to be linked instead.
#define USE_DEBUG_ 1



// MACRO:  FORCE_DEBUG_CONTEXT_APP_SYNCHRONIZATION_
// Dependencies: 
//     - Must also define USE_DEBUG_  (otherwise this macro has no effect) 
//
//  The macro 'FORCE_DEBUG_CONTEXT_APP_SYNCHRONIZATION_' specifically affects the behavior of the
//    OpenGL context. Defining this macro will cause the context to be synchronized with the application
//    so that any messages/warnings/errors reported by the context will be reported close to the code/operation
//    that caused them. Leaving this macro undefined will allow the context to operate asynchronously, which 
//    could cause a dramatic performance boost but could also result in context messages to be reported 
//    long after the Application event that triggered them. It is recommended to leave this enabled while
//    debugging unless absolutely necessary. 
#define FORCE_DEBUG_CONTEXT_APP_SYNCHRONIZATION_ 1

// ============================================================================================================
//  ( END SECTION 1 )
// ============================================================================================================






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  ( SECTION 2 )   Specific Behavior Overrides 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*                                              Choose at most one of the following two 
  +-------------------------------------------------------------------------------------------------------------------------------------+  
  // MACRO:   FORCE_GLAD_VERSION_DEBUG_                                                                                                 |
  // Dependencies:                                                                                                                      |
  //    - Must not also have defined FORCE_GLAD_VERSION_RELEASE_                                                                        |
  //                                                                                                                                    |
  //This macro is used to override the version of glad to be built with the project. glad is the utility library                        |
  //used to load the graphics-language. Leaving both of the glad-version-override macros undefined will result in                       |
  //the glad version being determined from the USE_DEBUG_ macro.                                                                        |   */
  //#define FORCE_GLAD_VERSION_DEBUG_ 1                                                                                     

  //                                                                                                                                    |
  // MACRO:   FORCE_GLAD_VERSION_RELEASE_                                                                                               |                                          
  // Dependencies:                                                                                                                      |
  //    - Must not also have defined FORCE_GLAD_VERSION_DEBUG_                                                                          |                                                              
  //                                                                                                                                    |
  //This macro is used to override the version of glad to be built with the project. glad is the utility library                        |
  //used to load the graphics-language. Leaving both of the glad-version-override macros undefined will result in                       |                                             
  //the glad version being determined from the USE_DEBUG_ macro.                                                                        |   */
  //#define FORCE_GLAD_VERSION_RELEASE_ 1
/*+-------------------------------------------------------------------------------------------------------------------------------------+   */





// MACRO:    ENABLE_BASIC_PRE_GLFUNCTION_CALLBACK_MESSAGE_
// Dependencies: 
//     - GLAD must be DEBUG version. 
//              (This macro will not have any effect if glad is not DEBUG version)
//
// This macro will enable a logging message to be printed out before each call to any OpenGL function loaded by
// glad*. This message announces the name of the function, and perhaps more details if I decide to get fancy.                          *OpenGL Extension functions might not be loaded by glad?
// This macro by itself will not enabled profiling of OpenGL functions. This macro and the 
// ENABLE_BASIC_POST_GLFUNCTION_CALLBACK_MESSAGE_ may be used simultaneously. Note that a compiler warning
// will be triggered if this setting is enabled while glad is using its RELEASE version. 
#define ENABLE_GLAD_PRE_CALLBACK_LOGGING_MESSAGES_ 1


// MACRO:    ENABLE_BASIC_POST_GLFUNCTION_CALLBACK_MESSAGE_
// Dependencies: 
//     - GLAD must be DEBUG version. 
//              (This macro will not have any effect if glad is not DEBUG version)
//
// This macro will enable a logging message to be printed out after each call to any OpenGL function loaded by
// glad* has returned. This message announces the name of the function, the parameters passed to it and will                          *OpenGL Extension functions might not be loaded by glad?
// announce an error if one has been detected after the function returns. This macro by itself will not enable
// profiling of OpenGL functions. This macro and the ENABLE_BASIC_PRE_GLFUNCTION_CALLBACK_MESSAGE_ may be used
// simultaneously.  Note that a compiler warning will be triggered if this setting is enabled while glad is
// using its RELEASE version. 
#define ENABLE_GLAD_POST_CALLBACK_LOGGING_MESSAGES_ 1


// MACRO:      ENABLE_GLFUNCTION_PROFILING_
// Dependencies:
//    -  GLAD must also be the DEBUG version. Turning on this setting while GLAD is not 
//            being built for debug will result in a compilation error.
//    -  The EasyLogging++ utility must be configured to support profiling. Turning on this setting
//            with profiling deactivated in EasyLogging++ will result in a compilation error.
//
// Enables profiling for each OpenGL function call. The profiling will be performed using the profiler that 
// comes built into the EasyLogging++ 3rd-party library. The profiler is supposed to be thread-safe, but 
// this has not been yet tested by me.
#define ENABLE_GLFUNCTION_PROFILING_ 1

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  ( END SECTION 2 )   
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~






// MACRO:   GL_MATHEMATICS_FORCE_ALIGNED (WRONG! Check which GLM version is being used...)
// Dependencies:
//    - None
//
// https://github.com/g-truc/glm/blob/master/manual.md#section2_9
// https://gamedev.stackexchange.com/questions/132549/how-to-use-glm-simd-using-glm-version-0-9-8-2



#endif //BUILD_SETTINGS_H_
