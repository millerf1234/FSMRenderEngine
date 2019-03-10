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

#include "FSMRenderEnvironment.h"
#include "UniversalIncludes.h"
#include "GraphicsLanguageFramework.h"
#include "FSMCallbackInitializer.h"

using namespace FSMEngineInternal;

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
FSMRenderEnvironment::FSMRenderEnvironment(bool dummyParameter) {
    (void)dummyParameter;//We don't care about the parameter, it exists just to identify this constructor  
    mGLFWIsInit_ = false;

}

FSMRenderEnvironment::FSMRenderEnvironment() : FSMRenderEnvironment(true) {
    //Because the delegating constructor was used, we know that FSMRenderEnvironment 
    //is now a complete type. Thus we don't need to worry about any further actions
    //we take here in this constructor causing failure which could have resulted in 
    //the destructor not getting called. 

    //Thus we can safely start doing the actual initialization here and throw any
    //exceptions if we need to

    //STEP 1
    if (!initializeGLFW()) {
        throw FSMException("An issue was encountered while initializing the "
            "Graphics Language Framework [GLFW] Library!\n");
    }

    LOG(INFO) << " [step 1]    GLFW Library has initialized! GLFW version: " << getGLFWRuntimeVersionString() << "\n";

    //STEP 2
    if (!createContext()) {
        throw FSMException("An issue was encountered while creating the GLFW window and OpenGL\n"
            "context! These are kinda sorta vital for this application to function properly, so \n"
            "the fact they failed means this whole application is probably going to have to crash!\n\n");
    }

    LOG(INFO) << "\n  [step 2]      Graphics Language Context Created!\n";

 
    //STEP 3
    if (!setupGLAD()) {
        throw FSMException("THe issue occurred setting up GLAD!\n");
    }

    LOG(INFO) << "  [step 3]     GLAD LOADED OPENGL SUCCESFULLY !~!!!  OMG ALMOST THERE !!!!\n";

    //STEP 4
    retrieveConnectedMonitors(); //this step can't fail...
}


FSMRenderEnvironment::~FSMRenderEnvironment() {
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



void FSMRenderEnvironment::doMonitorSelectionLoop() {

    // Define the viewport dimensions
    glViewport(0, 0, width, height);

    // Game loop
    while (!glfwWindowShouldClose(mContextWindow_)) {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(mContextWindow_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(mContextWindow_, true);
        }

        // Swap the screen buffers
        glfwSwapBuffers(mContextWindow_);
    }


}



void FSMRenderEnvironment::getGLVersion(int& versionMajor, int& versionMinor, bool& usesCompatMode) const noexcept {
    versionMajor = GLVersion.major;
    versionMinor = GLVersion.minor;
    usesCompatMode = false; //Should be safe to assume this code never will run in compatibility mode....
}

int FSMRenderEnvironment::getGLVersionMajor() const noexcept {
    return GLVersion.major;
}

int FSMRenderEnvironment::getGLVersionMinor() const noexcept {
    return GLVersion.minor;
}

std::string FSMRenderEnvironment::getGLVersionString() const noexcept {
    std::ostringstream version;
    version << "OpenGL Version " << std::to_string(GLVersion.major) << ".";
    version << std::to_string(GLVersion.minor) << " Core";
    return version.str();
}

void FSMRenderEnvironment::getGLFWCompiletimeVersion(int& compileVersionMajor,
    int& compileVersionMinor, int& compileVersionRevision) {
    compileVersionMajor = GLFW_VERSION_MAJOR;
    compileVersionMinor = GLFW_VERSION_MINOR;
    compileVersionRevision = GLFW_VERSION_REVISION;
}

std::string FSMRenderEnvironment::getGLFWCompiletimeVersionString() {
    return std::string(glfwGetVersionString()); //GLFW returns a compile-time generated string
}


void FSMRenderEnvironment::getGLFWRuntimeVersion(int& runtimeVersionMajor,
    int& runtimeVersionMinor, int& runtimeVersionRevision) const noexcept {
    glfwGetVersion(&runtimeVersionMajor, &runtimeVersionMinor, &runtimeVersionRevision);
}

std::string FSMRenderEnvironment::getGLFWRuntimeVersionString() const noexcept {
    int maj, min, rev;
    glfwGetVersion(&maj, &min, &rev);
    std::ostringstream version;
    version << "GLFW " << maj << "." << min << "." << rev;
    return version.str();
}

///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////     IMPLEMENTATION FUNCTIONS     /////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------------------
//                                     STEP 1 
//-----------------------------------------------------------------------------------------
bool FSMRenderEnvironment::initializeGLFW() {
    //Step 1.1
    setGlobalGLFWInvariants();
    
    //Step 1.2
    setPreGLFWInitCallbacks();

    //Step 1.3
    bool success = callTheGLFWInitFunc();
   
    //If Step 1.3 success, then Step 1.4
    if (success) {
        setPostGLFWInitCallbacks();
    }
    return success;
}

//--------------
//  STEP 1.1
//--------------
//This function must be called before glfwInit() is called
void FSMRenderEnvironment::setGlobalGLFWInvariants() noexcept {
    //Treat joystick hats as separate from Joystick buttons
    glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);

    //There are 2 other MacOS Cocoa-specific hints, but seeing as Apple is deprecating 
    //OpenGL it hardly feels worth doing more than just mentioning them here...
}

