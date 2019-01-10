//  File:                   GlobalGLIncludes.h
//
//  Important Consideration:    This file should be included BEFORE the 'glfw.h' library is 
//                              included in any file that requires 'glfw.h'
//
//  Description:            Intended to be included project wide.
//                          Combines the following GL-related header files:
//                                -   'glad.h' (which provides access to the OpenGL functions)
//                                -   'glm.h'  (Graphics language Math library, provides support for
//                                              interfacing with OpenGL by defining useful types 
//                                              (e.g. vector, matrix) and helpful functions (such as
//                                              the matrix-generation functions 'glm::lookat()' and
//                                              'glm::perspectiveFov()').
//
//
//  Note on Visual-Studio build variations of 'glad.h'
//                          The files 'glad.h' and 'glad.c' have 2 different versions, one which is
//                          meant for debugging and one which is meant for real-world use. The 
//                          solution for this project has been configured to select the proper 
//                          version based off the solution's build configuration. If compiling 
//                          this code outside of MSVC, please be sure to only include one version
//                          of this library.
//
//
//   References:
//            glad generator links
//
//
//            glm reference:  https://github.com/g-truc/glm/blob/0.9.9.2/doc/manual.pdf
//
//
//
//  Programmer:            Forrest Miller
//  Date:                  January 10, 2019
//

#pragma once

#ifndef GLOBAL_GL_INCLUDES_H_
#define GLOBAL_GL_INCLUDES_H_


//////////////////////////////
//  Set up the GLM library  //         (GLM is mathematics utility library)
//////////////////////////////

//First off, we want to force GLM to use SSE2 if at all possible since 
//Streaming SIMD support is where it is at for doing math really fast 
//(and there isn't a downside to using it as far as I know)
#ifndef GLM_FORCE_SSE2
#define GLM_FORCE_SSE2
#endif //GLM_FORCE_SSE2

//While having aligned data isn't strictly necessary, typically it is
//best to have aligned data as much as possible for communicating with 
//an external device (such as a GPU).
#ifndef GLM_FORCE_ALIGNED
#define GLM_FORCE_ALIGNED
#endif //GLM_FORCE_ALIGNED

//Include the main GLM library
#include "ThirdParty\glm\glm.hpp" 

//After including the main GLM header, it is still necessary to include
//some additional GLM extensions that are used in this project:

//'type_ptr' is a conversion function that allows GLM vector and GLM 
// matrix types to be accessed as though they are just arrays through
// a GLfloat* to their first element. This is necessary for sending them
// as uniforms to shader programs.
#include "ThirdParty\glm\gtc\type_ptr.hpp"   

//'Quaternion' is a special data type used to efficiently and cleanly express
// rotations in 3D. They are both cheaper and more robust than relying on Euler angles, 
// which suffer from pitfalls such as gimbal lock.
#include "ThirdParty\glm\gtc\quaternion.hpp" 

//'matrix_transform' provides functions for creating several useful matrices that 
// are used within the course of rendering. 
#include "ThirdParty\glm\gtc\matrix_transform.hpp"







#endif //GLOBAL_GL_INCLUDES_H_