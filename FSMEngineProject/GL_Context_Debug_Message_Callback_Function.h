// File:                GL_Context_Debug_Message_Callback_Function.h
//
// Description:         This file defines a function that matches the signature of the Debug
//                      Callback Function an OpenGL context uses for communicating messages. 
//                      Essentially this function provides a format to use to allow OpenGL to
//                      convey information to the application. This function must be provided
//                      to OpenGL through the use of the function glDebugMessageCallback(). 
//                      Furthermore, debugging must be enabled in the GL Context through the 
//                      function call glEnable(GL_DEBUG_CONTEXT). Furthmore, for these messages  
//                      to be synchronus with the events in the application that caused them, 
//                      the function call glEnable(GL_DEBUG_OUTPUT_SYNCHRONUS) must be made.
//                      There is more to ensuring the GL Context is a debug context than is 
//                      mentioned here, but those details go beyond the scope of this application.
//
// Programmer:          Forrest Miller
// Dates:               July 2018       First Implemented a Callback Function
//                      November 13, 2018   Rewrote Callback Function to be much more detailed
//                      April 3, 2019       Ported file over into FSMEngine project
//                      
// Changes:             November 14, 2018
//                        This function used to use 'fprintf' for every single print statement. However,
//						  if synchronization between the context and the application is not enabled, this 
//                        could lead to garbled output from overlapping write calls to fprintf due to the
//                        asynchronousity. To remedy this oversight, I have gone in and replaced all of 
//                        the fprintf calls with snprintf, which prints into a buffer. Then at the end,
//                        the buffer is printed all at once with a single call to fprintf. The code is 
//                        somewhat uglier, but should behave better for asynchronus callbacks.
//

#pragma once

#ifndef GL_CONTEXT_DEBUG_MESSAGE_CALLBACK_FUNCTION_H_
#define GL_CONTEXT_DEBUG_MESSAGE_CALLBACK_FUNCTION_H_

#include <cstdio>
#include "UniversalIncludes.h"
#include "GraphicsLanguage.h"

namespace FSMEngineInternal {

