//File:      Application.cpp
//
//  Programmer:   Forrest Miller
//  Date:         January 10, 2019


#include <istream>  //For std::cin.get()   to keep console open
#include "Application.h"


#define ALLOW_ELCC_CONFIGURATION  
#include "EasyLogConfiguration.h"


#include "FSMVideoMode.h"
#include "GraphicsLanguageFramework.h" 

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

    LOG(INFO) << "\n\n\nIt is VideoMode testing time now tho!\n";
    struct GLFWvidmode temp;
    temp.redBits = temp.greenBits = temp.blueBits = 8;
    temp.height = 16;
    temp.width = 25;
    temp.refreshRate = 60;
    
    FSMVideoMode tester(temp, 30, 33);

    LOG(INFO) << tester.toString() << std::endl;

    std::cin.get();
}
