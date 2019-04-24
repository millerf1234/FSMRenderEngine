//
// File:                               FSMJoystickHandle.cpp
//
// Desc:                               Handle for a connected Joystick
//
//


#include "FSMJoystickHandle.h"
#include "UniversalIncludes.h"

using namespace FSMJoystickInternal;

    FSMJoystickHandle::FSMJoystickHandle(JoystickID id) : mID_(id) {
        LOG(TRACE) << __FUNCTION__;
        
    }


    FSMJoystickHandle::~FSMJoystickHandle() noexcept {
        LOG(TRACE) << __FUNCTION__;
        
    }



