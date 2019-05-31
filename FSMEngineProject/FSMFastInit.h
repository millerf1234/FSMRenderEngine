
//
//  Free standing function that handles all the setup and 
//  initialization implicitly.
//
//


#pragma once

#ifndef FSM_FAST_INIT_H_
#define FSM_FAST_INIT_H_

#include <memory>

#include "FSMFastInitReturnBundle.h"


//Initialize the FSMEngine in one easy function call.
//   MUST HAVE CALLED FSM_ENGINE_BEGIN() first!
std::unique_ptr<FSMFastInitReturnBundle> FSM_DO_FAST_INIT() noexcept;


#endif //FSM_FAST_INIT_H_


