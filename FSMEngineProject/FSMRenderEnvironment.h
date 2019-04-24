
// Render Environment 
// Used to create a Render Engine
//
//  More documentation will be written once this class and the other parts of the
//  engine get enough pieces functional and put into place
//
// File created on March 9, 2019

#pragma once

#ifndef FSM_RENDER_ENGINE_H_
#define FSM_RENDER_ENGINE_H_

#include <cmath>    //floor func
#include <string>
#include <string_view>
#include <exception>
#include <system_error>                 //std::error_code
#include <vector>
#include <list>  //<forward_list>
//#include <optional>  //Turns out including <optional> is optional
#include <filesystem>
#include "FSMException.h"
#include "FSMMonitorHandle.h"
#include "GraphicsLanguage.h"

#include "JoystickStatePrinter.h"

constexpr const int width = 1920;
constexpr const int height = 9 * (width / 16);

static constexpr const int NO_MONITOR_TARGETED = -1;

class FSMRenderEnvironment final {
public:
    //             /////////////////////////////////////////////////////////////////
    //             //////////        Initialization and Destruction        /////////
    //             /////////////////////////////////////////////////////////////////

    //------------------------------
    //    RAII and GLFW
    //  GLFW requires the function glfwInit() to be called before it will be of much 
    //  use. GLFW also then expects the function glfwTerminate() to be called at the end
    //  of a programs operation so it can do proper clean-up after itself. To ensure
    //  these two functions always get called as required, the idiom known as Resource
    //  Acquisition Is Initialization [RAII] is followed. Thus this class ties its own 
    //  initialization and destruction directly to these function calls. This way it 
    //  is impossible for one to be missed. Thus don't call these functions directly
    //  outside of this class.
    //-----------------------------

    //Constructor is responsible for initializing the GLFW library and performing 
    //some very basic set-up. Upon completion of constructor, it should be possible
    //to query this object for available monitors and select one to run with. 
    //
    //[Implementation detail: Because it is necessary to have a valid context to 
    // load in the GL functions and perform other necessary basic tasks, this constructor
    // actually loads in a hidden window in order to create a valid context. This makes 
    // all of the other OpenGL initialization steps possible. When a window is then
    //created through this class, instead of using the GLFW function to create a whole
    //new window, this 
    FSMRenderEnvironment();

    //FSMRenderEnvironment(/* Take in a collection of Settings/data/assets to load */);

    //Handles the proper release of resources used by this engine  
    ~FSMRenderEnvironment() noexcept;


    //             /////////////////////////////////////////////////////////////////
    //             //////////               Public Interface               /////////
    //             /////////////////////////////////////////////////////////////////

    //Loop function that should be called within a loop once RenderEnvironment has 
    //successfully be created.
    // [ Things to do here:
    //     --  Check callback queues for updates,
    //     --  call glfwProcessEvents,
    //     --  [possibly] have this swap buffers/cycle frames or not
    void handleEvents();

    
    void doMonitorSelectionLoop();

    FSMMonitor getTargetMonitor();



    //                    /////////////////////////////////////////////////////////////////
    //                    //////////         Environment Query Functions          /////////
    //                    /////////////////////////////////////////////////////////////////

    //Sets parameters to match the version of OpenGL that contexts created through this class will use. This
    //will match the version of OpenGL that gets linked into the code at compile time as determined by the 3rd 
    //party library 'glad'. To change versions of OpenGL that will be used, simply replace 'glad.h' and 'glad.c'
    //with their respective configurations for that version. [This class was written targeting OpenGL 4.6] 
    void getGLVersion(int& versionMajor, int& versionMinor, bool& usesCompatMode) const noexcept;
    //Returns the OpenGL version major that contexts created through this object will use.  This value is set 
    //to match the version of OpenGL that gets linked into the code at compile time determined by the 3rd 
    //party library 'glad'.
    int getGLVersionMajor() const noexcept;
    //Returns the OpenGL version minor that contexts created through this object will use. This value is set 
    //to match the version of OpenGL that gets linked into the code at compile time determined by the 3rd 
    //party library 'glad'.
    int getGLVersionMinor() const noexcept;
    //Returns a string outlining the version of OpenGL that will be used when creating GL Contexts. These
    //values can only be specified as parameters during object construction. The returned string will 
    //look like (with the sections in-between the single quotes being replaced by their respective
    //integer values): 
    //    "OpenGL 'versionMajor'.'versionMinor' Core"                   
    std::string getGLVersionString() const noexcept;


