//File:                main.cpp
//
//Description:         Create the application and launch it
//
//Programmer:          Forrest Miller 
//Date:                January 9, 2019

#include <memory>
#include "ApplicationOld.h"

int main(int argc, char** argv) {
    std::unique_ptr<ApplicationOld> app = std::make_unique<ApplicationOld>(argc, argv);
    app->launch(); 
}