// File:                       FSMCallbackInitializer.cpp
//
//
//  Description:               Contains the implementation details which occur when setting up 
//                             the GLFW callback functions. See the header file to get the 
//                             functions which should be called outside this class.
//
//  Programmer:                Forrest Miller
//  Date:                      January 2, 2019
//

#include "FSMCallbackInitializer.h"
#include "FSMErrorCallback.h"
#include "FSMMonitorEventCallback.h"
#include "FSMJoystickEventCallback.h"


namespace FSMEngineInternal {


    class FSMCallbackInitializer::GlobalStateOfGLFWCallbacks {
    public:
        bool errorCBSet_; //GLFW's error callback
        bool joystickCBSet_; //Joystick connect/disconnect event callback
        bool monitorCBSet_; //Monitor connect/disconnect event callback
        GlobalStateOfGLFWCallbacks() {
            errorCBSet_ = false;
            joystickCBSet_ = false;
            monitorCBSet_ = false;
        }
        ~GlobalStateOfGLFWCallbacks() = default;
    };

    FSMCallbackInitializer::FSMCallbackInitializer() : mGlobalCBState_(nullptr) {
        mGlobalCBState_ = std::make_unique<FSMCallbackInitializer::GlobalStateOfGLFWCallbacks>();
        if (!mGlobalCBState_)
            throw ("Error Initializing Callback Functions!\n");
    }

    FSMCallbackInitializer::~FSMCallbackInitializer() = default;

    void FSMCallbackInitializer::setGLFWErrorCallback() noexcept {
        LOG(INFO) << "\n           Setting GLFW Error Callback Function!   \n";
        glfwSetErrorCallback(FSMErrorCallbackFunction);
        mGlobalCBState_->errorCBSet_ = true;
    }

    void FSMCallbackInitializer::setJoystickEventCallback() noexcept {
        LOG(INFO) << "\n           Setting GLFW Joystick event Callback Function!   \n";
        glfwSetJoystickCallback(JoystickEventCallbackInternal::graphicsLanguageFrameworkJoystickEventCallbackFunction);
    }

    void FSMCallbackInitializer::setMonitorEventCallback() noexcept {
        LOG(INFO) << "\n           Setting GLFW Monitor Event Callback Function!   \n";
        glfwSetMonitorCallback(MonitorEventCallbackInternal::graphicsLanguageFrameworkMonitorEventCallbackFunction);
        mGlobalCBState_->monitorCBSet_ = true;
    }

	
    bool FSMCallbackInitializer::errorCBWasSet() const noexcept {
        return mGlobalCBState_->errorCBSet_;
    }

    bool FSMCallbackInitializer::joystickEventCBWasSet() const noexcept {
        return mGlobalCBState_->joystickCBSet_;
    }

    bool FSMCallbackInitializer::monitorEventCBWasSet() const noexcept {
        return mGlobalCBState_->monitorCBSet_;
    }

/*

	//////////////////////////////////////////
	/////  Internal Function Prototypes  /////
	//////////////////////////////////////////
	namespace FSMCallbackImplementation {
		//Pre-initialization
		void setErrorCallback();
		//Post-initialization
		void setMonitorEventCallback();

	} //namespace FSMCallbackImplementation



    //----------------------------------------------------------------------------------------



	///////////////////////////////////////////////////////////////
	/////    Implementation for Functions listed in header    /////
	///////////////////////////////////////////////////////////////

	//To be called before GLFW is initialized
	void configurePreInitializationCallbackFunctions() {
		FSMCallbackImplementation::setErrorCallback();
	}


	//To be called after GLFW has been successfully initialized
	void configurePostInitializationCallbackFunctions() {
		FSMCallbackImplementation::setMonitorEventCallback();

	}



	//----------------------------------------------------------------------------------------


    ///////////////////////////////////////////////////////////////////
	//////  Implementation of Internal Callback Setup Functions  //////
	///////////////////////////////////////////////////////////////////

	namespace FSMCallbackImplementation {
		
		void setErrorCallback() {
			GLFWerrorfun previousErrorFun = glfwSetErrorCallback(FSMErrorCallbackFunction);
			
			//Question: Do GLFW applications using GLFW as a shared library (DLL) each get to set their error callback function independently?
			//Just in case there was an error callback function that was already set, probably best be safe and print out a warning that it has
			//been replaced by this application's error callback function. If GLFW is well behaved, this message will never be printed.
			if (previousErrorFun != nullptr) {
				fprintf(WRNLOG, "\nWarning! While setting up the Graphics Language Framework's error-reporting function,\n"
					"a previously set error function was replaced!\n");
			}
		}
	
		void setMonitorEventCallback() {
			glfwSetMonitorCallback(MonitorEventCallbackInternal::graphicsLanguageFrameworkMonitorEventCallbackFunction);
		}


	
	} //namespace FSMCallbackImplementation
*/

} //namespace FSMEngineInternal


