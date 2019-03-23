//File:                main.cpp
//
//Description:         Create the application and launch it
//
//Programmer:          Forrest Miller 
//Date:                January 9, 2019

#include <memory>
#include "Application.h"



int main(int argc, char ** argv) {
    std::unique_ptr<Application> app = std::make_unique<Application>(argc, argv);
    app->launch();
}