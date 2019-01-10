//File:                main.cpp
//
//Description:         Create the application and launch it
//
//Programmer:          Forrest Miller 
//Date:                January 9, 2019

#include <memory>
#include "Application.h"
#include "ThirdParty/easyloggingpp/include/easylogging++.h" 
INITIALIZE_EASYLOGGINGPP

int main(int argc, char ** argv) {
    START_EASYLOGGINGPP(argc, argv);
    std::unique_ptr<Application> app = std::make_unique<Application>();
    app->launch();
}