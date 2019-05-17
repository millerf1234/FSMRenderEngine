//File:      Application.cpp
//
//  Programmer:           Forrest Miller
//  Created Date:         January 10, 2019
//                [ Still Under Development as of May 17, 2019 ]


#include <istream>  //For std::cin.get()   to keep console open
#include "Application.h"

#include "GraphicsLanguage.h"
#include "FSMException.h"
#include "FSMInitConfig.h"

//The macro 'ALLOW_ELCC_CONFIGURATION' must be defined before this header can be included
#define ALLOW_ELCC_CONFIGURATION  1
#include "EasyLogConfiguration.h"

#include "AsciiTextFile.h"






//////////////////////////////////
//  Expected Asset Directories  //
//////////////////////////////////

const std::filesystem::path Application::RELATIVE_PATH_TO_ASSET_DATA_DIRECTORY = "\\..\\AssetData";
const std::filesystem::path Application::RELATIVE_PATH_TO_TEXTURES_DIRECTORY = "\\..\\AssetData\\Textures";
const std::filesystem::path Application::RELATIVE_PATH_TO_SHADERS_DIRECTORY = "\\..\\AssetData\\Shaders";
const std::filesystem::path Application::RELATIVE_PATH_TO_FONTS_DIRECTORY = "\\..\\AssetData\\Fonts";
const std::filesystem::path Application::RELATIVE_PATH_TO_AUDIO_DIRECTORY = "\\..\\AssetData\\Audio";
const std::filesystem::path Application::RELATIVE_PATH_TO_SETTINGS_DIRECTORY = "\\..\\AssetData\\Settings";




//////////////////////
//  Expected Files  //
//////////////////////

const std::filesystem::path Application::RELATIVE_PATH_TO_INIT_CONFIG_FILE = "\\..\\AssetData\\Settings\\InitConfig.json";




//////////////////////////////////////////
//  Implementation Function Prototypes  //           (The definitions for these functions can
//////////////////////////////////////////             be found towards the end of this file)

//Checks to see if there is an existing directory at the specified location.
//Function has been written to accept both Absolute and Relative (see note)
//filepaths.
//  [NOTE]: A relative filepath must be able to start in the directory containing  
//          this executable's file and though appending itself give the path to
//          the target directory. Relative filepaths may begin with either dots or
//          with the path delimiting character [i.e. a '\' or '/' depending on 
//          the OS/Environment] 
bool checkIfIsPathToDirectory(const std::filesystem::path&) noexcept;
//Please do not pass this function random bogus nonsense so that weird
//directories are not created.
//NOTE: It is recommended to only call this function after having first checked 
//the filepath using the related function 'checkIfIsPathToFile()' to make sure a 
//file doesn't already exist at this location.
void createDirectory(const std::filesystem::path&) noexcept;

//Checks to see if there is an existing file at the specified location.
//Function has been written to accept both Absolute and Relative (see note)
//filepaths.
//  [NOTE]: A relative filepath must be able to start in the directory containing  
//          this executable's file and though appending itself give the path to
//          the target file's location. Relative filepaths may begin with either
//          dots or with the path delimiting character [i.e. will be a '\' or '/'
//          depending on the Platform/OS/Environment]  
bool checkIfIsPathToFile(const std::filesystem::path&) noexcept;
//Please do not pass to this function a random path that goes somewhere weird.
//NOTE: It is recommended to only call this function after having first checked 
//the filepath using the related function 'checkIfIsPathToFile()' to make sure a 
//file doesn't already exist at this location.
void createJSONFile(const std::filesystem::path& filepath, std::string_view fileText) noexcept;


//Utility function for usage with the above implementation functions. Handles 
//converting several possible different path-input formats into a valid, absolute
//path.
std::filesystem::path getAbsolutePath(const std::filesystem::path&) noexcept;




///////////////////
//  Constructor  //
/////////////////// 

