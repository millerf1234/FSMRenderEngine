
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

#include <string>
#include <exception>
#include "FSMException.h"
#include "FSMMonitor.h"

constexpr const int width = 1240;
constexpr const int height = 980;

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

    //Handles the proper release of resources used by this engine  
    ~FSMRenderEnvironment();


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
    static void getGLFWCompiletimeVersion(int& compileVersionMajor, int& compileVersionMinor, int& compileVersionRevision);
    //Returns a string that looks like: "Compiled against GLFW 'major'.'minor'.'revision'", with the
    //'major', 'minor' and 'revision' replaced with their corresponding integer values. 
    static std::string getGLFWCompiletimeVersionString();
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



    /////////////////////////////////////////////////////////////////////////////////////////
    /////////////////                Implementation Functions
    /////////////////////////////////////////////////////////////////////////////////////////

    //-------------------------------------
    //  Implementation functions called by the constructor
    //-------------------------------------

    //DELEGATING CONSTRUCTOR
    FSMRenderEnvironment(bool); //Use this delegating constructor to ensure a complete type
    //                          //exists before any initialization operations which might
    //                          //throw/fail are performed.
        
    
    //STEP 1
    bool initializeGLFW(); 
    void setGlobalGLFWInvariants() noexcept;  //step 1.1
    void setPreGLFWInitCallbacks();           //step 1.2
    bool callTheGLFWInitFunc();               //step 1.3
    void setPostGLFWInitCallbacks();          //step 1.4

    //STEP 2
    bool createContext();
    void specifyContextHints() noexcept;      //step 2.1
    bool createContextAndWindow();            //step 2.2
    void setContextWindowCallbacks();         //step 2.3


    //STEP 3
    bool setupGLAD();
    void specifyDebugCallbacksWithGLAD();     //step 3.1
    bool loadOpenGLFunctions();                   //step 3.2

    
    //STEP 4
    void retrieveConnectedMonitors();

};



#endif //FSM_RENDER_ENGINE_H_
