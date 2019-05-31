
#include "FSMAudioCommon.h"

#if 0

namespace FSMEngineInternal {

    namespace FSMAudio {


        //ALERT!!! This Function's defineition is incomplete still and is a work in progress
        bool checkOpenALCErrorStack(ALCdevice* device) noexcept {
            LOG(TRACE) << __FUNCTION__;
            //IF device is nullptr, then throw an error

            ALCenum error = ALC_NO_ERROR;
            error = alcGetError(device); //This pops the error stack
            if (error != ALC_NO_ERROR) {
                std::ostringstream errMsg;
                //errMsg << "Detected that an error has occured in the audio language.\n";
                errMsg << "  The Error Code is 0x" << std::hex << error << "\n";
                std::string nameOfError("");
                switch (error) {
                case ALC_INVALID:
                    nameOfError = "\"AL_INVALID\"";
                    break;
                case ALC_INVALID_DEVICE:
                    nameOfError = "\"ALC_INVALID_DEVICE\"";
                    break;
                case ALC_INVALID_CONTEXT:
                    nameOfError = "\"ALC_INVALID_CONTEXT\"";
                    break;
                case ALC_INVALID_ENUM:
                    nameOfError = "\"AL_INVALID_ENUM\"";
                    break;
                case ALC_INVALID_VALUE:
                    nameOfError = "\"ALC_INVALID_VALUE\"";
                    break;
                case ALC_OUT_OF_MEMORY:
                    nameOfError = "\"ALC_OUT_OF_MEMORY\"";
                    break;
                default:
                    nameOfError = "[UNEXPECTED ENUM]";
                    break;
                }
                errMsg << "    This Error Code Corresponds to the error:  " << nameOfError << "\n\n";
                LOG(ERROR) << errMsg.str();
                return true;
            }
            return false;
        }



        bool checkOpenALErrorStack() noexcept {
            LOG(TRACE) << __FUNCTION__;
            ALenum error = AL_NO_ERROR;
            error = alGetError(); //This pops the error stack
            if (error != AL_NO_ERROR) {
                std::ostringstream errMsg;
                errMsg << "Detected that an error has occured in the audio language.\n";
                errMsg << "  The Error Code is 0x" << std::hex << error << "\n";
                std::string nameOfError("");
                switch (error) {
                case AL_INVALID:
                    nameOfError = "\"AL_INVALID\"";
                    break;
                case AL_INVALID_NAME:
                    nameOfError = "\"AL_INVALID_NAME\"";
                    break;
                case AL_INVALID_ENUM:
                    nameOfError = "\"AL_INVALID_ENUM\"";
                    break;
                case AL_INVALID_VALUE:
                    nameOfError = "\"AL_INVALID_VALUE\"";
                    break;
                case AL_INVALID_OPERATION:
                    nameOfError = "\"AL_INVALID_OPERATION\"";
                    break;
                case AL_OUT_OF_MEMORY:
                    nameOfError = "\"AL_OUT_OF_MEMORY\"";
                    break;
                default:
                    nameOfError = "[UNEXPECTED ENUM]";
                    break;
                }
                errMsg << "    This Error Code Corresponds to the error:  " << nameOfError << "\n\n";
                LOG(ERROR) << errMsg.str();
                return true;
            }
            return false;
        }


        AudioConfiguration::ListenerConfiguration::ListenerConfiguration(ALfloat listenerGain) noexcept {
            LOG(TRACE) << __FUNCTION__;

            //Set Gain 
            if (listenerGain < 0.0f || listenerGain == NAN)
                gain = 0.0f;
            else
                gain = listenerGain;

            //Set Listener Position
            pos[0] = 0.0f;
            pos[1] = 0.0f;
            pos[2] = 0.0f;

            //Set Listener Velocity
            vel[0] = 0.0f;
            vel[1] = 0.0f;
            vel[2] = 0.0f;

            //Set Listener Orientation
            std::array<ALfloat, 3> forward = { 0.0f,      // X    
                                               0.0f,      // Y   
                                               1.0f };    // Z   
            std::array<ALfloat, 3> up = { 0.0f,           // X  
                                          1.0f,           // Y
                                          0.0f };         // Z
            orientation = { forward[0], 
                            forward[1],
                            forward[2],
                            up[0],
                            up[1],
                            up[2] };
        };
    



        class AudioContext final {
            public:


            private:
                static size_t activeAudioContexts;
                ALCcontext* mContext;

        };

        size_t AudioContext::activeAudioContexts = 0u;

        class AudioDevice final {

        };

        class AudioListener final {

        };


    }  //namespace FSMAudio

}  //namespace FSMEngineInternal


#endif //#if 0