// Implementation Description: 
//       Most of the heavy initialization tasks performed by the Application
//       are saved for until after the constructor has returned successfully.
//
//       The tasks performed by the constructor are essentially as follows
//             First task: 
//                         Get the EasyLogger++ logging library initialized. 
//                         Be aware that in some configurations it is possible
//                         for any failures encountered while initializing logging
//                         to throw exceptions. Ideally these should always be caught
//                         without ever propogating beyond the constructor.
//
//             Second task:
//                         Print out some basic information regarding the Application
//                         version, Compilation/Build details, environment details, etc..
//
//              Third task:
//                         Locate the existance of the several main directories used for
//                         storing assets. If any of these directories are unlocateable,
//                         construct a directory. Doing so will allow a guarentee for each
//                         asset directory be guarenteed to exist for lifetime of Application.
//
//       and that is pretty much all there is to it 
//                         
Application::Application(int argc, char ** argv) : mRenderEnvironment_(nullptr) {

    //WAIT! Don't move the 'Initializing Application' message up here*!!!          *Times I have tried to
    //How the heck is that message going to get logged if  message                   do so counter:  4
    //logging hasn't been set up yet?
       
    if (!setupMessageLogs(argc, argv)) {
        std::exit(EXIT_FAILURE);
    } 
    LOG(TRACE) << __FUNCTION__;
    LOG(INFO) << "Initializing Application...";

    logApplicationBuildAndConfigurationDetails();

    validateAssetDirectories();
}




//////////////////
//  Destructor  //
//////////////////

Application::~Application() noexcept {
    LOG(TRACE) << __FUNCTION__;
    el::Loggers::flushAll();
}




void Application::launch() {
    LOG(TRACE) << __FUNCTION__;

    FSMInitConfig config = getInitConfig();

    LOG(INFO) << "Initiating Render Environment";//"Inchoating The Render Environment...";
    if (!createRenderEnvironment()) {
        LOG(ERROR) << "Failed to construct a render environment for this application.\n"
            "Unfortunatly this means it is time to crash...\n"
            "      [Press Enter To Terminate]   ";
        std::cin.get();
        std::exit(EXIT_FAILURE);
    }

    LOG(INFO) << "\nApplication launched....\n";
    mRenderEnvironment_->doMonitorSelectionLoop();
}




bool Application::setupMessageLogs(int argc, char** argv) {
    //LOG(TRACE) << __FUNCTION__; //This function doesn't get traced until logs are setup
    //Set up EasyLogger++ library
    try {
        initializeEasyLogger(argc, argv);
        if (configureEasyLogger()) {  //If there were no issues with setting up log files
            LOG(TRACE) << __FUNCTION__;
            return true;
        }
        else { //There was something that went wrong while setting up logs
            printf("Initializing Application...\n");
            printf("\n                                )=   [UH-OH!]   =(  ");
            printf("\n     ISSUE -- Unable to Configure Message Logs!\n"
                "Details: An issue has been encountered during the early initialization phase of\n"
                "this process. The issue occurred while this process was interfacing with the\n"
                "Operating System during its attempt to properly designate, allocate and acquire\n"
                "the resources necessary to allow for full message-logging functionality!\n");
#ifdef FECLFINAE
            //I wrote this message late at night and I have no idea what I am talking about here
            std::ostringstream feclfinae_msg;
            feclfinae_msg << "\n     SYSTEM REPORTED ISSUE        \n"
                << "Unable to provide full logging functionality due to an issue which\n"
                << "occurred while communicating with the OS!\n"
                << "\n     SOLUTION \n"
                << "Luckily this Application was built with the setting 'FECLFINAE' enabled!\n"
                << "'FECLFINAE' in essence is a designation that an application can declare itself\n"
                << "to satisfy. Doing so implies that despite some issue preventing this\n"
                << "Application from not being able to fully establish and rely upon its usual\n"
                << "expected custom message-logging protocols and behaviors, this loss of function-\n"
                << "ality has been deemed to not violate any of this softwares allowable design \n"
                << "tolerances. Thus the application should be able to expect to continue execution\n"
                << "with potentially reduced functionality!\n"
                << "\n     OUTCOME   \n"
                << "Conditions have been determined to be within acceptable tolerances\n"
                << "to allow for continued program execution!\n\n"
                << "         [Be advised that the Application may behave strangely]\n\n"
                << "\n[Press Enter To acknowledge that you have read the above text]  ";
            LOG(INFO) << feclfinae_msg.str();
            std::cin.get();
            LOG(INFO) << "\nRight then... On with this whole initialization business...";
            return true;
#else //Uh-oh 
            //Else if 'FECLFINAE' is not defined, our best course of action is to throw an exception
            printf("\nSince the autochthonous default logging configuration is most likely not up-to-snuff to meet\n"
                "this executable's demanding logging needs of this program,\n\n");
            throw (FSMException{ "Unable to properly set up message logging!" });
#endif //FECLFINAE
        }
    }
    catch (const FSMNamedException& e) {
        switch (e.getName()) {
        case FSMNamedException::NamedException::NO_GL_DRIVER:
            LOG(ERROR) << " System does not appear to have a driver which is\n"
                "                OpenGL compatible installed!\n";
            LOG(INFO) << "[Press enter to terminate process]";
            std::cin.get();
            std::exit(EXIT_FAILURE);
            break;

        default:
            throw; //Re-throws the caught exception (Note on re-throw behavior: will go to
            //      catch block in the next outer scope level, not one of the other catches at this scope)
            break;
        }
    }
    catch (const FSMException& e) { //Catch 
        printf("\nApplication encountered the following exception during runtime:\n%s", e.what());
        printf("\nSeeing as how this is an integral feature to this Application, the most\n"
            "logical thing to do here is to abort.\nPerhaps try a different operating system?\n");
        std::exit(EXIT_FAILURE);//or equivalently could call std::terminate();
    }
    catch (const std::exception& e) {
        printf("\nSome odd unexpected exception was thrown!\n");
        printf("Message:\n  %s", e.what());
        printf("\nThere is no clear way to respond to this issue. Application will close!\n");
        std::exit(EXIT_FAILURE);
    }
    return false;
}

