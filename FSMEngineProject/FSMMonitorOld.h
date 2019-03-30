//
//  File:          FSMMonitor.h
//  Class:         FSMMonitor
//  QuickInfo:   
//       Default Constructor    DISABLED
//       Copy Operations        ENABLED
//       Move Operations        ENABLED
//
//
// Description:          Wrapper class for a GLFWmonitor object. Provides a C++ interface for 
//                       interacting with these types.
//
// Todo:        Add option to create a custom gamma ramp
//
// Reference:     https://www.glfw.org/docs/latest/monitor_guide.html
//
// Programmer:   Forrest Miller
//

#pragma once

#ifndef FSM_MONITOR_OLD_H_
#define FSM_MONITOR_OLD_H_

//#include <forward_list>
//#include <memory>
//#include <optional>

#include "FSMVideoMode.h"

//Forward declare all the necessary underlying GLFW types
struct GLFWmonitor; 
struct GLFWgammaramp;

class FSMMonitorView1 {
	//This class only is for interfacing with the monitor (and possibly detecting if it has been disconnected.
	//It owns no data
};
//
//namespace FSMEngineInternal {
//
//	class FSMMonitorHandle1 {
//	public:
//		FSMMonitorHandle();
//
//		//Idea: I want this class to have a function that creates MonitorViews to be used publically
//	};
//
//};

