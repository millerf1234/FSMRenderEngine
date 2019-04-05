//
//
//
//
//

#pragma once

#ifndef PRE_AND_POST_OPENGL_FUNCTION_CALLBACKS_H_
#define PRE_AND_POST_OPENGL_FUNCTION_CALLBACKS_H_


namespace FSMEngineInternal {

    void customPreGLFuncCallCallback(const char* name, void* funcptr, int len_args, ...);

    void customPostGLFuncCallCallback(const char* name, void* funcptr, int len_args, ...);

} //namespace FSMEngineInternal




#endif //PRE_AND_POST_OPENGL_FUNCTION_CALLBACKS_H_