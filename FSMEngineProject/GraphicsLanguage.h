//
//  File:            GraphicsLanguage.h
//
//  Description:     Ties together the OpenGL programming lanuage and GLM mathematics 
//                   library to provide a complete set of useful functions for 
//                   graphics programming.
//
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
//                                          |                             |                    |
//                                          V                     "glm_includer.h"             |      
//                                 "glfw_includer.h"                      |                    |
//                                          |                             V                    |
//                                          |       +---------------------------------+        |
//                                          |       | THIS FILE  "GraphicsLanuage.h"  | <------+
//                                          |       +---------------------------------+      
//                                          V                             |
//                            "GraphicsLanguageFramework.h"  <------------+
//                                          |                             |
//                                          V                             |
//         Bottom                    Rest of the Codebase  <--------------+    
//
//                                                                      [ Observer that both this file "GraphicsLanguage.h" 
//                                                                          and its larger header relative "GraphicsLanguageFramework.h"
//                                                                          are intended to be included whereever they are needed.
//                                                                          This file doesn't implement any of the GLFW Window-Interface
//                                                                          related functionality, that comes from the other header ] 
//                                                                                          
//
//
//


#pragma once

#ifndef GRAPHICS_LANGUAGE_H_
#define GRAPHICS_LANGUAGE_H_


#include "ThirdParty/ThirdParty_IncludeWrappers/glad_includer.h"

#include "ThirdParty/ThirdParty_IncludeWrappers/glm_includer.h"


#endif //GRAPHICS_LANGUAGE_H_