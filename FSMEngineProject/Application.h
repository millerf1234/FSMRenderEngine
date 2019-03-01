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




class Application final {
public:
    Application(int argc = 0, char ** argv = nullptr);
    ~Application();

    void launch();

    //There should at (99.9% of) all times be only one Application object. Copying is not allowed...
    Application(const Application& that) = delete;
    Application& operator=(const Application& that) = delete;
private:
    static short applicationsInExistance; //Prevents multiple applications from being created

};

#endif //APPLICATION_H_