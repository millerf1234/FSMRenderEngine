

#include "FSMFastInit.h"

#include "FSMInitConfig.h"
#include "FSMRenderEnvironment.h"
#include "FSMInternalGlobalState.h"
#include "FSMGlobalInvariants.h"

#include "FSMInitFailureReporter.h"

#include "JsonParser.h"

using namespace FSMEngineInternal;
using namespace FSMEngineInvariant;





//////////////////////////////////////////////////////////////////////////////////////~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~
/*                                                                                      **\   z  z  z  ~  ~
                              FSM_FAST_INIT_FUNCTION_IMPLEMENTATION                     *    z  z  z  ~  ~
\*                                                                                      **/
//////////////////////////////////////////////////////////////////////////////////////~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~~~~

//Prototypes for the many differrent implementation functions
//These functions should appear in somewhat close to the order
//in which they are called

//  Pre-Setup Setup
std::unique_ptr<FSMFastInitReturnBundle> createAFastInitReturnBundle() noexcept;
bool checkToMakeSureReadyToCallFastInitFunc() noexcept;

//GO!
//Step 0-1) EnsureExpectedFileDirectoriesExist
bool verifyExistanceOfDefaultDirectories() noexcept;

bool checkForAnExistingDirectoryToAssetData() noexcept;
bool requestPermissionToCreateMissingDirectories() noexcept; //all directories when master directory didn't exist
void createAllDirectories(); /* THROWS EXCEPTIONS */

bool checkForAnExistingTexturesDirectory() noexcept;
bool requestPermissionToCreateMissingTexturesDirectory() noexcept;

bool checkForAnExistingShadersDirectory() noexcept;
bool requestPermissionToCreateMissingShadersDirectory() noexcept;

bool checkForAnExistingFontsDirectory() noexcept;
bool requestPermissionToCreateMissingFontsDirectory() noexcept;

bool checkForAnExistingAudioDirectory() noexcept;
bool requestPermissionToCreateMissingAudioDirectory() noexcept;

bool checkForAnExistingSettingsDirectory() noexcept;
bool requestPermissionToCreateMissingSettingsDirectory() noexcept;



//Step 0-2)  VerifyExistanceOfTheInitConfigFile
bool verifyExistanceOfTheInitConfigFile() noexcept;

//Step 0-2b) CreateANewInitConfigFile
bool createNewInitConfigFile() noexcept;

//Step 0-3  LoadTheInitConfigFile
std::unique_ptr<FSMInitConfig> loadInitConfigFromFile() noexcept;

//Step 1-0  CreateTheRenderEnvironment   /*ExpectExceptions*/
std::unique_ptr<FSMRenderEnvironment> createRenderEnvironment(const FSMInitConfig&); /*THORWS EXCEPTIONS!*/




typedef FSMFastInitReturnBundle::FastInitializationStatus InitStatus;
typedef FSMGlobalState::InitializationStatus::INIT_METHOD InitMethod;

