//
//  File:         FSMRenderEnvironment.cpp
//  
//  Desc:          See header for more details
//
//
//  Programmer:    Forrest Miller
//  Date:          Created March 9, 2019    [I doubt it will be finished today tho...]
//

#include <sstream>
#include <fstream>

#include "FSMRenderEnvironment.h"
#include "UniversalIncludes.h"
#include "FSMEngineSettings.h"
#include "GraphicsLanguageFramework.h"
#include "FSMCallbackInitializer.h"
#include "FSMJoystickInvariants.h"           //To get invariant for joystick hat behavior

#include "GL_Context_Debug_Message_Callback_Function.h"    //Function for reporting context callback messages
#include "Pre_And_Post_OpenGL_Function_Call_Callbacks.h"  //glad_debug allows for assignment of two callback functions to
//                                                         //bookend every call made to each 'gl____()' function.
//The following includes are needed for checking the event queues
#include "FSMMonitorEventCallback.h"
#include "FSMJoystickEventCallback.h"

#include "FSMMonitorHandle.h"
#include "FSMMonitor.h"
#include "FSMVideoMode.h"

#include "ReportDetailsOnGLImplementation.h"

using namespace FSMEngineInternal;
//using namespace FSMEngineDefaultInitializationConstants;

//This is the private internal delegating constructor that exists simply to 
//give each member of this class an initial value. By making use of this 
//constructor, it can be relied upon that this class's destructor will get called 
//in the event something goes wrong in while performing initialization within another
//constructor, thus preventing a resource leak. Without this delegating constructor, 
//any action that would prevent one of the other constructors from completing would 
//result in the destructor call getting skipped. 
//
//The parameter 'dummyParameter' to this constructor is just a decoy to give this 
//constructor a unique signature, this parameter is not used in any way. 
FSMRenderEnvironment::FSMRenderEnvironment(bool dummyParameter) noexcept {
    LOG(TRACE) << __FUNCTION__;
    (void)dummyParameter;//We don't care about the parameter, it exists just to 
    //                   //differentiate this constructor from default public one  
    mGLFWIsInit_ = false;
    mContextWindow_ = nullptr;
    mContextResetAwareness_ = true; 
    mJoystickStatePrintingEnabled_ = false;
    //Question: Can this next line fail? Should it be moved to be with other parts of constructor which can throw? 
    mJoystickInputPrinter_ = std::make_unique<JoystickStatePrinter>(); 

    mTargetMonitor_ = NO_MONITOR_TARGETED;

    mScreenClearColor_ = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
}

FSMRenderEnvironment::FSMRenderEnvironment() : FSMRenderEnvironment(true) {
    LOG(TRACE) << __FUNCTION__;
    //Because the delegating constructor was used, we know that FSMRenderEnvironment 
    //is now a complete type. Thus we don't need to worry about any further actions
    //we take here in this constructor throwing exceptions which otherwise would result in 
    //the destructor not getting called. 

    //Thus we can safely start doing the actual initialization here and throw any
    //exceptions if we need to

    LOG(INFO) << "\n [step 1]   Loading settings from initialization file!\n";

    //STEP 1
    if (!loadEngineSettings()) {
        throw FSMException("Unable to load settings!\n");
    } 

    //STEP 2
    if (!initializeGLFW()) {
        throw FSMException("An issue was encountered while initializing the "
            "Graphics Language Framework [GLFW] Library!\n");
    }

    std::ostringstream msg;
    msg << "[step 2]    GLFW Library has initialized! GLFW version: " << getGLFWRuntimeVersionString();
    LOG(INFO) << msg.str();

    //STEP 3
    if (!createContext()) {
        throw FSMNamedException(FSMNamedException::NamedException::NO_GL_DRIVER, "Failed to Create Context. No GL Driver Tho!");
        //throw FSMException("An issue was encountered while creating the GLFW window and OpenGL\n"
        //    "context! These are kinda sorta vital for this application to function properly, so \n"
        //   "the fact they failed means this whole application is probably going to have to crash!\n\n");
    }

    LOG(INFO) << "  [step 3]      Graphics Language Context Created!\n";

 
    //STEP 4
    if (!setupGLAD()) {
        throw FSMException("The issue occurred setting up GLAD!\n");
    }

    LOG(INFO) << "  [step 4]     GLAD LOADED OPENGL SUCCESFULLY !~!!!  OMG ALMOST THERE !!!!\n";

    LOG(INFO) << "\n  [step 5]     Configuring Engine from Loaded Settings\n";

    mContextResetAwareness_ = checkForContextResetStrategy();
     
    //This function is a work in progress at the moment
    LOG(INFO) << reportGLImplementationDetails();

}


FSMRenderEnvironment::~FSMRenderEnvironment() noexcept {
    LOG(TRACE) << __FUNCTION__;

    for (auto monHandleIter = mMonitors_.begin(); monHandleIter != mMonitors_.end(); monHandleIter++) {
        destroyMonitorHandle(*monHandleIter);
    }

    if (mGLFWIsInit_) {
        glfwTerminate();
        mGLFWIsInit_ = false;
    }
    else {
        LOG(WARNING) << ""
            "FSMRenderEnvironment destructor was called but its internal\n"
            "            data shows that GLFW was never initialized, thus\n"
            "            no call to glfwTerminate() has been made!\n";
    }
}


