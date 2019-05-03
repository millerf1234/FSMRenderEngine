
#include "FSMAudioCommon.h"


namespace FSMEngineInternal {

    namespace FSMAudio {

        //OpenAL 1.1 spec section 6.3.5 -- Retrieving Enumeration Values
        // Quote:  "Enumeration / token values are device independent,
        //          but tokens defined for extensions might not be present
        //          for a given device. Using a NULL handle is legal, but
        //          only the tokens defined by the AL core are guaranteed.
        //          Availability of extension tokens depends on the ALC
        //          extension."
        // So it is possible that the context will report enum values beyond what
        // we are looking for in the base OpenAL 1.1 spec. To detect these values we
        // can ask the context to identify additional information about these enums.
        // 
        //  This function:
        //      Asks context to identify an enum value. Will return true if specified 
        //      enum is a legit enum and will set the parameter string to the enum's name,
        //      or will return false if no enum is detected and string will be empty.
        bool detectEnum

        bool checkOpenALErrorStack() noexcept {
            LOG(TRACE) << __FUNCTION__;
            ALCenum error = AL_NO_ERROR;
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