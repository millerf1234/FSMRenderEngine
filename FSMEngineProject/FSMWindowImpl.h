//
// File:                             FSMWindowImpl.h
// 
// Description:                      Header for the FSMWindow Implementation
//
// Programmer:                       Forrest Miller
//

#pragma once

#ifndef FSM_WINDOW_IMPLEMENTATION_H_
#define FSM_WINDOW_IMPLEMENTATION_H_

#include "FSMWindow.h"
#include "GraphicsLanguageFramework.h"

class FSMWindow::FSMWindowImpl final {
public:
    FSMWindowImpl(GLFWwindow*);
    ~FSMWindowImpl() noexcept;

private:
    GLFWwindow* mHandle_;

};


#endif //FSM_WINDOW_IMPLEMENTATION