void FSMRenderEnvironment::handleEvents() {
	// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
    
    ///LOG(TRACE) << "CALLING glfwPollEvents Function!";
	glfwPollEvents();
    ///LOG(TRACE) << "Returned from glfwPollEvents Function!";
     
    ///std::string msg = std::string("mMonitors_ size is ") + std::to_string(mMonitors_.size());
    ///LOG_EVERY_N(120, DEBUG) << msg;
     
    //If context aware, then need to check status
    if (mContextResetAwareness_)
        checkForContextReset();
     
    //Do Joystick-printer Debug logic
    doJoystickPrinterLoopLogic();

	//Check queues?
	if (checkMonitorEventQueues()) {
		LOG(INFO) << "FSMEngine Detected a Monitor Event.";
		if (checkMonitorConnectionEventQueue()) {
			LOG(INFO) << "FSMEngine is ready to process the newly connected monitor!";
			auto newMonitor = getNextAvailableMonitorConnection();
			if (newMonitor.has_value()) {
				if (newMonitor.value() != nullptr) {
					LOG(INFO) << "Adding monitor with handle to memory address of " << newMonitor.value();
					LOG(INFO) << "Monitor Name: " << glfwGetMonitorName(newMonitor.value());
					mMonitors_.push_back(createMonitorHandle(newMonitor.value()));
				}
				else
					LOG(DEBUG) << "Uh-oh, the new monitor's handle was a nullptr!\n";
			}
			else {
				LOG(INFO) << "Uh-oh! The Monitor Connection Queue returned an empty optional!\n";
				LOG(DEBUG) << "FSMEngine has detected that the Monitor Connection Queue returned an empty optional!";
			}
		}
		else if (checkMonitorDisconnectionEventQueue()) {
			LOG(INFO) << "FSMEngine is ready to remove the recently disconnect monitor from it's collection of tracked displays!";
			auto disconnectedMonitor = getNextAvailableMonitorDisconnection();
			if (disconnectedMonitor.has_value()) {
				if (disconnectedMonitor.value() != nullptr) {
					LOG(INFO) << "Removing monitor with handle to memory address of " << disconnectedMonitor.value();
					//std::find_if(mMonitors_.begin(), mMonitors_.end(),
					//	[&](std::unique_ptr<FSMMonitorHandle> & mon) { return mon->hasHandle(disconnectedMonitor.value()); });
					///mMonitors_.remove_if([&](std::unique_ptr<FSMMonitorHandle> & mon) 
					///	{ return mon->hasHandle(disconnectedMonitor.value()); });
                   auto iter = std::find_if(mMonitors_.begin(), mMonitors_.end(),
                       [&](FSMMonitorHandle* mon) { 
                           return mon->hasHandle(disconnectedMonitor.value()); 
                       });
                   if (iter != mMonitors_.end()) {
                       destroyMonitorHandle(*iter);
                       mMonitors_.erase(iter);
                   }
                   else {
                       LOG(WARNING) << "WARNING! FSMEngine encountered a monitor in the Disconnect event queue that\n"
                           "         failed to match with any of the currently connected monitors!\n";
                   }
				}
				else
					LOG(DEBUG) << "Uh-oh, the disconnected monitor's handle was a nullptr!\n";
			}
			else {
				LOG(INFO) << "Uh-oh! The Monitor Disconnection Queue returned an empty optional!\n";
				LOG(DEBUG) << "FSMEngine has detected that the Monitor Disconnection Queue returned an empty optional!";
			}
		}
		else {
			LOG(INFO) << "That is odd. Neither the Monitor connection nor disconnection queues are\n"
				<< "populated despite a monitor event being detected!";
			LOG(DEBUG) << "FSMEngine has detected a possible Monitor Event Misfire!\n";
		}
	}
	if (checkJoystickEventQueues()) {
		LOG(INFO) << "FSMEngine Detected a Joystick Event.";
		if (checkJoystickConnectionEventQueue()) {
			LOG(INFO) << "FSMEngine is ready to process the newly connected joystick!";
			std::optional<int> newJS = getNextAvailableJoystickConnection();
			if (newJS.has_value()) {
				LOG(INFO) << "FSMEngine has successfully assimilated the joystick under JoyID " << newJS.value();
			}
			else {
				LOG(INFO) << "Uh-oh! The Joystick Connection Queue returned an empty optional!\n";
				LOG(WARNING) << "FSMEngine has detected that the Joystick Connection Queue returned an empty optional!";
			}
		}
		else if (checkJoystickDisconnectionEventQueue()) {
			LOG(INFO) << "FSMEngine is ready to remove the recently disconnected joystick from the list of connected input devices!";
			std::optional<int> jsToRemove = getNextAvailableJoystickDisconnection();
			if (jsToRemove.has_value()) {
				LOG(INFO) << "FSMEngine is no longer accepting input from joystick under JoyID " << jsToRemove.value();
			}
			else {
				LOG(INFO) << "Uh-oh! The Joystick Disconnection Queue returned an empty optional!\n";
				LOG(WARNING) << "FSMEngine has detected that the Joystick Disconnection Queue returned an empty optional!";
			}
		}
		else {
			LOG(INFO) << "That is odd. Neither the Joystick connection nor disconnection queues are\n"
				<< "populated despite a joystick event being detected!";
			LOG(WARNING) << "FSMEngine has detected a possible Joystick Event Misfire!\n";
		}
	}
}


