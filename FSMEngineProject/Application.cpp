//File:      Application.cpp
//
//  Programmer:   Forrest Miller
//  Date:         January 10, 2019


#include <istream>  //For std::cin.get()   to keep console open
#include "Application.h"


#define ALLOW_ELCC_CONFIGURATION  
#include "EasyLogConfiguration.h"


short Application::applicationsInExistance = 0;

Application::Application(int argc, char ** argv) {
    applicationsInExistance++;
    if (applicationsInExistance > 1) {
        printf("Too Many Applications In Existance!\n");
        std::terminate();
    }
    initializeEasyLogger(argc, argv);

    //LOG(INFO) << "Application is launching...\n";

    configureEasyLogger();

    LOG(INFO) << "Logging Utility Configured!\n";

    LOG(TRACE);

    //// Use default logger
    //el::Logger* defaultLogger = el::Loggers::getLogger("default");

    //// STL logging (`ELPP_STL_LOGGING` should be defined)
    //std::vector<int> i;
    //i.push_back(1);
    //defaultLogger->warn("My first ultimate log message %v %v %v", 123, 222, i);

    //// Escaping
    //defaultLogger->info("My first ultimate log message %% %%v %v %v", 123, 222);

}

Application::~Application() {
    //Flush all the remaining events stored in all loggers
    LOG(INFO) << "Application is closing";
    LOG(TRACE);
    el::Loggers::flushAll();
    
    //close down GLFW?

}

void Application::launch() {
    LOG(INFO) << "Application Launched!";
    LOG(TRACE);
    std::cin.get();
}
