//File:                               FSMJoystickEventCallback.h
//
// Description:                       Provides an interface for responding to Joystick Callback events.
//                                    See 'Anticipated Usage' section for proper usage.
//
//
// Anticipated Usage Pattern:         Once every frame or so, the application should call
//                                    the function 'checkJoystickEventQueues()'. It is expected
//                                    that 99.99% of the time this will return false, but in the 
//                                    event that this function does return true, the additional
//                                    functions can be used to poll both the ConnectionEvent and 
//                                    DisconnectionEvent queues to discover if either (or both)
//                                    has an event awaiting further processing. 
//                            
// Other info:                        IMPORTANT! Please be aware that the queues here will not perform
//                                    any validation on the data they receive from OS through the callback,
//                                    so please ensure the Application validates every event (i.e. check
//                                    to make sure the Joystick handle is not NULL; if worried about security
//                                    then after a connection event check to make sure the joystick handle for
//                                    odd properties or whatnot...
//                                    etc...)
//                                    
//                                    
// Programmer:                        Forrest Miller
//                                     [Shamelessly ripped off from the monitor event callback code. I should combine these 2...
// Date:                              March 9, 2019

#pragma once

#ifndef FSM_JOYSTICK_EVENT_CALLBACK_H_
#define FSM_JOYSTICK_EVENT_CALLBACK_H_

#include <queue>
#include <optional>

#include "GraphicsLanguageFramework.h"


namespace FSMEngineInternal {

    ///////////////////////////////////////////////////////////////////////////////////////
    ////////////      Interface to be used by the rest of the Application      ////////////
    ///////////////////////////////////////////////////////////////////////////////////////

    //Returns true if there are any joystick connection/disconnection events that
    //occurred recently which require processing by the Application. Returns false if
    //no joystick events require processing.
    bool checkJoystickEventQueues();

    //Check the joystick connection event queue to see if any joystick connection 
    //events have recently occurred which would require processing. Returns false
    //if there are no connection events to process.
    bool checkJoystickConnectionEventQueue();

    //Check the joystick disconnection event queue to see if any joystick disconnection 
    //events have recently occurred which would require processing. Returns false
    //if there are no disconnection events to process.
    bool checkJoystickDisconnectionEventQueue();

    //Retrieves the next availableJoystick Connection event off the 
    //Connection event queue and returns an optional-wrapped int
    //for the newly connected Joystick. Be aware that if the Joystick
    //Connection event queue is empty when this function is called, the 
    //returned optional will be empty.
    //WARNING! Despite being wrapped in the std::optional by this 
    //JoystickEvent queue system, no validation has yet been performed
    //on the underlying int. Please ensure to validate the 
    //value before using it (e.g. it could be an illegal value)
    std::optional<int> getNextAvailableJoystickConnection();

    //Retrieves the next available Joystick Disconnection event off the 
    //Disconnection event queue and returns an optional-wrapped int
    //for the recently disconnected Joystick. Be aware that if the Joystick 
    //Disconnection event queue is empty when this function is called, the 
    //returned optional will be empty. 
    //WARNING! Despite being wrapped in the std::optional by this 
    //JoystickEvent queue system, no validation has yet been performed
    //on the underlying int. Please ensure to validate the 
    //value before using it (e.g. it could be an illegal value)
    std::optional<int> getNextAvailableJoystickDisconnection();


    //////////////////////////////////////////////////
    //////   Implementation Details, Can Ignore   ////
    //////////////////////////////////////////////////
    namespace JoystickEventCallbackInternal {
        //Declaration of callback function to assign to GLFW and prototypes for the two queue functions.
        //Don't call these functions outside of the implementation
        void graphicsLanguageFrameworkJoystickEventCallbackFunction(int, int);
        std::queue<int, std::vector<int>>& recentJoystickConnections();
        std::queue<int, std::vector<int>>& recentJoystickDisconnections();
    } //namespace JoystickEventCallbackInternal

} //namespace FSMEngineInternal

#endif //GL_FRAMEWORK_JOYSTICK_EVENT_CALLBACK_H_