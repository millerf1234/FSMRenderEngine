//   
//  FILE:                    FSMPerformanceReport.h
// 
//  Keeps a record of the performance of the Rendering loop by recording the 
//  time between each frame.
//
//  Depends on GLFW 
//
//  Note:    I am unsure currently of how much public exposure should be given to this class.
//         As of right now in early development, I am leaning towards having this class being
//         meant for completely internal usage by the FSMRenderEnvironment with there being 
//         either a seperate collection of functions or an interfacing class that performs the
//         public interfacing duties.
//         
//
//  Usage Instructions:       
//                      To effectively use this object, simply create an instance of this class 
//                      that exists on the same thread as glfw's main thread and context.
// 
//

#pragma once

#ifndef FSM_FRAME_PERFORMANCE_REPORT_H_
#define FSM_FRAME_PERFORMANCE_REPORT_H_

#include "UniversalIncludes.h"                 //Logging
#include "GraphicsLanguageFramework.h"         //glfw 

class FSMFramePerformanceReport final {

    //Construct a FSMFramePerformanceReport object that can be used to 
    //provide automated frametime statistical gathering. 
    // [Input]   
    //      unsigned long long framesToKeep
    //        
    FSMFramePerformanceReport(unsigned long long additionalFramesToKeep = 0);

};

#endif //FSM_FRAME_PERFORMANCE_REPORT_H_