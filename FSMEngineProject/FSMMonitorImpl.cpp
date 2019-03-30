

#include "FSMMonitorImpl.h"

#include "UniversalIncludes.h"
#include "GraphicsLanguageFramework.h"

FSMMonitor::FSMMonitorImpl::FSMMonitorImpl(GLFWmonitor* handle) : mHandle_(handle) {
	LOG(TRACE) << __FUNCTION__;
	if (!handle)
		LOG(ERROR) << "ERROR! Monitor Handle is nullptr! This is bad!";
}


FSMMonitor::FSMMonitorImpl::~FSMMonitorImpl() {
	LOG(TRACE) << __FUNCTION__;
}

FSMMonitor::FSMMonitorImpl::FSMMonitorImpl(const FSMMonitor::FSMMonitorImpl& that) {
	LOG(TRACE) << __FUNCTION__;
}
FSMMonitor::FSMMonitorImpl::FSMMonitorImpl(FSMMonitor::FSMMonitorImpl&& that) noexcept {
	LOG(TRACE) << __FUNCTION__;
}
FSMMonitor::FSMMonitorImpl& FSMMonitor::FSMMonitorImpl::operator=(const FSMMonitor::FSMMonitorImpl& that) {
	LOG(TRACE) << __FUNCTION__;
}
FSMMonitor::FSMMonitorImpl& FSMMonitor::FSMMonitorImpl::operator=(FSMMonitor::FSMMonitorImpl&& that) noexcept {
	LOG(TRACE) << __FUNCTION__;
}

~FSMMonitorImpl() noexcept {
	LOG(TRACE) << __FUNCTION__;
}

int getVirtualPositionX() const noexcept {
	LOG(TRACE) << __FUNCTION__;
}

int getVirtualPositionY() const noexcept {
	LOG(TRACE) << __FUNCTION__;
}

float getContentScaleX() const noexcept {
	LOG(TRACE) << __FUNCTION__;
}

float getContentScaleY() const noexcept {
	LOG(TRACE) << __FUNCTION__;
}

FSMVideoMode getPrimaryVideoMode() const noexcept {
	LOG(TRACE) << __FUNCTION__;
}

std::vector<FSMVideoMode> getVideoModes() const noexcept {
	LOG(TRACE) << __FUNCTION__;
}

GLFWgammaramp getGammaRamp() const noexcept {
	LOG(TRACE) << __FUNCTION__;
}

void setGamma(float gamma) {
	LOG(TRACE) << __FUNCTION__;
}