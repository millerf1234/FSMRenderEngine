//File:          GLFrameworkMonitorEventCallback.cpp
//
//Description:   Implementation file for the GLFrameworkMonitorEventCallback function and 
//               its related components. 
//
//               I am going to try something here. The implementation is based off the Singleton pattern as laid out
//               in section 47 of Scott Meyer's book "Effective C++ 2e" (from 1998!). The idea is to hide each 
//               static object inside a function returning a reference to it. Pretty straight forward.
//
//
//               This implementation is operating somewhat under the assumption that it is highly unlikely 
//               for there to be a very large number of monitor connection events to trigger over the span
//               of a single frame. At least if for some reason there were to be many connections at one instant,
//               this code would not be the cause of any bottleneck.
//
//
//  About Assigning the Callback Function:    As of now this file assumes that the callback function 
//                                            for processing the Monitor events will be assigned else-
//                                            where in the code. 
//
//
//Programmer:    Forrest Miller
//Date:          January 2, 2019



#include "FSMMonitorEventCallback.h"
#include "UniversalIncludes.h"

namespace FSMEngineInternal {

	//-------------------------------------------------------
	//
	//        Implementation of the public interface
	//
	//-------------------------------------------------------

	bool checkMonitorEventQueues() {
        LOG_EVERY_N(60, TRACE) << __FUNCTION__;
        /*
		if ((MonitorEventCallbackInternal::recentMonitorConnections().empty()) &&
			(MonitorEventCallbackInternal::recentMonitorDisconnections().empty())) 
			return false;
		return true;
        */
        //Alternative Implementation (this cuts down on function calls required)
        static std::queue<GLFWmonitor*> * ptrToConnectionQueue = &(MonitorEventCallbackInternal::recentMonitorConnections());
        static std::queue<GLFWmonitor*> * ptrToDisconnectionQueue = &(MonitorEventCallbackInternal::recentMonitorDisconnections());
        if (ptrToConnectionQueue->empty() && ptrToDisconnectionQueue->empty())
            return false;
        return true;
	}


	bool checkMonitorConnectionEventQueue() {
        LOG(TRACE) << __FUNCTION__;
		if ((MonitorEventCallbackInternal::recentMonitorConnections().empty()))
			return false;
		return true;
	}

	
	bool checkMonitorDisconnectionEventQueue() {
        LOG(TRACE) << __FUNCTION__;
		if (MonitorEventCallbackInternal::recentMonitorDisconnections().empty())
			return false;
		return true;
	}


	std::optional<GLFWmonitor*> getNextAvailableMonitorConnection() {
        LOG(TRACE) << __FUNCTION__;
		if ((MonitorEventCallbackInternal::recentMonitorConnections().empty())) {
			return std::nullopt; //return the empty optional
		}
		else {
			std::optional<GLFWmonitor*> monitorHandle =
				std::make_optional<GLFWmonitor*>(MonitorEventCallbackInternal::recentMonitorConnections().front());
			MonitorEventCallbackInternal::recentMonitorConnections().pop(); //Remove monitor from queue
			return monitorHandle;
		}
	}


	std::optional<GLFWmonitor*> getNextAvailableMonitorDisconnection() {
        LOG(TRACE) << __FUNCTION__;
		if ((MonitorEventCallbackInternal::recentMonitorDisconnections().empty())) {
			return std::nullopt;
		}
		else {
            std::optional<GLFWmonitor*> monitorHandle =
                std::make_optional<GLFWmonitor*>(MonitorEventCallbackInternal::recentMonitorDisconnections().front());
			MonitorEventCallbackInternal::recentMonitorDisconnections().pop(); //Remove monitor from queue
			return monitorHandle;
		}
	}


	namespace MonitorEventCallbackInternal {

		//-------------------------------------------------------
		//
		//   Monitor Connection and Disconnection Event Queues
		//
		//-------------------------------------------------------

		//Two queues are declared following the Singleton pattern. These queues are responsible for 
		//storing the handles provided by GLFW to the monitorEventCallback function until they 
		//can be processed by the application.

		std::queue<GLFWmonitor*>& recentMonitorConnections() {
            LOG(TRACE) << __FUNCTION__;
			static std::queue<GLFWmonitor*> awaitingConnection;
			return awaitingConnection;
		}

		std::queue<GLFWmonitor*>& recentMonitorDisconnections() {
            LOG(TRACE) << __FUNCTION__;
			static std::queue<GLFWmonitor*> awaitingDisconnection;
			return awaitingDisconnection;
		}

	
		
		//-------------------------------------------------------
		//
		//   The MonitorEventCallback function to be assigned to GLFW 
		//
		//-------------------------------------------------------
	
		void graphicsLanguageFrameworkMonitorEventCallbackFunction(GLFWmonitor* handle, int event) {
            LOG(TRACE) << __FUNCTION__;
            LOG(INFO) << "\n\n\tA Monitor Event was detected! Event code " << event << std::endl;
			if (event == GLFW_CONNECTED) {
				recentMonitorConnections().push(handle);
			}
			else if (event == GLFW_DISCONNECTED) {
				recentMonitorDisconnections().push(handle);
			}
			else {  //Should never happen
                LOG(WARNING) << "GLFW monitor event callback was called with invalid event code!\n";
			}
		}

	} //namespace MonitorEventCallbackInternal

} //namespace FSMEngineInternal