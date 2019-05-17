 // File:                              FSMMonitor.h                                                                                                                    
// Class:                             FSMMonitor                                                                                                                      
// QuickInfo:                                                                                                                                                         
//          Default Constructor       DISABLED    [ See Note* --> ]                   *Note: FSMMonitor objects by design have all of their public constructors       
//          Copy Operations           ENABLED                                                (asides copy and move) restricted. The only way for these types to be    
//          Move Operations           ENABLED                                                 created is through internal FSM                                         
//                                                                                                                                                                    
//                                                                                                                                                                    
// See Also:                          FSMVideoMode                                                                                                                    
//                                                                                                                                                                    
//                                                                                                                                                                    
//                                                                                                                                                                    
//                                                                                                                                                                    
// Description:                       [In Leu of The Traditional Rambling Multi-Paragraphed Desciption You Are In All Likelyhood Regularly                            
//                                     Skipping-In-Entirety At This Point, An Alternative Experimental Approach Has Been Undertaken In Documenting                    
//                                      This Type]                                                                                                  
//                                                                                                                                                                    
//                                                                                                                                                                    
//                                            |                                              |                                                                        
//                                            |       __~~~~~~~~~~~~~~~~~~~~~~~~~~~~__       |                                                                        
//                                            |_~~~~~/~~~/                        \~~~\~~~~~_|                            Divide Between Internal                     
//                                            /_\___/=^=/  LifeCycle of a Monitor  \=^=\___/_\                                  and External                          
//                                            \__\__\_|_\_/______________________\_/_|_/__/__/                                      //                                
//                                                                                                                                 ((                                
//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   
//         | Engine Internals |                                                                                                      ))    | Client Interface |       
//         +------------------+         |              |                          |                                                 //     +------------------+       
//                                      |              |                          |                                                ((                                 
//                                      |[Monitor      |[Monitor Event Queues     |            [FSMEngine Collection of             \\                                
//     [Cloud of Asynchronous Time]     |  Event       |  Provide Temporary       |               FSMMonitorHandle Objects]          ))                               
//                                      |   Callback   |  Storage For Raw Monitor |                                                 //      +----------+              
//                                      |    Function] |   Handle Until Engine is |                                                ((       |          |              
//    ,==----------------------------.  |              |     Ready to Process]    |                      +------------------+       \\      |FSMMonitor|              
//   /++/  ~~  ~~  ~~  ~~  ~~  ~~   ++\ |              |      _________           | [FSMMonitorHandle    |                  |  ,-- --))-- --|          |              
//  /++/ ~~  ~~  ~~  ~~  ~~  ~~  ~~  ++\_/_____________|  ,+==\>>\>>\>>\_/        |   is Constructed     | FSMMonitorHandle |<-`    //      +----------+              
//        Monitor Connection Event      _(]=[HANDLE]=>]==(    }--|--|--|}>---->--->--->--->--->--->-->-->|                  |      ((                                 
//  \++\ ~~  ~~  ~~  ~~  ~~  ~~  ~~  ++/|\�������������|  `+==/>>/>>/>>/�\        |    From Queued       |  --------------  |       \\                                
//   \++\ ~~  ~~  ~~  ~~  ~~  ~~    ++/ |              |      ��������            |    Monitor Handle]   |                  |        ))                               
//    `------------------------------'  |              |   [Connection Queue]     |                      | FSMMonitorHandle |       //      +----------+              
//                                      |              |                          |                      |                  |<- -- ((- -- --|          |              
// [Each Event Consists of both a Raw   |The Monitor   |                          |                      |                  |       \\      |FSMMonitor|              
//  glfwMonitor Pointer Which Serves    |  Callback Fn |                          |                      |  --------------  |        ))     |          |              
//   as a Unique Handle and an Event    |  Will Ensure |   [Disconnection Queue]  |                      |                  |       //      +----------+              
//    Code for Connect/Disconnect]      |  The Monitor |   ,;=============,       |                      | FSMMonitorHandle |      ((                                 
//    ,==----------------------------.  | Handle       |  //              \\      |                      |                  |       \\                                
//   /++/  ~~  ~~  ~~  ~~  ~~  ~~   ++\ |              | //     _________  |\     |[Once a matching      |  --------------  |        ))                               
//  /++/ ~~  ~~  ~~  ~~  ~~  ~~  ~~  ++\_/_____________|//   \_/<</<</<</==+ \    |  handle is found     |                  |       //                                
//      Monitor Disconnection Event     (]=[HANDLE]=>=(/  ;-<{[--|--|--{------)<----<----<---<---<---<---| FSMMonitorHandle |      ((                                 
//  \++\ ~~  ~~  ~~  ~~  ~~  ~~  ~~  ++/�\�������������|  /  /�\<<\<<\<<\====J    | amongst the active   |                  |       \\                                
//   \++\ ~~  ~~  ~~  ~~  ~~  ~~    ++/ |              | (      ���������         |  FSMMonitorHandles,  +------------------+        ))                               
//    `------------------------------'  |              |  \                       |    it is Destroyed]                             //                                
//                                      |              |   \_                     |                                                ((                                 
//                                      |              |     �`->[FSMMonitorHandle|                                                 \\                                
//                                      |              |          Is Destroyed]   |                                                  ))                               
//                                                                                                                                  //                                
//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~((~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   
//                                                                                                                                  \\                                
//                                                                                                                                   ))                               
//  Psych! Here is a descripton anyways                                                                                                                               
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
//  Reference:  https://www.glfw.org/docs/3.3/monitor_guide.html#monitor_scale
//              https://www.glfw.org/docs/latest/monitor_guide.html
//
// Programmer:   Forrest Miller
// Coded In Spurts Starting November 2018 and continuing on into Summer 2019
//