void FSMRenderEnvironment::doMonitorSelectionLoop() {
    LOG(TRACE) << __FUNCTION__;

    // Define the viewport dimensions
    glViewport(0, 0, width, height);

	//int count = 0;
	//GLFWmonitor** monitorHandles = glfwGetMonitors(&count);

	//for (int i = 0; i < count; i++)
	//	mMonitors_.push_back(createMonitorHandle(monitorHandles[i]));

	if (!(mMonitors_.empty())) {
		FSMMonitor primary = mMonitors_.front()->get();
        
        //Is primary deleted by this point?
         
		//LOG(INFO) << "\tNumber of VideoModes: " << primary.getVideoModes().size();
//		int counter = 0;
		//for (FSMVideoMode v : primary.getVideoModes()) {
		//	LOG(INFO) << "VideoMode " << counter++;
		//	LOG(INFO) << "\tRefresh Rate: " << v.getRefreshRate();
		//	LOG(INFO) << "\tPixels X: " << v.getWidth() << ",    Pixels Y: " << v.getHeight();
		//	LOG(INFO) << "\tDPI X: " << v.getDPI_Width() << ",    DPI Y: " << v.getDPI_Height();
		//	LOG(INFO) << "\tDPI: " << v.getDPI_WidthHeightAverage() << "\n";
		//}
	}
	else {
        std::string msg = "\nUh-oh! mMonitors_ is as empty as can be!\n";
		LOG(INFO) << msg;
	}

    glEnable(GL_BLEND);
    
    // Game loop
    while (!glfwWindowShouldClose(mContextWindow_)) {

        static float time = 0.0f;
        static uint64_t counter = 0u;
        counter++;
        time += 0.05f;
        
		handleEvents();


        // Render
        
        if (counter % 150u == 0u) {
            setNextFrameClearColor(time);
            logToInfoNextFrameClearColor();
        }

        glClearColor(mScreenClearColor_.r, mScreenClearColor_.g, mScreenClearColor_.b, mScreenClearColor_.a);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(mContextWindow_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(mContextWindow_, true);
        }

        // Swap the screen buffers
        glfwSwapBuffers(mContextWindow_);

        
        
        //FSMCallbackInitializer* theCBInitializer = &FSMCallbackInitializer::callbackInitializer(); 
        //theCBInitializer->
    }


}



void FSMRenderEnvironment::getGLVersion(int& versionMajor, int& versionMinor, bool& usesCompatMode) const noexcept {
    LOG(TRACE) << __FUNCTION__;
    versionMajor = GLVersion.major;
    versionMinor = GLVersion.minor;
    usesCompatMode = false; //Should be safe to assume this code never will run in compatibility mode....
}

int FSMRenderEnvironment::getGLVersionMajor() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return GLVersion.major;
}

int FSMRenderEnvironment::getGLVersionMinor() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return GLVersion.minor;
}

std::string FSMRenderEnvironment::getGLVersionString() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    std::ostringstream version;
    version << glGetString(GL_VERSION);
    //version << "OpenGL Version " << std::to_string(GLVersion.major) << ".";
    //version << std::to_string(GLVersion.minor) << " Core";
    return version.str();
}

void FSMRenderEnvironment::getGLFWCompiletimeVersion(int& compileVersionMajor,
                                                     int& compileVersionMinor,
                                                     int& compileVersionRevision) noexcept {
    LOG(TRACE) << __FUNCTION__;
    compileVersionMajor = GLFW_VERSION_MAJOR;
    compileVersionMinor = GLFW_VERSION_MINOR;
    compileVersionRevision = GLFW_VERSION_REVISION;
}

std::string FSMRenderEnvironment::getGLFWCompiletimeVersionString() noexcept {
    LOG(TRACE) << __FUNCTION__;
    return std::string(glfwGetVersionString()); //GLFW returns a compile-time generated string
}


void FSMRenderEnvironment::getGLFWRuntimeVersion(int& runtimeVersionMajor,
                                                 int& runtimeVersionMinor,
                                                 int& runtimeVersionRevision) const noexcept {
    LOG(TRACE) << __FUNCTION__;
    glfwGetVersion(&runtimeVersionMajor, &runtimeVersionMinor, &runtimeVersionRevision);
}

std::string FSMRenderEnvironment::getGLFWRuntimeVersionString() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    int maj, min, rev;
    glfwGetVersion(&maj, &min, &rev);
    std::ostringstream version;
    version << "GLFW " << maj << "." << min << "." << rev;
    return version.str();
}





///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////     IMPLEMENTATION FUNCTIONS     /////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


// -----------------------------------------------------------------------------------------
//                                       STEP 1 
// -----------------------------------------------------------------------------------------
bool FSMRenderEnvironment::loadEngineSettings() {
    LOG(TRACE) << __FUNCTION__;

    
    //Step 1.1
    std::error_code ecFind{};
    bool settingsFileFound = locateSettingsFile(ecFind);

    if (settingsFileFound) {
        parseSettingsFile();                                              //Step 1.2
        return true;
    }

    else {
        if (!ecFind) {
            std::error_code ecCreate{};
            bool settingsFileCreated = generateSettingsFile(ecCreate);   //Step 1.1b
            if (settingsFileCreated && (!ecCreate)) {
                parseSettingsFile();                                     //Step 1.2
                return true;
            }
            else if (ecCreate) {
                 LOG(ERROR) << "\nError reported by Operating System while attempting\n"
                    << "to generate configuration file (" << /*SETTINGS_FILE_NAME*/ "UNNAMED" << ")!\n"
                    << "Operating System Error Message:\n\t"
                    << ecCreate.message() << std::endl;
            return false;
            }
            else {
                return false;
            }
        }
        else /*if (ecFind) */ { //
            LOG(ERROR) << "\nError reported by Operating System while attempting\n"
                << "to load configuration file (" << /*SETTINGS_FILE_NAME*/ "UNNAMED" << ")!\n"
                << "Operating System Error Message:\n\t"
                << ecFind.message() << std::endl;
            return false;
        }
    }

    ////Step 1.2  [Options A or B]
    //bool settingsLoaded = false;
    //if (initFileFound) {                //[1.2 OPTION A]
    //    settingsLoaded = parseInitFile();
    //}
    //else if (ec) {        //First check to see if OS reported an error
    //    LOG(ERROR) << "\nError reported by Operating System while attempting\n"
    //        << "to load configuration (" << SETTINGS_FILE_NAME << ") file!\n"
    //        << "Operating System Error Message:\n\t"
    //        << ec.message() << std::endl;
    //    return false;
    //}
    //else {                            //[1.2 OPTION B]    
    //    //generating a settings file could cause OS to report error, so we need 'ec' again  
    //    settingsLoaded = generateInitFile(ec);  
    //    if (ec) {
    //        LOG(ERROR) << "\nError reported by Operating System while attempting\n"
    //            << "to generate configuration (" << SETTINGS_FILE_NAME << ") file!\n"
    //            << "Operating System Error Message:\n\t"
    //            << ec.message() << std::endl;
    //        return false;
    //    }
    //}
    //return settingsLoaded;

//#undef LIKELY
//#undef UNLIKELY
}