std::unique_ptr<FSMFastInitReturnBundle> FSM_DO_FAST_INIT() noexcept {
    assert(FSMEngineInternal::pGlobalState != nullptr); //Must call FSM_ENGINE_BEGIN() before this function
    assert(pGlobalState->initializationStatus.loggingInitialized); //Must have logging initialized
    LOG(TRACE) << __FUNCTION__;
    
    try {
        //Pre-Setup Setup
        FSMInitFailureReporter failureReport;
        auto returnBundle = createAFastInitReturnBundle();
        if (!checkToMakeSureReadyToCallFastInitFunc()) {
            returnBundle->setInitializationStatus(InitStatus::FAILED);
            failureReport.addFailure(PointOfFailure::INVALID_PROGRAM_STATE_TO_CALL_INIT);
            returnBundle->failures = failureReport.createReport();
            return std::move(returnBundle);
        }
        pGlobalState->initializationStatus.choosenInitMethod = InitMethod::FAST_INIT;


        //Step 0-1    Ensure Existance of expected file directories to loadable assets
        bool directoriesExist = verifyExistanceOfDefaultDirectories();
        if (!directoriesExist) {
            returnBundle->setInitializationStatus(InitStatus::FAILED);
            failureReport.addFailure(PointOfFailure::CREATING_DEFAULT_DIRECTORIES);
            returnBundle->failures = failureReport.createReport();
            return std::move(returnBundle);
        }
        //Step 0-2    Ensure Existance of InitConfig file
        if (!verifyExistanceOfTheInitConfigFile()) {
            //Step 0-2b   CreateANewInitConfigFile
            if (!createNewInitConfigFile()) {
                returnBundle->setInitializationStatus(InitStatus::FAILED);
                failureReport.addFailure(PointOfFailure::LOADING_ENGINE_SETTINGS);
                returnBundle->failures = failureReport.createReport();
                return std::move(returnBundle);
            }
        }
        //Step 0-3   Load the Init Config File
        std::unique_ptr<FSMInitConfig> loadedConfig = loadInitConfigFromFile();
        if (!loadedConfig) {
            returnBundle->setInitializationStatus(InitStatus::FAILED);
            failureReport.addFailure(PointOfFailure::LOADING_ENGINE_SETTINGS);
            returnBundle->failures = failureReport.createReport();
            return std::move(returnBundle);
        }

        //Step 1-0   CreateTheRenderEnvironment
        try {
          //auto rendEnv = createRenderEnvironment(*loadedConfig);


        } 
        //Catch a lot of exceptions here and keep updating the init config to overcome issues
        catch (const FSMException& e) {
            (void)e;
        }


        return std::move(returnBundle);
    } catch (const std::bad_alloc& e) {
        //If this actually happens then program is pretty screwed...
        LOG(ERROR) << "Bad Allocation!\n";
        std::exit(EXIT_FAILURE);
    }
}




std::unique_ptr<FSMFastInitReturnBundle> createAFastInitReturnBundle() noexcept {
    LOG(TRACE) << __FUNCTION__;
    try {
        std::unique_ptr<FSMFastInitReturnBundle> FastInitBundle = std::make_unique<FSMFastInitReturnBundle>();
        FastInitBundle->setInitializationStatus(InitStatus::NOT_READY);
        return std::move(FastInitBundle);
    }
    catch (const std::exception& e) {
        fprintf(stderr, "\nException caught while allocating memory!\n"
            "Exception Message: %s\n", e.what());
        fprintf(stderr, "This is a bad thing to have happen!\n"
            "This program will now abort!\n");
        std::exit(EXIT_FAILURE);
    }
}

bool checkToMakeSureReadyToCallFastInitFunc() noexcept {
    LOG(TRACE) << __FUNCTION__;
    //if (!pGlobalState)
    //    return false; 
    //If an init method has already been choosen
    if (pGlobalState->initializationStatus.choosenInitMethod != InitMethod::UNCHOSEN) {
        return false;
    }
    return true;
}


//Step 0-1) EnsureExpectedFileDirectoriesExist
bool verifyExistanceOfDefaultDirectories() noexcept {
    LOG(TRACE) << __FUNCTION__;
    
    if (!checkForAnExistingDirectoryToAssetData()) {
        if (requestPermissionToCreateMissingDirectories())
            return true;
        else
            return false;
    }

    if (!checkForAnExistingTexturesDirectory()) {
        if (requestPermissionToCreateMissingTexturesDirectory())
            return true;
        else
            return false;
    }

    if (!checkForAnExistingShadersDirectory()) {
        if (requestPermissionToCreateMissingShadersDirectory())
            return true;
        else
            return false;
    }

    if (!checkForAnExistingFontsDirectory()) {
        if (requestPermissionToCreateMissingFontsDirectory())
            return true;
        else
            return false;
    }

    if (!checkForAnExistingAudioDirectory()) {
        if (requestPermissionToCreateMissingAudioDirectory())
            return true;
        else
            return false;
    }

    if (!checkForAnExistingSettingsDirectory()) {
        if (requestPermissionToCreateMissingSettingsDirectory())
            return true;
        else
            return false;
    }

    return true;
}

