

#pragma once

#ifndef FSM_MONITOR_IMPL_H_
#define FSM_MONITOR_IMPL_H_

#include "FSMMonitor.h"
#include "FSMVideoMode.h"

class FSMMonitor::FSMMonitorImpl final {
public:
	FSMMonitor::FSMMonitorImpl() = delete;
	FSMMonitor::FSMMonitorImpl(GLFWmonitor* handle) noexcept;

	FSMMonitor::FSMMonitorImpl(const FSMMonitor::FSMMonitorImpl&);
	FSMMonitor::FSMMonitorImpl(FSMMonitor::FSMMonitorImpl&&) noexcept;
	FSMMonitor::FSMMonitorImpl& operator=(const FSMMonitor::FSMMonitorImpl&);
	FSMMonitor::FSMMonitorImpl& operator=(FSMMonitor::FSMMonitorImpl&&) noexcept;

	~FSMMonitorImpl() noexcept;

	int getVirtualPositionX() const noexcept;
	int getVirtualPositionY() const noexcept;
	float getContentScaleX() const noexcept;
	float getContentScaleY() const noexcept;

	FSMVideoMode getPrimaryVideoMode() const noexcept;
	std::vector<FSMVideoMode> getVideoModes() const noexcept;

	GLFWgammaramp getGammaRamp() const noexcept;
	void setGamma(float gamma);


	//Special Implementation-Only function
	GLFWmonitor* get() const noexcept { return mHandle_; }

private:
	GLFWmonitor* mHandle_;
};




#endif //FSM_MONITOR_IMPL_H_

