//
// File:                            RenderDemoBase.h
// 
// Description:                     An abstract base class for every RenderDemo project to 
//                                  use. Provides a common interface for the Application to
//                                  use to invoke the RenderDemo. 
//
// Programmer:                      Forrest Miller
//

#pragma once

#ifndef RENDER_DEMO_BASE_H_
#define RENDER_DEMO_BASE_H_

#include "FSMRenderEnvironment.h"

class RenderDemoBase {
public:
    RenderDemoBase();
    RenderDemoBase(FSMRenderEnvironment& re) { (void)re; }
    virtual ~RenderDemoBase() = 0;

    //virtual void load() = 0;
   // virtual void launchWindowed(int, int, int, int) = 0;
   // virtual void launchFullscreen(int monitor) = 0;
   // virtual void launchWindowedFullscreen() = 0;

private:
    //Some sort of pointer to the RenderEnvironment is needed here
};



#endif //RENDER_DEMO_BASE_H_