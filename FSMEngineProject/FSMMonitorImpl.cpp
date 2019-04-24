

#include "FSMMonitorImpl.h"

#include <stdexcept>

#include "UniversalIncludes.h"
#include "GraphicsLanguageFramework.h"

FSMMonitor::FSMMonitorImpl::FSMMonitorImpl() {
	mHandle_ = nullptr;
	mName_ = "";
	mPrimaryVideoMode_ = nullptr;
}

FSMMonitor::FSMMonitorImpl::FSMMonitorImpl(GLFWmonitor* handle) : FSMMonitorImpl() {
	LOG(TRACE) << __FUNCTION__;
	if (!handle)
		throw std::invalid_argument("Unable to construct monitor from nullptr handle!");
	
	mHandle_ = handle;
	
	mName_ = glfwGetMonitorName(handle);
	
	int physicalWidth = 0;
	int physicalHeight = 0;
	glfwGetMonitorPhysicalSize(handle, &physicalWidth, &physicalHeight);

	const GLFWvidmode* primaryVidMode = glfwGetVideoMode(handle);
	mPrimaryVideoMode_ = std::make_unique<FSMVideoMode>(*primaryVidMode, physicalWidth, physicalHeight);

	int count = 0;
	const GLFWvidmode* modes = glfwGetVideoModes(handle, &count);
	mVideoModes_.reserve(count);
	for (int i = 0; i < count; i++)
		mVideoModes_.emplace_back(modes[i], physicalWidth, physicalHeight);

}

FSMMonitor::FSMMonitorImpl::FSMMonitorImpl(const FSMMonitor::FSMMonitorImpl& that) {
	LOG(TRACE) << __FUNCTION__;
	mHandle_ = that.mHandle_;
	mName_ = that.mName_;
	mPrimaryVideoMode_ = std::make_unique<FSMVideoMode>(*(that.mPrimaryVideoMode_));
	mVideoModes_.reserve(that.mVideoModes_.size());
	for (auto vidMode : that.mVideoModes_)
		mVideoModes_.push_back(vidMode); //mVideoModes_.emplace_back(vidMode); //Question: Does emplace_back call move operator?
}

FSMMonitor::FSMMonitorImpl::FSMMonitorImpl(FSMMonitor::FSMMonitorImpl&& that) noexcept {
	LOG(TRACE) << __FUNCTION__;
	mHandle_ = that.mHandle_;
	mName_ = that.mName_;
	mPrimaryVideoMode_ = std::move(that.mPrimaryVideoMode_);
	mVideoModes_.reserve(that.mVideoModes_.size());
	for (auto vidMode : that.mVideoModes_)
		mVideoModes_.emplace_back(vidMode);
}
FSMMonitor::FSMMonitorImpl& FSMMonitor::FSMMonitorImpl::operator=(const FSMMonitor::FSMMonitorImpl& that) {
	LOG(TRACE) << __FUNCTION__;
	if (this != &that) {
		mHandle_ = that.mHandle_;
		mName_ = that.mName_;
		mPrimaryVideoMode_ = std::make_unique<FSMVideoMode>(*(that.mPrimaryVideoMode_));
		mVideoModes_.reserve(that.mVideoModes_.size());
		for (auto vidMode : that.mVideoModes_)
			mVideoModes_.push_back(vidMode); //mVideoModes_.emplace_back(vidMode); //Question: Does emplace_back call move operator?
	}
	return *this;
}
FSMMonitor::FSMMonitorImpl& FSMMonitor::FSMMonitorImpl::operator=(FSMMonitor::FSMMonitorImpl&& that) noexcept {
	LOG(TRACE) << __FUNCTION__;
	if (this != &that) {
		mHandle_ = that.mHandle_;
		mName_ = that.mName_;
		mPrimaryVideoMode_ = std::move(that.mPrimaryVideoMode_);
		mVideoModes_.reserve(that.mVideoModes_.size());
		for (auto vidMode : that.mVideoModes_)
			mVideoModes_.emplace_back(vidMode);
	}
	return *this;
}

FSMMonitor::FSMMonitorImpl::~FSMMonitorImpl() noexcept {
	LOG(TRACE) << __FUNCTION__;
}

bool FSMMonitor::FSMMonitorImpl::isPrimary() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    if (!primary) {
        std::string msg = "Error! The function glfwGetPrimaryMonitor() returned NULL, which means\n"
            "  either a) there are no monitors detected, or b) an error occured!\n"
            "  In both cases there is a serious problem that just occured here!\n";
        LOG(ERROR) << msg;
        std::exit(EXIT_FAILURE);
    }
    return (mHandle_ == primary);
   
}

int FSMMonitor::FSMMonitorImpl::getVirtualPositionX() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	int xPos = 0;
	int yPos = 0;
	glfwGetMonitorPos(mHandle_, &xPos, &yPos);
	return xPos;
}

int FSMMonitor::FSMMonitorImpl::getVirtualPositionY() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	int xPos = 0;
	int yPos = 0;
	glfwGetMonitorPos(mHandle_, &xPos, &yPos);
	return yPos;
}

float FSMMonitor::FSMMonitorImpl::getContentScaleX() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	float xScale = 0.0f;
	float yScale = 0.0f;
	glfwGetMonitorContentScale(mHandle_, &xScale, &yScale);
	return xScale;
}

float FSMMonitor::FSMMonitorImpl::getContentScaleY() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	float xScale = 0.0f;
	float yScale = 0.0f;
	glfwGetMonitorContentScale(mHandle_, &xScale, &yScale);
	return yScale;
}

MonitorWorkarea FSMMonitor::FSMMonitorImpl::getWorkarea() const noexcept {
    MonitorWorkarea workarea = { 0, 0, 0, 0 };
    //glfwGetMonitorWorkarea(nullptr, nullptr, nullptr, nullptr, nullptr);//glfwGetMonitorWorkarea()
}

FSMVideoMode FSMMonitor::FSMMonitorImpl::getPrimaryVideoMode() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return *mPrimaryVideoMode_;
}

std::vector<FSMVideoMode> FSMMonitor::FSMMonitorImpl::getVideoModes() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return mVideoModes_;
}

std::string FSMMonitor::FSMMonitorImpl::getName() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return mName_;
}

GLFWgammaramp FSMMonitor::FSMMonitorImpl::getGammaRamp() const noexcept {
	LOG(TRACE) << __FUNCTION__;
	return *(glfwGetGammaRamp(mHandle_));
}

void FSMMonitor::FSMMonitorImpl::setGamma(float gamma) {
	LOG(TRACE) << __FUNCTION__;
	if (gamma > 0.0f)
		glfwSetGamma(mHandle_, gamma);
	else {
		throw std::invalid_argument("Unable to Set Gamma to Invalid Value of " +
			std::to_string(gamma));
	}
}