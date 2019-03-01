//File:        glm_includer.h
//
//Description:   This file is a wrapper for including the OpenGL Mathematics library 'glm.h' 
//
// GLM Configuration:
//            GLM has several various configuration options available to it which are determined at 
//                compile-time. As is common when specifing compile-time settings, these are 
//                enabled/disabled by defining/leaving-undefined a specific macro which determines
//                each setting. These macros are found within the GLM documentation. 
//
//            To keep the number of places in where project-wide build settings can be specified to a minimum,
//                all of the GLM-specific build macros can be found located in their own section of "BuildSettings.h"
//               
//             
//
// This header's position amongst the various Graphics-Language itermediate header include-chain structure:
//
//            Top                    "BuildSettings.h" ---------------------+
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
//                                                                                          to be included whereever they are needed]
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
#include "..\..\BuildSettings.h"


#include "..\glm\glm.hpp"



#endif   //GLM_INCLUDER_H_