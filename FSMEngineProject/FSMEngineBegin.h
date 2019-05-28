
//
// Free standing function that must be callled early in executable's lifetime.
// This function is needed to initialize basic core functionality of the FSMEngine.
// After calling this function, logging using the LOG() macro should be enabled project
// wide.
//

#pragma once

#ifndef FSM_ENGINE_BEGIN_H_
#define FSM_ENGINE_BEGIN_H_


//Function that invokes into existance foundational core stuff
//which the rest of FSMEngine will be built upon. After this
//function returns, it should be safe to fully utalize the 
//FSMEngine's logging functionality.
bool FSM_ENGINE_BEGIN(int argc, const char** argv) noexcept;





#endif //FSM_ENGINE_BEGIN_H_