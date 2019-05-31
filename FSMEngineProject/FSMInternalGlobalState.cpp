
// Something is weird in the FSMInternalGlobalState header file that 
// is causing link errors, to fix it this implementation file has been
// added



#include "FSMInternalGlobalState.h"

namespace FSMEngineInternal {
    

    /////////////////////////// GLFW Initialization is given special treatment ///////////////////////////////


    namespace {
        static std::shared_mutex initStatusMutex;
    }

    // CLASS:               InitStatusOfTheGLFWLibrary
       //
       // DESCRIPTION:
       //    The GLFW library is a core foundational library upon which much of the FSMEngine 
       // directly is built. Before being of any use (aside from a small collection of basic
       // functions) GLFW requires that its initialization function [called 'glfwInit()'] be
       // invoked and return sucessfully. Thus the initialization state of GLFW will be an 
       // intrinsically global determinant upon the behavior of any code that uses GLFW. 
       // 
       //   (say something here about a call to 'glfwTerminate()' being required) 
       //
       //    This leads quite naturally to the InitStatusOfTheGLFWLibrary class which is
       // implemented as a global thread-safe wrapper that encapsulates GLFW's initialization
       // state.
       // 
       // 
       // The GLFW library essentially exists as a layer between FSMEngine code and the
       // runtime OS/Platform. 
       //
       // NOTES:
       //  - THIS CLASS IS A SINGLETON.
       //        It Wraps a Global Bool Representing the State of GLFW.
       //  - I AM NOT VERY EXPERIENCED WITH MULTITHREADING
       //        The way the shared_mutex is used to protect read/write access may
       //        not be the correct implementation, I am experimenting with it still.
       //
    class FSMGlobalStateTracker::InitStatusOfTheGLFWLibrary final {
    public:
        //InitStatusOfTheGLFWLibrary() = delete; //noexcept { /* LOG(TRACE); */ }
        ~InitStatusOfTheGLFWLibrary() = default; // noexcept { LOG(TRACE) << __FUNCTION__; }
        InitStatusOfTheGLFWLibrary(const InitStatusOfTheGLFWLibrary&) = delete;
        InitStatusOfTheGLFWLibrary& operator=(const InitStatusOfTheGLFWLibrary&) = delete;

        //The following public function is used to get the status of the GLFW library.
        bool getStatus() const noexcept {
            LOG(TRACE) << __FUNCTION__;

            //As a reader, lock shared with other readers
            LOG(INFO) << "\n";
            LOG(INFO) << "   ---> Begin Read Request For Init Status Of GLFW";
            LOG(INFO) << "            [Attempting to lock_shared the shared_mutex]";
            initStatusMutex.lock_shared();
            LOG(INFO) << "            [Mutex Successfully Locked In Shared Mode]";
            bool status = initStatus;
            const char* statusMsg = status ? "Initialized" : "Not Initialized";
            LOG(INFO) << "            [Reading Value While Under Mutex Lock]";
            std::string msg = std::string("Value is ") + statusMsg + ".";
            LOG(INFO) << "                 " << statusMsg;
            LOG(INFO) << "            [Releasing Shared_Lock on Shared_Mutex]";
            initStatusMutex.unlock_shared();
            LOG(INFO) << "            [Shared_Mutex successfully released]\n\n";
            LOG(INFO) << "        End Of Read Request For Init Status Of GLFW --->";
            return status;
        }

    private:
        static bool initStatus;

        InitStatusOfTheGLFWLibrary() { }

        void updateGLFWInitStatus(bool status) noexcept {
            LOG(TRACE) << __FUNCTION__;
            //As a writer, lock exclusivly
            LOG(INFO) << "\n";
            LOG(INFO) << "   ---> Begin Write Request For Init Status Of GLFW";
            LOG(INFO) << "            [Attempting to lock exclusively the shared_mutex]";
            initStatusMutex.lock();
            LOG(INFO) << "            [Mutex Successfully Locked In Exclusive Mode]";
            initStatus = status;
            const char* statusMsg = status ? "Initialized" : "Not Initialized";
            LOG(INFO) << "            [Writing Value While Under Mutex Lock]";
            std::string msg = std::string("Value is now ") + statusMsg + ".";
            LOG(INFO) << "                 " << statusMsg;
            LOG(INFO) << "            [Releasing Exclusive Lock on Shared_Mutex]";
            initStatusMutex.unlock();
            LOG(INFO) << "            [Shared_Mutex successfully released]\n\n";
            LOG(INFO) << "        End Of Write Request For Init Status Of GLFW --->";
        }

        friend struct FSMGlobalStateTracker::InitializationStatus;
        //Friend of function that initializes glfw
        //Friend of function that std::atexit's glfwTerminate

    };


    bool FSMGlobalState::InitStatusOfTheGLFWLibrary::initStatus = false;

    FSMGlobalState::InitStatusOfTheGLFWLibrary& FSMGlobalState::InitializationStatus::libGLFW() noexcept {
        static InitStatusOfTheGLFWLibrary* glfwInitStatus = new InitStatusOfTheGLFWLibrary();
        return *glfwInitStatus;
    }



} //namespace FSMEngineInternal