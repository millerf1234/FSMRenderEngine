// 
// File:                                                           FSMEngine.h
//
// Header Status:
//                +++++++++++++++++++++++++++++++++++++++
//                +++                                 +++
//                ++   INCOMPLETE / WORK IN PROGRESS   ++
//                +++                                 +++
//                +++++++++++++++++++++++++++++++++++++++
//
// Description:                        Master header file that can be included
//                                     within a project to provide full access 
//                                              to all FSMEngine functionality.
//                                                                             
//                                                                             
//          [DEVELOPMENT NOTE-TO-SELf]                                                   
//  Guideline          This file is built to serve as the 'tip of the FSMEngine  
//                           spear'. It is the primary liaison between the full  
//                         FSMEngine architecture and client code. As such, the  
//                        documentation in this file should be held to a higher  
//                        standard of clarity and accuracy than may be expected  
//                         (and found) within most other source files. A design  
//                     emphasis for this file is to maintain focus, clarity and      
//                         a clear distinction between components. I don't know
//                      where I am going with this but I already am breaking my 
//                      rule for trying to hold the documentation for this file
//                                                       to a higher standard...
//                                                                             
//   +=============================+=============================+================================+   
//   |          [FEATURE]          |          [Header]           |         [Requirements]         |
//   +-----------------------------+-----------------------------+--------------------------------+
//   |   GLM OpenGL Math Library   |                             | No Initialization is required  |
//   |    -- GLM types             |                             |    to use GLM types/functions. |
//   |     (e.g. glm::vec3,        |                             |                                |
//   |           glm::mat4, etc.)  |     "GraphicsLanguage.h"    | Some GLM types/extensions re-  |
//   |    -- CG Util functions     |                             | quire explicit configurations  |
//   |   (e.g. proj(), frust(),    |                             | to be specified. See the GLM   |
//   |                             |                             | section within configuration   |
//   |                             |                             |      header "BuildSettings.h   |                              |
//   +-----------------------------+-----------------------------+--------------------------------+
//   |                             |                             |                                |
//   |   LOGGING LEVELS                           |                             |                                |
//   |                             |                             |                                |
//              
//                     +-------------+
//                     |    SETUP    |
//                     +-------------+
//           (Step 1)
//    DESCRIPTION:             Initialize and configure message logging  
//    IMPLEMENTATION:          Call function 
//    ENABLES:
//
//           (Step 2)
//    DESCRIPTION:
//    IMPLEMENTATION:
//
//           (Step 3)
//    DESCRIPTION:
//    IMPLEMENTATION:
//
//           (Step 4)
//    DESCRIPTION:
//    IMPLEMENTATION:
//
//


#pragma once

#ifndef FSM_ENGINE_H_
#define FSM_ENGINE_H_

#include "GraphicsLanguageFramework.h"
#include "UniversalIncludes.h"
#include "FSMInitConfig.h"

#include "FSMEngineBegin.h"
#include "FSMFastInit.h"


#endif //FSM_ENGINE_H_