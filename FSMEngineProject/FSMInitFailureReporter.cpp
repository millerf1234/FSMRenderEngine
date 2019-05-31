

#include "FSMInitFailureReporter.h"

#include "UniversalIncludes.h" //LOG

namespace FSMEngineInternal {

   FSMInitFailureReporter::FSMInitFailureReporter() noexcept {
       LOG(TRACE) << __FUNCTION__;
   }

   FSMInitFailureReporter::~FSMInitFailureReporter() noexcept {
       LOG(TRACE) << __FUNCTION__;
   }

   void FSMInitFailureReporter::addFailure(PointOfFailure pof) noexcept {
       LOG(TRACE) << __FUNCTION__;
       mReportedFailures_.push_back(pof);
   }

   FSMInitFailureReport FSMInitFailureReporter::createReport() const noexcept {
       LOG(TRACE) << __FUNCTION__;
       return FSMInitFailureReport(mReportedFailures_);
   }


} //namespace FSMEngineInteral