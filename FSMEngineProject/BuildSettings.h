// File:                     BuildSettings.h
// 
//                     [TL;DR] The (incomplete) documentation in this file contains known MISTAKES.  
//  NOTE TO OTHER PEOPLE:        This file is a configuration file. When this header was first introduced to the 
//                           codebase, every macro written in it was meant to include thorough documentation.  
//                           Unfortunately the reality of coding is that things change, and to make matters 
//                           worse there were mistakes in the original design of the code. Ideally one day 
//                           in the future I will rewrite this entire file to make sure everything stated
//                           is 100% correct, but unfortunately for this document in its current state 
//                           there are quite a few things documented that turned out not to behave in
//                           reality the way I was expecting (A major example being my attempt to toggle
//                           which of the versions of the library 'glad' was compiled through macros). 
// 
//  NOTE TO SELF:           STOP WRITING DOCUMENTATION. Most of this will be wrong anyways.
//                          Wait until things have been proven to work and aren't in mid development
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
//
//
//               TODO   Add more settings
//
//                
//
//
//
//
//
//
//
//  
//       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       ~~~~~~~~~~~~~~~~~~~~~~~~~~                         Table of Contents For AVAILABLE SETTINGS                         ~~~~~~~~~~~~~~~~~~~~~~~~~~          
//       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//        
//  +==================================================================================================================================================================================+                
//  | Section Number |        Section Title        |     Section Description                                                                                                           |          
//  +================+=============================+===================================================================================================================================+          
//  |                |                             |                                                                                                                                   |        
//  |  Section 0)    | "Project Invariants"        |  These aren't really you usual type of settings that you get to 'customize' but then again its                                    |                                                      
//  |                |                             |    settings like this that necessitated the existence of a section 'Zero' in the first place...                                   |                                                                     
//  +----------------+-----------------------------+-----------------------------------------------------------------------------------------------------------------------------------+                                                                    
//  |                |                             | This section (known affectionately to the French as "section un") rightfully contains the two far most influential build macros   |                                     
//  |                |                             |   available across this entire project. The first is the all-knowing and all-powerful master-universal-debug on-off control, which|                                                                  
//  |                |  "Global Project DEBUG      |   is what is really in charge of controlling the behavior of how the code is built, regardless of what-ever misinformed nonsense  |                                           
//  |  Section 1)    |            Behavior"        |   the compiler thinks the build settings are*. The second setting, while only able to have an effect when allowed by the first,   |    *I spent some time down the rabbit hole of trying to determine how best to take advantage of         
//  |                |                             |   gets final (and first) say over how much the underlying OpenGL context is left alone to do its thing by enabling forced         |       common DEBUG macros most compilers provide. Luckily I was early in my design process when            
//  |                |                             |   synchronization between it and the application.                                                                                 |     I decided to just rely on my own. See: https://stackoverflow.com/questions/2290509/debug-vs-ndebug     
//  +----------------+-----------------------------+-----------------------------------------------------------------------------------------------------------------------------------+                                                                                        
//  |                |                             |       WARNING! THIS SECTION HAS SETTINGS THAT DON'T WORK AS DOCUMENTED BECAUSE I MADE ASSUMPTIONS THAT WERE WRONG!                |                                                     
//  |  Section 2)    |      "GLAD Behavior"        | Basically provides manual overrides for building the GLAD OpenGL function loader library in a DEBUG or RELEASE configuration      |                                                              
//  |                |                             |   plus some additional options which affect how verbose GLAD should be when using its DEBUG configuration.                        |                                                                            
//  +----------------+-----------------------------+-----------------------------------------------------------------------------------------------------------------------------------+                                                                                         
//  |                |                             |                                                                                                                                   |                                                                                   
//  |  Section 3)    | "GLM Specifications         |   All of the settings here are used to control the GL Mathematics library. Probably not too much of interest here outside of      |                                                                                       
//  |                |    and Behavior Settings"   |       forcefully disabling several options for testing performance profiling comparisons.                                         |                                                                
//  +----------------+-----------------------------+-----------------------------------------------------------------------------------------------------------------------------------+                                                                                   
//  |                |                             | The most important setting among this group is what I have decided to call "FECLPINAE", which is an acronym for "Failure to       |                                                                                            
//  |                |                             |    Establish Custom Logging Protocols Is Not An Error". This macro will come into play very early in the Applications runtime,    |                                                                          
//  |  Section 4)    |  "EasyLogging++ Behavior"   |    and failure to establish proper logging without this defined will result in a very fast crash.                                 |     
//  |                |                             | There is also Logging-specific settings, such as the ability to enable/disable some of the various logging outputs.               |            
//  |                |                             |                                                                                                                                   |         
//  +----------------+-----------------------------+-----------------------------------------------------------------------------------------------------------------------------------+                                                                        
//  |                |                             |                                                                                                                                   |                                          
//  |                |                             |                                                                                                                                   |                                                                     
//  |                |                             |                                                                                                                                   |                             
//  +----------------+-----------------------------+-----------------------------------------------------------------------------------------------------------------------------------+                                                             
//  |                |                             |                                                                                                                                   |                                                                                       
//  |                |                             |                                                                                                                                   |                                                                
//  |                |                             |                                                                                                                                   |                                               
//  +----------------+-----------------------------+-----------------------------------------------------------------------------------------------------------------------------------+
//

