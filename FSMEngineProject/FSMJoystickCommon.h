//
// File:                     FSMJoystickCommon.h
//
// Quick Description:        Provides common types, definitions and components that are shared across all
//                           FSMJoystick types.
//
//                           These include:
//                                   - Core Dependencies (i.e. GLFW is a core dependency)
//                                   - Constants 
//                                   - Internal Exception Type
//                                   - JoystickID Type-safe wrapper
//
//
//
//
// Note:              
//                         [ UPDATE ]
//                              I still have the Joystick hat hint getting set in a place very close to where 
//                           glfwInit() gets called, but now instead of this function specifying the hint directly 
//                           with a hard-coded value it will look for a constant value which is defined by this 
//                           header.
//                                                                                                                    
//                         [ ORIGINAL NOTE ]  
//                                    Who/Where Should Global Hint GLFW_JOYSTICK_HAT_BUTTONS Be Set 
//                                                                                                                    
//                               During the very early initialization phase, in the time immediately before when
//                           GLFW is first initialized with its function glfwInit(), one of several tasks to be
//                           performed is to specify to GLFW the global hints as to how the GLFW framework 
//                           should behave. In particular, within GLFW 3.3 [the most recent revision of GLFW
//                           available as of April 2019 when this project was built], the only project-relevant
//                           hint which is to be set here is the hint regarding Joystick-Hat input behavior. 
//                               Part of me feels like the function that is in charge of setting this behavior
//                           should be implemented in this file as a free-standing function, but just in case future 
//                           GLFW releases add relevant global hints that concern other functionality besides 
//                           joysticks, I am leaving the function for specifying global GLFW hints to be defined
//                           close to where it is to be called.                         
//                           
//
//
//
//
//
// Programmer:               Forrest Miller
// Date:                     March 31, 2019
//


#pragma once

#ifndef FSM_JOYSTICK_COMMON_H_
#define FSM_JOYSTICK_COMMON_H_

#include "GraphicsLanguageFramework.h"
#include "UniversalIncludes.h"


#include "FSMException.h"
#ifndef CONSTRUCT_FROM_STRING_VIEW
#define CONSTRUCT_FROM_STRING_VIEW 1
#include <string_view>
#endif

namespace FSMJoystickInternal {


    // /--------------------------------------\  
    // |           Constant Values            |    
    // \--------------------------------------/  


    static constexpr const int MAX_XINPUT_GAMEPADS = 4;
    static constexpr const float AXIS_DEAD_ZONE_SIZE = 0.025f;


	//Represents the maximum supported number of simultaneously connected Joysticks
	static constexpr const int MAX_JOY_ID = GLFW_JOYSTICK_LAST;

	//Max Input value that can be read from an axis (Axes include  
	static constexpr const float AXIS_INPUT_MAX = 1.0f;
	static constexpr const float AXIS_INPUT_MIN = -1.0f;

    



    // /------------------------------------\  
    // |         Internal Exception         |     
    // \------------------------------------/


    class FSMJoystickException final : public FSMException {
    public:
        enum class JoystickException { ILLEGAL_JOYSTICK_ID, };
#ifdef CONSTRUCT_FROM_STRING_VIEW
       /** Constructor (String type)
        *  @param name    The name of this Joystick exception
        *  @param message The exception message.
        */
        FSMJoystickException(JoystickException joyExcept, std::string_view msg) noexcept :
            FSMException(msg), mName_(joyExcept) {
            LOG(TRACE) << __FUNCTION__;
        }
#else 
    /** Constructor (C strings).
     *  @param name    The name of this named exception
     *  @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the char* lies
     *                 with the caller.
     */
        explicit FSMNamedException(JoystickException name, const char* message) noexcept :
        FSMException(message), mName_(name) { //Let FSMException's constructor handle the potential nullptr 'message'
        LOG(TRACE) << __FUNCTION__;
    }

    /** Constructor (C++ STL strings). 
     *  @param name    The name of this named exception
     *  @param message The error message.
     */
    explicit FSMNamedException(JoystickException name, const std::string& message) noexcept :
        FSMException(message), mName_(name) {
        LOG(TRACE) << __FUNCTION__;
    }
#endif // CONSTRUCT_FROM_STRING_VIEW


        virtual ~FSMJoystickException() noexcept {
            LOG(TRACE) << __FUNCTION__;
        }

        /** Returns the stored enum value representing this Joystick 
         *   exception's name.
         * @return  The enum value representing this exception's name.
        */
        JoystickException getName() const noexcept { return mName_; }

    private:
        const JoystickException mName_;
    };



    // /------------------------------------\  
    // |         Internal Exception         |     
    // \------------------------------------/


    //Wrapper type that will only fully initialize properly if 
    //constructed using a valid ID. Attempting to construct this 
    //type with an invalid ID will throw a 
class JoystickID final {
public:
    JoystickID() = delete;

    ~JoystickID() noexcept {
        LOG(TRACE) << __FUNCTION__;
    }
    //Constructor must be passed a valid JoyID or a JoystickException
    //of name Illegal_Joystick_ID will be thrown.
    JoystickID(int JoyID) noexcept(false) : mID_(JoyID) {
        LOG(TRACE) << __FUNCTION__;
        if ((JoyID < GLFW_JOYSTICK_1) || (JoyID > MAX_JOY_ID)) {
            std::ostringstream exceptionMessage;
            exceptionMessage << "Unable to construct JoystickID object with the illegal ID "
                << std::to_string(JoyID) << "\nOnly IDs " << std::to_string(JoyID) << "to " << MAX_JOY_ID
                << " are legal JoyID values!\n";
            throw FSMJoystickException(FSMJoystickException::JoystickException::ILLEGAL_JOYSTICK_ID,
                exceptionMessage.str());
        }
    }

    //Copy and move constructors are both straightforward
    JoystickID(const JoystickID& that) noexcept : mID_(that.mID_) {
        LOG(TRACE) << __FUNCTION__;
    }
    JoystickID(JoystickID&& that) noexcept : mID_(that.mID_) {
        LOG(TRACE) << __FUNCTION__;
    }

    //Allow Object's to be explicitly cast to ints
    explicit operator int() const noexcept {
        LOG(TRACE) << __FUNCTION__;
        return mID_;
    }

    bool operator<(const JoystickID& that) const noexcept {
        LOG(TRACE) << __FUNCTION__;
        if (mID_ < that.mID_)
            return true;
        return false;
    }

    bool operator<=(const JoystickID& that) const noexcept {
        LOG(TRACE) << __FUNCTION__;
        if (mID_ <= that.mID_)
            return true;
        return false;
    }

    bool operator>(const JoystickID& that) const noexcept {
        LOG(TRACE) << __FUNCTION__;
        if (mID_ < that.mID_)
            return true;
        return false;
    }

    bool operator>=(const JoystickID& that) const noexcept {
        LOG(TRACE) << __FUNCTION__;
        if (mID_ <= that.mID_)
            return true;
        return false;
    }

    bool operator==(const JoystickID& that) const noexcept {
        LOG(TRACE) << __FUNCTION__;
        if (mID_ == that.mID_)
            return true;
        return false;
    }

    bool operator!=(const JoystickID& that) const noexcept {
        LOG(TRACE) << __FUNCTION__;
        if (mID_ != that.mID_)
            return true;
        return false;
    }

private:
    const int mID_;

};


} //namespace FSMJoystickInternal

#endif //FSM_JOYSTICK_COMMON_H_