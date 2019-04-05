
//  Work in progress..
//
//  [The Idea]      The FSMMonitorHandle type is used internally by the FSMEngine to 
//                  wrap monitor handles acquired from GLFW. This type has both its 
//                  constructors and destructor marked as private. These 
//                  
//                  Due to the dynamic nature which may occur due to monitor connection/
//                  disconnection events, this type is not meant to be constructed/destructed
//                  outside of internally in response to these events.
//
//
//
//      Quick Overview of FSMEngine's Assortment of Monitor Classes
//
//           LifeCycle of a Monitor
//   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                                                                        _____________________________
//                                                                                       /`    Loop Function 'FSMEngine::processEvents()'
//                                                                          ____,;======{  Will Check Monitor Queues For                           
//                                                                       __//¯¯¯¯`       \,    Pending Events That Need Processing
//                                                                     _//¯¯`             ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                         
//                                                                    //¯`                                          
//                                                                `'\¯||¯/`                                            
//                                      |             |              \||/        |  
//                                      |             |               VV         |   
//                                      |             |                          |
//                                      |[Monitor     |[Monitor Event Queues     |            [FSMEngine Collection of
//     [Clouds of Asynchronous Time]    |  Event      |  Provide Temporary       |               FSMMonitorHandle Objects]
//                                      |   Callback  |  Storage For Raw Monitor |      
//                                      |    Function]|   Handle Until Engine is |                 
//    ,==----------------------------.  |             |     Ready to Process]    |                     +------------------+     
//   /++/  ~~  ~~  ~~  ~~  ~~  ~~   ++\ |             |      _________           | [FSMMonitorHandle   |                  |        
//  /++/ ~~  ~~  ~~  ~~  ~~  ~~  ~~  ++\_/____________|  ,+==\>>\>>\>>\_/        |   is Constructed    | FSMMonitorHandle |            
//        Monitor Connection Event      _(]=[HANDLE]=>]=(    }--|--|--|}>--->----->----->---->----->---|                  |           
//  \++\ ~~  ~~  ~~  ~~  ~~  ~~  ~~  ++/|\¯¯¯¯¯¯¯¯¯¯¯¯|  `+==/>>/>>/>>/¯\        |    From Queued      |  --------------  |      
//   \++\ ~~  ~~  ~~  ~~  ~~  ~~    ++/ |             |      ¯¯¯¯¯¯¯¯            |     Monitor Handle] |                  |     
//    `------------------------------'  |Depending On |   [Connection Queue]     |                     | FSMMonitorHandle |            
// [Each Event Consists of both a Raw   | Event Type, |                          |                     |                  |         
//  glfwMonitor Pointer Which Serves    | Callback fn |                          |                     |  --------------  |                 
//   as a Unique Handle and an Event    |  Will Push  |   [Disconnection Queue]  |                     |                  |                   
//    Code for Connect/Disconnect]      |  the Handle |   ,;=============,       |                     | FSMMonitorHandle |           
//    ,==----------------------------.  |   Onto...   |  //              \\      |                     |                  |       
//   /++/  ~~  ~~  ~~  ~~  ~~  ~~   ++\ |             | //     _________  |\     |[Once a matching     |  --------------  |     
//  /++/ ~~  ~~  ~~  ~~  ~~  ~~  ~~  ++\_/____________|//   \_/<</<</<</==+ \    |  handle is found    |                  |      
//       Monitor Disconnection Event    (]=[HANDLE]=>=(/  ;-<{[--|--|--{-----)--<---<---<---<---<---<--| FSMMonitorHandle |       
//  \++\ ~~  ~~  ~~  ~~  ~~  ~~  ~~  ++/¯\¯¯¯¯¯¯¯¯¯¯¯¯|  /  /¯\<<\<<\<<\====J    | amongst the active  |                  |        
//   \++\ ~~  ~~  ~~  ~~  ~~  ~~    ++/ |             | (      ¯¯¯¯¯¯¯¯¯         |  FSMMonitorHandles, +------------------+       
//    `------------------------------'  |             |  \                       |    it is Removed]                 
//                                      |             |   \_                     |                          
//                                      |             |     ¯`->[FSMMonitorHandle|                  
//                                      |             |         Is Destroyed]    |                         
//                                                                                                       
//                                                                                               
//                                                                                     
//                                                                                 
//                                                                                 
//   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Description:
//            Monitor connections and disconnections are processed completely autonomously by the FSMEngine 
//          [as long as its 'processEvents()' function is  called consistently]. Due to the external hardware nature
//          of monitors, creation and destruction of their wrapper type is to be done only internally by the engine so 
//          that each object's lifetime matches a physical connection's lifetime.
//           
//            To enable the Application to query details and interact with a connected monitor, the Engine provides the public
//          type FSMMonitor. This type is meant only to be temporary and thus is only guaranteed to be valid for the duration
//          of the local stack of a function acquiring it [Basically it is best to always re-acquire these objects from
//          the engine whenever they are needed]. These types are meant to serve merely as temporary interfaces to a monitor
//          and assume no ownership over the resource. 
//
//             FSMMonitors can be created by a FSMMonitorHandle object using the handle's 'get()' function.
//                                 
//
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
		FSMMonitorHandle(FSMMonitorHandle&&) noexcept;
		FSMMonitorHandle& operator=(const FSMMonitorHandle&) = delete;
		//FSMMonitorHandle& operator=(FSMMonitorHandle&&) noexcept;

       

		FSMMonitor get() noexcept;

		bool hasHandle(const GLFWmonitor* handle) const noexcept { return (mHandle_ == handle); }

		
	private:
		GLFWmonitor* mHandle_;

         ~FSMMonitorHandle() noexcept;

		//friend std::unique_ptr<FSMMonitorHandle> createMonitorHandle(GLFWmonitor* handle);
        friend FSMMonitorHandle* createMonitorHandle(GLFWmonitor* handle);
		//friend void destroyMonitorHandle(std::unique_ptr<FSMMonitorHandle>);
        friend void destroyMonitorHandle(FSMMonitorHandle*) noexcept;
		FSMMonitorHandle(GLFWmonitor* handle);
		//friend std::unique_ptr<FSMMonitorHandle> std::make_unique<FSMMonitorHandle>(GLFWmonitor*);
	};

	//Factory Function -- Will throw a std::invalid_argument exception if handle is not valid
    FSMMonitorHandle* createMonitorHandle(GLFWmonitor* handle);
	//std::unique_ptr<FSMMonitorHandle> createMonitorHandle(GLFWmonitor* handle);

	//Anti-Factory Function
    void destroyMonitorHandle(FSMMonitorHandle* handle) noexcept;
	//void destroyMonitorHandle(std::unique_ptr<FSMMonitorHandle>) noexcept;

	

} //namespace FSMEngineInternal

#endif //FSM_MONITOR_HANDLE_H_

