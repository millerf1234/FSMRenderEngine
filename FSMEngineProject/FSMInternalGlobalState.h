
//
//  I am not sure yet if this is going to become a permanent part of this project
//
//
//


#pragma once

#ifndef FSM_GLOBAL_STATE_H_
#define FSM_GLOBAL_STATE_H_

#include <filesystem>
#include <shared_mutex>
#include <sstream>
#include "UniversalIncludes.h"

namespace FSMEngineInternal {

    typedef struct FSMGlobalStateTracker {

        struct InitializationStatus {
            bool completeGlobalStateSet = false;
            bool fullyInitialized = false;
            bool loggingInitialized = false;
            bool engineInitConfigRecieved = false;

            enum class INIT_METHOD {
                UNCHOSEN,
                FAST_INIT,
                SKIP_FILESYSTEM,
                LOAD_FILE_THEN_CUSTOMIZE
            };
            INIT_METHOD choosenInitMethod = INIT_METHOD::UNCHOSEN;
        };
        InitializationStatus initializationStatus;

        struct ContextState {
            bool isResetAware = false;
            bool isDebug = false;
            bool isAsync = true;

        };
        ContextState contextState;

        struct CaughtExceptions {
            unsigned long long exceptionsCaught = 0ULL;
            unsigned long long namedExceptionsCaught = 0ULL;
            std::ostringstream reportStream;
        };
        CaughtExceptions caughtExceptionsReport;

        struct ActiveDirectory {
            bool allowCurrentDirectoryOverride = true;
            std::filesystem::path pathToDirectory;
        };
        ActiveDirectory currentlyActiveDirectory;

    } FSMGlobalState;

     FSMGlobalState* pGlobalState = nullptr;

    FSMGlobalState& createTheInitialGlobalState() noexcept { 
        static FSMGlobalState globalState;
        return globalState;
    }





    /////////////////////////// GLFW Initialization is given special treatment ///////////////////////////////


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










} //namespace FSMEngineInternal


#endif //FSM_GLOBAL_STATE_H_