//
//class FSMMonitor final {
//public:
//	FSMMonitor() = delete;
//
//private:
//	FSMMonitor(GLFWmonitor* handle);
//	~FSMMonitor() noexcept { ; }
//};
//
//namespace FSMEngineInternal {
//
//};
//
//
//
//
//class FSMMonitorBeta final {
//public:
//    //Must have a GLFWmonitor* handle provided for construction seeing as this handle is
//    //a primary intrinsic to this class.
//    FSMMonitorBeta() = delete;
//    //Only construct objects of this type with values that were returned from glfwGetMonitors(). 
//    //Once assigned it's GLFWmonitor* from the provided parameter, this value can not be changed.
//    //Failure to assign a valid GLFWmonitor* will cause undefined behavior from this class.
//    FSMMonitorBeta(GLFWmonitor * handle);
//
//    ~FSMMonitorBeta() noexcept; 
//
//    //Monitors are to be uniquely acquired from the OS through GLFW, so 
//    //it doesn't make sense to copy them 
//    FSMMonitorBeta(const FSMMonitorBeta&) = delete;
//    //Moving a Monitor is acceptable though since the uniqueness of the monitor
//    //is preserved
//    FSMMonitorBeta(FSMMonitorBeta&&) noexcept;
//    //Monitors are to be uniquely acquired from the OS through GLFW, so 
//    //it does not make sense to copy them, thus creating multiple copies of 
//    //what is supposed to be a unique resource.
//    FSMMonitorBeta& operator=(const FSMMonitorBeta&) = delete;
//    //Moving a Monitor object is perfectly acceptable since the uniqueness of this types'
//    //underlying monitor handle is preserved
//    FSMMonitorBeta& operator=(FSMMonitorBeta&&) noexcept;
//
//    //Returns a const pointer to const GLFWmonitor representing this monitor's handle. 
//    //There is a chance this function could throw if this monitor has been disconnected but
//    //for some reason the instance of the class still exists. [See the member function 
//    //'getMonitorHandleSafe()' for the guaranteed 'noexcept' version of this function]
//    const GLFWmonitor* const getMonitorHandle() const noexcept(false);// { return mHandle_; }
//
//
//    //Returns a std::optional wrapping a const pointer to const GLFWmonitor representing this
//    //monitor's handle, unless for some reason this monitor's handle is no longer valid or has 
//    //lost its uniqueness, in which case an empty optional will he returned. By allowing for 
//    //the returning of empty optionals, this function provides increased safety and is able to 
//    //provide a 'noexcept' guarantee.
//    std::optional< const GLFWmonitor* const> getMonitorHandleSafe() const noexcept;
//
//    //Marks this monitor as disconnected
//    void disconnect() noexcept; // { mIsConnected_ = false; }
//    
//    //Returns the connection status of this monitor. 
//    bool getIsConnected() const noexcept;
//
//
//    //After a monitor-disconnection event by a monitor other than this one, there is a chance
//    //the virtual position of this monitor object will be updated. Thus, this function can be
//    //used to update this monitor's virtual screen position.
//    void updateVirtualPosition() /*noexcept*/;
//
//    //Returns the Virtual Desktop screen X coordinate used for this monitor's viewport. 
//    //Measured from the top left corner of monitor. Coordinate will be unique for each monitor.
//    int getVirtualPositionX() const noexcept;
//
//
//    //Returns the Virtual Desktop screen Y coordinate used for this monitor's viewport. 
//    //Measured from the top left corner of monitor. Coordinate will be unique for each monitor.
//    int getVirtualPositionY() const noexcept;
//
//
//    //In case the videoMode of this monitor is changed, it is recommended that the content scale
//    //values stored be updated. This function is how those values get updated.
//    void updateContentScale() noexcept;
//
//    //Returns the currently stored content scale for this monitor. 
//    // [ Note that modifications made to this monitor's video mode will not
//    //   automatically update this object's stored content 
//    //   scale, so any changes to a monitor's videomode should
//    //   always be followed with a call to member function
//    //   'updateContentScale()' to ensure this function returns 
//    //   accurate results... ]
//    float getContentScaleX() const noexcept;
//        
//    
// //Returns the currently stored content scale for this monitor. 
// // [ Note that modifications made to this monitor's video mode will not
// //   automatically update this object's stored content 
// //   scale, so any changes to a monitor's videomode should
// //   always be followed with a call to member function
// //   'updateContentScale()' to ensure this function returns 
// //   accurate results... ]
//    float getContentScaleY() const noexcept;
//
//    //Returns the number of video modes available for this monitor
//    size_t getNumberOfAvailableVideoModes() const noexcept;
//    //Returns the primary video mode for this monitor
//    FSMVideoMode getPrimaryVideoMode() const noexcept;
//
//
//    //Returns the videomode located at the specified index from
//    //this monitors detected available videomodes. Indexing beyond
//    //the end of the array will return the highest-indexed video mode. 
//    FSMVideoMode getSpecificVideoMode(size_t index) const;
//
//    /* //Returns a vector containing all available video modes.
//    std::vector<FSMVideoMode> getAvailableVideoModes() const; */
//    //
//    /*
//    //Returns false if this monitor's handle is nullptr and true otherwise
//    bool handleNotNull() const { return (mHandle_ == nullptr); }
//
//    //Should always return false in well-formed code
//    bool wasMoved() const { return mWasMoved_; }
//
//    //Compares GLFWmonitor* handles for equality. Note that if both monitors have nullptr as a handle, they will
//    //compare as equal. However, if there are monitors with null handles, then something wrong already has happened.
//    bool operator==(const FSMMonitorBeta& that) const { return (mHandle_ == that.mHandle_); }
//    //Compares GLFWmonitor* handles for inequality. Note that if both monitors have nullptr as a handle, they will
//    //compare as equal. However, if there are monitors with null handles, then something wrong already has happened.
//    bool operator!=(const FSMMonitorBeta& that) const { return (mHandle_ != that.mHandle_); }
//
//    //Compares this object's monitor handle with the provided monitor handle for equality. If this object's 
//    //handle is nullptr and second parameter is nullptr, then equality comparison will return true.
//    //However, if there are monitors with null handles, then something wrong already has happened.
//    bool operator==(const GLFWmonitor* that) const { return (mHandle_ == that); }
//    //Compares this object's monitor handle with the provided monitor handle for equality. If this object's 
//    //handle is nullptr and second parameter is nullptr, then inequality comparison will return false.
//    //However, if there are monitors with NULL handles, then something wrong already has happened.
//    bool operator!=(const GLFWmonitor* that) const { return (mHandle_ != that); }
//
//    //Returns the gamma ramp for this monitor. 
//    ///GLFWgammaramp getGammaRamp() const {
//       ///return mGammaRamp_;
//    ///}
//    //Has GLFW generate a new 256-element gamma-ramp from the specified exponent parameter. Please
//    //ensure that gamma is a positive finite value. Note that the parameter 'gamma' is used as an
//    //exponent when calculating the new ramp. The example code for this uses a value of 1.0f when
//    //setting a gamma ramp.
//    void setCustomGamma(float gamma);
//
//    */
//
//};
//
//
//#ifdef USE_OLD_FSMMONITOR_CODE_
//
//	class FSMMonitorBeta final {
//	public:
//		FSMMonitorBeta() = delete;
//		//Only construct objects of this type with values that were returned from glfwGetMonitors(). 
//		//Once assigned it's GLFWmonitor* from the provided parameter, this value can not be changed.
//		//Failure to assign a valid GLFWmonitor* will cause undefined behavior from this class.
//		FSMMonitorBeta(GLFWmonitor *);
//
//		~FSMMonitorBeta() noexcept; //= default;
//
//		FSMMonitorBeta(const FSMMonitorBeta&) = delete;
//		FSMMonitorBeta(FSMMonitorBeta&&) noexcept;
//		FSMMonitorBeta& operator=(const FSMMonitorBeta&) = delete;
//		FSMMonitorBeta& operator=(FSMMonitorBeta&&) noexcept;
//
//		//Returns a const pointer to const GLFWmonitor representing this monitor's handle. 
//		const GLFWmonitor* const getMonitorHandle() const { return mHandle_; }
//
//		//Marks the monitor as disconnected
//		void disconnect() { mIsConnected_ = false; }
//
//		//Returns the connection status of this monitor. 
//		bool getIsConnected() const { return mIsConnected_; }
//
//		//After a monitor-disconnection event by a monitor other than this one, there is a chance
//		//the virtual position of this monitor object will be updated. Thus, this function can be
//		//used to update this monitor's virtual screen position.
//		void updateVirtualPosition();
//
//		//Returns the Virtual Desktop screen X coordinate used for this monitor's viewport. 
//		//Measured from the top left corner of monitor. Coordinate will be unique for each monitor.
//		int getVirtualPositionX() const {
//			return mVirtualXPos_;
//		}
//
//		//Returns the Virtual Desktop screen Y coordinate used for this monitor's viewport. 
//		//Measured from the top left corner of monitor. Coordinate will be unique for each monitor.
//		int getVirtualPositionY() const {
//			return mVirtualYPos_;
//		}
//
//		//In case the videoMode of this monitor is changed, it is recommended that the content scale
//		//values stored be updated. This function is how those values get updated.
//		void updateContentScale();
//
//		//Returns the stored content scale for this monitor. Note that changing the monitor's video mode 
//		//will not automatically update this object's stored content scale, so any changes to a monitor's
//		//videomode should be followed with a call to this object's member function  'updateContentScale()'
//		float getContentScaleX() const {
//			return mContentScaleX_;
//		}
//
//		//Returns the stored content scale for this monitor. Note that changing the monitor's video mode 
//		//will not automatically update this object's stored content scale, so any changes to a monitor's
//		//videomode should be followed with a call to this object's member function  'updateContentScale()'
//		float getContentScaleY() const {
//			return mContentScaleY_;
//		}
//
//		//Returns the number of video modes available for this monitor
//		size_t getNumberOfAvailableVideoModes() const;
//		//Returns the primary video mode for this monitor
//		FSMVideoMode getPrimaryVideoMode() const;
//		//Returns the videomode located at the specified index from this monitors detected available
//		//videomodes. Indexing beyond the end of the array will return the highest-indexed video mode. 
//		FSMVideoMode getSpecificVideoMode(size_t index) const;
//		
//        /* //Returns a vector containing all available video modes.
//		std::vector<FSMVideoMode> getAvailableVideoModes() const; */ 
//        //
//
//		//Returns false if this monitor's handle is nullptr and true otherwise
//		bool handleNotNull() const { return (mHandle_ == nullptr); }
//
//		//Should always return false in well-formed code
//		bool wasMoved() const { return mWasMoved_; }
//
//		//Compares GLFWmonitor* handles for equality. Note that if both monitors have nullptr as a handle, they will
//		//compare as equal. However, if there are monitors with null handles, then something wrong already has happened.
//		bool operator==(const FSMMonitorBeta& that) const { return (mHandle_ == that.mHandle_); }
//		//Compares GLFWmonitor* handles for inequality. Note that if both monitors have nullptr as a handle, they will
//		//compare as equal. However, if there are monitors with null handles, then something wrong already has happened.
//		bool operator!=(const FSMMonitorBeta& that) const { return (mHandle_ != that.mHandle_); }
//
//		//Compares this object's monitor handle with the provided monitor handle for equality. If this object's 
//		//handle is nullptr and second parameter is nullptr, then equality comparison will return true.
//		//However, if there are monitors with null handles, then something wrong already has happened.
//		bool operator==(const GLFWmonitor* that) const { return (mHandle_ == that); }
//		//Compares this object's monitor handle with the provided monitor handle for equality. If this object's 
//		//handle is nullptr and second parameter is nullptr, then inequality comparison will return false.
//		//However, if there are monitors with NULL handles, then something wrong already has happened.
//		bool operator!=(const GLFWmonitor* that) const { return (mHandle_ != that); }
//
//		//Returns the gamma ramp for this monitor. 
//		///GLFWgammaramp getGammaRamp() const {
//           ///return mGammaRamp_;
//		///}
//		//Has GLFW generate a new 256-element gamma-ramp from the specified exponent parameter. Please
//		//ensure that gamma is a positive finite value. Note that the parameter 'gamma' is used as an
//		//exponent when calculating the new ramp. The example code for this uses a value of 1.0f when
//		//setting a gamma ramp.
//		void setCustomGamma(float gamma);
//
//	private:
//
//        class MonitorImpl;
//        std::unique_ptr<MonitorImpl> pMonitorImpl_;
//
//		//Each GLFWmonitor object is assigned a unique handle of type GLFWmonitor* that will remain valid until the
//		//monitor is disconnected or GLFW is uninitialized. Thus each object of this type can be identified by its handle
//		const GLFWmonitor* mHandle_;
//
//		//Object tracking fields
//		bool mWasMoved_; //This is just to provide extra safety for invalidating an object after it has been moved
//		bool mIsConnected_; 	
//		bool mIsPrimary_; //GLFW will determine if a monitor is primary or not, a primary monitor typically is one that contains UI/Desktop
//		
//		//Monitor-specific data/specifications
//		size_t mAvailableVideoModes_;
//        std::forward_list<FSMVideoMode> mVideoModes_;//std::vector<VideoMode> mVideoModes_;
//		std::unique_ptr<FSMVideoMode> mPrimaryVideoMode_;
//
//		//The virtual desktop position (measured in screen coordinates) is unique for each monitor.
//		int mVirtualXPos_, mVirtualYPos_;
//		//The content scale is the ratio between the currently in-use DPI of the monitor and the default DPI. 
//		//These values will be useful for properly sizing text and other UI elements.
//		float mContentScaleX_, mContentScaleY_;
//
//		struct GLFWgammaramp* mGammaRamp_;
//		
//		std::string mName_;
//		
//
//
//		//Private helper functions:
//		void initialize(); //Gives initial values to this object's member fields
//		bool checkIfIsPrimaryMonitor(); //Compares monitor handle to handle returned by glfwGetPrimaryMonitor()
//		void aquireMonitorDetails(); //Gets monitor properties from GLFW
//		
//
//		//The following functions are called within aquireMonitorDetails() 
//		void getMonitorName();
//		void getVideoModes();
//		void getMonitorPhysicalSize(int& w, int& h);
//		
//
//	};
//
//#endif //USE_OLD_FSMMONITOR_

#endif //FSM_MONITOR_OLD_H_
