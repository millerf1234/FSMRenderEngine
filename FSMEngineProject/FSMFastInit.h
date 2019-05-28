
//
//  Free standing function that handles all the setup and 
//  initialization implicitly.
//
//


#pragma once

#ifndef FSM_FAST_INIT_H_
#define FSM_FAST_INIT_H_

#include <memory>

class FSMWindowContext; //High-Level class that encapsulates both a window and a context


std::unique_ptr<FSMFastInitReturnBundle> FSM_DO_FAST_INIT() noexcept;


class FSMFastInitReturnBundle {
public:
    
    class FailureReport {
    public:
        enum class PointOfFailure {
            CREATING_DEFAULT_DIRECTORIES,
            LOADING_ENGINE_SETTINGS,
            LOADING_OTHER_SETTINGS,
            SETTING_CALLBACKS,
            GLFW_INIT,
            CONTEXT_CREATION,
            GLAD_LOAD_OPENGL
        };

        int getNumFailures() const noexcept {
            return mFailuresEncountered_.size();
        }

        std::vector<PointOfFailure>::const_iterator getFailureCIterator() const noexcept {
            return mFailuresEncountered_.cbegin();
        }

        const std::vector<PointOfFailure>::const_iterator getFailureEndCIterator() const noexcept {
            return mFailuresEncountered_.cend();
        }

    private:
        std::vector<PointOfFailure> mFailuresEncountered_;
        friend FSMFastInitReturnBundle;
    };

    FailureReport failures;

    enum class InitializationStatus {
        NOT_READY,
        FAILED,
        ALL_STEPS_COMPLETE_WITH_ERRORS,
        ALL_STEPS_COMPLETE_WITH_WARNINGS,
        FLAWLESS_INIT_SUCCESS
    };

    InitializationStatus get() const noexcept {
        return mInitStatus_;
    }

private:

    InitializationStatus mInitStatus_ = InitializationStatus::NOT_READY;

    // bool mFlawlessInitSuccess_
    //Only will become true if every step in the initialization process
    //performed by FSM_FAST_INIT() completes without a single error/warning. 
    bool mFlawlessInitSuccess_; 

    // bool mAllStepsComplete_ 
    //Will become true if FSM_FAST_INIT() is able to get enough 
    //critical components online to support further rendering.
    //However use caution as there may be some unusual engine behavior
    //due to 
    bool mAllStepsComplete_; //True if enough initialization was completed that 

};









#endif //FSM_FAST_INIT_H_


