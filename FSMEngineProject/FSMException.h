//
// File:        FSMException.h
//
// Description:   This file contains definitions for FSMEngine-specific exception classes.
//
//                As a general guideline, to keep exception handling straight-forward, client 
//                code can typically expect FSMExceptions to only be thrown within the constructors
//                of FSMEngine objects.  Each FSMEgnine type will (hopefully) have all of its 
//                exceptions clearly documented, and any functionality besides a constructor that 
//                may throw will (hopefully) be clearly documented. Each type's constructor will
//                document the specific exceptions it may throw. 
//                
//                There are 2 types of FSMExceptions to be aware of.
//                Both types publicly inherit from the built-in C++ type std::exception.
//
//                The two exception types are:
//
//                     FSMException:       This is the more general of the exception types and typically
//                                         is not specific enough for it to be expected for client code to 
//                                         know how to respond. Each FSMException type has it's own stored
//                                         string data member which contains a message providing details as
//                                         to the nature of the stored exception. Recommended action upon
//                                         catching an exception of this type is to report it to the LOGS and
//                                         either re-attempt the operation or crash gracefully. 
//                 
//                    FSMNamedException:   This is the more specific of the two exception types, though it in
//                                         fact inherits from FSMException so can be caught by catch statements
//                                         looking for the first type. These exceptions represent clearly-
//                                         documented conditions that client code can actively be prepared to 
//                                         have an appropriate response to. These exceptions contain a public 
//                                         enumeration type representing every possible named exception. Upon 
//                                         catching a named exception, a switch statement can be utilized which
//                                         contains appropriate responses to any expected named exception.
//
//                          
//               MORE DOCUMENTATION FORTHCOMING AS CODE PROGRESSES...
//
//                Multi-thread support forthcoming
//
//  Code is based off top answer at: 
//      https://stackoverflow.com/questions/8152720/correct-way-to-inherit-from-stdexception
//
//
//  IMPORTANT: Note Scope Of Re-thrown Exceptions  https://stackoverflow.com/questions/6185957/rethrowing-exceptions
//
//  Design Question:  Should the calls to these exception's constructors/destructors be recorded within
//                     the trace log? I for now am leaning towards not having them logged as part of the
//                     trace since that will keep things simpler. Hopefully choosing not to log these calls
//                     won't come back to bite me (or another programmer) one day in the future...
//
//
//
//         Table of Named Exceptions:
//          [Note: It looks like the OS handles reporting a missing '.dll', so no exception is needed for this event]     
//                                                                                                                                   
//        +==========================+=================================================================================+  
//        |      EXCEPTION NAME      |                              EXCEPTION DESCRIPTION                              |  
//        +==========================+=================================================================================+  
//        |                          |  Expected to be thrown during construction of FSMRenderEnvironment if there is  |  
//        |       NO_GL_DRIVER       |   a failure to detect a valid OpenGL compatible driver on the current system.   |  
//        |                          |  Seeing as this is a major issue, recommended response is to crash gracefully.  |  
//        +--------------------------+---------------------------------------------------------------------------------+   
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        +--------------------------+---------------------------------------------------------------------------------+   
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        +--------------------------+---------------------------------------------------------------------------------+   
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        +--------------------------+---------------------------------------------------------------------------------+   
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        +--------------------------+---------------------------------------------------------------------------------+   
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        +--------------------------+---------------------------------------------------------------------------------+   
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        |                          |                                                                                 |  
//        +--------------------------+---------------------------------------------------------------------------------+
//                             
//
//


#pragma once

#ifndef FSM_EXCEPTION_H_
#define FSM_EXCEPTION_H_

#include <string>
#include <stdexcept>  //#include <exception>
#include "UniversalIncludes.h"  //For LOG message writing

//If string_view isn't available, comment the following 2 lines
#define CONSTRUCT_FROM_STRING_VIEW 1
#include <string_view>




class FSMException : public std::exception {
public:

#ifdef CONSTRUCT_FROM_STRING_VIEW
    /** Constructor (String type)
        @param message The exception message.
    */
    FSMException(std::string_view message) noexcept : mMessage_(message) {
        LOG(TRACE) << __FUNCTION__;
    }
#else 
    /** Constructor (C strings).
     *  @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the char* lies
     *                 with the caller.
     */
    explicit FSMException(const char* message) noexcept :
        mMessage_((message != nullptr)?(message):("")) {
        LOG(TRACE) << __FUNCTION__;
    }