void Application::validateAssetDirectories() const noexcept {
    LOG(TRACE) << __FUNCTION__;

    //First see if the top directory holding all the asset sub-directories exists
    if (!checkIfIsPathToDirectory(RELATIVE_PATH_TO_ASSET_DATA_DIRECTORY)) {
        createDirectory(RELATIVE_PATH_TO_ASSET_DATA_DIRECTORY);
        createDirectory(RELATIVE_PATH_TO_SETTINGS_DIRECTORY);
        createDirectory(RELATIVE_PATH_TO_TEXTURES_DIRECTORY);
        createDirectory(RELATIVE_PATH_TO_SHADERS_DIRECTORY);
        createDirectory(RELATIVE_PATH_TO_FONTS_DIRECTORY);
        createDirectory(RELATIVE_PATH_TO_AUDIO_DIRECTORY);
        return;
    }
        
    if (!checkIfIsPathToDirectory(RELATIVE_PATH_TO_SETTINGS_DIRECTORY))
        createDirectory(RELATIVE_PATH_TO_SETTINGS_DIRECTORY);
    if (!checkIfIsPathToDirectory(RELATIVE_PATH_TO_TEXTURES_DIRECTORY))
        createDirectory(RELATIVE_PATH_TO_TEXTURES_DIRECTORY);
    if (!checkIfIsPathToDirectory(RELATIVE_PATH_TO_SHADERS_DIRECTORY))
        createDirectory(RELATIVE_PATH_TO_SHADERS_DIRECTORY);
    if (!checkIfIsPathToDirectory(RELATIVE_PATH_TO_FONTS_DIRECTORY))
        createDirectory(RELATIVE_PATH_TO_FONTS_DIRECTORY);
    if (!checkIfIsPathToDirectory(RELATIVE_PATH_TO_AUDIO_DIRECTORY))
        createDirectory(RELATIVE_PATH_TO_AUDIO_DIRECTORY);
}

FSMInitConfig Application::getInitConfig() const noexcept {

    FSMInitConfig tmp;

    if (checkIfIsPathToFile(RELATIVE_PATH_TO_INIT_CONFIG_FILE))
        tmp.test = 1;
    else
        createJSONFile(RELATIVE_PATH_TO_INIT_CONFIG_FILE, "//This is a test!");




    return tmp;
}


