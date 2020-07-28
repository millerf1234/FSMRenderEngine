//File:        glm_includer.h
//
//Description:   This file is a wrapper for including the OpenGL Mathematics library 'glm.h' 
//
// GLM Configuration:
//            GLM has several various configuration options available to it which are determined at 
//                compile-time. As is common when specifying compile-time settings, these are 
//                enabled/disabled by defining/leaving-undefined a specific macro which determines
//                each setting. These macros are found within the GLM documentation. 
//
//            To keep the number of places in where project-wide build settings can be specified to a minimum,
//                all of the GLM-specific build macros can be found located in their own section of "BuildSettings.h"
//               
//             
//
// This header's position amongst the various Graphics-Language intermediate header include-chain structure:
//
//            Top                  "FSMEngineBuildConfig.h" ----------------+
//                                            |                             |
//                                            |                             |
//                                            V                             |
//                                   "glad_includer.h" ---------------------)-------------------+
//                                            |                             V                   |
//                                            |         +------------------------------+        |
//                                            |         | THIS FILE  "glm_includer.h"  |        |
//                                            V         +------------------------------+        |
//                                   "glfw_includer.h"                      |                   |
//                                            |                             V                   |
//                                            |                    "GraphicsLanuage.h" <--------+
//                                            |                             |
//                                            V                             |
//                              "GraphicsLanguageFramework.h"  <------------+
//                                            |                             |
//                                            V                             |
//           Bottom                    Rest of the Codebase  <--------------+    [Both "GraphicsLanguage.h" and "GraphicsLanguageFramework.h" are intended 
//                                                                                          to be included wherever they are needed]
//
//
//
//
//
//
//


#pragma once

#ifndef GLM_INCLUDER_H_
#define GLM_INCLUDER_H_

//All of the specific macros which control GLM's build settings are set within "BuildSettings.h"
#include "..\..\FSMEngineBuildConfig.h"


//As promised in section 3 of "BuildSettings.h", if the special macro is defined requesting a 
//complete rundown of every specified GLM macro, then it shall be done.
#ifdef FSM_ENGINE_REPORT_GLM_REQUESTED_CONFIGURATION
#pragma message ( ""                                                            \
"                       +-----------------------------------------------+\n"    \
"                       |     Reporting Complete GLM Configuration      |\n"    \
"                       +-----------------------------------------------+\n"    )

//So the syntax here is a little weird because it turns out every call to '#pragma message()'
//automatically is appended with a new line by the compiler. Thus to keep the responses on the same
//line as their
//prompt, there is some macro trickery at play that produces the desired output. The key behind
//the syntax is that there can be any number of string within the parenthesis of a 'message' 
//call. Finally, to understand why there always needs to be an empty string at the start of each
//message, try deleting one of them and then compiling. The compiler doesn't like that the first
//argument to 'message()' is a macro, it wants a legit string there instead. So the empty string 
//is vital to formulating the correct output.
//
//Here is what that looked like:

/*
#define msg "\tGLM_FORCE_MESSAGES:           "
#ifdef GLM_FORCE_MESSAGES 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#define msg "\tGLM_FORCE_CXX98:              "
#ifdef GLM_FORCE_CXX98 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg
*/
//etc...

//And now as an update I have decided the output was still too ugly when printed single-line by single-line
//so I have come up with the following masterpiece of joyful pre-processor abuse to get my well-formatted 
//output (I even was nice and included some c-style inline comments to give the nested statements a small
//indentation, hopefully slightly improving readability):

#define L_MSG "\tGLM_FORCE_MESSAGES:           "
#define R_MSG "            GLM_FORCE_CXX98:              "
#ifdef GLM_FORCE_MESSAGES 
/* */#ifdef GLM_FORCE_CXX98 
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Undefined")
/* */#endif 
#else 
/* */#ifdef GLM_FORCE_CXX98 
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Undefined")
/* */#endif 
#endif 
#undef L_MSG
#undef R_MSG


#define L_MSG "\tGLM_FORCE_PLATFORM_UNKNOWN:   "
#define R_MSG "            GLM_FORCE_CXX11:              "
#ifdef GLM_FORCE_PLATFORM_UNKNOWN
/* */#ifdef GLM_FORCE_CXX11 
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Undefined")
/* */#endif 
#else 
/* */#ifdef GLM_FORCE_CXX11 
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Undefined")
/* */#endif 
#endif 
#undef L_MSG
#undef R_MSG


#define L_MSG "\tGLM_FORCE_COMPILER_UNKNOWN:   "
#define R_MSG "            GLM_FORCE_CXX14:              "
#ifdef GLM_FORCE_COMPILER_UNKNOWN
/* */#ifdef GLM_FORCE_CXX14 
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Undefined")
/* */#endif 
#else 
/* */#ifdef GLM_FORCE_CXX14 
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Undefined")
/* */#endif 
#endif 
#undef L_MSG
#undef R_MSG


#define L_MSG "\tGLM_FORCE_ARCH_UNKNOWN:       "
#define R_MSG "            GLM_FORCE_CXX17:              "
#ifdef GLM_FORCE_ARCH_UNKNOWN       
/* */#ifdef GLM_FORCE_CXX17 
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Undefined")
/* */#endif 
#else 
/* */#ifdef GLM_FORCE_CXX17 
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Undefined")
/* */#endif 
#endif 
#undef L_MSG
#undef R_MSG


