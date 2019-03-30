//
//  
//
//  Reference:  https://www.glfw.org/docs/3.3/monitor_guide.html#monitor_scale
//              https://www.glfw.org/docs/latest/monitor_guide.html
//
// Programmer:   Forrest Miller
//
//

#pragma once

#ifndef FSM_MONITOR_H_
#define FSM_MONITOR_H_

#include <vector>
#include <string>
#include <memory>

//Forward declare types as needed
class FSMVideoMode;
struct GLFWmonitor;
struct GLFWgammaramp;
namespace FSMEngineInternal {
	class FSMMonitorHandle;
}


class FSMMonitor final {
public:
	FSMMonitor() = delete;
	FSMMonitor(const FSMMonitor&);
	FSMMonitor(FSMMonitor&&) noexcept;
	FSMMonitor& operator=(const FSMMonitor&);
	FSMMonitor& operator=(FSMMonitor&&) noexcept;
	~FSMMonitor() noexcept;

	//------------------------------------------------------------------------------
	//---------------------------        INTEFACE        ---------------------------
	//------------------------------------------------------------------------------
	
	int getVirtualPositionX() const noexcept;
	int getVirtualPositionY() const noexcept;
	float getContentScaleX() const noexcept;
	float getContentScaleY() const noexcept;

	FSMVideoMode getPrimaryVideoMode() const noexcept;
	std::vector<FSMVideoMode> getVideoModes() const noexcept;

	std::string getName() const noexcept;

	GLFWgammaramp getGammaRamp() const noexcept;


	//Sets a new gamma value for this monitor. The effects of this function will 
	//follow the GLFW documentation for the function glfwSetGamma(). The GLFW
	//documentation has the following note on setting Gamma:
	//    "The software controlled gamma ramp is applied in addition to the
	//     hardware gamma correction, which today is usually an approximation of
	//     sRGB gamma. This means that setting a perfectly linear ramp, or gamma
	//     1.0, will produce the default (usually sRGB-like) behavior."
	//It is recommended to be conservative when calling this function, not 
	//straying to far from the base value of 1.0f
	//Please be aware that this function will throw a std::invalid_argument 
	//exception if gamma is a non-positive value [i.e. gamma must be >0.0f]
	void setGamma(float gamma) noexcept(false);

private:
	FSMMonitor(GLFWmonitor* handle);
	friend class FSMEngineInternal::FSMMonitorHandle;
	class FSMMonitorImpl;
	std::unique_ptr<FSMMonitorImpl> pImpl_;
};

#endif //FSM_MONITOR_H_