void Application::logApplicationBuildAndConfigurationDetails() noexcept {
    LOG(TRACE) << __FUNCTION__;

    LOG(INFO) << " ";
    LOG(INFO) << "Reporting Executable's Detail";
    //Here would be a good location to log Application version number and other such info
    LOG(INFO) << "  Version                      0.0 Build 0.0.02"; //Eventually a real version system will be implemented
    LOG(INFO) << "  Compiler:                    MSVC " << _MSC_VER;
    LOG(INFO) << "  Compile Date:               " << __DATE__;
    LOG(INFO) << "  Compile Time:               " << __TIME__;
    LOG(INFO) << "  Implementation Is Hosted:   " << (__STDC_HOSTED__ ? "True" : "False");
    LOG(INFO) << "  _HAS_CXX20                  " << (   _HAS_CXX20   ? "True" : "False");
    LOG(INFO) << " ";
}

bool Application::createRenderEnvironment() {
    LOG(TRACE) << __FUNCTION__;
    try {
        //Create the Render Environment
        mRenderEnvironment_ = std::make_unique<FSMRenderEnvironment>();
        return true;
    }
    catch (const FSMNamedException& e) {
        switch (e.getName()) {
        case FSMNamedException::NamedException::NO_GL_DRIVER:
            LOG(ERROR) << " Unable to detect the existance of an OpenGL-capable driver.";
            //std::ostringstream minGlVersionMsg;
            //minGlVersionMsg << " This program requires a driver which supports OpenGL " << FSM_MINIMUM_GL_VERSION_MAJOR
            LOG(INFO) << "[Press enter to terminate process]";
            std::cin.get();
            std::exit(EXIT_FAILURE);
        }
    }
    catch (const FSMException& e) {
        LOG(FATAL) << "The following exception was encountered: \n" << e.what()
            << "\nProgram is unable to continue running!\n";
    }
    catch (...) {
        LOG(FATAL) << "\nAn Unexpected exception has been thrown!\n"
            << "Application is not sure how to respond...\n";
    }
    return false;
}







bool checkIfIsPathToDirectory(const std::filesystem::path& directory) noexcept {
    LOG(TRACE) << __FUNCTION__;

    return true;
#ifdef OLDE 
    static std::filesystem::path executablesFilepath = std::filesystem::current_path();
    std::filesystem::path pathToDir;
    if (directory.is_absolute())
        pathToDir = directory;
    else if (*(directory.string().cbegin()) == '\\')
        pathToDir = executablesFilepath.string() + directory.string();
    else
        pathToDir = executablesFilepath.string() + "\\" + directory.string();


    if (std::filesystem::exists(pathToDir)) {
        if (std::filesystem::is_directory(pathToDir))
            return true;
        else {
            std::ostringstream errmsg;
            errmsg << "BAD FILESYSTEM PATH!\n"
                "Guilty path: " << pathToDir.string() << "\n"
                "(Normalized):" << pathToDir.lexically_normal().string() << "\n";
            errmsg << "The path to an Expected Asset Directory was found to\n"
                "represent an extant item, but this item is not a directory!\n"
                "To prevent anything from getting screwed up, this program will\n"
                "now crash...\n";
            LOG(ERROR) << errmsg.str();
            std::exit(EXIT_FAILURE);
        }
    }
    //else
    return false;
#endif
}

bool checkIfIsPathToFile(const std::filesystem::path& filepath) noexcept {
    LOG(TRACE) << __FUNCTION__;

    return true;
#ifdef OLDE 
    static std::filesystem::path executablesFilepath = std::filesystem::current_path();
    std::filesystem::path pathToFile = executablesFilepath.string() + filepath.string();

    if (std::filesystem::exists(pathToFile)) {
        if (std::filesystem::is_regular_file(pathToFile))
            return true;
        else {
            std::ostringstream errmsg;
            errmsg << "Guilty Filepath: " << pathToFile.string() << "\n"
                "The filepath to the requested location was found to\n"
                "represent an existing location, but the item at this location\n"
                "is not a file!\n"
                "To prevent anything from getting screwed up, this program will\n"
                "now crash...\n";
            LOG(ERROR) << errmsg.str();
            std::exit(EXIT_FAILURE);
        }
    }
    //else
    return false;
#endif
}

