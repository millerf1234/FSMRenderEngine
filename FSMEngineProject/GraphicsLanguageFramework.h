//
//  File:            GraphicsLanguageFramework.h
//
//  Description:     Ties together the OpenGL programming lanuage and GLM mathematics 
//                   libraries from the "GraphicsLanguage.h" header and incorperates the 
//                   additional "glfw3.h" header to create the complete to provide a complete 
//                   set of useful functions for 
//                   graphics programming.   (something like that)
//
//                   This means including this header provides:
//                           1) The Entire OpenGL 4.6 Core Lanuage 
//                           2) All Imported Functionality from GLM to provide access to GLSL types and syntax
//                           3) All of the Window, Utility and UI types and functions that come with "GLFW3.h"
//
//                  Essentially, this is the one header to rule them all
//
//  Big Picture:
//
// This header's position amongst the various Graphics-Language itermediate header include-chain structure:
//
//          Top                    "BuildSettings.h" ---------------------+
//                                          |                             |
//                                          |                             |
//                                          V                             |
//                                 "glad_includer.h" ---------------------)--------------------+
//                                          |                             |                    |
//                                          |                             V                    |
//                                          V                     "glm_includer.h"             |      
//                                 "glfw_includer.h"                      |                    |
//                                          |                             |                    |
//                                          |                             V                    |
//                                          V                   "GraphicsLanuage.h" <----------+
//                +-------------------------------------------+           |    
//                | THIS FILE  "GraphicsLanguageFramework.h"  | <---------+
//                +-------------------------------------------+           |
//                                          |                             |
//                                          V                             |
//         Bottom                    Rest of the Codebase  <--------------+    
//
//                                                                      [ Observer that both the less-extensive header file "GraphicsLanguage.h" 
//                                                                          and its this header file "GraphicsLanguageFramework.h"
//                                                                          are intended to be included freely everywhere they are needed.
//                                                                          This file has all of the additional  GLFW Window-UserInterface-Utility
//                                                                          related functionality that is missing from the "GraphicsLanguage.h"
//                                                                          header ] 
//                                                                                          
//
//
//

#pragma once

#ifndef GRAPHICS_LANGUAGE_FRAMEWORK_H_
#define GRAPHICS_LANGUAGE_FRAMEWORK_H_




#include "GraphicsLanguage.h"   

#include "ThirdParty/ThirdParty_IncludeWrappers/glfw_includer.h"








#endif //GRAPHICS_LANGUAGE_FRAMEWORK_H_
