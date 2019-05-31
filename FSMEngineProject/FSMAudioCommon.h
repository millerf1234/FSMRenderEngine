//
// File:                    FSMAudioCommon.h
//
//
//
//
//
// Programmer:              Forrest Miller
//
//


#pragma once

#ifndef FSM_AUDIO_COMMON_H_
#define FSM_AUDIO_COMMON_H_

#if 0

#include <array>
#include <string>
#include <sstream>

#include "UniversalIncludes.h"
#include "..\ThirdParty_IncludeWrappers\openAL_includer.h"
#include "FSMException.h"

//For Testing purposes 
//#include "LinearlyIndependentVectorPair.h"

namespace FSMEngineInternal {
    namespace FSMAudio {

        static constexpr const ALfloat LISTENER_DEFAULT_GAIN = 5.0f;

//        class FSMAudioException final : public FSMException {
//        public:
//            
//#ifdef CONSTRUCT_FROM_STRING_VIEW
//            /** Constructor (String type)
//             *  @param name    The name of this Joystick exception
//             *  @param message The exception message.
//             */
//            FSMAudioException(JoystickException joyExcept, std::string_view msg) noexcept :
//                FSMException(msg), mName_(joyExcept) {
//                LOG(TRACE) << __FUNCTION__;
//            }
//#else 
//            /** Constructor (C strings).
//             *  @param name    The name of this named exception
//             *  @param message C-style string error message.
//             *                 The string contents are copied upon construction.
//             *                 Hence, responsibility for deleting the char* lies
//             *                 with the caller.
//             */
//            explicit FSMNamedException(JoystickException name, const char* message) noexcept :
//                FSMException(message), mName_(name) { //Let FSMException's constructor handle the potential nullptr 'message'
//                LOG(TRACE) << __FUNCTION__;
//            }
//
//            /** Constructor (C++ STL strings).
//             *  @param name    The name of this named exception
//             *  @param message The error message.
//             */
//            explicit FSMNamedException(JoystickException name, const std::string& message) noexcept :
//                FSMException(message), mName_(name) {
//                LOG(TRACE) << __FUNCTION__;
//            }
//#endif // CONSTRUCT_FROM_STRING_VIEW
//
//
//            virtual ~FSMJoystickException() noexcept {
//                LOG(TRACE) << __FUNCTION__;
//            }
//
//            /** Returns the stored enum value representing this Joystick
//             *   exception's name.
//             * @return  The enum value representing this exception's name.
//            */
//            JoystickException getName() const noexcept { return mName_; }
//
//        private:
//            const JoystickException mName_;
//        };




        //Checks the OpenAL stack for an error. It is reccommended to call this function after EVERY
        //single OpenAL function call. Will return true if there was an error. Will return false if 
        //there was no error on the stack.
        bool checkOpenALErrorStack() noexcept;

        typedef struct AudioConfiguration {
            ALuint deviceNo;
            struct ListenerConfiguration {
                ALfloat gain;
                std::array<ALfloat, 3> pos;
                std::array<ALfloat, 3> vel;
                /*
                 * Quote From OpenAL 1.1 Specification [Section 4.2.1 'Listener Attributes']:
                 *    AL_ORIENTATION is a pair of 3-tuples consisting of an 'at' vector and an 'up'
                 *    vector, where the 'at' vector represents the 'forward' direction of the 
                 *    listener and the orthogonal projection of the 'up' vector into the 
                 *    subspace perpendicular to the 'at' vector represents the 'up' direction for 
                 *    the listener. OpenAL expects two vectors that are linearly independent. These 
                 *    vectors are not expected to be normalized. If the two vectors are linearly
                 *    dependent, behavior is undefined.
                 */
                std::array<ALfloat, 6> orientation; 
                ListenerConfiguration(ALfloat listenerGain = LISTENER_DEFAULT_GAIN) noexcept;
            };
            struct ListenerConfiguration listenerConfig;
        } AudioConfiguration;





      


        






    } //namespace FSMAudio

} //namespace FSMEngineInternal

#endif //#if 0

#endif //FSM_AUDIO_COMMON_H_








#if 0

//
//  AudioRenderer.hpp
//  Star Suzerian
//
//  This code was highly inspired by the tutorial code at:
//                                   https://ffainelli.github.io/openal-example/
//
//  Created by Forrest Miller on 6/20/18.
//  Copyright © 2018 Forrest Miller. All rights reserved.
//

#ifndef AudioRenderer_h
#define AudioRenderer_h

#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <memory>
#include <sstream>

#include "OpenAL.h" //This include includes all the other includes
//#include "al.h"   // <AL/al.h>   if not on mac?
//#include "alc.h"  // <AL/alc.h>  if not on mac?
//#include <OpenAL/al.hs> //?? Not needed?

#include "AudioSource.h"


class AudioRenderer {
public:
    AudioRenderer(bool printMessages = true);
    ~AudioRenderer();

    bool getIsValid() const { return this->mIsValid; }

    void getListenerPosition(float& x, float& y, float& z);
    void getListenerVelocity(float& x, float& y, float& z);
    void getListenerOrientation(ALfloat* vectorOfSixFloats);
    float getListenerGain();

    void setListenerGain(float gain);

    //AudioSource* spawnAudioSource(float x, float y, float z, ALCchar* filepath);//Use this to spawn audio noises

    ///Idea for interface:
    /*
     create audio buffer -- return a handle to reference created buffer
     create audio source -- a source (position, parameters)



    */
private:
    ALCdevice* mDevice;
    ALCcontext* mContext;
    bool mIsValid;

    std::list<AudioSource*> sources;

    //Helper functions
    bool initialize(bool printMessages);

    bool checkForError(); ///Returns false if no error, true if there is an error

    void enumerateAudioDevices(std::vector<const ALCchar*>& devices);
    bool createAudioDevice(const ALCchar* deviceName);
    bool createAudioContext();

    bool configureListenerPosition();
    bool configureListenerGain(float gain);

    void parseDevicesIntoVector(std::vector<const ALchar*>&);
    void printNamesOfEnumeratedDevices(size_t, const std::vector<const ALCchar*>&) const;
};



#endif // AudioRenderer_h

#endif //0