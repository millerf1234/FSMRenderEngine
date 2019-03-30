


#include "FSMMonitor.h"
#include "FSMMonitorImpl.h"
#include "UniversalIncludes.h"



FSMMonitor::FSMMonitor(GLFWmonitor* handle) {
	LOG(TRACE) << __FUNCTION__;
	pImpl_ = std::make_unique<FSMMonitorImpl>(handle);
}

FSMMonitor::FSMMonitor(const FSMMonitor& that) {
	LOG(TRACE) << __FUNCTION__;
	pImpl_ = std::make_unique<FSMMonitorImpl>(that.pImpl_.get());
}

FSMMonitor::FSMMonitor(FSMMonitor&& that) {
	LOG(TRACE) << __FUNCTION__;
	pImpl_ = std::move(that.pImpl_);
}

FSMMonitor& FSMMonitor::operator=(const FSMMonitor& that) {
	LOG(TRACE) << __FUNCTION__;
	pImpl_ = std::make_unique<FSMMonitorImpl>(that.pImpl_.get());
}

FSMMonitor& FSMMonitor::operator=(FSMMonitor&& that) noexcept {
	LOG(TRACE) << __FUNCTION__;
	pImpl_ = std::move(that.pImpl_);
}

FSMMonitor::~FSMMonitor() noexcept {
	LOG(TRACE) << __FUNCTION__;
	//That's it?
}

int FSMMonitor::getVirtualPositionX() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->getVirtualPositionX();
}

int FSMMonitor::getVirtualPositionY() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->getVirtualPositionY();
}

float FSMMonitor::getContentScaleX() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->getContentScaleX();
}

float FSMMonitor::getContentScaleY() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->getContentScaleY();
}

FSMVideoMode FSMMonitor::getPrimaryVideoMode() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->getPrimaryVideoMode();
}

std::vector<FSMVideoMode> FSMMonitor::getVideoModes() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->getVideoModes();
}

GLFWgammaramp FSMMonitor::getGammaRamp() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->getGammaRamp();
}

void FSMMonitor::setGamma(float gamma) {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->setGamma();
}