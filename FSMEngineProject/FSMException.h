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
//                Maybe --> Multi-thread support forthcoming
//
//  My Code was somewhat inspired by the top answer of: 
//      https://stackoverflow.com/questions/8152720/correct-way-to-inherit-from-stdexception
//
//
//  IMPORTANT: Note Scope Of Re-thrown Exceptions  https://stackoverflow.com/questions/6185957/rethrowing-exceptions
//
//
//         Table of Named Exceptions:
//          [Note: It looks like the OS handles reporting a missing '.dll', so no exception is needed for this event]     
//                                                                                                                                   
//        +==========================+=================================================================================+  
//        |      EXCEPTION NAME      |                              EXCEPTION DESCRIPTION                              |  
//        +==========================+=================================================================================+   
//        |                          |                                                                                 |  
//        |  LOGGING_INIT_FAILURE    |     Expected to be thrown if something goes wrong while setting up Logging.     |  
//        |                          |                                                                                 |  
//        +--------------------------+---------------------------------------------------------------------------------+   
//        |                          |  Expected to be thrown during construction of FSMRenderEnvironment if there is  |  
//        |       NO_GL_DRIVER       |   a failure to detect a valid OpenGL compatible driver on the current system.   |  
//        |                          |  Seeing as this is a major issue, recommended response is to crash gracefully.  |  
//        +--------------------------+---------------------------------------------------------------------------------+   
//        |                          |       Expected to be thrown if there is an error that occurs while using        |  
//        |     FILESYSTEM_ERROR     |        functionality found in the C++17 library <filesystem>                    |  
//        |                          |                                                                                 |  
//        +--------------------------+---------------------------------------------------------------------------------+    
//        |                          |                                                                                 |  
//        |   NO_MONITORS_DETECTED   |    Expected to be thrown if FSMEngine is unable to detect any valid monitors.   |  
//        |                          |                                                                                 |  
//        +--------------------------+---------------------------------------------------------------------------------+   
//        |                          |   Expected to be thrown during the creation of a FSMWindowContext if the        |  
//        |  INVALID_MONITOR_INDEX   |   requested monitor index represents a value beyond the end of the array of     |  
//        |                          |   available monitors.                                                           |  
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
#include <exception> //<stdexcept>  //#include <exception>
#include "UniversalIncludes.h"  //For LOG message writing
#include <string_view>



    //  ________________________________________________________________  //
    //                                                                    //
    //                    FSMException Global Constants                   //
    //  ________________________________________________________________  //
    //                                                                    //
namespace FSMExceptionInternal {
    static const std::string DEFAULT_MESSAGE = "No Message Is Available For This FSMException!";
} //namespace FSMExceptionInternal





    //  ________________________________________________________________  //
    //                                                                    //
    //                            FSMException                            //
    //  ________________________________________________________________  //
    //                                                                    //


//   Generic Exception Class -- FSMException          [Inherits from std::exception]
// This class represents the most general occurances of exception-triggering
// conditions, and as such it does not expose much functionality meant for helping
// the exception-handler routine which may catch it many courses of action beyond 
// reporting the contents of the internally stored message string and properly 
// ensuring program termination if the fatal flag is enabled.
class FSMException : public std::exception {
public:


    //   Constructor 
    // Creates a generic FSMException object. Due to the limited amount of 
    // functionality provided by this object, it is very important that it 
    // be constructed with a highly-detailed message string upon construction.
    //
    //    @param - std::string_view message     
    //          A string detailing as much information as possible with regards 
    //          to the nature of the exception. This probably should include details 
    //          such as where in the code this exception is being thrown, any possible 
    //          state information that would be pertinent to discovering why the
    //          exception was triggered, and maybe even possible courses of action that
    //          could be taken to prevent the issue.

    //  [OPTIONAL SECOND PARAMETER]
    //     @param - bool isFatal
    //          If the exception-triggering state or condition which led to this 
    //          exception being raised is severe enough in nature such that there 
    //          is no sense in attempting a recovery, this can be signaled to 
    //          any exception-handling code which catches this type by adding the 
    //          bool 'true' as a second parameter during construction. Setting this
    //          flag let's any code which catches this exception that something seriously 
    //          wrong has occured and it should forgo any attempts at recovery to 
    //          instead report the exceptions message and then terrminate the process.
    FSMException(std::string_view message,
                 bool isFatal = false) noexcept :
                                                 std::exception(
                                                      (message.empty() 
                                                          ? message.data() 
                                                           : FSMExceptionInternal::DEFAULT_MESSAGE.c_str()) ),
                                                 mIsFatal_(isFatal) {
        LOG(TRACE) << __FUNCTION__;
        if (mIsFatal_) {
            LOG(ERROR) << "Detected that a FSMException which is marked as fatal\n"
                "is in the midst of being thrown!\n";
        }
        else {
            LOG(WARNING) << "Detected that a non-fatal FSMException is in the process\n"
                "of being thrown!\n";
        }
    }

