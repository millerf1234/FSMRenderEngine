
//  Work in progress..

#pragma once

#ifndef FSM_MONITOR_HANDLE_H_
#define FSM_MONITOR_HANDLE_H_

#include "FSMMonitor.h"

struct GLFWmonitor;

namespace FSMEngineInternal {

	class FSMMonitorHandle final {
	public:
		FSMMonitorHandle() = delete;
		FSMMonitorHandle(const FSMMonitorHandle&) = delete;
		FSMMonitorHandle(FSMMonitorHandle&&) = default;
		FSMMonitorHandle& operator=(const FSMMonitorHandle&) = delete;
		FSMMonitorHandle& operator=(FSMMonitorHandle&&) = default;

		~FSMMonitorHandle();
		FSMMonitor get() noexcept;

	private:
		GLFWmonitor* mHandle_;

		friend std::unique_ptr<FSMMonitorHandle> createMonitorHandle(GLFWmonitor* handle);
		friend void destroyMonitorHandle(std::unique_ptr<FSMMonitorHandle>);
		FSMMonitorHandle(GLFWmonitor* handle);
		
	};

	//Factory Function -- Will throw a std::invalid_argument exception if handle is not valid
	std::unique_ptr<FSMMonitorHandle> createMonitorHandle(GLFWmonitor* handle);

	//Anti-Factory Function
	void destroyMonitorHandle(std::unique_ptr<FSMMonitorHandle>) noexcept;

	

} //namespace FSMEngineInternal

#endif //FSM_MONITOR_HANDLE_H_