//Please do not pass this function random bogus nonsense so that weird directories are not created
void createDirectory(const std::filesystem::path& directory) noexcept {
    LOG(TRACE) << __FUNCTION__;
    
#ifdef OLDE 
    static std::filesystem::path executablesFilepath = std::filesystem::current_path();
    std::filesystem::path pathToDir = executablesFilepath.string() + directory.string();
    LOG(INFO) << "INFO!!  Creating Directory " << pathToDir.string();

    std::error_code ec = { };
    std::filesystem::create_directory(pathToDir, ec);

    if (ec) {
        LOG(ERROR) << "\nUnable to locate or create a directory for assets at path: " << pathToDir.string();
        std::exit(EXIT_FAILURE);
    }

#endif
}

//Please do not pass this function random bogus nonsense so that weird directories are not created
void createJSONFile(const std::filesystem::path& filepath, std::string_view fileText) noexcept {
    LOG(TRACE) << __FUNCTION__;

#ifdef OLDE
    if (filepath.empty() || true) {
        std::ostringstream fatalMsg;
        fatalMsg << "\n\n\n!!!!ERROR!!!!!\n";
        fatalMsg << "WHAT THE HECK ARE YOU DOING! DO NOT CALL THE 'createJSONFile()' function\n";
        fatalMsg << "with an empty filepath! Luckily the code here takes the extra step to verify\n";
        fatalMsg << "that the requested std::filesystem::path object is not completly empty!\n";
        fatalMsg << "Because of how bad the code calling this function is for not providing a\n";
        fatalMsg << "filepath, the Application does not deserve to keep operating. Prepare to crash!\n";

        LOG(FATAL) << fatalMsg.str();

        std::exit(EXIT_FAILURE);
    }

    static std::filesystem::path executablesFilepath = std::filesystem::current_path();
    std::filesystem::path pathToFile = executablesFilepath.string() + filepath.string();
    LOG(INFO) << "INFO!!  Creating JSON File at Path" << pathToFile.string();

    //Reality check for debug safety, can remove this function once project is further along in 
    //its development. Basically I just want to try to have a safety-measure in place to prevent
    //any accidental creation of files/directories in weird locations.
    const std::filesystem::path expectedDirectory("C:\\Users\\Forrest\\source\\repos\\FSMEngine\\AssetData\\Settings");
    const std::filesystem::path actualDirectory = pathToFile.root_directory().lexically_normal();
    if (expectedDirectory != actualDirectory) {
        std::ostringstream wrnMsg;
        wrnMsg << "\n\nAttention! The function 'createJSONFile()' was called to create the file\n\t\t\"";
        wrnMsg << pathToFile.filename().string() << "\".\n";
        wrnMsg << "This file is to be created in the directory \n\t\"" << actualDirectory.string() << "\"\n";
        wrnMsg << "Please Verify that this is indeed where the file should be\n"
            "created by pressing [ENTER] around 5 or so times.\n";
        LOG(WARNING) << wrnMsg.str();
        for (int i = 0; i < 5; i++) {
            std::cin.get();
            std::cout << "\n  (" << (5 - i) << " presses remain)\n";
        }
    }
#endif
}



//Utility function for usage with the above implementation functions. Handles 
//converting several possible different path-input formats into a valid, absolute
//path.
std::filesystem::path getAbsolutePath(const std::filesystem::path& path) noexcept {
    LOG(TRACE) << __FUNCTION__;

    static std::filesystem::path directoryOfExecutable = std::filesystem::current_path();
    std::filesystem::path absoluteFilepath("");

    if (path.empty())
        LOG(FATAL) << "NOOO!! BAD!!!  " << __FUNCTION__;

    if (path.is_absolute())
        absoluteFilepath = path;
    else if (*(path.string().cbegin()) == '\\') 
        absoluteFilepath = directoryOfExecutable.string() + path.string();
    else
        absoluteFilepath = directoryOfExecutable.string() + "\\" + path.string();

    //Do some reality checks
    LOG(INFO) << "Reality Check! Absolute Path is: \n     \"" << absoluteFilepath.string() << "\"\n";
    LOG(INFO) << "Reality Check Round 2! Path is absolute: " << ((absoluteFilepath.is_absolute()) ? "True!" :
        "false     :(  \n");
    return absoluteFilepath;
}