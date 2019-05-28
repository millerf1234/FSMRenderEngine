

#include "Application.h"


Application::Application(int argc, char** argv) noexcept {
    
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

    mBundle_ = FSM_DO_FAST_INIT(); //Here is where the black vodoo magic happens
    
    if (mBundle_->getNumFailures() > 0)
        reportFastInitFailures(*mBundle_);
    
    else {
        //We are ready to go...


    }

}


void Application::reportFastInitFailures(const FSMFastInitReturnBundle& bundle) noexcept {
    LOG(TRACE) << __FUNCTION__;
    assert(bundle.getNumFailures > 0);

    std::ostringstream fastInitFailureReport;
    if (bundle.getNumFailures() == 1)
        fastInitFailureReport << "1 Error";
    else
        fastInitFailureReport << bundle.getNumFailures() << " Errors";
    fastInitFailureReport << " were encountered within FSM_DO_FAST_INIT().\n";
    fastInitFailureReport << "Failures were encountered while performing the following tasks:\n";

    auto failureIterator = bundle.getFailureCIterator();
    do {
        fastInitFailureReport << "  - ";
        switch (*failureIterator) {
        default:
            fastInitFailureReport << "INVALID_ENUM\n";
            break;   
        case FSMFastInitReturnBundle::PointOfFailure::CREATING_DEFAULT_DIRECTORIES:
            fastInitFailureReport << "Loading Engine Settings File\n";
            break;
        case FSMFastInitReturnBundle::PointOfFailure::LOADING_ENGINE_SETTINGS:
            fastInitFailureReport << "Loading Engine Settings File\n";
            break;
        case FSMFastInitReturnBundle::PointOfFailure::LOADING_OTHER_SETTINGS:
            fastInitFailureReport << "Loading An Unspecified File\n";
            break;
        case FSMFastInitReturnBundle::PointOfFailure::SETTING_CALLBACKS:
            fastInitFailureReport << "Setting Callback Functions\n";
            break;
        case FSMFastInitReturnBundle::PointOfFailure::GLFW_INIT:
            fastInitFailureReport << "Calling GLFW's Init Function\n";
            break;
        case FSMFastInitReturnBundle::PointOfFailure::CONTEXT_CREATION:
            fastInitFailureReport << "Creating a Context\n";
            break;
        case FSMFastInitReturnBundle::PointOfFailure::GLAD_LOAD_OPENGL:
            fastInitFailureReport << "LOADING OpenGL Functions Using glad\n";
            break;
        }
        failureIterator++;
    } while (failureIterator != bundle.getFailureEndCIterator());
    fastInitFailureReport << "\n\n";

    LOG(ERROR) << fastInitFailureReport.str();
}