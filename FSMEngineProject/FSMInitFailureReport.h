
// File:                A report on details for a failure encountered during one of the 
//                      FSMEngine initialization functions. 
//                      Note that this class is intended to be general enough to be 
//                      useable by every FSMInit function. 
//
//     Idea:          Inside an init function, a FSMFailureReporter is created and updated
//                    during the initialization process. Once the init function is ready to
//                    return, the FSMFailureReporter is able to generate a FSMInitFailureReport
//                    object, which itself can then be sent out to the public. 


#pragma once

#ifndef FSM_INIT_FAILURE_REPORT_H_
#define FSM_INIT_FAILURE_REPORT_H_

#include <vector>

namespace FSMEngineInternal {
    class FSMInitFailureReporter;
}

enum class PointOfFailure {
    INVALID_PROGRAM_STATE_TO_CALL_INIT,
    CREATING_DEFAULT_DIRECTORIES,
    LOADING_ENGINE_SETTINGS,
    LOADING_OTHER_SETTINGS,
    SETTING_CALLBACKS,
    GLFW_INIT,
    CONTEXT_CREATION,
    GLAD_LOAD_OPENGL
};

class FSMInitFailureReport {
public:

    ~FSMInitFailureReport() noexcept;
    FSMInitFailureReport(const FSMInitFailureReport&) noexcept;
    FSMInitFailureReport(FSMInitFailureReport&&) noexcept;
    FSMInitFailureReport& operator=(const FSMInitFailureReport&) noexcept;
    FSMInitFailureReport& operator=(FSMInitFailureReport&&) noexcept;


    size_t getNumFailures() const noexcept;

    //Gets a constant iterator to the front of the vector of failures
    std::vector<PointOfFailure>::const_iterator getFailureCIterator() const noexcept;

    //Gets a constant iterator to the position one after the last Failure in the vector of
    //failures. (Note that 'cbegin == cend' means the vector is empty)  
    const std::vector<PointOfFailure>::const_iterator getFailureEndCIterator() const noexcept;

private:
    std::vector<PointOfFailure> mFailuresEncountered_;
    friend class FSMEngineInternal::FSMInitFailureReporter;
    FSMInitFailureReport(const std::vector<PointOfFailure>&) noexcept;
};




#endif //FSM_INIT_FAILURE_REPORT_H_
