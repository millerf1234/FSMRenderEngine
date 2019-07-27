


#include "FSMMonitor.h"
#include "FSMMonitorImpl.h"
#include "UniversalIncludes.h"
#include "GraphicsLanguageFramework.h"


FSMMonitor::FSMMonitor(GLFWmonitor* handle) {
	LOG(TRACE) << __FUNCTION__;
	pImpl_ = std::make_unique<FSMMonitorImpl>(handle);
}

FSMMonitor::FSMMonitor(const FSMMonitor& that) {
	LOG(TRACE) << __FUNCTION__;
	//pImpl_ = std::make_unique<FSMMonitorImpl>(that.pImpl_.get());
	pImpl_ = std::make_unique<FSMMonitorImpl>(*(that.pImpl_));
}

FSMMonitor::FSMMonitor(FSMMonitor&& that) noexcept {
	LOG(TRACE) << __FUNCTION__;
	pImpl_ = std::move(that.pImpl_);
}

FSMMonitor& FSMMonitor::operator=(const FSMMonitor& that) {
	LOG(TRACE) << __FUNCTION__;
	if (this != &that) {
		//pImpl_ = std::make_unique<FSMMonitorImpl>(that.pImpl_.get());
		//Is this the correct way to implement this operator?
		pImpl_ = std::make_unique<FSMMonitorImpl>(*(that.pImpl_));
	}
	return *this;
}

FSMMonitor& FSMMonitor::operator=(FSMMonitor&& that) noexcept {
	LOG(TRACE) << __FUNCTION__;
	if (this != &that) {
		pImpl_ = std::move(that.pImpl_);
	}
	return *this;
}

FSMMonitor::~FSMMonitor() noexcept {
	LOG(TRACE) << __FUNCTION__;
	//That's it?
}

bool FSMMonitor::isPrimary() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return pImpl_->isPrimary();
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

MonitorWorkarea FSMMonitor::getWorkarea() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return pImpl_->getWorkarea();
}

FSMVideoMode FSMMonitor::getPrimaryVideoMode() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->getPrimaryVideoMode();
}

std::vector<FSMVideoMode> FSMMonitor::getVideoModes() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->getVideoModes();
}

std::string FSMMonitor::getName() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->getName();
}

const GLFWmonitor* FSMMonitor::getHandle() const noexcept {
    return pImpl_->getHandle();
}

GLFWgammaramp FSMMonitor::getGammaRamp() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->getGammaRamp();
}

void FSMMonitor::setGamma(float gamma) noexcept(false) {
	LOG(TRACE) << __FUNCTION__;
	return pImpl_->setGamma(gamma);
}