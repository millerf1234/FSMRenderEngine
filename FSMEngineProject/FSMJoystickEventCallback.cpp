
//File:          GLFrameworkJoystickEventCallback.cpp
//
//Description:   Implementation file for the GLFrameworkJoystickEventCallback function and 
//               its related components. 
//
//               I am going to try something here. The implementation is based off the Singleton pattern as laid out
//               in section 47 of Scott Meyer's book "Effective C++ 2e" (from 1998!). The idea is to hide each 
//               static object inside a function returning a reference to it. Pretty straight forward.
//
//
//               This implementation is operating somewhat under the assumption that it is highly unlikely 
//               for there to be a very large number of monitor connection events to trigger over the span
//               of a single frame. At least if for some reason there were to be many connections at one instant,
//               this code would not be the cause of any bottleneck.
//
//
//  About Assigning the Callback Function:    As of now this file assumes that the callback function 
//                                            for processing the Monitor events will be assigned else-
//                                            where in the code. 
//
//
//Programmer:    Forrest Miller
//Date:          January 2, 2019



#include "FSMJoystickEventCallback.h"
#include "UniversalIncludes.h"

namespace FSMEngineInternal {

    //-------------------------------------------------------
    //
    //        Implementation of the public interface
    //
    //-------------------------------------------------------

    bool checkJoystickEventQueues() {
        if ((JoystickEventCallbackInternal::recentJoystickConnections().empty()) &&
            (JoystickEventCallbackInternal::recentJoystickDisconnections().empty()))
            return false;
        return true;
    }


    bool checkJoystickConnectionEventQueue() {
        if ((JoystickEventCallbackInternal::recentJoystickConnections().empty()))
            return false;
        return true;
    }


    bool checkJoystickDisconnectionEventQueue() {
        if (JoystickEventCallbackInternal::recentJoystickDisconnections().empty())
            return false;
        return true;
    }


    std::optional<int> getNextAvailableJoystickConnection() {
        if ((JoystickEventCallbackInternal::recentJoystickConnections().empty())) {
            return std::nullopt; //return the empty optional
        }
        else {
            std::optional<int> joystickHandle =
                std::make_optional<int>(JoystickEventCallbackInternal::recentJoystickConnections().front());
            JoystickEventCallbackInternal::recentJoystickConnections().pop(); //Remove Joystick from queue
            return joystickHandle;
        }
    }


    std::optional<int> getNextAvailableJoystickDisconnection() {
        if ((JoystickEventCallbackInternal::recentJoystickDisconnections().empty())) {
            return std::nullopt;
        }
        else {
            std::optional<int> joystickHandle =
                std::make_optional<int>(JoystickEventCallbackInternal::recentJoystickDisconnections().front());
            JoystickEventCallbackInternal::recentJoystickDisconnections().pop(); //Remove Joystick from queue
            return joystickHandle;
        }
    }


    namespace JoystickEventCallbackInternal {

        //-------------------------------------------------------
        //
        //   Joystick Connection and Disconnection Event Queues
        //
        //-------------------------------------------------------

        //Two queues are declared following the Singleton pattern. These queues are responsible for 
        //storing the handles provided by GLFW to the JoystickEventCallback function until they 
        //can be processed by the application.

        std::queue<int, std::vector<int>>& recentJoystickConnections() {
            static std::queue<int, std::vector<int>> awaitingConnection;
            return awaitingConnection;
        }

        std::queue<int, std::vector<int>>& recentJoystickDisconnections() {
            static std::queue<int, std::vector<int>> awaitingDisconnection;
            return awaitingDisconnection;
        }



        //-------------------------------------------------------
        //
        //   The JoystickEventCallback function to be assigned to GLFW 
        //
        //-------------------------------------------------------

        void graphicsLanguageFrameworkJoystickEventCallbackFunction(int joyID, int event) {
            LOG(INFO) << "\n\n\tA Joystick Event was detected! Event code " << event << std::endl;
            if (event == GLFW_CONNECTED) {
                recentJoystickConnections().push(joyID);
            }
            else if (event == GLFW_DISCONNECTED) {
                recentJoystickDisconnections().push(joyID);
            }
            else {  //Should never happen
                LOG(WARNING) << "GLFW Joystick event callback was called with invalid event code!\n";
            }
        }

    } //namespace JoystickEventCallbackInternal

} //namespace FSMEngineInternal