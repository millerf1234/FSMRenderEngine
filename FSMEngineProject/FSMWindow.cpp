//
// File:                            FSMWindow.h
//
// Description:                     The implementation that calls the real implementation
//


#include "FSMWindow.h"
#include "FSMWindowImpl.h"

#include "UniversalIncludes.h"


FSMWindow::FSMWindow(GLFWwindow* w) {
    LOG(TRACE) << __FUNCTION__;
    pImpl_ = std::make_unique<FSMWindowImpl>(w);
}


FSMWindow::~FSMWindow() noexcept {
    LOG(TRACE) << __FUNCTION__;
}
