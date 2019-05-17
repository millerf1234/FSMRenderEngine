

#pragma once

#ifndef FSM_INIT_CONFIG_H_
#define FSM_INIT_CONFIG_H_

#include <filesystem>

class FSMInitConfig final {
public:
    FSMInitConfig() = default;
    //FSMInitConfig(std::filesystem::path pathToInitFile);

    int test;
    float test2;

private:


};

#endif //FSM_INIT_CONFIG_H_