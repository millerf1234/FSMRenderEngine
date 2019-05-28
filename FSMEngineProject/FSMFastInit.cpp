

#include "FSMFastInit.h"

#include "FSMGlobalState.h"


typedef std::unique_ptr<FSMFastInitReturnBundle>  ReturnBundlePtr;

//Prototypes for the many differrent implementation functions
//These functions should apear in somewhat close to the order
//in which they are called


ReturnBundlePtr createAFastInitReturnBundle() noexcept;


std::unique_ptr<FSMFastInitReturnBundle> FSM_DO_FAST_INIT(int argc, char* argv) noexcept {

    


    ReturnBundlePtr fiBundle;
   


    return fiBundle;
}





ReturnBundlePtr createAFastInitReturnBundle() noexcept {
    try {
        std::unique_ptr<FSMFastInitReturnBundle> FastInitBundle = std::make_unique<FSMFastInitReturnBundle>();
        return FastInitBundle;
    }
    catch (const std::exception& e) {
        fprintf(stderr, "\nException caught while allocating memory!\n"
            "Exception Message: %s\n", e.what());
        fprintf(stderr, "This is a bad thing to have happen!\n"
            "This program will now abort!\n");
        std::exit(EXIT_FAILURE);
    }
}