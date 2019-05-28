
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


class FSMFastInitReturnBundle {
public:
    

    enum class PointOfFailure { LOGGING_INIT, LOADING_ENGINE_SETTINGS, LOADING_OTHER_SETTINGS, GLFW_INIT, 
        CONTEXT_CREATION, GLAD_LOAD_OPENGL, };
    
    int getNumFailures() const noexcept { return mFailuresEncountered_.size(); }
    std::vector<PointOfFailure>::const_iterator getFailureCIterator() const noexcept {
        return mFailuresEncountered_.cbegin();
    }

private:
    bool mInitSuccess_;
    std::vector<PointOfFailure> mFailuresEncountered_;
};



std::unique_ptr<FSMFastInitReturnBundle> FSM_DO_FAST_INIT(int argc, char* argv) noexcept;





#endif //FSM_FAST_INIT_H_


