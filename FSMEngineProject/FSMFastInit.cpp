

#include "FSMFastInit.h"

#include "FSMInitConfig.h"
#include "FSMRenderEnvironment.h"
#include "FSMInternalGlobalState.h"






//////////////////////////////////////////////////////////////////////////////////////~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~
/*                                                                                      **\   z  z  z  ~  ~
                              FSM_FAST_INIT_RETURN_BUNDLE_IMPLEMENTATION                *    z  z  z  ~  ~
\*                                                                                      **/   
//////////////////////////////////////////////////////////////////////////////////////~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~~~~









//////////////////////////////////////////////////////////////////////////////////////~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~
/*                                                                                      **\   z  z  z  ~  ~
                              FSM_FAST_INIT_FUNCTION_IMPLEMENTATION                *    z  z  z  ~  ~
\*                                                                                      **/
//////////////////////////////////////////////////////////////////////////////////////~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//////////////////////////////////////////////////////////////////////////////////////~~~~~~~

//Prototypes for the many differrent implementation functions
//These functions should apear in somewhat close to the order
//in which they are called

//  Pre-Setup Setup
std::unique_ptr<FSMFastInitReturnBundle> createAFastInitReturnBundle() noexcept;

//GO!
//Step 0-1) EnsureExpectedFileDirectoriesExist
bool verifyExistanceOfDefaultDirectories() noexcept;

bool checkForAnExistingDirectoryToAssetData() noexcept;
bool requestPermissionToCreateMissingDirectories() noexcept; //all directories when master directory didn't exist

bool checkForAnExistingShadersDirectory() noexcept;
bool requestPermissionToCreateMissingShadersDirectory() noexcept;

bool checkForAnExistingTexturesDirectory() noexcept;
bool requestPermissionToCreateMissingTexturesDirectory() noexcept;

bool checkForAnExistingFontsDirectory() noexcept;
bool requestPermissionToCreateMissingFontsDirectory() noexcept;

bool checkForAnExistingAudioDirectory() noexcept;
bool requestPermissionToCreateMissingAudioDirectory() noexcept;

bool checkForAnExistingSettingsDirectory() noexcept;
bool requestPermissionToCreateMissingSettingsDirectory() noexcept;

bool checkForAnExistingTexturesDirectory() noexcept;
bool requestPermissionToCreateMissingTexturesDirectory() noexcept;

//Step 0-2)  VerifyExistanceOfTheInitConfigFile
bool verifyExistanceOfTheInitConfigFile() noexcept;

//Step 0-2b) CreateANewInitConfigFileAndLoadIt();
bool createNewInitConfigFile() noexcept;

//Step 0-3  LoadTheInitConfigFile();
std::unique_ptr<FSMInitConfig> loadInitConfigFromFile() noexcept;

//Step 1-0  CreateTheRenderEnvironment   /*ExpectExceptions*/
std::unique_ptr<FSMRenderEnvironment> createRenderEnvironment(const FSMInitConfig&); /*THORWS EXCEPTIONS!*/



std::unique_ptr<FSMFastInitReturnBundle> FSM_DO_FAST_INIT() noexcept {
    LOG(TRACE) << __FUNCTION__;
    try {
        //Pre-Setup Setup
        std::unique_ptr<FSMFastInitReturnBundle> returnBundle = createAFastInitReturnBundle();

        //Step 0-1    Ensure Existance of expected file directories to loadable assets
        bool directoriesExist = verifyExistanceOfDefaultDirectories();
        if (!directoriesExist)
            returnBundle->
        //Step 0-2    Ensure Existance of InitConfig file
        if ()



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