#define L_MSG "\tGLM_FORCE_CXX_UNKNOWN:        "
#define R_MSG "            GLM_FORCE_CSS_UNKNOWN:        "
#ifdef GLM_FORCE_CXX_UNKNOWN       
/* */#ifdef GLM_FORCE_CSS_UNKNOWN 
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Undefined")
/* */#endif 
#else 
/* */#ifdef GLM_FORCE_CSS_UNKNOWN 
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Undefined")
/* */#endif 
#endif 
#undef L_MSG
#undef R_MSG


#define L_MSG "\tGLM_FORCE_EXPLICIT_CTOR:      "
#define R_MSG "            GLM_FORCE_INLINE:             "
#ifdef GLM_FORCE_EXPLICIT_CTOR       
/* */#ifdef GLM_FORCE_INLINE 
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Defined  " R_MSG "Undefined")
/* */#endif 
#else 
/* */#ifdef GLM_FORCE_INLINE 
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Defined")
/* */#else  
/*   */#pragma message("" L_MSG "Undefined" R_MSG "Undefined")
/* */#endif 
#endif 
#undef L_MSG
#undef R_MSG


#define msg "                                           GLM_FORCE_DEFAULT_ALIGNED_GENTYPES:     "
#ifdef GLM_FORCE_DEFAULT_ALIGNED_GENTYPES 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#define IntrinsicsMsg "              GLM Intrinsics\n"
#define msg "\tGLM_FORCE_PURE:               "
#ifdef GLM_FORCE_PURE 
#pragma message("" InstrinsicsMsg  "" msg "Defined")
#else 
#pragma message("" IntrinsicsMsg "" msg "Undefined")
#endif
#undef IntrinsicsMsg
#undef msg

#define msg "\tGLM_FORCE_SSE2:               "
#ifdef GLM_FORCE_SSE2 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#define msg "\tGLM_FORCE_SSE3:               "
#ifdef GLM_FORCE_SSE3 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#define msg "\tGLM_FORCE_SSSE3:              "
#ifdef GLM_FORCE_SSSE3 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#define msg "\tGLM_FORCE_SSE41:              "
#ifdef GLM_FORCE_SSE41 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#define msg "\tGLM_FORCE_SSE42:              "
#ifdef GLM_FORCE_SSE42 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#define msg "\tGLM_FORCE_AVX:                "
#ifdef GLM_FORCE_AVX 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#define msg "\tGLM_FORCE_AVX2:               "
#ifdef GLM_FORCE_AVX2 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")  
#endif 
#undef msg

#define msg "\tGLM_FORCE_AVX512:             "
#ifdef GLM_FORCE_AVX512 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#define msg "\tGLM_FORCE_SINGLE_ONLY:        "
#ifdef GLM_FORCE_SINGLE_ONLY
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#define msg "\tFloating Point Precision:     "
#ifdef GLM_FORCE_PRECISION_LOWP_FLOAT
#pragma message("" msg "LOW")
#elif defined GLM_FORCE_PRECISION_MEDIUMP_FLOAT
#pragma message("" msg "MEDIUM")
#elif defined GLM_FORCE_PRECISION_HIGHP_FLOAT
#define message("" msg "HIGH")
#else 
#pragma message("" msg "HIGH [BY DEFAULT]")
#endif
#undef msg

#ifndef GLM_FORCE_SINGLE_ONLY
#define msg "\tDouble Precision:             "
#ifdef GLM_FORCE_PRECISION_LOWP_DOUBLE
#pragma message("" msg "LOW")
#elif defined GLM_FORCE_PRECISION_MEDIUMP_DOUBLE
#pragma message("" msg "MEDIUM")
#elif defined GLM_FORCE_PRECISION_HIGHP_DOUBLE
#define message("" msg "HIGH")
#else 
#pragma message("" msg "HIGH [BY DEFAULT]")
#endif
#undef msg
#endif  //GLM_FORCE_SINGLE_ONLY

#define msg "\t[Signed] Integer Precision:   "
#ifdef GLM_FORCE_PRECISION_LOWP_INT
#pragma message("" msg "LOW")
#elif defined GLM_FORCE_PRECISION_MEDIUMP_INT
#pragma message("" msg "MEDIUM")
#elif defined GLM_FORCE_PRECISION_HIGHP_INT
#define message("" msg "HIGH")
#else 
#pragma message("" msg "HIGH [BY DEFAULT]")
#endif
#undef msg

#define msg "  [Unsigned] Integer Precision: "
#ifdef GLM_FORCE_PRECISION_LOWP_UINT
#pragma message("" msg "LOW")
#elif defined GLM_FORCE_PRECISION_MEDIUMP_UINT
#pragma message("" msg "MEDIUM")
#elif defined GLM_FORCE_PRECISION_HIGHP_UINT
#define message("" msg "HIGH")
#else 
#pragma message("" msg "HIGH [BY DEFAULT]")
#endif
#undef msg

#define msg "\tGLM_FORCE_SWIZZLE:            "
#ifdef GLM_FORCE_SWIZZLE 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#define msg "\tGLM_FORCE_SIZE_T_LENGTH:      "
#ifdef GLM_FORCE_SIZE_T_LENGTH 
#pragma message("" msg "Defined")
#else 
#pragma message("" msg "Undefined")
#endif 
#undef msg

#pragma message("\n\n")

#endif //FSM_ENGINE_REPORT_GLM_REQUESTED_CONFIGURATION


#include "..\glm\glm.hpp"


#endif   //GLM_INCLUDER_H_