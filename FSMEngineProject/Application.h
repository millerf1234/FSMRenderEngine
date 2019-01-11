//  File:                    Application.h
//  Class:                   Application
//
//
//  Updated Description:    Has a constructor that sets some global state. As of right now, the 
//                          constructor does not perform any actions that will throw (at least in
//                          my code, not sure about 3rd party code it calls).  
//                          This class has one function called 'launch()', which creates an instance
//                          of the main-menu class, which then everything else runs from.
//
//
//  Old Description:             'The one class to rule them all...' 
//                           What is there to say? This class provides the global encapsulating backbone
//                           for the Application to run inside. Essentially this class can be thought 
//                           of in terms of the three phases of execution it implements:
// 
//               [Phase 1]   APPLICATION CONSTRUCTION
//                               This phase follows the RAII-design pattern, which in this particular case 
//                               means that 
//                               
//                           
//                           
//                           
// Programmer:   Forrest Miller                           
// Date:         January 9, 2019
//

#pragma once

#ifndef APPLICATION_H_
#define APPLICATION_H_



#include "GLIncludes.h"

//#define GLFW_INCLUDE_GLCOREARB
//#include "ThirdParty/GLFW/glfw3.h"

class Application final {
public:
    Application(int argc = 0, char ** argv = nullptr);
    ~Application();

    void launch();
};

#endif //APPLICATION_H_