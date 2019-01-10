//File:      Application.cpp
//
//  Programmer:   Forrest Miller
//  Date:         January 10, 2019


#include <istream>  //For std::cin.get()   to keep console open
#include "Application.h"

#include "EasyLogConfiguration.h"



Application::Application(int argc, char ** argv) {
    configureEasyLogger(argc, argv);

    LOG(INFO) << "Constructor for application called!\n";


    glfwInit();

    glfwTerminate();
}


Application::~Application() {

}

void Application::launch() {
    LOG(INFO) << "Application Launched!";

    std::cin.get();
}
