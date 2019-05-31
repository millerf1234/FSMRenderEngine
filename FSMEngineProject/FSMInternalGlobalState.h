
//
//  I am not sure yet if this is going to become a permanent part of this project
//
//
//  This file is quite messy. Sorry
//
//
//



#pragma once

#ifndef FSM_GLOBAL_STATE_H_
#define FSM_GLOBAL_STATE_H_

#include <memory>
#include <filesystem>
#include <shared_mutex>
#include <sstream>
#include "UniversalIncludes.h"

namespace FSMEngineInternal {

    typedef struct FSMGlobalStateTracker {

        class InitStatusOfTheGLFWLibrary;

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

            InitStatusOfTheGLFWLibrary& libGLFW() noexcept;
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



    inline FSMGlobalState* createTheInitialGlobalState() noexcept {
        static FSMGlobalState* globalState = new FSMGlobalState();
        pGlobalState = globalState;
        return globalState;
    }








   

} //namespace FSMEngineInternal


#endif //FSM_GLOBAL_STATE_H_