//--------------
//  STEP 1.2
//--------------
void FSMRenderEnvironment::setPreGLFWInitCallbacks() {
    //The callback initializer is a singleton object. We can get a pointer to it though to 
    //interact with it for the course of this function. 
    FSMCallbackInitializer* cbInitializer = &(FSMCallbackInitializer::callbackInitializer());

    cbInitializer->setGLFWErrorCallback();
}

//--------------
//  STEP 1.3
//--------------
bool FSMRenderEnvironment::callTheGLFWInitFunc() {
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

//--------------
//  STEP 1.4
//--------------
void FSMRenderEnvironment::setPostGLFWInitCallbacks() {
    //We are going to use our friend the cbInitializer to set some more callbacks
    FSMCallbackInitializer* cbInitializer = &(FSMCallbackInitializer::callbackInitializer());
    cbInitializer->setJoystickEventCallback();
    cbInitializer->setMonitorEventCallback();
}




//-----------------------------------------------------------------------------------------
//                                     STEP 2 
//-----------------------------------------------------------------------------------------
bool FSMRenderEnvironment::createContext() {
    mContextWindow_ = nullptr; //Since this variable is just a temporary handle until I write a 
    //                         full-blown window-context class, it is fine to cheat and just initialize 
    //                         it right here

    //Step 2.1
    specifyContextHints();

    //Step 2.2
    bool success = createContextAndWindow();

    //Step 2.3
    if (success) {
        setContextWindowCallbacks();
    }

    return success;
}

//--------------
//  STEP 2.1
//--------------
void FSMRenderEnvironment::specifyContextHints() noexcept {

    //for now just do this:
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, (getGLVersionMajor() != 0) ? getGLVersionMajor() : 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, (getGLVersionMinor() != 0) ? getGLVersionMinor() : 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET); //Can be set to either GLFW_NO_RESET_NOTIFICATION or GLFW_LOSE_CONTEXT_ON_RESET
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);


}

//--------------
//  STEP 2.2
//--------------
bool FSMRenderEnvironment::createContextAndWindow() {
    mContextWindow_ = glfwCreateWindow(width, height, "My Title", NULL, NULL);
    if (mContextWindow_) {
        LOG(INFO) << "\nSuccessfully created a current context!\n";
        //Context must be made current here in order to load dependencies [GLAD Step 3]
        glfwMakeContextCurrent(mContextWindow_); 
    }
    //return static_cast<bool>(mContextWindow_);
    return (mContextWindow_ != nullptr);
}

//--------------
//  STEP 2.3
//--------------
void FSMRenderEnvironment::setContextWindowCallbacks() {

    LOG(INFO) << "\n\n      WIndow Context Callbacks would be specified right here"
        "                 once they get implemented!!!\n\n";
}



//-----------------------------------------------------------------------------------------
//                                     STEP 3 
//-----------------------------------------------------------------------------------------
bool FSMRenderEnvironment::setupGLAD() {
    
    //Step 3.1
    specifyDebugCallbacksWithGLAD();
    
    //Step 3.2
    bool success = loadOpenGLFunctions();


    return success;
}


//--------------
//  STEP 3.1
//--------------
void FSMRenderEnvironment::specifyDebugCallbacksWithGLAD() {

    LOG(INFO) << "\n\n    GLAD CALLBACKS WOULD BE SPECIFIED HERE ONCE THINGS ARE FURTHER ALONG!\n\n\n\n";
    /* example from glad's examples
//#ifdef GLAD_DEBUG
//void pre_gl_call(const char *name, void *funcptr, int len_args, ...) {
//    printf("Calling: %s (%d arguments)\n", name, len_args);
//}
//#endif
//*/
}

//--------------
//  STEP 3.2
//--------------
bool FSMRenderEnvironment::loadOpenGLFunctions() {
    //Load OpenGL functions once window context has been set
    LOG(INFO) << "!Loading Graphics Language Functions...\n";
    int success = gladLoadGL();
    if (success) 
        return true;
    return false;
}


//-----------------------------------------------------------------------------------------
//                                     STEP 4 
//-----------------------------------------------------------------------------------------
void FSMRenderEnvironment::retrieveConnectedMonitors() {

    LOG(INFO) << "\nHere is where a list of already connected monitors would\n"
        "be loaded, which then would allow for the\n"
        "Application to call a function of FSMRenderEnvironment to enumerate\n"
        "These monitors, which then a user could select an appropriate choice from...\n\n"
        "  [or something like that]\n\n\n\n\n";
        
    return;
}