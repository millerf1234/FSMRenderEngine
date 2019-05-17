//  File:                    Application.h
//  Class:                   Application
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

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "FSMRenderEnvironment.h"

class FSMInitConfig;

class Application final {
public:
    Application(int argc = 0, char** argv = nullptr);
    ~Application() noexcept;

    void launch();

    // Copying is not allowed 
    Application(const Application& that) = delete;
    Application& operator=(const Application& that) = delete;

private:
    //         Fields
    static const std::filesystem::path RELATIVE_PATH_TO_ASSET_DATA_DIRECTORY;
    static const std::filesystem::path RELATIVE_PATH_TO_SETTINGS_DIRECTORY;
    static const std::filesystem::path RELATIVE_PATH_TO_TEXTURES_DIRECTORY;
    static const std::filesystem::path RELATIVE_PATH_TO_SHADERS_DIRECTORY;
    static const std::filesystem::path RELATIVE_PATH_TO_FONTS_DIRECTORY;
    static const std::filesystem::path RELATIVE_PATH_TO_AUDIO_DIRECTORY;
    static const std::filesystem::path RELATIVE_PATH_TO_INIT_CONFIG_FILE;

    std::unique_ptr<FSMRenderEnvironment> mRenderEnvironment_ = nullptr;




    //         Private Member Functions
    bool setupMessageLogs(int argc, char** argv);
    void validateAssetDirectories() const noexcept;
    FSMInitConfig getInitConfig() const noexcept;   
    void logApplicationBuildAndConfigurationDetails() noexcept;
    bool createRenderEnvironment();
};

#endif //APPLICATION_H_