    //Retrieves the Major, Minor and Revision numbers corresponding to the version of GLFW used 
    //during compile time. These values can be compared with the shared-library GLFW version loaded
    //at runtime (which could be helpful for debugging).
    static void getGLFWCompiletimeVersion(int& compileVersionMajor, int& compileVersionMinor, int& compileVersionRevision) noexcept;
    //Returns a string that looks like: "Compiled against GLFW 'major'.'minor'.'revision'", with the
    //'major', 'minor' and 'revision' replaced with their corresponding integer values. 
    static std::string getGLFWCompiletimeVersionString() noexcept;
    //Retrieves the Major, Minor and Revision numbers corresponding to the version of GLFW linked with 
    //during run time. These values can be compared with the compile-time GLFW version (which could be 
    //helpful for debugging purposes).
    void getGLFWRuntimeVersion(int& runtimeVersionMajor, int& runtimeVersionMinor, int& runtimeVersionRevision) const noexcept;
    //Returns a string that looks like: "Running against GLFW 'major'.'minor'.'revision'", with the
    //'major', 'minor' and 'revision' replaced with the corresponding integer values retrieved from
    //the GLFW library which gets linked at run-time. 
    std::string getGLFWRuntimeVersionString() const noexcept;



    //No Copying or Moving allowed
    FSMRenderEnvironment(const FSMRenderEnvironment&) = delete;
    FSMRenderEnvironment(FSMRenderEnvironment&&) = delete;
    FSMRenderEnvironment& operator=(const FSMRenderEnvironment&) = delete;
    FSMRenderEnvironment& operator=(FSMRenderEnvironment&&) = delete;


private:
     
     
     
    bool mGLFWIsInit_;
    struct GLFWwindow* mContextWindow_;

    int mTargetMonitor_;
    std::list<FSMEngineInternal::FSMMonitorHandle*> mMonitors_;

     //For Joystick Debug
    bool mJoystickStatePrintingEnabled_;
    std::unique_ptr<JoystickStatePrinter> mJoystickInputPrinter_;
    //For context reset (should be enabled at construction and will disable 
    //itself after determining what reset strategy is to be used.
    bool mContextResetAwareness_;

    glm::vec4 mScreenClearColor_;

    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////                Implementation Functions
    /////////////////////////////////////////////////////////////////////////////////////////

    //-------------------------------------
    //  Implementation functions called by the constructor
    //-------------------------------------

    //DELEGATING CONSTRUCTOR
    explicit FSMRenderEnvironment(bool) noexcept; //Use this delegating constructor to ensure a complete type
    //                                   //exists before any initialization operations which might
    //                                   //fail and throw are performed. This helps ensure correct 
    //                                   //resource allocation and deallocation is able to occur.
                                                                                
                                                                                                  
    //STEP 1                                                                    
    bool loadEngineSettings();                                                                    
    bool locateSettingsFile(std::error_code& ec);                    //step 1.1 
    bool generateSettingsFile(std::error_code& ec);                  //step 1.2b [Only if needed] 
    void parseSettingsFile();                                        //step 1.2a 
    bool verifySettings() const noexcept;                            //step 1.3                   
                                                                                
                                                                                                  
                                                                                
    //STEP 2                                                                                      
    bool initializeGLFW();                                                      
    void setGlobalGLFWInvariants() noexcept;                         //step 2.1                   
    void setPreGLFWInitCallbacks();                                  //step 2.2 
    bool callTheGLFWInitFunc();                                      //step 2.3                   
    void setPostGLFWInitCallbacks();                                 //step 2.4 
                                                                                                  
                                                                                
    //STEP 3                                                                                      
    bool createContext();                                                       
    void specifyContextHints() noexcept;                             //step 3.1                   
    int retrieveConnectedMonitors() noexcept;                        //step 3.2 
    std::string reportNumberOfConnectedMonitors() const noexcept;    //step 3.2b [Optional]       
    std::string reportPropertiesOfSelectedMonitor() const noexcept;  //step 3.2c [Optional]
    bool createContextAndWindow();                                   //step 3.3                   
    void setContextWindowCallbacks();                                //step 3.4 
                                                                                                  
                                                                                
    //STEP 4                                                                                      
    bool setupGLAD();                                                           
    void specifyDebugCallbacksWithGLAD() noexcept;                   //step 4.1 [Debug Only]      
    bool loadOpenGLFunctions() noexcept;                             //step 4.2 
    void setInitialContextState() noexcept ;                         //Step 4.3                   
                                                                                
                                                                                                  
    //STEP 5                                                                    
    bool checkForContextResetStrategy() noexcept;                                                 
                                                                                
    //WARNING! reportGLImplementationDetails has an Incomplete Implementation currently and       
    //        doesn't print out much useful information                         
    std::string reportGLImplementationDetails() const noexcept;   //Optional and not very useful  
                                                                                
                                                                                                  

    //-------------------------------------
    //--    Upkeep Loop Functions
    //-------------------------------------

    //RENDER
    void setNextFrameClearColor(float time) noexcept;
    void logToInfoNextFrameClearColor() noexcept;

    //LOGIC
    void doJoystickPrinterLoopLogic() noexcept; //Function to be called as part of each frame
   
    bool checkForContextReset(); 
};



#endif //FSM_RENDER_ENGINE_H_
