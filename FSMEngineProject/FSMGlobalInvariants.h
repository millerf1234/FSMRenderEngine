//                                                                                         
// File:                    FSMEngineInvariants.h                                          
//                                                                                         
// Desciption:              A collection of values that are invariant to FSMEngine. Any    
//                          variable defined in this file shall be considered invariant    
//                          across all FSMEngine-related code. As a consequence of         
//                          introducing invariants to the codebase, code can be written    
//                          which depends on these values existing and having the same     
//                          value each time the engine is used.                            
//                                                                                         
//                          Essentially DO NOT change these values without being prepared  
//                          to make potentially major and extensive modifications to the   
//                          code.                                                          
//                                                                                         
// Programmer:              Forrest Miller                                                 
// Date:                    April 2019                                                     
//                                                                                         
//

#pragma once

#ifndef FSM_ENGINE_INVARIANTS_H_
#define FSM_ENGINE_INVARIANTS_H_

namespace FSMEngineInvariant {
    
    // /--------------------------------------\  
    // |          OpenGL Invariants           |  
    // \--------------------------------------/  
    //static constexpr const 
    static constexpr const int DEFAULT_GL_VERSION_MAJOR = 4;
    
    
    // /--------------------------------------\  
    // |         Joystick Invariants          |  
    // \--------------------------------------/  
    
    
    
     
     
     
     
} //namespace FSMEngineInvariant

#endif //FSM_ENIGNE_INVARIANTS_H_