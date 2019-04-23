

#pragma once

#ifndef FSM_JOYSTICK_INVARIANTS_H_
#define FSM_JOYSTICK_INVARIANTS_H_

namespace FSMJoystickInternal {

    static constexpr const bool JOYSTICK_REPORTS_HAT_INPUT_AS_BUTTONS = false;

    static constexpr const int MAX_XINPUT_GAMEPADS = 4;
    
    static constexpr const float AXIS_DEAD_ZONE_SIZE_SMALL = 0.001f;
    static constexpr const float AXIS_DEAD_ZONE_SIZE_MEDIUM = 0.01f;
    static constexpr const float AXIS_DEAD_ZONE_SIZE_LARGE = 0.025f;


    //Represents the maximum supported number of simultaneously connected Joysticks
    static constexpr const int MAX_JOY_ID = GLFW_JOYSTICK_LAST;

    //Max Input value that can be read from an axis (Axes include  
    static constexpr const float AXIS_INPUT_MAX = 1.0f;
    static constexpr const float AXIS_INPUT_MIN = -1.0f;


} //namespace FSMJoystickInternal


#endif //FSM_JOYSTICK_INVARIANTS_H_