    /** Constructor (C++ STL strings).
     *  @param message The error message.
     */
    explicit FSMException(const std::string& message) noexcept :
        mMessage_(message) {
            LOG(TRACE) << __FUNCTION__;
    }
#endif // CONSTRUCT_FROM_STRING_VIEW

    /** Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~FSMException() noexcept { LOG(TRACE) << __FUNCTION__; }

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *          is in possession of the Exception object. Callers must
     *          not attempt to free the memory.
     */
    virtual const char* what() const noexcept override {        
        LOG(TRACE) << __FUNCTION__;
        return mMessage_.c_str();
    }

protected:
    /** Error message.
     */
    std::string mMessage_;
};


class FSMNamedException final : public FSMException {
public:
    enum class NamedException { NO_GL_DRIVER, INVALID_ASCII_SEQ /*, NO_INIT_CONFIG_FILE_FOUND*/ };

#ifdef CONSTRUCT_FROM_STRING_VIEW
    /** Constructor (String type)
     *  @param name    The name of this named exception
     *  @param message The exception message.
    */
    FSMNamedException(NamedException name, std::string_view message) noexcept
        : FSMException(message), mName_(name) {
        LOG(TRACE) << __FUNCTION__;
        std::string exceptionName;
        switch (name) {
        case NamedException::NO_GL_DRIVER:
            exceptionName = "NO_GL_DRIVER";
            break;
        case NamedException::INVALID_ASCII_SEQ:
            exceptionName = "INVALID_ASCII_SEQ";
            break;
       // case NamedException::NO_INIT_CONFIG_FILE_FOUND:
       //     exceptionName = "NO_INIT_CONFIG_FILE_FOUND";
       //     break;
        default:
            exceptionName = "DEFAULT_CASE_IN_SWITCH_STATEMENT";
            LOG(WARNING) << "\nWARNING! DEFAULT CASE WAS CHOSEN IN FSMNamedException's CONSTRUCTOR!\n";
            break;
        }
#ifdef  LOG_NAMED_FSM_EXCEPTIONS_AS_WARNINGS
        LOG(WARNING) <<  exceptionName;
#endif // LOG_NAMED_FSM_EXCEPTIONS_AS_WARNINGS
#ifdef  LOG_NAMED_FSM_EXCEPTIONS_AS_ERRORS
        LOG(ERROR) <<  exceptionName;
#endif //LOG_NAMED_FSM_EXCEPTIONS_AS_ERRORS
       
    }
#else 
    /** Constructor (C strings).
     *  @param name    The name of this named exception
     *  @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the char* lies
     *                 with the caller.
     */
    explicit FSMNamedException(NamedException name, const char* message) noexcept :
        FSMException(message), mName_(name) { //Let FSMException's constructor handle the potential nullptr 'message'
        LOG(TRACE) << __FUNCTION__;   
         std::string exceptionName;
        switch (name) {
        case NamedException::NO_GL_DRIVER:
            exceptionName = "NO_GL_DRIVER";
            break;
        case NamedException::INVALID_ASCII_SEQ:
            exceptionName = "INVALID_ASCII_SEQ";
            break;
        default:
            exceptionName = "DEFAULT_CASE_IN_SWITCH_STATEMENT";
            LOG(WARNING) << "\nWARNING! DEFAULT CASE WAS CHOSEN IN FSMNamedException's CONSTRUCTOR!\n";
            break;
        }
#ifdef  LOG_NAMED_FSM_EXCEPTIONS_AS_WARNINGS
        LOG(WARNING) <<  exceptionName;
#endif // LOG_NAMED_FSM_EXCEPTIONS_AS_WARNINGS
#ifdef  LOG_NAMED_FSM_EXCEPTIONS_AS_ERRORS
        LOG(ERROR) <<  exceptionName;
#endif //LOG_NAMED_FSM_EXCEPTIONS_AS_ERRORS
    }

    /** Constructor (C++ STL strings). 
     *  @param name    The name of this named exception
     *  @param message The error message.
     */
    explicit FSMNamedException(NamedException name, const std::string& message) noexcept :
        FSMException(message), mName_(name) {
        LOG(TRACE) << __FUNCTION__;
    }
#endif // CONSTRUCT_FROM_STRING_VIEW

    virtual ~FSMNamedException() noexcept {
        LOG(TRACE) << __FUNCTION__;
    }

    /** Returns the stored enum value representing this exception's name.
     * @return The enum value representing this exception's name.
    */
    NamedException getName() const noexcept { return mName_; }

private:
    const NamedException mName_;
};



#endif //FSM_EXCEPTION_H_