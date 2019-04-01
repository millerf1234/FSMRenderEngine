//
// File:                               JoystickPort.h
// Class:                              JoystickPort
//         [Alternative names]              - JoystickAttachmentPoint  
//                                          - JoystickDock             
//                                          - JoystickListener         
//
// Description:                        When the FSMEngine is initialized, there will be GLFW_JOYSTICK_MAX
//
//
// Programmer:                         Forrest S. Miller
//  

#pragma once

#ifndef JOYSTICK_PORT_H_
#define JOYSTICK_PORT_H_

#include <optional>
#include <memory>

#include "FSMJoystickCommon.h"

//This class is a type that is given out to client code
class FSMJoystickInputState {
public:
    void getStateForCurrentFrame() { 
        //yeah!;
    }
};

std::shared_ptr<FSMJoystickInputState> getJoystickInputStateListener(FSMJoystickInternal::JoystickID id) {
    return nullptr;
}

namespace FSMJoystickInternal {

    //This type is meant to have one static instance for each available Joystick port. 
    //This type will be invariant for as long as the engine is running, so that it can 
    //serve as an always-extant intermediary between any other part of the code that 
    //wants to do joystick-related activities
    class JoystickPort final {
    public:


    private:
        const JoystickID mID_;

        std::optional<std::shared_ptr<FSMJoystickInputState>> mInputState_;
        
        //Have another type also here that 
    };


} //namespace FSMJoystickInternal

#endif //JOYSTICK_PORT_H_

