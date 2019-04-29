//File:      Application.cpp
//
//  Programmer:           Forrest Miller
//  Created Date:         January 10, 2019


#include <istream>  //For std::cin.get()   to keep console open
#include "Application.h"

#include "GraphicsLanguage.h"
#include "FSMException.h"

//The macro 'ALLOW_ELCC_CONFIGURATION' must be defined before this header can be included
#define ALLOW_ELCC_CONFIGURATION  1
#include "EasyLogConfiguration.h"


#include "AsciiTextFile.h"

#include "JSON_Test.h" //Experimental

Application::Application(int argc, char ** argv) noexcept : mRenderEnvironment_(nullptr) {


    //WAIT! Don't move the 'Initializing Application' message up here*!!!                 *Times I have tried to
    //How the heck is that message going to get logged if  message                            do so counter:  4
    //logging hasn't been set up yet?
    
    //
    //BUG (of sorts)
    //   The intro message 'Initializing Application..." is printed twice if logging fails to 
    //   properly configure yet the application is set to keep running 
    //

    if (!setupMessageLogs(argc, argv)) {
        std::exit(EXIT_FAILURE);
    }
    LOG(TRACE) << __FUNCTION__;

    //Once we make it beyond this point, all message logging will be done
    //though the Easylogger++ library 

    LOG(INFO) << "Initializing Application...";
    LOG(INFO) << "Creating directory for LOG files...DONE";

    //Here Would be a good location to log Application version number and other such info
    LOG(INFO) << "Application Version 0.0 Build 0.0.01"; //Eventually a real version system will be implemented
    LOG(INFO) << "Compiler:                    MSVC " << _MSC_VER;
    LOG(INFO) << "Compile Date:               " << __DATE__;
    LOG(INFO) << "Compile Time:               " << __TIME__;
    LOG(INFO) << "Implementation Is Hosted:   " << ((__STDC_HOSTED__) ? "True" : "False");

    
    
    LOG(INFO) << "Parsing a test JSON!\n";
    JSON_Test testJSONParser;
    std::filesystem::path path = std::filesystem::current_path();
    testJSONParser.parseFile(path);


    LOG(INFO) << "Inchoating The Render Environment...";
    if (!createRenderEnvironment()) {
        LOG(ERROR) << "Failed to construct a render environment for this application.\n"
            "Unfortunatly this means it is time to crash...\n"
            "      [Press Enter To Terminate]   ";
        std::cin.get();
        std::exit(EXIT_FAILURE);
    }

    LOG(INFO) << "\n       Application's Constructor has completed running!\n";

}


Application::~Application() {
    LOG(TRACE) << __FUNCTION__;
    el::Loggers::flushAll();
}

void Application::launch() {
    LOG(TRACE) << __FUNCTION__;
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