#pragma once

#ifndef BUILD_SETTINGS_H_
#define BUILD_SETTINGS_H_


// ------------------------------------------------------------------------------------------------------------
//  ( SECTION 0 )   PROJECT BUILD INVARIANTS                  
// ------------------------------------------------------------------------------------------------------------
//  - PROJECT MUST TARGET x64
//  - PROJECT MUST TARGET SAME OPENGL VERSION AS GLAD                    (Will be OpenGL 4.6 most likely)
//  - PROJECT MUST BE BUILT IN ENVIRONMENT WHICH SUPPORTS SSE2           (This is implied by requiring 'x64' though since 'x64' hardware by definition has support for SSE2 written into its standard)
//  - Expected Runtime Environment of Windows 8.1 or newer               (Project currently has not been tested on platforms which aren't Windows 10, but 8.1 should work fine in theory)
//  - Support For the C++17 header <filesystem> is essentially expected  (It may be possible to completely disable and remove all parts of this project that depend of it within a few hours)
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





// !!!!!!!!!!!!!!!!!            WARNING SOME OF THE SECTION 2 SETTINGS DON'T WORK            !!!!!!!!!!!!!!!!!
// SO IT TURNS OUT THAT I WAS WRONG AND THE WAY THE COMPILER COMPILES DOESN'T ALLOW THE FOLLOWING SETTING TO 
//  WORK THE WAY I DOUCMENTED THEM TO WORKL... I SUGGEST ALL OF SECTION 2 BE IGNORED UNTIL I CAN REWRITE IT!
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  ( SECTION 2 )  GLAD Behavior Overrides 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    Broken -- Do Not Use    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxx   Instead Control Glad Included Version Using MSVC Build Settings and the USE_DEBUG_ macro   xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
/// /*                                              Choose at most one of the following two                                         
///  +-------------------------------------------------------------------------------------------------------------------------------------+  
///
///  // MACRO:   FORCE_GLAD_VERSION_DEBUG_                                                                                                 |
///  // Dependencies:                                                                                                                      |
///  //    - Must not also have defined FORCE_GLAD_VERSION_RELEASE_                                                                        |
///  //                                                                                                                                    |
///  //This macro is used to override the version of glad to be built with the project. glad is the utility library                        |
///  //used to load the graphics-language. Leaving both of the glad-version-override macros undefined will result in                       |
///  //the glad version being determined from the USE_DEBUG_ macro.                                                                        |   */
///  //                                                                                                                                    
///#define FORCE_GLAD_VERSION_DEBUG_ 1                                                                                     
///
///  //                                                                                                                                    |
///  // MACRO:   FORCE_GLAD_VERSION_RELEASE_                                                                                               |                                          
///  // Dependencies:                                                                                                                      |
///  //    - Must not also have defined FORCE_GLAD_VERSION_DEBUG_                                                                          |                                                              
///  //                                                                                                                                    |
///  //This macro is used to override the version of glad to be built with the project. glad is the utility library                        |
///  //used to load the graphics-language. Leaving both of the glad-version-override macros undefined will result in                       |                                             
///  //the glad version being determined from the USE_DEBUG_ macro.                                                                        |   */
///  //
///  //#define FORCE_GLAD_VERSION_RELEASE_ 1
///
/// /*+-------------------------------------------------------------------------------------------------------------------------------------+   */
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



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


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    NOT YET IMPLEMENTED    xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// MACRO:      ENABLE_GLFUNCTION_PROFILING_
// Dependencies:
//    -  GLAD must also be the DEBUG version. Turning on this setting while GLAD is not 
//            being built for debug will result in a compilation error.
//    -  The EasyLogging++ utility must be configured to support profiling [see section 4]. Turning on 
//            this setting with profiling deactivated in EasyLogging++ will result in a compilation error.
//
// Enables profiling for each OpenGL function call. The profiling will be performed using the profiler that 
// comes built into the EasyLogging++ 3rd-party library. The profiler is supposed to be thread-safe, but 
// this has not been yet tested by me.
//#define ENABLE_GLFUNCTION_PROFILING_ 1
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///  ( END SECTION 2 )   
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  ( SECTION 3 )  GLM CONFIGURATION AND SETTINGS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
/* //
//   +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
//   /                                                                                                                          \  
//   \  FIRST A QUICK AND GENERAL SIDE-NOTE REGARDING GLM AND ITS CONFIGURATION MACROS:                                         /  
//   /     GLM has a tendency to change the names of its configuration macros between releases. I have                          \  
//   \     fallen into the trap in the past of thinking I was activating a cool GLM feature when in reality I was               / 
//   /     simply defining a macro that was never checked for by GLM and basically served no purpose whatsoever.                \ 
//   \     An excellent way to avoid having this happen [if a cool 'new' GLM macro is listed out in the wild] is                / 
//   /     to first consult with the GLM manual to see if the macro is listed. Chances are pretty good that if                  \  
//   \     the macro is not listed anywhere in the GLM manual, then that macro is not something which is relevant               / 
//   /     in regards to this Application's underlying GLM library.                                                             \ 
//   \                                                                                                                          / 
//   /   For reference:                                                                                                         \    
//   \         GLM Version in Use During Development:        0.9.9.3   [Last Updated Oct 31 2018]                               /   
//   /         GLM Manual:                                   https://github.com/g-truc/glm/blob/0.9.9.2/doc/manual.pdf          \    
//   \                                                                                                                          / 
//   /                                                                                                                          \   
//   +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+ 
*/ //
//