// --------------  
//    STEP 1.1     
// --------------
//Parameter 'ec' will be set to any errors which may be reported by the OS when calling functions from 'std::filesystem'
bool FSMRenderEnvironment::locateSettingsFile(std::error_code& ec) {
    LOG(TRACE) << __FUNCTION__;
    auto currentPath = std::filesystem::current_path(ec);
    if (ec)
        return false;
    else {
#if 1
        return true;
#elif defined ReadyToHaveEngineParseFilesForSettings
        //Find out if the 'settings.fini' file exists in this directory
        LOG(DEBUG) << "Looking for '" << /*SETTINGS_FILE_NAME*/ "UNNAMED" << "' file in directory " << currentPath.string() << std::endl;
        std::filesystem::path settingsFile{ currentPath.string() + "\\" + /*DEFAULT_SETTINGS_DIRECTORY_NAME*/ + "\\" + /*SETTINGS_FILE_NAME*/ "UNNAMED" };
        settingsFile.make_preferred();
        LOG(DEBUG) << "The full path that is being verified for existence is: " << settingsFile.string() << std::endl;
        bool fileExists = std::filesystem::exists(settingsFile, ec);
        if (ec)
            return false;
        else
            return fileExists;
#endif
    }
}


// --------------
//    STEP 1.2  [OPTION A: File Exists]
// --------------
void FSMRenderEnvironment::parseSettingsFile() {
    LOG(TRACE) << __FUNCTION__;
    LOG(INFO) << "\nSettings file found! Parsing now...\n";
    //return true;
}



// --------------
//   STEP 1.2  [OPTION B: FILE DOES NOT EXIST]  (yet)
// --------------
bool FSMRenderEnvironment::generateSettingsFile(std::error_code& ec) {
    LOG(TRACE) << __FUNCTION__;
    /*
    LOG(INFO) << "\nUnable to find an existing settings file. Generating a settings file...\n";
    auto currentPath = std::filesystem::current_path(ec);
    if (ec)
        return false;
    std::filesystem::path settingsDirectoryPath{ currentPath.string() + "\\" + DEFAULT_SETTINGS_DIRECTORY_NAME };

    std::filesystem::create_directory(settingsDirectoryPath, ec); //Has no effect if the directory already exists
    if (ec)
        return false;

    std::filesystem::path settingsFilePath{ settingsDirectoryPath.string() + "\\" + SETTINGS_FILE_NAME };

    std::ofstream settingsFile{ settingsFilePath };

    settingsFile << ""
        << "#   Welcome to the settings file. This is where the configuration for the \n"
        << "# FSMEngineProject is set. The format for this file is loosely based off  \n"
        << "# the '.ini' file format which is commonly seen in other programs.        \n"
        << "#                                                                         \n"
        << "#                                                                         \n"
        << "#   Below this header you can find a collection of all of the various     \n"
        << "# configuration settings which affect the runtime behavior of this        \n"
        << "# program. Before editing settings, it is suggested you read the          \n"
        << "# following formatting rules:                                             \n"
        << "#                                                                         \n"
        << "#   (-) Any line beginning with a '#' is a comment and will be            \n"
        << "#         ignored during parsing.                                         \n"
        << "#                                                                         \n"
        << "#   (-) Names for each setting are caps sensitive, so it is suggested     \n"
        << "#         to not edit any settings names as they already appear.          \n"
        << "#                                                                         \n"
        << "#   (-) Each Setting has a unique name identifier, which starts with      \n"
        << "#         a letter which identifies what type of variable that setting    \n"
        << "#         is expecting to parse. Here are the rules for each:             \n"
        << "#              'b' stands for boolean and should be only 0 or 1           \n"
        << "#              's' stands for string and should be restricted to          \n"
        << "#                     ASCII characters only. Spaces are allowed. Please   \n"
        << "#                     enclose the string in double quotes, and to add a   \n"
        << "#                     double quote to the string, use the escape sequence \n"
        << "#                     ' \\\" '. To add a backslash, double it up like       \n"
        << "#                     ' \\\\ '.                                             \n"
        << "#              'i' stands for integer and thus can be any positive        \n"
        << "#                     or negative whole number (without being too         \n"
        << "#                     ridiculously large). No commas please!              \n"
        << "#              'u' stands for unsigned. This means any non-negative       \n"
        << "#                     integer (again without making it excessively        \n"
        << "#                     large).                                             \n"
        << "#              'f' stands for floating point. This is a number that can   \n"
        << "#                     be either positive or negative, and is allowed to   \n"
        << "#                     contain a single decimal point (aka 'period') to    \n"
        << "#                     have it be a non-whole number. Please note that     \n"
        << "#                     this program uses 32-bit precision for this type,   \n"
        << "#                     so no need to provide excessive precision when      \n"
        << "#                     setting these values.                               \n"
        << "#              'c' stands for character. This should be restricted to a   \n"
        << "#                     single ASCII character surrounded by single quotes. \n"
        << "#                                                                         \n"
        << "#                                                                         \n"
        << "#   (-) Certain settings may be more restrictive than settings of their   \n"
        << "#         type allow. These should be commented where they apply.         \n"
        << "#                                                                         \n"
        << "#                                                                         \n"
        << "#   (-) Settings are organized by category. Any line starting with a '['  \n"
        << "#          character represents the start of a new category. These exist  \n"
        << "#          solely to help human readers and will be ignored by this       \n"
        << "#          programs parser as if they were a comment.                     \n"
        << "#                                                                         \n"
        << "#                                                                         \n"
        << "#   (-) Typically spaces will be ignored throughout the document where-   \n"
        << "#          ever they might appear, but then again this parser isn't going \n"
        << "#          to be winning any \"Worlds-Greatest-Parser awards anytime soon \n"
        << "#          so don't be surprised if confusing or ambiguous syntax causes  \n"
        << "#          it to choke.                                                   \n"
        << "#                                                                         \n"
        << "#                                                                         \n"
        << "#   (-) One last thing, one of the primary design rules for this format   \n"
        << "#          that anything that fails to parse will be replaced with a new  \n"
        << "#          defaulted value for that setting. Thus don't worry if you mess \n"
        << "#          up, because the worst that will happen is that your setting    \n"
        << "#          will be ignored and replaced with a properly-formated setting. \n"
        << "#          This goes for anything this parser doesn't agree with reading. \n"
        << "#          So if something seems off, just check this settings file and   \n"
        << "#          you should be able to get a pretty clear sense of if you are   \n"
        << "#          obeying these syntax rules properly or not.                    \n"
        << "\n\n\n\n"
        << "  [General]                                                               \n";

    return true;
    */
    return true;
}

