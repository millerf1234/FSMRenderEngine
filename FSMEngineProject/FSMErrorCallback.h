//File:                     FSMErrorCallback.h
//
//  Defines the callback function to be used with GLFW for reporting errors. This function is
//  to be assigned once per application and will be used for every window/context. Note that this
//  behavior is different from every other available GLFW callback, which are assigned on a per-context
//  or per-window basis. This is the only callback function that does not require GLFW to be initialized 
//  by the function glfwInit() having called.
//
//  Important Note!: The description message parameter will be UTF-8 encoded.
//                     A UTF-8 sequence can only start with values 0xC0 or greater,
//                     so there is no need to worry about possible interference
//                     with ASCII.
//  For more information on UTF-8, see: https://www.cprogramming.com/tutorial/unicode.html
//  
//
// Programmer:              Forrest Miller
// Date:                    December 13, 2018




//++           _REFERENCE_                  
//--     https://www.glfw.org/docs/latest/group__errors.html
//  
//  #define       GLFW_NOT_INITIALIZED   0x00010001
//      	GLFW has not been initialized. 
// 
//  #define       GLFW_NO_CURRENT_CONTEXT   0x00010002
//       	No context is current for this thread.
// 
//  #define       GLFW_INVALID_ENUM   0x00010003
//          One of the arguments to the function was an invalid enum value. 
// 
//  #define       GLFW_INVALID_VALUE   0x00010004
// 	        One of the arguments to the function was an invalid value. 
// 
//  #define       GLFW_OUT_OF_MEMORY   0x00010005
//          A memory allocation failed. 
// 
//  #define       GLFW_API_UNAVAILABLE   0x00010006
//          GLFW could not find support for the requested API on the system. 
//
//  #define       GLFW_VERSION_UNAVAILABLE   0x00010007
//          The requested OpenGL or OpenGL ES version is not available. 
// 
//  #define       GLFW_PLATFORM_ERROR   0x00010008
//          A platform-specific error occurred that does not match any of the more specific categories. 
// 
//  #define       GLFW_FORMAT_UNAVAILABLE   0x00010009
//          The requested format is not supported or available. 
// 
//  #define       GLFW_NO_WINDOW_CONTEXT   0x0001000A
//          The specified window does not have an OpenGL or OpenGL ES context. 
//
//



#pragma once

#ifndef FSM_ERROR_CALLBACK_H_
#define FSM_ERROR_CALLBACK_H_

#include <sstream>
#include "UniversalIncludes.h" 
#include "GraphicsLanguageFramework.h"

//Callback function for GLFW error reporting. Unlike [almost*] every other GLFW callback function,             
//this function is set globally for the entire library as opposed to on a per-window basis.                               *Joystick and Monitor callbacks are exceptions
namespace FSMEngineInternal {


	inline void FSMErrorCallbackFunction(int error, const char* description) {
        LOG(TRACE) << __FUNCTION__;


        static constexpr const char* infoLogMsg = "\n\n"
            "  ###################################################\n"
            "  #` _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/`#\n"
            "  #`+ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ +`#\n"
            "  #`|          GLFW REPORTED AN ERROR !!!         |`#\n"
            "  #`|   Check Error Log File for more detail...   |`#\n"
            "  #`+_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_+`#\n"
            "  #` / / / / / / / / / / / / / / / / / / / / / / / `#\n"
            "  ###################################################\n"
            "\n\n";


        std::ostringstream errorMessage;
        
        //Build a formated error message printout
       
        errorMessage << "\n\n";
        errorMessage << "GLFW Error " << error << "!     [";
        switch (error) {
        case (GLFW_NOT_INITIALIZED):
            errorMessage << "GLFW_NOT_INITIALIZED";
            break;
        case (GLFW_NO_CURRENT_CONTEXT):
            errorMessage << "GLFW_NO_CURRENT_CONTEXT";
            break;
        case (GLFW_INVALID_ENUM):
            errorMessage << "GLFW_INVALID_ENUM";
            break;
        case (GLFW_INVALID_VALUE):
            errorMessage << "GLFW_INVALID_VALUE";
            break;
        case (GLFW_OUT_OF_MEMORY):
            errorMessage << "GLFW_OUT_OF_MEMORY";
            break;
        case (GLFW_API_UNAVAILABLE):
            errorMessage << "GLFW_API_UNAVAILABLE";
            break;
        case (GLFW_VERSION_UNAVAILABLE):
            errorMessage << "GLFW_VERSION_UNAVAILABLE";
            break;
        case (GLFW_PLATFORM_ERROR):
            errorMessage << "GLFW_PLATFORM_ERROR";
            break;
        case (GLFW_FORMAT_UNAVAILABLE):
            errorMessage << "GLFW_FORMAT_UNAVAILABLE";
            break;
        case (GLFW_NO_WINDOW_CONTEXT):
            errorMessage << "GLFW_NO_WINDOW_CONTEXT";
            break;
        default:
            errorMessage << "!!!!  Unrecognized GLFW Error  !!!!";
        }
        errorMessage << "]\n";
        errorMessage << "ERROR DESCRIPTION: " << description << "\n";
        errorMessage << "\n\n";
        
        //Print the message to the ERRLOG
        LOG(ERROR) << errorMessage.str();
	}
} //namespace FSMEngineInternal



#endif //GL_FRAMEWORK_ERROR_CALLBACK_H_