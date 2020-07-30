

#include "Application.h"


Application::Application(int argc, const char** argv) noexcept {
    
    mBundle_ = nullptr;

    if (!FSM_ENGINE_BEGIN(argc, argv))
        std::exit(EXIT_FAILURE);
    LOG(TRACE) << __FUNCTION__;
}


Application::~Application() noexcept {
    LOG(TRACE) << __FUNCTION__;
}

void Application::launch() noexcept {
    LOG(TRACE) << __FUNCTION__;

    mBundle_ = FSM_DO_FAST_INIT(); //Here is where the black voodoo magic happens
    
    if (mBundle_->failures)
        reportFastInitFailures(*mBundle_);
      
    
    else {
        //We are ready to go...
        LOG(INFO) << "\nPretend the Application launches and starts doing really cool things here...\n";

    }

}


void Application::reportFastInitFailures(const FSMFastInitReturnBundle& bundle) noexcept {
    LOG(TRACE) << __FUNCTION__;
   
    if (!bundle.failures.has_value())
        return;
    if (bundle.failures->getNumFailures() < 1)
        assert(true); //The optional containing the failureReport should not exist if no failures were reported

    std::ostringstream fastInitFailureReport;
    if (bundle.failures->getNumFailures() == 1)
        fastInitFailureReport << "1 Error";
    else
        fastInitFailureReport << bundle.failures->getNumFailures() << " Errors";
    fastInitFailureReport << " were encountered within FSM_DO_FAST_INIT().\n";
    fastInitFailureReport << "Failures were encountered while performing the following tasks:\n";

    auto failureIterator = bundle.failures->getFailureCIterator();
    do {
        fastInitFailureReport << "  - ";
        switch (*failureIterator) {
        default:
            fastInitFailureReport << "INVALID_ENUM\n";
            break;   
        case PointOfFailure::INVALID_PROGRAM_STATE_TO_CALL_INIT:
            fastInitFailureReport << "The Global State Forbids FAST_INIT()\n";
            break;
        case PointOfFailure::CREATING_DEFAULT_DIRECTORIES:
            fastInitFailureReport << "Loading Engine Settings File\n";
            break;
        case PointOfFailure::LOADING_ENGINE_SETTINGS:
            fastInitFailureReport << "Loading Engine Settings File\n";
            break;
        case PointOfFailure::LOADING_OTHER_SETTINGS:
            fastInitFailureReport << "Loading An Unspecified File\n";
            break;
        case PointOfFailure::SETTING_CALLBACKS:
            fastInitFailureReport << "Setting Callback Functions\n";
            break;
        case PointOfFailure::GLFW_INIT:
            fastInitFailureReport << "Calling GLFW's Init Function\n";
            break;
        case PointOfFailure::CONTEXT_CREATION:
            fastInitFailureReport << "Creating a Context\n";
            break;
        case PointOfFailure::GLAD_LOAD_OPENGL:
            fastInitFailureReport << "LOADING OpenGL Functions Using glad\n";
            break;
        }
        failureIterator++;
    } while (failureIterator != bundle.failures->getFailureEndCIterator());
    fastInitFailureReport << "\n\n";

    LOG(ERROR) << fastInitFailureReport.str();
}