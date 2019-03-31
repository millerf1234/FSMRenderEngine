

#pragma once

#ifndef JOYSTICK_HANDLE_H_
#define JOYSTICK_HANDLE_H_


typedef int JoyID;

class JoystickHandle final {
public:
	//JoystickHandle(JoyID id) : mJoyID_(id) { ; }
	~JoystickHandle();

private:
	JoyID mJoyID_;
};



#endif //JOYSTICK_HANDLE_H_