// --------------  
//    STEP 1.3     
// --------------  
bool FSMRenderEnvironment::verifySettings() const noexcept {
    bool incompatibilityDetected = false;

    //Verify that exceptions are being logged
    if constexpr ((LOG_NAMED_FSM_EXCEPTIONS_AS_WARNINGS) ||
        (LOG_NAMED_FSM_EXCEPTIONS_AS_ERRORS)) {

    }
    return !incompatibilityDetected;
}

// -----------------------------------------------------------------------------------------
//                                      STEP 2 
// -----------------------------------------------------------------------------------------
bool FSMRenderEnvironment::initializeGLFW() {
    LOG(TRACE) << __FUNCTION__;
    //Step 2.1
    setGlobalGLFWInvariants();
    
    //Step 2.2
    setPreGLFWInitCallbacks();

    //Step 2.3
    bool success = callTheGLFWInitFunc();
   
    //If Step 2.3 success, then Step 2.4
    if (success) {
        setPostGLFWInitCallbacks();
    }
    return success;
}

// --------------
//    STEP 2.1
// --------------
//This function must be called before glfwInit() is called
void FSMRenderEnvironment::setGlobalGLFWInvariants() noexcept {
    LOG(TRACE) << __FUNCTION__;

    //Specifiy if should treat joystick hats as separate from Joystick buttons
    if (FSMJoystickInternal::JOYSTICK_REPORTS_HAT_INPUT_AS_BUTTONS)
        glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_TRUE);
    else 
        glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);

    //There are 2 other MacOS Cocoa-specific hints, but seeing as Apple is deprecating 
    //OpenGL it hardly feels worth doing more than just mentioning them here...
}

// --------------
//    STEP 2.2
// --------------
void FSMRenderEnvironment::setPreGLFWInitCallbacks() {
    LOG(TRACE) << __FUNCTION__;
    //The callback initializer is a singleton object. We can get a pointer to it though to 
    //interact with it for the course of this function. 
    FSMCallbackInitializer* cbInitializer = &(FSMCallbackInitializer::callbackInitializer());

    cbInitializer->setGLFWErrorCallback();
}

// --------------
//    STEP 2.3
// --------------
bool FSMRenderEnvironment::callTheGLFWInitFunc() {
    LOG(TRACE) << __FUNCTION__;
    //This is it... The function to initialize GLFW. Before this function is called, be
    //sure that all GLFW invariants have been set and that the GLFW error callback function
    //has been specified. 
    int success = glfwInit();
    if (!success) {
        mGLFWIsInit_ = false;
        return false;
    }
    else {
        mGLFWIsInit_ = true;
        return true;
    }
}

// --------------
//    STEP 2.4
// --------------
void FSMRenderEnvironment::setPostGLFWInitCallbacks() {
    LOG(TRACE) << __FUNCTION__;
    //We are going to use our friend the cbInitializer to set some more callbacks
    FSMCallbackInitializer* cbInitializer = &(FSMCallbackInitializer::callbackInitializer());
    cbInitializer->setJoystickEventCallback();
    cbInitializer->setMonitorEventCallback();
}




