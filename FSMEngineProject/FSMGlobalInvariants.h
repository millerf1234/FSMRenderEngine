//                                                                                         
// File:                    FSMEngineInvariants.h                                          
//                                                                                         
// Desciption:              A collection of values that are invariant to FSMEngine. Any    
//                          variable defined in this file shall be considered invariant    
//                          across all FSMEngine-related code. As a consequence of         
//                          introducing invariants to the codebase, code can be written    
//                          which depends on these values existing and having the same     
//                          value each time the engine is used.                            
//                                                                                         
//                          Essentially DO NOT change these values without being prepared  
//                          to make potentially major and extensive modifications to the   
//                          code.                                                          
//                                                                                         
// Programmer:              Forrest Miller                                                 
// Date:                    April 2019                                                     
//                                                                                         
//

#pragma once

#ifndef FSM_ENGINE_INVARIANTS_H_
#define FSM_ENGINE_INVARIANTS_H_

#include <filesystem>

namespace FSMEngineInvariant {
    
    //////////////////////////////////
    //  Expected Asset Directories  //
    //////////////////////////////////

    const std::filesystem::path RELATIVE_PATH_TO_ASSET_DATA_DIRECTORY = "\\..\\AssetData";
    const std::filesystem::path RELATIVE_PATH_TO_TEXTURES_DIRECTORY = "\\..\\AssetData\\Textures";
    const std::filesystem::path RELATIVE_PATH_TO_SHADERS_DIRECTORY = "\\..\\AssetData\\Shaders";
    const std::filesystem::path RELATIVE_PATH_TO_FONTS_DIRECTORY = "\\..\\AssetData\\Fonts";
    const std::filesystem::path RELATIVE_PATH_TO_AUDIO_DIRECTORY = "\\..\\AssetData\\Audio";
    const std::filesystem::path RELATIVE_PATH_TO_SETTINGS_DIRECTORY = "\\..\\AssetData\\Settings";




    //////////////////////
    //  Expected Files  //
    //////////////////////

    const std::filesystem::path RELATIVE_PATH_TO_INIT_CONFIG_FILE = "\\..\\AssetData\\Settings\\InitConfig.json";


    // /--------------------------------------\  
    // |          OpenGL Invariants           |  
    // \--------------------------------------/  
    //static constexpr const 
    static constexpr const int DEFAULT_GL_VERSION_MAJOR = 4;
    
    
    // /--------------------------------------\  
    // |         Joystick Invariants          |  
    // \--------------------------------------/  
    
    
    
    //  ________________________________________________________________  //
    //                                                                    //
    //                  Member Data Accessor Function(s)                  //
    //  ________________________________________________________________  //
    //                                                                    //

     
     
     
} //namespace FSMEngineInvariant

#endif //FSM_ENIGNE_INVARIANTS_H_