    //   Desctructor
    // Needed to allow for additional types to inherit from this type.
    virtual ~FSMException() noexcept {
        LOG(TRACE) << __FUNCTION__; 
    }

    //If this returns true, then the fact that this exception was thrown 
    //signifies something has failed catastophically enough to warrent a
    //bypass of all attempts at recovery in leu of logging as much detail 
    //as is practical before terminating the process.
    bool isFatal() const noexcept {
        LOG(TRACE) << __FUNCTION__;
        return mIsFatal_;
    }

private: 
    const bool mIsFatal_;
};




    //  ________________________________________________________________  //
    //                                                                    //
    //                         FSMNamedException                          //
    //  ________________________________________________________________  //
    //                                                                    //

//   Specific Exception Class -- FSMNamedException       [Inherits from FSMException]
// This class is built on top of its more general-purposed parent class 
// FSMException. This class is meant to be used for representing a wide 
// range of typically highly-specific exception-raising conditions that 
// have been clearly documented throughout the code as being a possible 
// outcome from calling certain functions and thus will have an exception
// handler routine ready and waiting just a short ways up the callstack for
// detecting and responding to the exception specifically.
class FSMNamedException final : public FSMException {
public:

    //  ________________________________________________________________  //
    //                                                                    //
    //                      FSMNamedException Names                       //
    //  ________________________________________________________________  //
    //                                                                    //
    
    enum class ExceptionName {
        LOGGING_INIT_FAILURE,
        FILESYSTEM_ERROR,
        NO_GL_DRIVER,
        NO_MONITORS_DETECTED,
        INVALID_MONITOR_INDEX
    };

    class NamedException {
    public:
        NamedException() = delete;
        NamedException(const NamedException&) noexcept;
        NamedException(NamedException&&) noexcept;
        NamedException& operator=(const NamedException&) noexcept;
        NamedException& operator=(NamedException&&) noexcept;
        NamedException(const ExceptionName) noexcept;
        ~NamedException() noexcept;

        //         Operators

        bool operator==(const NamedException&) const noexcept;
        bool operator!=(const NamedException&) const noexcept;

        //         Accessors

        //Get the string that contains this FSMNamedException's exception name
        std::string nameStr() const noexcept;
        //Get the enum value representing the name of this FSMNamedException's
        //named exception
        ExceptionName nameEnum() const noexcept;

    private:
        ExceptionName mName_;
        std::string mNameStr_;
    };
    

    //   Constructor 
    // Creates a FSMNamedException object.
    //  
    //    @param - ExceptionName name
    //          The ExceptionName enum value representing the name of the exception
    //          this object is to represent. 
    //
    //    @param - std::string_view message 
    //          A message containing information about the nature of what caused 
    //          this exception to need to be thrown. 
    // 
    //  [OPTIONAL THIRD PARAMETER]
    //    @param - bool isFatal
    //          An optional third parameter may be passed in at construction which 
    //          serves as a flag to let any exception-handling code which catches this
    //          object know that the conditions that caused the exception to be raised
    //          are serious enough that the best course of action will be process 
    //          termination. To enable this flag, pass in 'true' as the third parameter.
    FSMNamedException(ExceptionName name,
                      std::string_view message,
                      bool isFatal = false) noexcept :
                                                       FSMException(message,
                                                                    isFatal),
                                                       mName_(name) {
        LOG(TRACE) << __FUNCTION__;
    }

    virtual ~FSMNamedException() noexcept {
        LOG(TRACE) << __FUNCTION__;
    }

    bool operator==(const FSMNamedException& that) const noexcept {
        LOG(TRACE) << __FUNCTION__;
        return (mName_ == that.mName_);
    }

    bool operator!=(const FSMNamedException& that) const noexcept {
        LOG(TRACE) << __FUNCTION__;
        return (mName_ != that.mName_);
    }

    //Returns an object which contains two fields (an enum and a string) that
    //when composed together will represent this NamedException's name. 
    NamedException getName() const noexcept { 
        LOG(TRACE) << __FUNCTION__;
        return mName_;
    }

    //Returns the name of this exception just as a string
    std::string getNameStr() const noexcept {
        LOG(TRACE) << __FUNCTION__;
        return mName_.nameStr();
    }
private:
    const NamedException mName_;
};







#endif //FSM_EXCEPTION_H_