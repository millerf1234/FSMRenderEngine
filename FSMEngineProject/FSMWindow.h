
//todo  documentation


#pragma once

#ifndef FSM_WINDOW_H_
#define FSM_WINDOW_H_

#include <memory>

//Forward Declare necesary GLFW types
struct GLFWwindow;

class FSMWindow {
public:
    FSMWindow() = delete;
    FSMWindow(GLFWwindow*);
    ~FSMWindow() noexcept;

private:
    class FSMWindowImpl;
    std::unique_ptr<FSMWindowImpl> pImpl_;
};


#endif //FSM_WINDOW_H_

