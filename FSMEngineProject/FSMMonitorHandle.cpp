

#include "FSMMonitorHandle.h"
#include <stdexcept>
#include "GraphicsLanguageFramework.h" //For GLFW functions
#include "UniversalIncludes.h" //For logging

namespace FSMEngineInternal {



    FSMMonitorHandle* createMonitorHandle(GLFWmonitor* handle) {
        LOG(TRACE) << __FUNCTION__;
        //Confirm that handle is valid
		bool found = false;
		if (handle) {
			int count;
			GLFWmonitor** monitors = glfwGetMonitors(&count);
			int i = 0;
			do {
				if (i >= count)
					break;
				if (handle == monitors[i])
					found = true;
				i++;
			} while (!found);
		}
		if (!found)
			throw (std::invalid_argument("Invalid Monitor Handle"));
        return new FSMMonitorHandle(handle);
    }

	//std::unique_ptr<FSMMonitorHandle> createMonitorHandle(GLFWmonitor* handle) {
	//	LOG(TRACE) << __FUNCTION__;
	//	//Confirm that handle is valid
	//	bool found = false;
	//	if (handle) {
	//		int count;
	//		GLFWmonitor** monitors = glfwGetMonitors(&count);
	//		int i = 0;
	//		do {
	//			if (i >= count)
	//				break;
	//			if (handle == monitors[i])
	//				found = true;
	//			i++;
	//		} while (!found);
	//	}
	//	if (!found)
	//		throw (std::invalid_argument("Invalid Monitor Handle"));
	//	//return std::make_unique<FSMMonitorHandle>(handle); //See references for why std::make_unique doesn't work here
	//	return std::unique_ptr<FSMMonitorHandle>(new FSMMonitorHandle(handle)); 
	//}


    void destroyMonitorHandle(FSMMonitorHandle * handle) noexcept {
        LOG(TRACE) << __FUNCTION__;
        if (!handle)
            LOG(ERROR) << "\nError! Unable to free invalid handle!\n";
        else 
            delete handle;
    }

	//void destroyMonitorHandle(std::unique_ptr<FSMMonitorHandle>) noexcept {
	//	LOG(TRACE) << __FUNCTION__;
	//	//That's all?
	//}

    FSMMonitorHandle::FSMMonitorHandle(FSMMonitorHandle&& that) noexcept :
        mHandle_{that.mHandle_} {
            LOG(TRACE) << __FUNCTION__;
            LOG(DEBUG) << "Alert! FSMMonitorHandle move constructor was called!\n";
            that.mHandle_ = nullptr;
    }



	FSMMonitorHandle::~FSMMonitorHandle() noexcept {
		LOG(TRACE) << __FUNCTION__;
	}

	FSMMonitor FSMMonitorHandle::get() noexcept {
		LOG(TRACE) << __FUNCTION__;
		return FSMMonitor(mHandle_);
	}

	FSMMonitorHandle::FSMMonitorHandle(GLFWmonitor* handle) : mHandle_(handle) {
		LOG(TRACE) << __FUNCTION__;
	}




} //namespace FSMEngineInternal


