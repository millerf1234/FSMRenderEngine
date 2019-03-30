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
	~FSMMonitor();

	//------------------------------------------------------------------------------
	//---------------------------        INTEFACE        ---------------------------
	//------------------------------------------------------------------------------
	
	int getVirtualPositionX() const noexcept;
	int getVirtualPositionY() const noexcept;
	float getContentScaleX() const noexcept;
	float getContentScaleY() const noexcept;

	FSMVideoMode getPrimaryVideoMode() const noexcept;
	std::vector<FSMVideoMode> getVideoModes() const noexcept;

	GLFWgammaramp getGammaRamp() const noexcept;
	void setGamma(float gamma);

private:
	FSMMonitor(GLFWmonitor* handle);
	friend class FSMEngineInternal::FSMMonitorHandle;
	class FSMMonitorImpl;
	std::unique_ptr<FSMMonitorImpl> pImpl_;
};

#endif //FSM_MONITOR_H_