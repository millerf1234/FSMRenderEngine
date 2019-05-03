


#include "FSMAudioTest.h"

#include <vector>

namespace FSMEngineInternal {

    namespace FSMAudio {
        
        static constexpr const size_t ENUMERATED_AUDIO_DEVICE_NAME_LENGTH_LIMIT = 2048u;

        typedef struct EnumeratedAudioDevice {
            ALchar nameBuffer[ENUMERATED_AUDIO_DEVICE_NAME_LENGTH_LIMIT];
            EnumeratedAudioDevice() { 
                LOG(TRACE) << __FUNCTION__;
                for (size_t i = 0u; i < ENUMERATED_AUDIO_DEVICE_NAME_LENGTH_LIMIT; i++)
                    nameBuffer[i] = '\0';
            }
        } EnumeratedAudioDevice;

        bool checkForAudioDeviceEnumerationExtension() noexcept;
        std::vector<EnumeratedAudioDevice> enumerateAudioDevices() noexcept;
        void logEnumeratedDevices(const std::vector<EnumeratedAudioDevice>&) noexcept;

        bool FSMAudioTest::initialized = false;

        bool FSMAudioTest::initializeAudio(AudioConfiguration config) {
            LOG(TRACE) << __FUNCTION__;
            if (checkForAudioDeviceEnumerationExtension()) {
                //Enumerate Devices
                auto devices = enumerateAudioDevices();
                logEnumeratedDevices(devices);
            }
            else {
                //We are forced to use the implementation's designated default device 

            }
            return false;
        }



        bool checkForAudioDeviceEnumerationExtension() noexcept {
            LOG(TRACE) << __FUNCTION__;

            ALboolean enumerationAvailable = AL_FALSE;

            //First parameter as NULL means we want to retrieve information
            //from all available devices
            enumerationAvailable = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");

            if (checkOpenALErrorStack())
                return false;

            if (AL_TRUE == enumerationAvailable)
                return true;
            else
                return false;
        }

        std::vector<EnumeratedAudioDevice> enumerateAudioDevices() noexcept {
            LOG(TRACE) << __FUNCTION__;
            std::vector<EnumeratedAudioDevice> devices;
            const ALchar* deviceNameList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
            if (checkOpenALErrorStack()) { return devices; }
            
            //still more todo

            return devices;
        }

        void logEnumeratedDevices(const std::vector<EnumeratedAudioDevice>& audioDeviceNames) noexcept {
            LOG(TRACE) << __FUNCTION__;
            LOG(INFO) << "Detected the following Audio Devices: ";
            for (auto device : audioDeviceNames)
                LOG(INFO) << "  " << device.nameBuffer;
            LOG(INFO) << "\n\n";
        }


    } //namespace FSMAudio

} //namespace FSMEngineInternal


/* 
void AudioRenderer::parseDevicesIntoVector(std::vector<const ALchar*>& devices) {
    const ALchar * deviceNames = alcGetString(NULL, ALC_DEVICE_SPECIFIER); //Get device list
    if (checkForError()) { return; }
    bool reachedEndOfList = false;
    size_t nameLength = 0;
    std::ostringstream deviceName;
    const ALchar * nameIterator, *nextAfterNameIterator;
    nameIterator = deviceNames;
    nextAfterNameIterator = (deviceNames + 1);
    do {
        if ( (*nameIterator) == '\0') {
            nameLength = 0;
            static std::vector<std::string> names;
            names.push_back(deviceName.str());
            std::vector<std::string>::iterator mostRecentName = names.end();
            mostRecentName--; //Move iterator to be at the last item in the vector
            devices.push_back(static_cast<const ALchar *>((*mostRecentName).c_str()));
            if (devices.size() > MAX_DEVICE_NAMES_TO_RECORD) {
                std::cout << "\n\nWarning! Excessive number of Audio Devices detected!\n";
                std::cout << "Only the first " << MAX_DEVICE_NAMES_TO_RECORD;
                std::cout << " devices will be recorded!" << std::endl;
                return;
            }
            //clear the stringstream
            std::ostringstream().swap(deviceName);

            //Check to see if we reached the end of the list
            if ( (*nextAfterNameIterator) == '\0' ) {
                reachedEndOfList = true;
                continue;
            }
        }
        else {
            deviceName << (*nameIterator);
            if (deviceName.str().size() > MAX_DEVICE_NAME_LENGTH) {
                std::cout << "\nWARNING! One of the detected audio devices ";
                std::cout << "has a name that exceeds the maximum name limit\n";
                deviceName << '\0'; //Give the device name a terminating character
                std::cout << "\nThe device name that is causing the issue is: \n";
                std::cout << deviceName.str() << std::endl;
                return;
            }
            nameIterator = nextAfterNameIterator++;
        }
    } while (!reachedEndOfList);
}
*/