//                                              NOW ON TO THE GLM MACROS!

// MACRO:  FSM_ENGINE_REPORT_GLM_REQUESTED_CONFIGURATION
// Dependencies:  
//     - none
//
// This macro is not actually an official GLM macro, but instead a special macro which has been introduced into this project with
// the intent of providing a handy straight forward method for comparing what inputs are going into GLM regarding how it should
// configure itself versus its own self-message reporting process (though neither macro is in anyway dependent on the other being
// defined). Basically the implementation for this macro is the inclusion of an extra preprocessor directive which appears  
// immediately before the line where the instruction to include GLM's main header exists. This preprocessor directive basically
// will look to see if the macro 'FSM_ENGINE_REPORT_GLM_REQUESTED_CONFIGURATION' has been defined. If it hasn't been defined, then
// the preprocessor just keeps moving right along begin including GLM.  However, if this macro has been defined, there is a simple
// implementation for the preprocessor to simply go down the list of each macro and report it as either defined or undefined (with 
// a few exceptions made to save space*). Be advised that this still will form a fair amount of information being reported, with
// every setting being reported no matter if it is off or on. 
//  (*The space saving comes from only reporting which of the 3 active possible options [4 if counting 'off'] is active when
//      reporting on 'GLM_FORCE_PRECISION_***')
//  (Double precision is skipped if GLM set to be forced to only use single-precision)
//#define FSM_ENGINE_REPORT_GLM_REQUESTED_CONFIGURATION 1


