//File:      Application.cpp
//
//  Programmer:   Forrest Miller
//  Date:         January 10, 2019


#include <istream>  //For std::cin.get()   to keep console open
#include "Application.h"


#ifndef ELPP_DEFAULT_LOGGER
  #define ELPP_DEFAULT_LOGGER "MsgLog"
#endif
//#ifndef ELPP_DEFAULT_PERFORMANCE_LOGGER
//  #define ELPP_DEFAULT_PERFORMANCE_LOGGER "PerfLog"
//#endif



#include "EasyLogConfiguration.h"
//#include "FSMEngine_GlobalConfigurationMacrosForEasyLoggingPP.h"

Application::Application(int argc, char ** argv) {
    
    initializeEasyLogger(argc, argv);

    LOG(INFO) << "Application is launching...\n";

    configureEasyLogger();

    LOG(INFO) << "Logging Utility Configured!\n";

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
    el::Loggers::flushAll();

}

void Application::launch() {
    LOG(INFO) << "Application Launched!";

    std::cin.get();
}
