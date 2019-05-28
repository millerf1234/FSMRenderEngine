
//
// Any documentation written up here will very very very likely be wrong 
//
//  So I have not written any. Part of my goal though is to have the FSMEngine  
//  interface and types be so intuitive that they explain themselves.

#pragma once

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "FSMEngine.h" //This one header contains all the magic

class Application {
public:
    Application() = delete;
    Application(const Application& that) = delete;
    Application& operator=(const Application& that) = delete;


    Application(int argc = 0, char** argv = nullptr) noexcept;

    ~Application() noexcept;

    void launch() noexcept;
};




#endif //APPLICATION_H_
