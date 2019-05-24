//  File:                    ApplicationOld.h
//  Class:                   ApplicationOld
//
//  This class is still undergoing development so no specific description is
//  yet available. The overall idea here though is to provide the 'main()'
//  function with something to invoke. Early initialization steps are performed 
//  by this class itself enough to get many of the core systems online. After 
//  completing its initialization, this class will invoke either 
//           (Option A) a user-interactive menu/titlescreen, or
//           (Option B) directly enter into a render demo
//                           
// Programmer:   Forrest Miller                           
// Date:         January - May 2019
//

#pragma once

#ifndef APPLICATION_OLD_H_
#define APPLICATION_OLD_H_

#include "FSMRenderEnvironment.h"

class FSMInitConfig;

class ApplicationOld final {
public:
    ApplicationOld(int argc = 0, char** argv = nullptr);
    ~ApplicationOld() noexcept;

    void launch();

    // Copying is not allowed 
    ApplicationOld(const ApplicationOld& that) = delete;
    ApplicationOld& operator=(const ApplicationOld& that) = delete;

private:
    //Static Members

    static const std::filesystem::path RELATIVE_PATH_TO_ASSET_DATA_DIRECTORY;
    static const std::filesystem::path RELATIVE_PATH_TO_SETTINGS_DIRECTORY;
    static const std::filesystem::path RELATIVE_PATH_TO_TEXTURES_DIRECTORY;
    static const std::filesystem::path RELATIVE_PATH_TO_SHADERS_DIRECTORY;
    static const std::filesystem::path RELATIVE_PATH_TO_FONTS_DIRECTORY;
    static const std::filesystem::path RELATIVE_PATH_TO_AUDIO_DIRECTORY;

    static const std::filesystem::path RELATIVE_PATH_TO_INIT_CONFIG_FILE;

    //         Fields


    //Store the current filepath at the time the executable is launched.
    //Note: https://en.cppreference.com/w/cpp/filesystem/current_path has me
    //      worried that relying on 'current_path()' is potentially insecure.
    //      My current work around is to try to set the 'current_path()' as
    //      early as possible in the program's lifetime.
    const std::filesystem::path ABSOLUTE_FILEPATH_TO_EXECUTABLE;

    std::unique_ptr<FSMRenderEnvironment> mRenderEnvironment_ = nullptr;

    //         Private Member Functions
    bool setupMessageLogs(int argc, char** argv);
    void validateAssetDirectories() const noexcept;
    FSMInitConfig getInitConfig() const noexcept;   
    void logApplicationBuildAndConfigurationDetails() noexcept;
    bool createRenderEnvironment();
};

#endif //APPLICATION_OLD_H_