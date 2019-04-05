


#include "Pre_And_Post_OpenGL_Function_Call_Callbacks.h"

#include "UniversalIncludes.h"
#include "GraphicsLanguage.h"

namespace FSMEngineInternal {



    void customPreGLFuncCallCallback(const char* name, void* funcptr, int len_args, ...) {
        LOG(TRACE) << __FUNCTION__;
        LOG(TRACE) << name;
    }


    void customPostGLFuncCallCallback(const char* name, void* funcptr, int len_args, ...) {
        LOG(TRACE) << __FUNCTION__;

        GLenum error_code;

        error_code = glad_glGetError();

        if (error_code != GL_NO_ERROR) {
            std::ostringstream errorMsg;
            errorMsg << "     [GL ERROR " << error_code << "]\n";
               
            errorMsg << "GL Error Reported during function call \'" << name << "(";
            if (1 < len_args)
                errorMsg << " [" << len_args << " parameter arguments] )";
            else if (1 == len_args)
                errorMsg << " [1 parameter argument] )";
            else
                errorMsg << ")";
            errorMsg << "\'.";

            LOG(INFO) << errorMsg.str();
            LOG(ERROR) << errorMsg.str();  
        }
    }
      
      
       
       
} //namespace FSMEngineInternal 
    
   
   
  