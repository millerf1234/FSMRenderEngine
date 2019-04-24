//
// FILE:                         RenderDemoBase.h
// 
// Description:                  Base class that provides a common interface and 
//                               universally shared functionality for every type
//                               of RenderDemo class that derives from this.
//
// Programmer:                   Forrest Miller
//


#include "RenderDemoBase.h"

#include "UniversalIncludes.h"


RenderDemoBase::RenderDemoBase() {
    LOG(TRACE) << __FUNCTION__;
}


RenderDemoBase::~RenderDemoBase() {
    LOG(TRACE) << __FUNCTION__;
}
