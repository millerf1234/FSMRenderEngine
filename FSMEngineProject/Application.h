//  File:                    Application.h
//  Class:                   Application
//
//  Description:                'The one class to rule them all...' 
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
//                      
// 
//                           
//                           
//
//

#pragma once

#ifndef APPLICATION_H_
#define APPLICATION_H_

#define GLFW_INCLUDE_GLCOREARB
#include "ThirdParty/easyloggingpp/include/easylogging++.h"
#include "ThirdParty/GLFW/glfw3.h"

class Application final {
public:
    Application(int argc = 0, char ** argv = nullptr);
    ~Application();

    void launch();
};

#endif //APPLICATION_H_