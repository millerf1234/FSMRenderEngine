//File:                main.cpp
//
//Description:         Create the application and launch it
//
//Programmer:          Forrest Miller 
//Date:                January 9, 2019

#include <memory>
#include "Application.h"

class DelegateConstructorTest {
public:
    DelegateConstructorTest() : DelegateConstructorTest(5) {
        throw "Error!\n";
    }
    DelegateConstructorTest(int x) {
        mX_ = x;
    }
    ~DelegateConstructorTest() {
        printf("Destructor called!\n");
    }

    int mX_;
};


int main(int argc, char ** argv) {
    try {
        DelegateConstructorTest t;
    }
    catch (...) {
        printf("\nCaught it tho!\n");
    }
    std::unique_ptr<Application> app = std::make_unique<Application>(argc, argv);
    app->launch();
}