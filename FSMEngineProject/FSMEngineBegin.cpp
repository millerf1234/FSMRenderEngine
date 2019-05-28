
#include "FSMEngineBegin.h"
#include "GraphicsLanguage.h"
#include "FSMException.h"
#include "FSMInitConfig.h"

//Must define following macro before including header
#define ALLOW_ELCC_CONFIGURATION 1
#include "EasyLogConfiguration.h"

#include "FSMGlobalState.h"



//And here we have the implementation functions
std::filesystem::path getPathToTheCurrentlyActiveFilesystemDirectory() noexcept;
void fabricateACompleteGlobalState() noexcept;
void setActiveFilesystemDirectoryInGlobalState(std::filesystem::path pathToDir) noexcept;
bool enableLoggingFunctionality(int argc, const char** argv) noexcept;
void updateLoggingStatusInGlobalState() noexcept;


void FSM_ENGINE_BEGIN(int argc, const char** argv) noexcept {
    auto dirPath = getPathToTheCurrentlyActiveFilesystemDirectory();
    fabricateACompleteGlobalState();
    setActiveFilesystemDirectoryInGlobalState(dirPath);

    if (!enableLoggingFunctionality(argc, argv)) {
        fprintf(stderr, "\nAn error was encountered while attempting to initialize\n"
            "the logging functionality.\n\n    TIME TO CRASH!\n");
        std::exit(EXIT_FAILURE);
    }
    LOG(TRACE) << __FUNCTION__;

    void updateLoggingStatusInGlobalState() noexcept;
}



std::filesystem::path getPathToTheCurrentlyActiveFilesystemDirectory() noexcept {
    std::error_code ec;
    auto path = std::filesystem::current_path(ec);
    if (ec.operator bool()) {
        fprintf(stderr, "An error was encountered accessing the filesystem\n"
            "very early in this executables lifetime.\nError has value %d and MSG reports as: \n%s\n",
            ec.value(), ec.message());
        std::exit(EXIT_FAILURE);
    }
    return path;
}


void fabricateACompleteGlobalState() noexcept {
    //Here goes nothing...
    FSMEngineInternal::pGlobalState = &(FSMEngineInternal::createTheInitialGlobalState());

    //Confirm the global state is ready by having it mark itself as ready
    FSMEngineInternal::pGlobalState->initializationStatus.completeGlobalStateSet = true;
}


void setActiveFilesystemDirectoryInGlobalState(std::filesystem::path pathToDir) noexcept {
    FSMEngineInternal::pGlobalState->currentlyActiveDirectory.pathToDirectory = pathToDir;
}



bool enableLoggingFunctionality(int argc, char** argv) noexcept {
    //There is always a chance something might go screwy here with 
    //initializing the logging functionality but we are deep in 
    //'noexcept' land, so all exceptions must end here with us.
    try {
        initializeEasyLogger(argc, argv);
        if (configureEasyLogger())
            LOG(INFO) << "Logging Functionality Enabled...\n";
        else
            return false;

        LOG(TRACE) << __FUNCTION__;

        return true;



    }
    catch (const FSMNamedException& e) {
        switch (e.getName().name()) {
        default:
            fprintf(stderr, "Unrecognized FSMNamedException was caught while initializing\n"
                "logging functionality. The exceptions name is: \"%s\"\n", e.getNameStr());
            break;
        case FSMNamedException::ExceptionName::LOGGING_INIT_FAILURE:
            fprintf(stderr, "Well golly gee look at that! It looks like the logging failed to init.\n"
                "I guess the chances of anything else working right from here are slim.\n"
                "\n\n    TIME TO CRASH!\n");
            std::cin.get();
            std::exit(EXIT_FAILURE);
            
        }
        
        if (e.isFatal())
            goto doom;
        else 
            goto gloom ;
    }
    catch (const FSMException& e) {
        fprintf(stderr, "\nA FSMNAmedException was caught while trying to initialize logging.\n"
            "Here is its message: %s\n", e.what());

        fprintf(stderr, "\n\n\nThings are looking pretty doomed from here...\n");
        goto doom;
    }

   
gloom:   //It turns out doom and gloom are not that different of fates
doom:
    fprintf(stderr, "\nBest course of action from this point forward is to crash. Here goes...\n");
    fprintf(stderr, "\n    [Press Enter To Acknowledge Your Doom]  \n");
    std::cin.get();
    std::exit(EXIT_FAILURE);
}


////////////////////
////   So from This point onwards we know Logging functionality is available

void updateLoggingStatusInGlobalState() noexcept {
    LOG(TRACE) << __FUNCTION__;
    FSMEngineInternal::pGlobalState->initializationStatus.loggingInitialized = true;
}