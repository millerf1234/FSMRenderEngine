//
//  File:                  FSMEngineInternal.h
//
//  Description:           Contains global internal engine constants, settings and variables
//

//
//  


#pragma once

#ifndef FSM_ENGINE_INTERNAL_H_
#define FSM_ENGINE_INTERNAL_H_

#include "UniversalIncludes.h"

#include <shared_mutex>

namespace FSMEngineInternal {


    namespace {
        std::shared_mutex initStatusMutex;
    }

    // THIS CLASS IS A SINGLETON. It Wraps a Global Bool Representing the State of GLFW
    class InitStatusOfTheGLFWLibrary final {
    public: 
        InitStatusOfTheGLFWLibrary() = delete;
        ~InitStatusOfTheGLFWLibrary() noexcept { LOG(TRACE) << __FUNCTION__; }
        InitStatusOfTheGLFWLibrary(const InitStatusOfTheGLFWLibrary&) = delete;
        InitStatusOfTheGLFWLibrary& operator=(const InitStatusOfTheGLFWLibrary&) = delete;

        //The following public function is used to get the status of the GLFW library
        bool getInitStatus() const noexcept {
            LOG(TRACE) << __FUNCTION__;
            initStatusMutex.lock_shared();
            bool status = initStatus;
            initStatusMutex.unlock_shared();
            return status;
        }

    private:
        static bool initStatus;

        void updateGLFWInitStatus(bool status) noexcept {
            LOG(TRACE) << __FUNCTION__;
            initStatusMutex.lock();
            status = status;
            initStatusMutex.unlock();
        }
        //Todo: add
        //Friend of function that initializes glfw
        //Friend of function that std::atexit's glfwTerminate

    };

    bool InitStatusOfTheGLFWLibrary::initStatus = false;







}  //namespace FSMEngineInternal

#endif //FSM_ENGINE_INTERNAL_H_