// -----------------------------------------------------------------------------------------
//                                      STEP 3 
// -----------------------------------------------------------------------------------------
bool FSMRenderEnvironment::createContext() {
    LOG(TRACE) << __FUNCTION__;
    mContextWindow_ = nullptr; //Since this variable is just a temporary handle until I write a 
    //                         full-blown window-context class, it is fine to cheat and just initialize 
    //                         it right here

    //Step 3.1
    specifyContextHints();

    //Step 3.2 
    const int monitorCount = retrieveConnectedMonitors();
    LOG(INFO) << reportNumberOfConnectedMonitors(); //3.2b
    LOG(INFO) << reportPropertiesOfSelectedMonitor(); //3.2c

    //step 3.3
    //assignUserPointerToEachMonitor() const noexcept;

    //Step 3.4
    bool success = createContextAndWindow();

    //Step 3.5
    if (success) {
        setContextWindowCallbacks();
    }

    return success;
}

// --------------
//    STEP 3.1
// --------------
void FSMRenderEnvironment::specifyContextHints() noexcept {
    LOG(TRACE) << __FUNCTION__;
    //for now just do this:
    glfwDefaultWindowHints();

    constexpr int GL_VERSION_MAJOR = 4;
    constexpr int GL_VERSION_MINOR = 6;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_STEREO, GLFW_FALSE); //Setting this to 'true' causes a crash
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET); //Can be set to either GLFW_NO_RESET_NOTIFICATION or GLFW_LOSE_CONTEXT_ON_RESET
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    //This is stuff like force context to flush contents whenever released.
    //This application probably won't be releasing it's context all that much though
    glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR); 
   

}

// --------------
//    STEP 3.2
// --------------
int FSMRenderEnvironment::retrieveConnectedMonitors() noexcept {
    LOG(TRACE) << __FUNCTION__;
    int count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    if (count > 0) {
        for (int i = 0; i < count; i++) 
            mMonitors_.push_back(createMonitorHandle(monitors[i]));
    }
    
    return count;
}

// --------------
//    STEP 3.2b  [Optional]
// --------------
std::string FSMRenderEnvironment::reportNumberOfConnectedMonitors() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    constexpr const size_t NO_MONITORS = 0u;
    constexpr const size_t ONE_MONITOR = 1u;
    std::ostringstream report;
    size_t numMonitors = mMonitors_.size();
    if (NO_MONITORS == numMonitors)
        report << "This Application was unable to detect any valid monitors available for use!\n";
    else if (ONE_MONITOR == numMonitors)
        report << "Detected there is a single monitor available for use!\n";
    else
        report << "Detected there are " << numMonitors << " monitors available on this system!\n";
    return report.str();
}

// --------------
//    STEP 3.2c  [Optional]
// --------------
std::string FSMRenderEnvironment::reportPropertiesOfSelectedMonitor() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    
    //TODO  Change needed: Have FSMEngine track which monitor is primary by verifying 
    //                     and potentially updating which of the monitors is currently
    //                     the primary monitor when processing each connection/disconnection
    //                     event. Right now the FSMMonitors are just updating their internal
    //                     flag for their primary status each time they are queried with function
    //                    'isPrimary()'. This just feels error-prone and will probably lead to 
    //                     issues in the future if not fixed.
    //                     

    if (mTargetMonitor_ != NO_MONITOR_TARGETED) {
        if (mTargetMonitor_ < mMonitors_.size()) {  //Make sure targeted monitor exists
            auto monitorIter = mMonitors_.cbegin();
            
        }
    }
    
    std::ostringstream report;

    return report.str();
}


// --------------
//    STEP 3.3
// --------------
bool FSMRenderEnvironment::createContextAndWindow() {
    LOG(TRACE) << __FUNCTION__;
    mContextWindow_ = glfwCreateWindow(width, height, "My Amazing Application", NULL, NULL);
    if (mContextWindow_) {
        LOG(INFO) << "\nSuccessfully created a current context!\n";
        //Context must be made current here in order to load dependencies [GLAD Step 3]
        glfwMakeContextCurrent(mContextWindow_); 
    }
    //return static_cast<bool>(mContextWindow_);
    return (mContextWindow_ != nullptr);
}

// --------------
//    STEP 3.4
// --------------
void FSMRenderEnvironment::setContextWindowCallbacks() {
    LOG(TRACE) << __FUNCTION__;
    LOG(INFO) << "\n\n      WIndow Context Callbacks would be specified right here"
        "                 once they get implemented!!!\n\n";
}



// -----------------------------------------------------------------------------------------
//                                      STEP 4 
// -----------------------------------------------------------------------------------------
bool FSMRenderEnvironment::setupGLAD() {
    LOG(TRACE) << __FUNCTION__;

    //Step 4.1 [Debug Only]
#ifdef GLAD_DEBUG
    specifyDebugCallbacksWithGLAD();
#endif

    //Step 4.2
    bool success = loadOpenGLFunctions();


    //Step 4.3
    if (success)
        setInitialContextState();

    return success;
}


// --------------
//    STEP 4.1    [Debug Only]
// --------------
void FSMRenderEnvironment::specifyDebugCallbacksWithGLAD() noexcept {
    LOG(TRACE) << __FUNCTION__;
#ifdef ENABLE_GLAD_PRE_CALLBACK_LOGGING_MESSAGES_
        glad_set_pre_callback(customPreGLFuncCallCallback);
#endif 
#ifdef ENABLE_GLAD_POST_CALLBACK_LOGGING_MESSAGES_
        glad_set_post_callback(customPostGLFuncCallCallback);
#endif 
}

