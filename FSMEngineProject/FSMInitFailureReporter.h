

#pragma once

#ifndef FSM_INIT_FAILURE_REPORTER_H_
#define FSM_INIT_FAILURE_REPORTER_H_

#include <vector>
#include "FSMInitFailureReport.h"

namespace FSMEngineInternal {

    class FSMInitFailureReporter {
    public:
        FSMInitFailureReporter() noexcept;
        ~FSMInitFailureReporter() noexcept;

        void addFailure(PointOfFailure pof) noexcept;

        //Creates an InitFailureReport based upon all failures
        //reported to this class
        FSMInitFailureReport createReport() const noexcept;
        
    private:
        std::vector<PointOfFailure> mReportedFailures_;
    };


} //namespace FSMEngineInternal

#endif //FSM_INIT_FAILURE_REPORTER_H_

