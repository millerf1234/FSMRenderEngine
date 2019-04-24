//
// File:                                    FSMJoystickHandle.h
//
// Description:                             Handle for a connected Joystick. Designed for internal use by the
//                                          FSMEngine. Object's lifetime mirrors the lifetime of a single 
//                                          connected gamepad.
//                                          This type is built more to assist with bookkeeping rather than providing
//                                          functionality.
//
// Programmer:                              Forrest Miller
// Date:                                    April 23, 2019

#pragma once

#ifndef FSM_JOYSTICK_HANDLE_H_
#define FSM_JOYSTICK_HANDLE_H_

#include "FSMJoystickCommon.h"

class FSMJoystickHandle {
public:
    FSMJoystickHandle() = delete;
    ~FSMJoystickHandle() noexcept;

private:
    const FSMJoystickInternal::JoystickID mID_;


    FSMJoystickHandle(FSMJoystickInternal::JoystickID);
    

};

#endif //FSM_JOYSTICK_HANDLE_H_