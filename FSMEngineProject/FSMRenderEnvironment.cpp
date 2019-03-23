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
#include "GraphicsLanguageFramework.h"
#include "FSMCallbackInitializer.h"

using namespace FSMEngineInternal;
using namespace FSMEngineDefaultInitializationConstants;

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
    //we take here in this constructor throwing exceptions which otherwise would result in 
    //the destructor not getting called. 

    //Thus we can safely start doing the actual initialization here and throw any
    //exceptions if we need to

    LOG(INFO) << "\n [step 1]   Loading settings from initialization file!\n";

    //STEP 1
    if (!loadSettings()) {
        throw FSMException("Unable to load settings from the '.FIni' file!\n");
    } 

    //STEP 2
    if (!initializeGLFW()) {
        throw FSMException("An issue was encountered while initializing the "
            "Graphics Language Framework [GLFW] Library!\n");
    }

    LOG(INFO) << "\n [step 2]    GLFW Library has initialized! GLFW version: " << getGLFWRuntimeVersionString() << "\n";

    //STEP 3
    if (!createContext()) {
        throw FSMException("An issue was encountered while creating the GLFW window and OpenGL\n"
            "context! These are kinda sorta vital for this application to function properly, so \n"
            "the fact they failed means this whole application is probably going to have to crash!\n\n");
    }

    LOG(INFO) << "\n  [step 3]      Graphics Language Context Created!\n";

 
    //STEP 4
    if (!setupGLAD()) {
        throw FSMException("THe issue occurred setting up GLAD!\n");
    }

    LOG(INFO) << "  [step 4]     GLAD LOADED OPENGL SUCCESFULLY !~!!!  OMG ALMOST THERE !!!!\n";

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
        // Clear the color-buffer
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
bool FSMRenderEnvironment::loadSettings() {
    //C++20 
#if __has_cpp_attribute(likely)  
#define LIKELY [[likely]]
#define UNLIKELY [[unlikely]]
#else 
#define LIKELY     
#define UNLIKELY     
#endif 
   
    //Step 1.1
    std::error_code ecFind{};
    bool settingsFileFound = locateSettingsFile(ecFind);

    if (settingsFileFound) LIKELY {
        parseSettingsFile();                                              //Step 1.2
        return true;
    }

    else UNLIKELY{
        if (!ecFind) LIKELY {
            std::error_code ecCreate{};
            bool settingsFileCreated = generateSettingsFile(ecCreate);   //Step 1.1b
            if (settingsFileCreated && (!ecCreate)) LIKELY {
                parseSettingsFile();                                     //Step 1.2
                return true;
            }
            else if (ecCreate) UNLIKELY {
                 LOG(ERROR) << "\nError reported by Operating System while attempting\n"
                    << "to generate configuration file (" << /*SETTINGS_FILE_NAME*/ "UNNAMED" << ")!\n"
                    << "Operating System Error Message:\n\t"
                    << ecCreate.message() << std::endl;
            return false;
            }
            else UNLIKELY {
                return false;
            }
        }
        else /*if (ecFind) */ UNLIKELY { //
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

#undef LIKELY
#undef UNLIKELY
}




//--------------
//  STEP 1.1
//--------------
//Parameter 'ec' will be set to any errors which may be reported by the OS when calling functions from 'std::filesystem'
bool FSMRenderEnvironment::locateSettingsFile(std::error_code& ec) {
    auto currentPath = std::filesystem::current_path(ec);
    if (ec)
        return false;
    else {
#if 1
        return true;
#elif ready
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


//--------------
//  STEP 1.2  [OPTION A: File Exists]
//--------------
void FSMRenderEnvironment::parseSettingsFile() {
    LOG(INFO) << "\nSettings file found! Parsing now...\n";
    //return true;
}



//--------------
//  STEP 1.2  [OPTION B: FILE DOES NOT EXIST]  (yet)
//--------------
bool FSMRenderEnvironment::generateSettingsFile(std::error_code& ec) {
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


//-----------------------------------------------------------------------------------------
//                                     STEP 2 
//-----------------------------------------------------------------------------------------
bool FSMRenderEnvironment::initializeGLFW() {
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

//--------------
//  STEP 2.1
//--------------
//This function must be called before glfwInit() is called
void FSMRenderEnvironment::setGlobalGLFWInvariants() noexcept {
    //Treat joystick hats as separate from Joystick buttons
    glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);

    //There are 2 other MacOS Cocoa-specific hints, but seeing as Apple is deprecating 
    //OpenGL it hardly feels worth doing more than just mentioning them here...
}

//--------------
//  STEP 2.2
//--------------
void FSMRenderEnvironment::setPreGLFWInitCallbacks() {
    //The callback initializer is a singleton object. We can get a pointer to it though to 
    //interact with it for the course of this function. 
    FSMCallbackInitializer* cbInitializer = &(FSMCallbackInitializer::callbackInitializer());

    cbInitializer->setGLFWErrorCallback();
}

//--------------
//  STEP 2.3
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
//  STEP 2.4
//--------------
void FSMRenderEnvironment::setPostGLFWInitCallbacks() {
    //We are going to use our friend the cbInitializer to set some more callbacks
    FSMCallbackInitializer* cbInitializer = &(FSMCallbackInitializer::callbackInitializer());
    cbInitializer->setJoystickEventCallback();
    cbInitializer->setMonitorEventCallback();
}




//-----------------------------------------------------------------------------------------
//                                     STEP 3 
//-----------------------------------------------------------------------------------------
bool FSMRenderEnvironment::createContext() {
    mContextWindow_ = nullptr; //Since this variable is just a temporary handle until I write a 
    //                         full-blown window-context class, it is fine to cheat and just initialize 
    //                         it right here

    //Step 3.1
    specifyContextHints();

    //Step 3.2 
    retrieveConnectedMonitors();

    //Step 3.3
    bool success = createContextAndWindow();

    //Step 3.3
    if (success) {
        setContextWindowCallbacks();
    }

    return success;
}

//--------------
//  STEP 3.1
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
//  STEP 3.2
//--------------
void FSMRenderEnvironment::retrieveConnectedMonitors() {

   LOG(INFO) << "\nHere is where a list of already connected monitors would\n"
        "be loaded, which then would allow for the\n"
        "Application to call a function of FSMRenderEnvironment to enumerate\n"
        "These monitors, which then a user could select an appropriate choice from...\n\n"
        "  [or something like that]\n\n\n\n\n";
        
    return;
}

//--------------
//  STEP 3.3
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
//  STEP 3.4
//--------------
void FSMRenderEnvironment::setContextWindowCallbacks() {

    LOG(INFO) << "\n\n      WIndow Context Callbacks would be specified right here"
        "                 once they get implemented!!!\n\n";
}



//-----------------------------------------------------------------------------------------
//                                     STEP 4 
//-----------------------------------------------------------------------------------------
bool FSMRenderEnvironment::setupGLAD() {
    
    //Step 4.1
    specifyDebugCallbacksWithGLAD();
    
    //Step 4.2
    bool success = loadOpenGLFunctions();


    return success;
}


//--------------
//  STEP 4.1
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
//  STEP 4.2
//--------------
bool FSMRenderEnvironment::loadOpenGLFunctions() {
    //Load OpenGL functions once window context has been set
    LOG(INFO) << "!Loading Graphics Language Functions...\n";
    int success = gladLoadGL();
    if (success) 
        return true;
    return false;
}



//This was moved to be step 3.2
////-----------------------------------------------------------------------------------------
////                                     STEP 5 
////-----------------------------------------------------------------------------------------
//void FSMRenderEnvironment::retrieveConnectedMonitors() {
//
//   LOG(INFO) << "\nHere is where a list of already connected monitors would\n"
//        "be loaded, which then would allow for the\n"
//        "Application to call a function of FSMRenderEnvironment to enumerate\n"
//        "These monitors, which then a user could select an appropriate choice from...\n\n"
//        "  [or something like that]\n\n\n\n\n";
//        
//    return;
//}