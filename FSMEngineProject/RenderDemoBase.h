//
// File:                            RenderDemoBase.h
// 
// Description:                     An abstract base class for every RenderDemo project to 
//                                  use. Provides a common interface for Application to
//                                  invoke each RenderDemo. 
//
// Programmer:                      Forrest Miller
//

#pragma once

#ifndef RENDER_DEMO_BASE
#define RENDER_DEMO_BASE

class RenderDemoBase {
public:
    RenderDemoBase();
    virtual ~RenderDemoBase() = 0;
};



#endif //RENDER_DEMO_BASE