

#pragma once

#ifndef FSM_MONITOR_IMPL_H_
#define FSM_MONITOR_IMPL_H_

#include "FSMMonitor.h"
#include "FSMVideoMode.h"

class FSMMonitor::FSMMonitorImpl final {
public:
	FSMMonitor::FSMMonitorImpl();
	FSMMonitor::FSMMonitorImpl(GLFWmonitor* handle);

	FSMMonitor::FSMMonitorImpl(const FSMMonitor::FSMMonitorImpl&);
	FSMMonitor::FSMMonitorImpl(FSMMonitor::FSMMonitorImpl&&) noexcept;
	FSMMonitor::FSMMonitorImpl& operator=(const FSMMonitor::FSMMonitorImpl&);
	FSMMonitor::FSMMonitorImpl& operator=(FSMMonitor::FSMMonitorImpl&&) noexcept;

	~FSMMonitorImpl() noexcept;

    bool isPrimary() const noexcept;

	int getVirtualPositionX() const noexcept;
	int getVirtualPositionY() const noexcept;
	float getContentScaleX() const noexcept;
	float getContentScaleY() const noexcept;

    MonitorWorkarea getWorkarea() const noexcept;

	FSMVideoMode getPrimaryVideoMode() const noexcept;
	std::vector<FSMVideoMode> getVideoModes() const noexcept;

	std::string getName() const noexcept;

	GLFWgammaramp getGammaRamp() const noexcept;

	//Throws a std::invalid_argument exception if gamma is not 
	//a positive value
	void setGamma(float gamma) noexcept(false);

private:
	GLFWmonitor* mHandle_;
	std::string mName_;
	std::unique_ptr<FSMVideoMode> mPrimaryVideoMode_;
	std::vector<FSMVideoMode> mVideoModes_;
};




#endif //FSM_MONITOR_IMPL_H_