    //The detail provided in this message could use a more detailed implementation
    static void GLAPIENTRY printGraphicsContextMessageCallback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam) {
        LOG(TRACE) << __FUNCTION__;

        //Implementation Detail:
        //       Due to the possibility that synchronization may be disabled between the application and the graphics context,
        //       this debug message will forgo the typical use of multiple calls to 'fprintf' for printing its output statement.
        //       Instead, the function 'snprintf' will be used, which prints each message into a buffer of known size. Once 
        //       the entire message has been completed into the buffer, the buffer is printed using a single call to 'fprintf'.
        //       This will prevent logging messages originating from the Application from appearing interlaced with the 
        //       components of a message from the context. Please note however that a complete context debug message could 
        //       appear in the middle of an Application message that is written over multiple 'fprintf' calls.


        static constexpr const size_t BUFFER_SIZE_TOTAL = 4096u; //This should be big enough to cover all cases [hopefully]

        static constexpr const size_t BUFFER_SPACE_REQUIRED_FOR_FORMATTING = 320u;


#define BUFFER_SIZE (BUFFER_SIZE_TOTAL-bufferIter)

        size_t bufferIter = 0u;
        char msgBuff[BUFFER_SIZE_TOTAL];

        //Just in case the callback message is too large for the buffer 
        if (static_cast<size_t>(length) > (BUFFER_SIZE - BUFFER_SPACE_REQUIRED_FOR_FORMATTING)) {
            //fprintf(ERRLOG, "\nGL Callback Error But The Message is too long for formatting!\nMsg: %s", message);
            std::string errorMsg = std::string("GL Callback Error But The Message is too long for formatting!\n    Message: ") +
                std::string(message);
            LOG(ERROR) << errorMsg;
            return;
        }

        //Print a message based off severity 
        if ((severity == GL_DEBUG_SEVERITY_HIGH) || (severity == GL_DEBUG_SEVERITY_MEDIUM)) {


            //Message Format depends on the message's source

            //-------------------------------------------------------------
            // Third-Party       Medium-High Priority      Call Back Message
            //-------------------------------------------------------------
            if (source == GL_DEBUG_SOURCE_THIRD_PARTY) {
                bufferIter += snprintf(msgBuff, BUFFER_SIZE,
                    "\n<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>\n");
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                    "Third Party CALLBACK ( ID = 0x%05X,  Severity = ", id); //strlen is 44
                if (GL_DEBUG_SEVERITY_HIGH) {
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "HIGH! ) "); //total strlen now is 52
                }
                else {
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "MEDIUM )"); //total strlen now is 52
                }
                switch (type) {   //The intention here is to have 'type' be right justified on same line
                case (GL_DEBUG_TYPE_ERROR):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = ERROR]\n");
                    break;
                case (GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = DEPRECATED BEHAVIOR]\n");
                    break;
                case (GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = UNDEFINED BEHAVIOR]\n");
                    break;
                case (GL_DEBUG_TYPE_PERFORMANCE):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = PERFORMANCE]\n");
                    break;
                case (GL_DEBUG_TYPE_PORTABILITY):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = PORTABILITY]\n");
                    break;
                case (GL_DEBUG_TYPE_MARKER):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = MARKER]\n");
                    break;
                case (GL_DEBUG_TYPE_PUSH_GROUP):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = PUSH GROUP]\n");
                    break;
                case (GL_DEBUG_TYPE_POP_GROUP):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = POP GROUP]\n");
                    break;
                case (GL_DEBUG_TYPE_OTHER):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = UNKNOWN]\n");
                    break;
                default:
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = INVALID_ENUM]\n");
                    break;
                }

                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", message);
                snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                    "\n<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>\n");

            } //The fprintf call is made at the end of all the MEDIUM/HIGH priority source cases

            //-------------------------------------------------------------
            // Application       Medium-High Priority      Call Back Message
            //-------------------------------------------------------------

            else if (source == GL_DEBUG_SOURCE_APPLICATION) {
                bufferIter += snprintf(msgBuff, BUFFER_SIZE,
                    "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                    "Application CALLBACK ( ID = 0x%05X,  Severity = ", id); //strlen is 44
                if (GL_DEBUG_SEVERITY_HIGH) {
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "HIGH! ) "); //total strlen now is 52
                }
                else {
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "MEDIUM )"); //total strlen now is 52
                }
                switch (type) {   //The intention here is to have 'type' be right justified on same line 
                case (GL_DEBUG_TYPE_ERROR):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = ERROR]\n");
                    break;
                case (GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = DEPRECATED BEHAVIOR]\n");
                    break;
                case (GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = UNDEFINED BEHAVIOR]\n");
                    break;
                case (GL_DEBUG_TYPE_PERFORMANCE):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = PERFORMANCE]\n");
                    break;
                case (GL_DEBUG_TYPE_PORTABILITY):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = PORTABILITY]\n");
                    break;
                case (GL_DEBUG_TYPE_MARKER):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = MARKER]\n");
                    break;
                case (GL_DEBUG_TYPE_PUSH_GROUP):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = PUSH GROUP]\n");
                    break;
                case (GL_DEBUG_TYPE_POP_GROUP):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = POP GROUP]\n");
                    break;
                case (GL_DEBUG_TYPE_OTHER):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = UNKNOWN]\n");
                    break;
                default:
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = INVALID_ENUM]\n");
                    break;
                }

                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", message);
                snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                    "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"); //__ characters

            }



            //-------------------------------------------------------------
            // GL Context       Medium-High Priority      Call Back Message
            //-------------------------------------------------------------

            else if (source != GL_DEBUG_SOURCE_OTHER) {
                bufferIter += snprintf(msgBuff, BUFFER_SIZE,
                    "\n*******************************************************************************\n"); //80 characters
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                    "GL Context CALLBACK ( ID = 0x%05X,  Severity = ", id); //strlen is 43
                if (GL_DEBUG_SEVERITY_HIGH) {
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                        "HIGH! )  "); //total strlen now is 52  (extra spaces at end of string are intentional)
                }
                else {
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                        "MEDIUM ) "); //total strlen now is 52
                }

                switch (type) {   //The intention here is to have 'type' be right justified on same line 
                case (GL_DEBUG_TYPE_ERROR):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = ERROR]\n");
                    break;
                case (GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = DEPRECATED BEHAVIOR]\n");
                    break;
                case (GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = UNDEFINED BEHAVIOR]\n");
                    break;
                case (GL_DEBUG_TYPE_PERFORMANCE):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = PERFORMANCE]\n");
                    break;
                case (GL_DEBUG_TYPE_PORTABILITY):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = PORTABILITY]\n");
                    break;
                case (GL_DEBUG_TYPE_MARKER):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = MARKER]\n");
                    break;
                case (GL_DEBUG_TYPE_PUSH_GROUP):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = PUSH GROUP]\n");
                    break;
                case (GL_DEBUG_TYPE_POP_GROUP):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = POP GROUP]\n");
                    break;
                case (GL_DEBUG_TYPE_OTHER):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = OTHER]\n");
                    break;
                default:
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%28s", "[type = INVALID_ENUM]\n");
                    break;
                }

                switch (source) {
                case (GL_DEBUG_SOURCE_API):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "[Source: GL API]\n");
                    break;
                case (GL_DEBUG_SOURCE_WINDOW_SYSTEM):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "[Source: WINDOW SYSTEM]\n");
                    break;
                case (GL_DEBUG_SOURCE_SHADER_COMPILER):
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "[Source: SHADER COMPILER]\n");
                    break;
                default:
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "[Source: UNKNOWN ENUM]\n");
                    break;
                }

                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", message);
                snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                    "\n*******************************************************************************\n"); //80 characters
            }

            //-------------------------------------------------------------
            // UNKNOWN SOURCE       Medium-High Priority      Call Back Message
            //-------------------------------------------------------------

            else {
                bufferIter += snprintf(msgBuff, BUFFER_SIZE,
                    "\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "UNKNOWN SOURCE CALLBACK!!! PRIORITY IS ");
                if (GL_DEBUG_SEVERITY_HIGH) {
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "HIGH!\n");
                }
                else {
                    bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "MEDIUM\n");
                }

                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "\nMessage is: %s", message);
                snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                    "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            }

            //Now print the buffer with a single call to fprintf
            //fprintf(ERRLOG, "%s", msgBuff);
            LOG(WARNING) << msgBuff;
            LOG(INFO) << msgBuff;
            return;
        }

        //-------------------------------------------------------------
        // ANY SOURCE               Low Priority           Call Back Message
        //-------------------------------------------------------------

        else if (severity == GL_DEBUG_SEVERITY_LOW) {


            bufferIter += snprintf(msgBuff, BUFFER_SIZE,
                "\n-------------------------------------------------------------------------------\n");
            bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                "\n[LOW PRIORITY] GL CALLBACK (Source = ");

            switch (source) {
            case GL_DEBUG_SOURCE_API:   //The source is from direct usage of OpenGL API
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "OpenGL API, ");
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "Window System, ");
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "Shader Compiler, ");
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "Third Party, ");    //Message originated from a third-party source 
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "Application, ");
                break;
            case GL_DEBUG_SOURCE_OTHER:
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "OTHER, ");
                break;
            default:
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "UNKNOWN, ");
                break;
            }

            bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "ID = 0x%05X, Type = ", id);


            switch (type) {   //The intention here is to have 'type' be right justified on same line 
            case (GL_DEBUG_TYPE_ERROR):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", "ERROR)\n");
                break;
            case (GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", "DEPRECATED BEHAVIOR)\n");
                break;
            case (GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", "UNDEFINED BEHAVIOR)\n");
                break;
            case (GL_DEBUG_TYPE_PERFORMANCE):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", "PERFORMANCE)\n");
                break;
            case (GL_DEBUG_TYPE_PORTABILITY):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", "PORTABILITY)\n");
                break;
            case (GL_DEBUG_TYPE_MARKER):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", "MARKER)\n");
                break;
            case (GL_DEBUG_TYPE_PUSH_GROUP):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", "PUSH GROUP)\n");
                break;
            case (GL_DEBUG_TYPE_POP_GROUP):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", "POP GROUP)\n");
                break;
            case (GL_DEBUG_TYPE_OTHER):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", "OTHER)\n");
                break;
            default:
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", "INVALID_ENUM)\n");
                break;
            }
            bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "%s", message);
            snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                "\n-------------------------------------------------------------------------------\n");
        }



        //-------------------------------------------------------------
        // ANY SOURCE                Context Notification        Call Back Message
        //-------------------------------------------------------------


        else {
            bufferIter += snprintf(msgBuff, BUFFER_SIZE,
                "\n*******************   GL Context Notification    *******************\n");
            bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "[ID = 0x%05X, Type = ", id);
            switch (type) {
            case (GL_DEBUG_TYPE_ERROR):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "ERROR]");
                break;
            case (GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "DEPRECATED_BEHAVIOR]");
                break;
            case (GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "UNDEFINED_BEHAVIOR]");
                break;
            case (GL_DEBUG_TYPE_PERFORMANCE):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "PERFORMANCE]");
                break;
            case (GL_DEBUG_TYPE_PORTABILITY):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "PORTABILITY]");
                break;
            case (GL_DEBUG_TYPE_MARKER):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "MARKER]");
                break;
            case (GL_DEBUG_TYPE_PUSH_GROUP):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "PUSH_GROUP]");
                break;
            case (GL_DEBUG_TYPE_POP_GROUP):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "POP_GROUP]");
                break;
            case (GL_DEBUG_TYPE_OTHER):
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "OTHER]");
                break;
            default:
                bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "INVALID TYPE ENUM!!!]");
                break;
            }

            bufferIter += snprintf(msgBuff + bufferIter, BUFFER_SIZE, "\n%s", message);
            snprintf(msgBuff + bufferIter, BUFFER_SIZE,
                "\n********************************************************************\n");
        }

        LOG(WARNING) << msgBuff;
        LOG(INFO) << msgBuff;
        //fprintf(MSGLOG, "%s\n", msgBuff);
    }



} //namespace FSMEngineInternal

//Undefine macro so as to avoid polluting other files
#ifdef BUFFER_SIZE 
#undef BUFFER_SIZE
#endif 



//Extra formatting for just in case:
			//switch (type) {
			//case (GL_DEBUG_TYPE_ERROR):
	        //
			//	break;
			//case (GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):
	        //
			//	break;
			//case (GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):
	        //
			//	break;
			//case (GL_DEBUG_TYPE_PERFORMANCE):
	        //
			//	break;
			//case (GL_DEBUG_TYPE_PORTABILITY):
	        //
			//	break;
			//case (GL_DEBUG_TYPE_MARKER):
	        //
			//	break;
			//case (GL_DEBUG_TYPE_PUSH_GROUP):
	        //
			//	break;
			//case (GL_DEBUG_TYPE_POP_GROUP):
	        //
			//	break;
			//case (GL_DEBUG_TYPE_OTHER):
	        //
			//	break;
			//default:
	        //
			//	break;
			//}
		
	




#endif //GL_CONTEXT_DEBUG_MESSAGE_CALLBACK_FUNCTION_H_

