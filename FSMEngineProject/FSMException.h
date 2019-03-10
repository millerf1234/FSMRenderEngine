//
// File:        FSMException.h
//
// Description:   Class that is used for exceptions that may occur during 
//                FSMEngine usage.
//
//                Each exception will contain a message explaining what is wrong.
//                In most (if not all) cases, there is no proper response to one of
//                these exceptions other than to crash somewhat gracefully.
//
//                Multi-thread support forthcoming
//
//  Code is based off top answer at: 
//      https://stackoverflow.com/questions/8152720/correct-way-to-inherit-from-stdexception

#pragma once

#ifndef FSM_EXCEPTION_H_
#define FSM_EXCEPTION_H_

#include <string>
#include <exception>


class FSMException : public std::exception {
public:
    /** Constructor (C strings).
     *  @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the char* lies
     *                 with the caller.
     */
    explicit FSMException(const char* message) :
        msg_(message) {}

    /** Constructor (C++ STL strings).
     *  @param message The error message.
     */
    explicit FSMException(const std::string& message) :
        msg_(message) {}

    /** Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~FSMException() {}

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *          is in possession of the Exception object. Callers must
     *          not attempt to free the memory.
     */
    virtual const char* what() const {
        return msg_.c_str();
    }

protected:
    /** Error message.
     */
    std::string msg_;
};



#endif //FSM_EXCEPTION_H_