bool checkForAnExistingDirectoryToAssetData() noexcept {
    LOG(TRACE) << __FUNCTION__;
    std::filesystem::path path = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_ASSET_DATA_DIRECTORY.string();
    path = path.lexically_normal();
    return std::filesystem::exists(path);
}
bool requestPermissionToCreateMissingDirectories() noexcept { //all directories when master directory didn't exist
    LOG(TRACE) << __FUNCTION__;
    LOG(INFO) << "Unable to detect the existance of any of the default file";
    LOG(INFO) << "directories for loading assets. These directories are required";
    LOG(INFO) << "in order for the FSMEngine to operate properly.\n";
    LOG(INFO) << "Directories will be created at:";
    try {
        std::filesystem::path path = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_ASSET_DATA_DIRECTORY.string();
        path = path.lexically_normal();
        LOG(INFO) << "         " << path.string();
        LOG(INFO) << "   [Press ENTER to create directories]\n";
        std::cin.get();
        LOG(INFO) << "    Creating Directories for Assets!\n";

        createAllDirectories();
        return true;
    }
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while attempting to create all asset directories!";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}

void createAllDirectories() {
    LOG(TRACE) << __FUNCTION__;

    std::filesystem::path path = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_ASSET_DATA_DIRECTORY.string();
    path = path.lexically_normal();
    LOG(INFO) << "   Creating Assets Directory";
    std::filesystem::create_directory(path);

    std::filesystem::path pathToTextures = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_TEXTURES_DIRECTORY.string();
    pathToTextures = pathToTextures.lexically_normal();
    LOG(INFO) << "      Creating Directory for Textures";
    std::filesystem::create_directory(pathToTextures);

    std::filesystem::path pathToShaders = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_SHADERS_DIRECTORY.string();
    pathToShaders = pathToShaders.lexically_normal();
    LOG(INFO) << "      Creating Directory for Shaders";
    std::filesystem::create_directory(pathToShaders);

    std::filesystem::path pathToFonts = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_FONTS_DIRECTORY.string();
    pathToFonts = pathToFonts.lexically_normal();
    LOG(INFO) << "      Creating Directory for Fonts";
    std::filesystem::create_directory(pathToFonts);

    std::filesystem::path pathToAudio = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_AUDIO_DIRECTORY.string();
    pathToAudio = pathToAudio.lexically_normal();
    LOG(INFO) << "      Creating Directory for Audio";
    std::filesystem::create_directory(pathToAudio);

    std::filesystem::path pathToSettings = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_SETTINGS_DIRECTORY.string();
    pathToSettings = pathToSettings.lexically_normal();
    LOG(INFO) << "      Creating Directory for Settings";
    std::filesystem::create_directory(pathToSettings);

}

bool checkForAnExistingTexturesDirectory() noexcept {
    LOG(TRACE) << __FUNCTION__;
    try {
        std::filesystem::path pathToTextures = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_TEXTURES_DIRECTORY.string();
        pathToTextures = pathToTextures.lexically_normal();
        return std::filesystem::exists(pathToTextures);
    }
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while verifying existence of \'Textures\' directory!";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}
bool requestPermissionToCreateMissingTexturesDirectory() noexcept {
    LOG(TRACE) << __FUNCTION__;
    LOG(INFO) << "Unable to detect existance of a directory for Texture Assets!";
    LOG(INFO) << "  [Press ENTER to generate missing directory]\n";
    std::cin.get();
    try {
        std::filesystem::path pathToTextures = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_TEXTURES_DIRECTORY.string();
        pathToTextures = pathToTextures.lexically_normal();
        LOG(INFO) << "      Creating Directory for Textures";
        std::filesystem::create_directory(pathToTextures);
    }
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while attempting to create a directory for Textures";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}

bool checkForAnExistingShadersDirectory() noexcept {
    LOG(TRACE) << __FUNCTION__;
    try {
        std::filesystem::path pathToShaders = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_SHADERS_DIRECTORY.string();
        pathToShaders = pathToShaders.lexically_normal();
        return std::filesystem::exists(pathToShaders);
    }
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while attempting to verify existence of the \'Shaders\' directory";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}
bool requestPermissionToCreateMissingShadersDirectory() noexcept {
    LOG(TRACE) << __FUNCTION__;
    LOG(INFO) << "Unable to detect existance of a directory for Shader Assets!\n";
    LOG(INFO) << "  [Press ENTER to generate missing directory]\n";
    std::cin.get();
    try {
        std::filesystem::path pathToShaders = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_SHADERS_DIRECTORY.string();
        pathToShaders = pathToShaders.lexically_normal();
        LOG(INFO) << "      Creating Directory for Shaders";
        std::filesystem::create_directory(pathToShaders);
        return true;
    }
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while attempting to create a directory for \'Shaders\'";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}

bool checkForAnExistingFontsDirectory() noexcept {
    LOG(TRACE) << __FUNCTION__;
    try {
        std::filesystem::path pathToFonts = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_FONTS_DIRECTORY.string();
        pathToFonts = pathToFonts.lexically_normal();
        return std::filesystem::exists(pathToFonts);
    }
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while attempting to verify the existence of the \'Fonts\' directory";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}
bool requestPermissionToCreateMissingFontsDirectory() noexcept {
    LOG(TRACE) << __FUNCTION__;
    LOG(INFO) << "Unable to detect existance of a directory for Font Assets!\n";
    LOG(INFO) << "  [Press ENTER to generate missing directory]\n";
    std::cin.get();
    try {
        std::filesystem::path pathToFonts = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_FONTS_DIRECTORY.string();
        pathToFonts = pathToFonts.lexically_normal();
        LOG(INFO) << "      Creating Directory for Fonts";
        std::filesystem::create_directory(pathToFonts);
        return true;
    }
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while attempting to create the missing \'Fonts\' directory";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}

bool checkForAnExistingAudioDirectory() noexcept {
    LOG(TRACE) << __FUNCTION__;
    try {
        std::filesystem::path pathToAudio = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_AUDIO_DIRECTORY.string();
        pathToAudio = pathToAudio.lexically_normal();
        return std::filesystem::exists(pathToAudio);
    } 
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while attempting to verify existence of the \'Audio\' directory";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}
bool requestPermissionToCreateMissingAudioDirectory() noexcept {
    LOG(TRACE) << __FUNCTION__;
    LOG(INFO) << "Unable to detect existance of a directory for Audio Assets!\n";
    LOG(INFO) << "  [Press ENTER to generate missing directory]\n";
    std::cin.get();
    try {
        std::filesystem::path pathToAudio = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_AUDIO_DIRECTORY.string();
        pathToAudio = pathToAudio.lexically_normal();
        LOG(INFO) << "      Creating Directory for Audio";
        std::filesystem::create_directory(pathToAudio);
        return true;
    }
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while attempting to create the missing \'Audio\' directory";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}

bool checkForAnExistingSettingsDirectory() noexcept {
    LOG(TRACE) << __FUNCTION__;
    try {
        std::filesystem::path pathToSettings = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_SETTINGS_DIRECTORY.string();
        pathToSettings = pathToSettings.lexically_normal();
        return std::filesystem::exists(pathToSettings);
    }
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while checking for the existence of the \'Settings\' directory!";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}
bool requestPermissionToCreateMissingSettingsDirectory() noexcept {
    LOG(TRACE) << __FUNCTION__;
    LOG(INFO) << "Unable to detect existance of a directory for Settings Assets!\n";
    LOG(INFO) << "  [Press ENTER to generate missing directory]\n";
    std::cin.get();
    try {
        std::filesystem::path pathToSettings = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_SETTINGS_DIRECTORY.string();
        pathToSettings = pathToSettings.lexically_normal();
        LOG(INFO) << "      Creating Directory for Settings";
        std::filesystem::create_directory(pathToSettings);
        return true;
    }
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while attempting to create the missing \'Settings\' directory!";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}

//Step 0-2)  VerifyExistanceOfTheInitConfigFile
bool verifyExistanceOfTheInitConfigFile() noexcept {
    LOG(TRACE) << __FUNCTION__;
    try {
        std::filesystem::path path = pGlobalState->currentlyActiveDirectory.pathToDirectory.string() + RELATIVE_PATH_TO_INIT_CONFIG_FILE.string();
        path = path.lexically_normal();
        return std::filesystem::exists(path);
    }
    catch (const std::filesystem::filesystem_error& e) {
        LOG(ERROR) << "An Error Occured while attempting to verify existence of the InitConfig file!\n";
        LOG(ERROR) << "Exception Message: " << e.what();
        return false;
    }
}

//Step 0-2b) CreateANewInitConfigFile
bool createNewInitConfigFile() noexcept {
    LOG(TRACE) << __FUNCTION__;

    return true;
}

//Step 0-3  LoadTheInitConfigFile
std::unique_ptr<FSMInitConfig> loadInitConfigFromFile() noexcept {
    LOG(TRACE) << __FUNCTION__;

    return nullptr;
}