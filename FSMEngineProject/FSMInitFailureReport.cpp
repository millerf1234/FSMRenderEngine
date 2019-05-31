

#include "FSMInitFailureReport.h"
#include "UniversalIncludes.h"  //LOG


FSMInitFailureReport::FSMInitFailureReport(const std::vector<PointOfFailure>& failures) noexcept {
    LOG(TRACE) << __FUNCTION__;

    mFailuresEncountered_.assign(failures.cbegin(), failures.cend());
}

FSMInitFailureReport::~FSMInitFailureReport() noexcept {
    LOG(TRACE) << __FUNCTION__;
}

FSMInitFailureReport::FSMInitFailureReport(const FSMInitFailureReport& otherReport) noexcept {
    LOG(TRACE) << __FUNCTION__;
    mFailuresEncountered_.assign(otherReport.mFailuresEncountered_.cbegin(), otherReport.mFailuresEncountered_.cend());
}

FSMInitFailureReport::FSMInitFailureReport(FSMInitFailureReport&& otherReport) noexcept {
    LOG(TRACE) << __FUNCTION__;
    mFailuresEncountered_.swap(otherReport.mFailuresEncountered_);
}


FSMInitFailureReport& FSMInitFailureReport::operator=(const FSMInitFailureReport& otherReport) noexcept {
    LOG(TRACE) << __FUNCTION__;
    if (this != &otherReport) 
    mFailuresEncountered_.assign(otherReport.mFailuresEncountered_.cbegin(), otherReport.mFailuresEncountered_.cend());
    return *this;
}

FSMInitFailureReport& FSMInitFailureReport::operator=(FSMInitFailureReport&& otherReport) noexcept {
    LOG(TRACE) << __FUNCTION__;
    if (this != &otherReport)
    mFailuresEncountered_.swap(otherReport.mFailuresEncountered_);
    return *this;
}

size_t FSMInitFailureReport::getNumFailures() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    
    return mFailuresEncountered_.size();
    
}

std::vector<PointOfFailure>::const_iterator FSMInitFailureReport::getFailureCIterator() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return mFailuresEncountered_.cbegin();
}

const std::vector<PointOfFailure>::const_iterator FSMInitFailureReport::getFailureEndCIterator() const noexcept {
    LOG(TRACE) << __FUNCTION__;
    return mFailuresEncountered_.cend();
}
