


#pragma once

#include "FSMAudioCommon.h"

namespace FSMEngineInternal {

    namespace FSMAudio {


class FSMAudioTest {
public:
    static bool initializeAudio(AudioConfiguration config) noexcept;
    static bool checkIfAudioInitialized() noexcept { return initialized; }


private:

    static bool initialized;


};


    } //namespace FSMAudio

} //namespace FSMEngineInternal