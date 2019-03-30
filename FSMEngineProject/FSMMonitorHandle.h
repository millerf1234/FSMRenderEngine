
//  Work in progress..
//
// References:
//   WHy std::make_unique might not work here 
//          https://stackoverflow.com/questions/33905030/how-to-make-stdmake-unique-a-friend-of-my-class

#pragma once

#ifndef FSM_MONITOR_HANDLE_H_
#define FSM_MONITOR_HANDLE_H_

#include "FSMMonitor.h"
#include <memory>

struct GLFWmonitor;

namespace FSMEngineInternal {

	class FSMMonitorHandle final {
	public:
		FSMMonitorHandle() = delete;
		FSMMonitorHandle(const FSMMonitorHandle&) = delete;
		FSMMonitorHandle(FSMMonitorHandle&&) = default;
		FSMMonitorHandle& operator=(const FSMMonitorHandle&) = delete;
		FSMMonitorHandle& operator=(FSMMonitorHandle&&) = default;

		~FSMMonitorHandle() noexcept;
		FSMMonitor get() noexcept;

		bool hasHandle(const GLFWmonitor* handle) const noexcept { return (mHandle_ == handle); }

		
	private:
		GLFWmonitor* mHandle_;

		friend std::unique_ptr<FSMMonitorHandle> createMonitorHandle(GLFWmonitor* handle);
		//friend void destroyMonitorHandle(std::unique_ptr<FSMMonitorHandle>);
		FSMMonitorHandle(GLFWmonitor* handle);
		
		//friend std::unique_ptr<FSMMonitorHandle> std::make_unique<FSMMonitorHandle>(GLFWmonitor*);
	};

	//Factory Function -- Will throw a std::invalid_argument exception if handle is not valid
	std::unique_ptr<FSMMonitorHandle> createMonitorHandle(GLFWmonitor* handle);

	//Anti-Factory Function
	//void destroyMonitorHandle(std::unique_ptr<FSMMonitorHandle>) noexcept;

	

} //namespace FSMEngineInternal

#endif //FSM_MONITOR_HANDLE_H_