// MACRO:  GLM_FORCE_MESSAGES
// Dependencies: 
//     - none
//
// This macro will toggle the level of verbosity displayed by GLM during the compilation process. If GLM undergoes
// compilation with this macro defined, the effect will be to have GLM provide a detailed summary of everything it knows
// through the compilation log. To briefly summarize what to expect from GLM's summary, first it will begin its output with
// announcing information related to itself that basically is just the version number of the library. This will be followed
// by all of the various specifics GLM detects about the compiler and build environment, which will include what C++ standard
// it detects is in use and possibly what advanced CPU instruction set it detects it can use to provide a fast SIMD-implementation.
// Note that it is possible to use macros to prevent GLM from detecting any of these settings.  After getting past the current
// platform and environment settings, it will announce each of its configuration options. Finally it will announce each additional
// extension that has been requested for inclusion with the library. 
//#define GLM_FORCE_MESSAGES 1


// MACROS: GLM_FORCE_PLATFORM_UNKNOWN
//         GLM_FORCE_COMPILER_UNKNOWN
//         GLM_FORCE_ARCH_UNKNOWN
//         GLM_FORCE_CXX_UNKNOWN
// Dependencies: 
//       - It appears as though defining both one of these settings combined with a specific 
//         GLM override specifying a specific version for that setting will cause the other macro
//         to win out. In other words, to actually disable GLM detection of one of these environment 
//         variables, it can't be told to use a specific one automatically as well. 
//
// These four macros can be used individually or in any combination together to force GLM to fail at detecting the 
// environment setting corresponding to each of their names, respectively (arch means architecture). I am not sure 
// why you would ever want to do this but I suppose it was nice of GLM's implementers to provide us with these option...
//#define GLM_FORCE_PLATFORM_UNKNOWN 1
//#define GLM_FORCE_COMPILER_UNKNOWN 1
//#define GLM_FORCE_ARCH_UNKNOWN 1
//#define GLM_FORCE_CXX_UNKNOWN 1


//MACRO: GLM_FORCE_CSS_UNKOWN
// Dependencies:
//     - none  (probably)      [I haven't tested for any dependencies/exclusions with this and the previous 4 commands]
//
// Similar to the four above macros in that it prevents GLM from detecting a part of the build environment during 
// compilation. The documentation states that this macro is able to prevent GLM from detecting the "C++ Compiler
// Standard Support" (see GLM Manual section 2.5). I am not sure what exactly this means or what impact is has,
// but feel free to play around with it if you like disabling features for fun and profit.
//#define GLM_FORCE_CSS_UNKNOWN 1


// MACRO:  GLM_FORCE_EXPLICIT_CTOR
// Dependencies: 
//     - none
//
// Prevents implicit conversions between GLM types. Essentially there are conversions allowed by the GLSL
// that are probably not the desired behavior we want to be allowing in out application, such as  
// implicit conversions between vector and matrix types, or even worse implicit conversions from 
// integer vectors to float vectors. (My Opinion: Typically conversions such as these are always best done explicitly
// rather than allowing them to happen implicitly. This mostly is to always have in place as many options as possible 
// so as to avoid unexpected bugs plus to a much lesser extent to prevent unnecessary type conversions which may in theory
// impact performance). To understand why this macro is even needed at all is due to the guiding philosophy which determines
// the nature of GLM's implementation. For GLM, the main goal is to mimic the GLSL standard as closly as possible, so thus by
// default since GLSL allows implicit type conversions such as the ones mentioned above to happen in a perfectly legal statement,
// it is by default usable in GLM unless the macro "GLM_FORCE_EXPLICIT_CTOR" is defined here in order for explicitly disabling them.
//   (My Opinion: You should leave this macro defined to disable implicit conversions)
#define GLM_FORCE_EXPLICIT_CTOR 1