// --------------
//    STEP 4.2
// --------------
bool FSMRenderEnvironment::loadOpenGLFunctions() noexcept {
    LOG(TRACE) << __FUNCTION__;
    //Load OpenGL functions once window context has been set
    LOG(INFO) << "Loading Graphics Language Functions...\n";
    int success = gladLoadGL();
    if (success) 
        return true;
    return false;
}


// --------------
//    STEP 4.3
// --------------
void FSMRenderEnvironment::setInitialContextState() noexcept {
    LOG(TRACE) << __FUNCTION__;

    LOG(INFO) << "Setting Global Context State To Hardcoded [Soon To Be \"Loaded\"] values";

#ifdef USE_DEBUG_ 
    glEnable(GL_DEBUG_OUTPUT);
#ifdef FORCE_DEBUG_CONTEXT_APP_SYNCHRONIZATION_ 
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
#else 
    glDisable(GL_DEBUG_OUTPUT);
    glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif 

    std::ostringstream msg;
    msg << "Setting GL Context Callback Function so we can hear all of the context's complaints about\n"
        << "the plethora of deprecated behavior and improper usage we are soon to put it through.\n\n";
    LOG(INFO) << msg.str();
    //With 'glDebugMessageCallback()', the second parameter is for a user-assignable void pointer that will be
    //passed to the callback function each time it is called. We don't use this feature here, so it is left as nullptr
    glDebugMessageCallback(printGraphicsContextMessageCallback, nullptr); 
}



//-----------------------------------------------------------------------------------------
//                                     STEP 5 
//-----------------------------------------------------------------------------------------
bool FSMRenderEnvironment::checkForContextResetStrategy() noexcept {
    LOG(TRACE) << __FUNCTION__;
   
    bool contextResetAwarenessEnabled;
          
    //See also: https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glGet.xml
    //          https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetGraphicsResetStatus.xhtml 

    int resetStrategy = 0;    
    glGetIntegerv(GL_RESET_NOTIFICATION_STRATEGY, &resetStrategy);
         
    std::ostringstream resetStrategyReport;
    resetStrategyReport << "\n\n           [CONTEXT RESET STRATEGY NOTIFICATION]\n"
        << "Reset Strategy is................0x" << resetStrategy << "\n";
    if (resetStrategy == GL_NO_RESET_NOTIFICATION) {
        contextResetAwarenessEnabled = false;
        resetStrategyReport << "This corresponds to strategy.....NO_RESET_NOTIFICATION\n";
        resetStrategyReport << "\nALERT! Context will not attempt to reset and recover from\n"
            "any issues with the GL context\n"
            "  In other words:      ( GPU Crash ==> Application Crash )\n";
    }
    else if (resetStrategy == GL_LOSE_CONTEXT_ON_RESET) {
        contextResetAwarenessEnabled = true;
        resetStrategyReport << "This corresponds to strategy.....LOSE_CONTEXT_ON_RESET\n";
        resetStrategyReport << "\nThis Application is actively context-aware and will attempt\n"
            "to do a full self-recovery in the event of any context-loss\n"
            "educing event.\n";
    }
    else {
        contextResetAwarenessEnabled = false;
        resetStrategyReport << "\nWhoops! This is an unexpected return value for this function call!\n";
        LOG(ERROR) << "\nContext reported unexpected/invalid context reset strategy\n";
    }

    std::string resetStrat = resetStrategyReport.str();
    LOG(INFO) << resetStrat;
    LOG(DEBUG) << "\n[DEBUG]\n" << resetStrat << "\nDEBUG\n";


    return contextResetAwarenessEnabled;
}

std::string FSMRenderEnvironment::reportGLImplementationDetails() const noexcept {
    LOG(TRACE) << __FUNCTION__;

    std::ostringstream detailsOnGLImpl;
    const char* indent = "     ";
    //const char* sectionDelimiter = "~~~~~~~~~~~~--~~~~~~~~~~--~~~~~~~~~~--~~~~~~~~~~--~~~~~~~~~~--~~~~~~~~~~~~";


    detailsOnGLImpl << "\n\n" << getGLImplentationReportHeader() << "\n";
    
    detailsOnGLImpl << getImplementationsPreferencesForFormat(GL_RGB8); //GL_RGB9_E5
 
    return detailsOnGLImpl.str();
}




// -------------------------------------------------------------------------------------------------
//                                   Loop Logic Functions
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void FSMRenderEnvironment::setNextFrameClearColor(float time) noexcept {
    LOG(TRACE) << __FUNCTION__;
    float temp = std::min(1.0f, std::abs(mScreenClearColor_.r + std::sin(time + mScreenClearColor_.r)));
    mScreenClearColor_.r = mScreenClearColor_.g;
    mScreenClearColor_.g = mScreenClearColor_.b;
    mScreenClearColor_.b = temp;
    if (std::abs(mScreenClearColor_.r - mScreenClearColor_.g) < 0.01f)
        mScreenClearColor_.g += 0.4f;
    else if (std::abs(mScreenClearColor_.r - mScreenClearColor_.b) < 0.01f)
        mScreenClearColor_.b += 0.4f;
}


void FSMRenderEnvironment::logToInfoNextFrameClearColor() noexcept {
    std::ostringstream msg;
    msg << "New Background  {R: " << mScreenClearColor_.r << ", G: "
        << mScreenClearColor_.g << ", B: " << mScreenClearColor_.b << "}\n\n";
    LOG(INFO) << msg.str();
}