#pragma once

#ifndef FSM_MONITOR_H_
#define FSM_MONITOR_H_

#include <vector>
#include <string>
#include <memory>

//Forward declare types as needed
class FSMVideoMode;
struct GLFWmonitor;
struct GLFWgammaramp;
namespace FSMEngineInternal {
	class FSMMonitorHandle;
}

struct MonitorWorkarea {
    int xPos;
    int yPos;
    int width;
    int height;
};

class FSMMonitor final {
public:
	FSMMonitor() = delete;
	FSMMonitor(const FSMMonitor&);
	FSMMonitor(FSMMonitor&&) noexcept;
	FSMMonitor& operator=(const FSMMonitor&);
	FSMMonitor& operator=(FSMMonitor&&) noexcept;
	~FSMMonitor() noexcept;

	//------------------------------------------------------------------------------
	//---------------------------        INTEFACE        ---------------------------
	//------------------------------------------------------------------------------
	
    //Checks to see if this object is currently the primary monitor. Only one 
    //monitor can be primary at a time.
    bool isPrimary() const noexcept;

	int getVirtualPositionX() const noexcept;
	int getVirtualPositionY() const noexcept;
	float getContentScaleX() const noexcept;
	float getContentScaleY() const noexcept;

    //Returns the position (in screen coordinates) of the upper-left corner of the
    //workarea along with the length and width of the workarea. The work area is
    //defined as the part of the screen that is not occluded by the Operating 
    //System's task bars or UI. 
    MonitorWorkarea getWorkarea() const noexcept;

	FSMVideoMode getPrimaryVideoMode() const noexcept;
	std::vector<FSMVideoMode> getVideoModes() const noexcept;

	std::string getName() const noexcept;

	GLFWgammaramp getGammaRamp() const noexcept;


	//Sets a new gamma value for this monitor. The effects of this function will 
	//follow the GLFW documentation for the function glfwSetGamma(). The GLFW
	//documentation has the following note on setting Gamma:
	//    "The software controlled gamma ramp is applied in addition to the
	//     hardware gamma correction, which today is usually an approximation of
	//     sRGB gamma. This means that setting a perfectly linear ramp, or gamma
	//     1.0, will produce the default (usually sRGB-like) behavior."
	//It is recommended to be conservative when calling this function, not 
	//straying to far from the base value of 1.0f
	//Please be aware that this function will throw a std::invalid_argument 
	//exception if gamma is a non-positive value [i.e. gamma must be >0.0f]
	void setGamma(float gamma) noexcept(false);

private:
	FSMMonitor(GLFWmonitor* handle);
	friend class FSMEngineInternal::FSMMonitorHandle;
	class FSMMonitorImpl;
	std::unique_ptr<FSMMonitorImpl> pImpl_;
};

#endif //FSM_MONITOR_H_