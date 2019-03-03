//File:        glfw_includer.h
//
//Description:   This file is a wrapper for including the OpenGL Framework library 'glfw3.h'. 
//               
// This header's position amongst the various Graphics-Language itermediate header include-chain structure:
//
//
//
//            Top                    "BuildSettings.h" ---------------------+
//                                            |                             |
//                                            |                             |
//                                            V                             |
//                                   "glad_includer.h" ---------------------)-------------------+
//                                            |                             |                   |
//                                            V                             V                   |
//                 +------------------------------------+           "glm_includer.h"            |
//                 |  THIS FILE      "glfw_includer.h"  |                   |                   |
//                 +------------------------------------+                   V                   |
//                                            |                    "GraphicsLanuage.h" <--------+
//                                            |                             |
//                                            V                             |
//                              "GraphicsLanguageFramework.h"  <------------+
//                                            |                             |
//                                            V                             |
//           Bottom                    Rest of the Codebase  <--------------+    [Both "GraphicsLanguage.h" and "GraphicsLanguageFramework.h" are intended 
//                                                                                          to be included whereever they are needed]


#pragma once

#ifndef GLFW_INCLUDER_H_
#define GLFW_INCLUDER_H_

#include "glad_includer.h"


#define GLFW_INCLUDE_GLCOREARB

#include "..\\GLFW\\glfw3.h" 






#endif //GLFW_INCLUDER_H_