void FSMRenderEnvironment::doJoystickPrinterLoopLogic() noexcept {
     LOG_EVERY_N(60, TRACE) << __FUNCTION__;

     //If the pointer for some reason got deleted
     if (!mJoystickInputPrinter_)
         mJoystickInputPrinter_ = std::make_unique<JoystickStatePrinter>();

     GLFWwindow* mainRenderWindow = mContextWindow_;

     static unsigned long long iterationsSinceLastJoystickStatePrintingLastModified = 0ull;
     iterationsSinceLastJoystickStatePrintingLastModified++;
	
	//Perform Input Checking
	if (iterationsSinceLastJoystickStatePrintingLastModified > 9ull) { //Have a slight delay between when inputs are accepted

		if ((glfwGetKey(mainRenderWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ||
			(glfwGetKey(mainRenderWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)) {

			if (glfwGetKey(mainRenderWindow, GLFW_KEY_J) == GLFW_PRESS) {
				mJoystickStatePrintingEnabled_ = !mJoystickStatePrintingEnabled_;
                iterationsSinceLastJoystickStatePrintingLastModified = 0ull;
                std::ostringstream msg;
                msg << "Joystick Output Reporting Is Now ";
                if (mJoystickStatePrintingEnabled_)
                    msg << "Enabled\n";     
                else
                    msg << "Disabled\n";
                LOG(INFO) << msg.str();
			}

			if (mJoystickStatePrintingEnabled_) {
                //Check if need to reset stored default state for joystick (this would be necessary if
                //buttons were held down when stick connected)
				if (glfwGetKey(mainRenderWindow, GLFW_KEY_K) == GLFW_PRESS) {
                    std::ostringstream msg;
                    msg << "Reacquiring Default State for Joystick " << mJoystickInputPrinter_->id() << "\n";
                    LOG(INFO) << msg.str();
					mJoystickInputPrinter_->reaquireDefaultState();
					iterationsSinceLastJoystickStatePrintingLastModified = 0ull;
				}

				//Toggle Message Output Mode
				if (glfwGetKey(mainRenderWindow, GLFW_KEY_H)) {
					mJoystickInputPrinter_->toggleMessageOutputMode();
                    std::ostringstream msg;
                    msg << "\nJoystick Output Mode Set To: ";
                    if (mJoystickInputPrinter_->isUsingEchoInputPrintMode())
						msg <<  "ECHO INPUT\n";
					else
						msg << "ECHO FULL STATE\n";
                    LOG(INFO) << msg.str();
					iterationsSinceLastJoystickStatePrintingLastModified = 0ull;
				}

                if (glfwGetKey(mainRenderWindow, GLFW_KEY_I)) {
                    LOG(INFO) << mJoystickInputPrinter_->getJoystickInfo();
                }
                   

				if (glfwGetKey(mainRenderWindow, GLFW_KEY_SEMICOLON)) {
					//increment joystick ID to poll
					mJoystickInputPrinter_->nextJoystick();
                    std::ostringstream msg;
                    msg << "\nNow reporting input from Joystick ";
                    msg << mJoystickInputPrinter_->id();
                    LOG(INFO) << msg.str();
					iterationsSinceLastJoystickStatePrintingLastModified = 0ull;
				}

				if (glfwGetKey(mainRenderWindow, GLFW_KEY_APOSTROPHE)) {
					//decrement joystick ID to poll
					mJoystickInputPrinter_->previousJoystick();
                    std::ostringstream msg;
                    msg << "\nNow reporting input from Joystick ";
                    msg << mJoystickInputPrinter_->id();
                    LOG(INFO) << msg.str();
					iterationsSinceLastJoystickStatePrintingLastModified = 0ull;
				}
			}
		}
	}

	//Perform logic
	if (mJoystickStatePrintingEnabled_) {
		mJoystickInputPrinter_->printState();
	}


}


bool FSMRenderEnvironment::checkForContextReset() {
    LOG_EVERY_N(60, TRACE) << __FUNCTION__;
     
    bool resetDetected = false;

	GLenum contextResetStatus = glGetGraphicsResetStatus();
    if (contextResetStatus != GL_NO_ERROR) {
        std::ostringstream resetStatusMsg;
        switch (contextResetStatus) {
        //case GL_NO_ERROR:
        //    fprintf(MSGLOG, "\nglGetGraphicsResetStatus() returned enum GL_NO_ERROR!\n");
        //    resetDetected = false;
        //    break;
        case GL_GUILTY_CONTEXT_RESET:
            resetStatusMsg << "\nglGetGraphicsResetStatus() returned enum GL_GUILTY_CONTEXT_RESET!\nThis means this program caused the issue!\n";
            resetDetected = true;
            break;
        case GL_INNOCENT_CONTEXT_RESET:
            resetStatusMsg << "\nglGetGraphicsResetStatus() returned enum GL_INNOCENT_CONTEXT_RESET!\nThis means something else caused the reset!\n";
            resetDetected = true;
            break;
        case GL_UNKNOWN_CONTEXT_RESET:
            resetStatusMsg << "\nglGetGraphicsResetStatus() returned enum GL_UNKNOWN_CONTEXT_RESET!\n"
                "This means it is not known what happened or why, but what happened was bad!!!\n";
            resetDetected = true;
            break;
        default:
            resetStatusMsg << "\nglGetGraphicsResetStatus() returned unexpected enum (i.e. DEFAULT CASE WAS CHOSEN FOR SOME REASON!)\n";
            resetDetected = false;
            break;
        }
        LOG(INFO) << resetStatusMsg.str();
        LOG(ERROR) << resetStatusMsg.str();
    }

	return resetDetected;
 }