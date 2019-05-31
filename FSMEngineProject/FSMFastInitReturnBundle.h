

#pragma once

#ifndef FSM_FAST_INIT_RETURN_BUNDLE_H_
#define FSM_FAST_INIT_RETURN_BUNDLE_H_

#include <vector>
#include <optional>

#include "FSMInternalGlobalState.h"
#include "FSMInitFailureReport.h"


class FSMWindowContext; //High-Level class that encapsulates both a window and a context




class FSMFastInitReturnBundle {
public:

    std::optional<FSMInitFailureReport> failures = std::nullopt;

    enum class FastInitializationStatus {
        NOT_READY,
        FAILED,
        ALL_STEPS_COMPLETE_WITH_ERRORS,
        ALL_STEPS_COMPLETE_WITH_WARNINGS,
        FLAWLESS_INIT_SUCCESS
    };

    FastInitializationStatus get() const noexcept {
        return mInitStatus_;
    }

    void setInitializationStatus(FastInitializationStatus initStatus) noexcept {
        mInitStatus_ = initStatus;
    }

private:

    FastInitializationStatus mInitStatus_ = FastInitializationStatus::NOT_READY;

    // bool mFlawlessInitSuccess_
    //Only will become true if every step in the initialization process
    //performed by FSM_FAST_INIT() completes without a single error/warning. 
    bool mFlawlessInitSuccess_;

    

};











#endif //FSM_FAST_INIT_RETURN_BUNDLE_H_

