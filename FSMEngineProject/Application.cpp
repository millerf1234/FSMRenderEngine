//File:      Application.cpp
//
//  Programmer:   Forrest Miller
//  Date:         January 10, 2019


#include <istream>  //For std::cin.get()   to keep console open
#include "Application.h"

#include "GraphicsLanguage.h"
#include "FSMException.h"

#define ALLOW_ELCC_CONFIGURATION  
#include "EasyLogConfiguration.h"


Application::Application(int argc, char ** argv) : mRenderEnvironment_(nullptr) {
    
    //Set up EasyLogger++ library
    try {
        initializeEasyLogger(argc, argv);
        if (!configureEasyLogger()) {
            printf("\n\nAn error occurred while attempting to set up Log files for this App!\n\n");
            throw ( FSMException{ "Unable to properly set up message logging!" } );
        }
        LOG(INFO) << "\n\nInitializing Application...\n";
        LOG(INFO) << "    Creating directory for LOG files...DONE\n";
        LOG(INFO) << "    Loading Application Render Environment...";
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

    //Once we make it beyond this point, all messaging will be done though the Easylogger++
    //logging library. 

    try {
        //Create the Render Environment
        mRenderEnvironment_ = std::make_unique<FSMRenderEnvironment>();
    }
    catch (const FSMException& e) {
        LOG(FATAL) << "The following exception was encountered: \n" << e.what()
            << "\nProgram is unable to continue running!\n";
    }
    catch (...) {
        LOG(FATAL) << "\nAn Unexpected exception has been thrown!\n"
            << "Application is not sure how to respond...\n";
    }


    mRenderEnvironment_->doMonitorSelectionLoop();

}


Application::~Application() {
    el::Loggers::flushAll();
}

void Application::launch() {

}