//MACROS: GLM_FORCE_CXX17
//        GLM_FORCE_CXX14
//        GLM_FORCE_CXX11
//        GLM_FORCE_CXX98
//
// Dependencies:
//    - Defining Any One of the following while also defining GLM_FORCE_CXX_UNKOWN will result in the 
//        specified macro from this set winning out. 
//    - Defining a newer C++ version from amongst these macros will always cancel out any effect 
//        a macro requesting an older version might have. This means using all at once will lead to
//        C++17 being selected. Or as another example, using both GLM_FORCE_CXX98 and GLM_FORCE_CXX14
//        will always result in C++14 being used.
//
// These macros allow for specific versions of the C++ language standard to be used for building GLM. While in
// theory this would be very handy if the rest of the code were being written to conform to one of the older
// standards, it turns out that the project is pretty firmly entrenched in C++17 land. Perhaps these macros 
// may come into play at a later date once C++20 and beyond come into being.
//#define GLM_FORCE_CXX98 1
//#define GLM_FORCE_CXX11 1
//#define GLM_FORCE_CXX14 1
#define GLM_FORCE_CXX17 1


//Macro: GLM_FORCE_INLINE
// Dependencies: 
//       - none
//
//Forces GLM to inline its functions as much as possible. This should (probably) 
//result in faster executing code, assuming the compiler isn't already inlining 
//each function call to its fullest extent possible. Unless the code is displaying
//odd behavior that ceases if disabled (in which case should warrant further 
//investigation beyond just disabling this macro and moving on), this macro is
//recommended to be always be in use.
#define GLM_FORCE_INLINE 1


//Macro: GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 
// Dependencies:
//      -None     [But any use of processor intrinsics / 
//                  specialized instruction sets will require this macro]
//
//
// Blah Blah Blah Data Alignment Blah Blah Required for CPU intrinsics Blah Blah SIMD
// is great blah blah
//
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 1     


//ADDITIONAL GLM MACROs which I have yet to write any documentation
//
//  Intrinsics
//#define GLM_FORCE_PURE 0
//define GLM_FORCE_SSE2 1
//#define GLM_FORCE_SSE3 1
//#define GLM_FORCE_SSSE3 1
//#define GLM_FORCE_SSE41 1
//#define GLM_FORCE_SSE42 1
//#define GLM_FORCE_AVX 1
//#define GLM_FORCE_AVX2 1
//#define GLM_FORCE_AVX512 1
//
//
//  [There are 12 data-type-precision specified macros I haven't included]
//
//
//#define GLM_FORCE_SINGLE_ONLY 1    //Prevent GLM from ever using 64-bit types 
//
//#define GLM_FORCE_SWIZZLE 1  //Enables Swizzling
//
#define GLM_FORCE_SIZE_T_LENGTH 1 
//
//#define GLM_FORCE_SILENT_WARNINGS
//
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE 1
//
//#define GLM_FORCE_LEFT_HANDED 1
//
#define GLM_FORCE_UNRESTRICTED_GENTYPE 1
/*
//After including the main GLM header, it is still necessary to include
//some additional GLM extensions that are used in this project:

//'type_ptr' is a conversion function that allows GLM vector and GLM 
// matrix types to be accessed as though they are just arrays through
// a GLfloat* to their first element. This is necessary for sending them
// as uniforms to shader programs.
#include "ThirdParty\glm\gtc\type_ptr.hpp"   

//'Quaternion' is a special data type used to efficiently and cleanly express
// rotations in 3D. They are both cheaper and more robust than relying on Euler angles, 
// which suffer from pitfalls such as gimbal lock.
#include "ThirdParty\glm\gtc\quaternion.hpp" 

//'matrix_transform' provides functions for creating several useful matrices that 
// are used within the course of rendering. 
#include "ThirdParty\glm\gtc\matrix_transform.hpp"
*/



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  ( END SECTION 3 )   
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~







// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  ( SECTION 4 )  EasyLogging++ SETTINGS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//Macro: FECLPINAE
// Dependencies: 
//       - none
//
//By defining this macro one can dictate the Application's appropriate response to the event that any one of 
//   several Logging-utility-configuration function calls (which get made very early on during initialization)
//   fail. This macro goes by the very appropriately named and easy-to-remember acronym 'FECLPINAE', which 
//   clearly, cleanly and concisely expands to produce the statement "Failure to Establish Custom Logging 
//   Protocols Is Not An Error". 
//Essentially this macro boils down to whether the Application should crash on the spot if it can't get what
//   it wants for writing to it's many LOGS, or if it should just suck it up and continue onwards relying 
//   upon the default behavior of the Easylogger++ logging utility. It should be noted that the default behavior
//   will not result in anything being written to LOG files, so if this feature is determined to be important, then
//   there is no shame in not defining this macro.
#define FECLFINAE "Failure to Establish Custom Logging Protocols Is Not An Error"


//Macro: ENABLE_TRACE_LOG
// Dependencies: 
//       - none
//
//By defining this macro, the use of the trace log will be enabled. The trace log is an odd beast for me currently
//during development in that I feel I know exactly what it should be doing but I have not for the life of me been 
//able to get it to behave in the way I want. It's like in order for it to be verbose enough to actually 
//log all of the information that I would expect to have from a trace, it requires 3-4 lines at minimum to squeeze 
//everything in. Ideally the trace log should be short, concise, while yet also announcing important information that
//composes a 'trace', namely the name of the current file, function and line number. And adding in the current time 
//is preferable. I have yet to find a way to report all of these details in a concise enough manner to make me actually
//want to use the trace log... Actually wait I have an idea... See the macro after this one.
#define ENABLE_TRACE_LOG 1



/*  //UPDATE: REMOVED THIS OPTION!
//Macro:  TRACE_LOG_IGNORE_ALL_PASSED_MESSAGES                                     
// Dependencies: 
// -  The Trace Log must be enabled for this macro to have any effect
//
// Basically by enabling this macro the trace log will be signaled to know that its job of
//   simply reporting the trace information is tough enough and that it should ignore any 
//   incoming data passed to it. This is risky since something important may be missed, but
//   then again sending important messages to the trace log in-and-of-itself seems fairly 
//   inadvisable. Having this available might also make the Trace log usable for reporting 
//   the trace!
#define TRACE_LOG_IGNORE_ALL_PASSED_MESSAGES "YouUsedToCallMeOnYourTraceLog_LateNightWhenYouNeedMyStackTrace"
*/




/* //The following section is a work in progress and is currently not supported 
//
//     Note that the following 2 macros are used by EasyLogging++ during its initialization
//     to determine what names it will assign to the 2 default logging targets. I highly
//      recommend that these do NOT get changed because that would just be bad...
#ifndef ELPP_DEFAULT_LOGGER
//#define ELPP_DEFAULT_LOGGER "FSM_LOG"
//#define FSM_LOG ELPP_DEFAULT_LOGGER
#endif

#ifndef ELPP_DEFAULT_PERFORMANCE_LOGGER
//#define ELPP_DEFAULT_PERFORMANCE_LOGGER "FSM_PERFLOG"
//#define FSM_PERFLOG ELPP_DEFAULT_PERFORMANCE_LOGGER
#endif
//                                                                  */


//Make sure if #define ENABLE_GLFUNCTION_PROFILING_ from section 2 is enabled while 
//profiling is not enabled in this section a compilation error is triggered

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  ( END SECTION 4 )   
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




#endif //BUILD_SETTINGS_H_
