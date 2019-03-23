//File:      Application.cpp
//
//  Programmer:   Forrest Miller
//  Date:         January 10, 2019


#include <istream>  //For std::cin.get()   to keep console open
#include "Application.h"

#include "GraphicsLanguage.h"
#include "FSMException.h"

//The macro 'ALLOW_ELCC_CONFIGURATION' must be defined before this header can be included
#define ALLOW_ELCC_CONFIGURATION  1
#include "EasyLogConfiguration.h"


#include "AsciiTextFile.h"

Application::Application(int argc, char ** argv) : mRenderEnvironment_(nullptr) {


    //WAIT! Don't move the 'Initializing Application' message up here*!!!                 *Times I have tried to
    //How the heck is that message going to get logged if the message                          do so counter:  3
    //logging hasn't been set up yet? Instead just pretend logging isn't ready
    //yet and print it.
    
    //
    //BUG (of sorts)
    //   The intro message 'Initializing Application..." is printed twice if logging fails to 
    //   properly configure yet the application is set to keep running 
    //

    if (!setupMessageLogs(argc, argv)) {
        std::exit(EXIT_FAILURE);
    }

    //Once we make it beyond this point, all message logging will be done
    //though the Easylogger++ library. 

    LOG(INFO) << "Initializing Application...";
    LOG(INFO) << "Creating directory for LOG files...DONE";
   

    LOG(INFO) << "Inchoating A Render Environment...";
    if (!createRenderEnvironment()) {
        std::exit(EXIT_FAILURE);
    }

    AsciiTextFile temp("C:\\Users\\Forrest\\source\\repos\\FSMEngine\\FSMEngineProject\\LOGS\\2019_081_15_10_33\\info.log");
    std::cout << "\nINFO LOG HAS " << temp.countNumberOfLines() << " lines of ASCII text!\n";
    AsciiTextFile temp2("C:\\Users\\Forrest\\source\\repos\\FSMEngine\\FSMEngineProject\\LOGS\\2019_081_15_10_33\\info.log");
    std::cout << "\nINFO LOG HAS " << temp2.countNumberOfLines() << " lines of ASCII text!\n";


    //The rest of the constructor code below here is just testing some functions



    //Test GLFW compile version reporting
    int compileVersionMajor = 0;
    int compileVersionMinor = 0;
    int compileVersionRevision = 0;
    mRenderEnvironment_->getGLFWCompiletimeVersion(compileVersionMajor, compileVersionMinor, compileVersionRevision);
    LOG(INFO) << "Program was compiled to work with GLFW " << compileVersionMajor << "."
        << compileVersionMinor << "." << compileVersionRevision << std::endl;
   
    LOG(INFO) << " Using object's print function: " << mRenderEnvironment_->getGLFWCompiletimeVersionString() << "\n";
    
    //Test GLFW runtime version reporting
    LOG(INFO) << "GLFW Version Loaded at Runtime String: " << mRenderEnvironment_->getGLFWRuntimeVersionString() << "\n\n";

    //Test GLAD OpenGL Version Reporting
    int openGLVerMajor = 0;
    int openGLVerMinor = 0;
    bool runWIthOpenGLInCompatabilityMode = false;
    mRenderEnvironment_->getGLVersion(openGLVerMajor, openGLVerMinor, runWIthOpenGLInCompatabilityMode);
    LOG(INFO) << "Program was compiled to target OpenGL " << openGLVerMajor << "." << openGLVerMinor << " "
        << ((runWIthOpenGLInCompatabilityMode) ?  "Compatibility" : "Core" ) << std::endl;

    LOG(INFO) << "OpenGL version constructed by class: \'" << mRenderEnvironment_->getGLVersionString() << "\n\n" << std::endl;

    

    LOG(INFO) << "\n       Applications Constructor has completed running!\n";

}


Application::~Application() {
    el::Loggers::flushAll();
}

void Application::launch() {
    LOG(INFO) << "\nApplication launched....\n";
    mRenderEnvironment_->doMonitorSelectionLoop();
}



bool Application::setupMessageLogs(int argc, char** argv) {

    //Set up EasyLogger++ library
    try {
        initializeEasyLogger(argc, argv);
        if (configureEasyLogger()) {  //If there were no issues with setting up log files
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
            LOG(INFO) << "\n     SYSTEM REPORTED ISSUE        \n"
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
            std::cin.get();
            LOG(INFO) << "\nRight then... On with this whole initialization business..." << std::endl;
            return true;
#else //Uh-oh 
            //Else if 'FECLFINAE' is not defined, our best course of action is to throw an exception
            printf("\nSince the autochthonous default logging configuration is most likely not up-to-snuff to meet\n"
                "this executable's demanding logging needs of this program,\n\n");
            throw (FSMException{ "Unable to properly set up message logging!" });
#endif //FECLFINAE
        }
    }
    catch (const FSMException& e) { //Catch 
        printf("\nApplication encountered the following exception during runtime:\n%s", e.what());
        printf("\nSeeing as how this is an integral feature to this Application, the most\n"
            "logical thing to do here is to abort.\nPerhaps try a different operating system?\n");
        std::exit(EXIT_FAILURE);//std::terminate();
    }
    catch (...) {
        printf("\nSome odd unexpected exception was thrown!\n");
        printf("\nThere is no clear way to respond to this issue. Application will close!\n");
        std::exit(EXIT_FAILURE);
    }
    return false;
}


bool Application::createRenderEnvironment() {
    try {
        //Create the Render Environment
        mRenderEnvironment_ = std::make_unique<FSMRenderEnvironment>();
        return true;
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