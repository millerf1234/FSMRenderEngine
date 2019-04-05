

//  Based off the following
//      
//       https://www.khronos.org/opengl/wiki/GLAPI/glGetInternalformat
//      https://gist.github.com/rdb/83b1d952e3808f100465
//      https://www.khronos.org/opengl/wiki/Common_Mistakes#Slow_pixel_transfer_performance
//     --  OpenGL Programming Guide 9e Page 280

#pragma once

#ifndef REPORT_DETAILS_ON_GL_IMPLEMENTATION_H_
#define REPORT_DETAILS_ON_GL_IMPLEMENTATION_H_


#include <string>
#include <vector>
#include <sstream>
#include "GraphicsLanguage.h"

//
//enum BaseInternalFormats {
//    red = GL_RED,
//    rg = GL_RG,
//    rgb = GL_RGB,
//    rgba = GL_RGBA,
//};
//

namespace {
    const char* indent = "     ";
    const char* sectionDelimiter = "~~~~~~~~~~~~--~~~~~~~~~~--~~~~~~~~~~--~~~~~~~~~~--~~~~~~~~~~--~~~~~~~~~~~~";
}


std::string getGLImplentationReportHeader() noexcept {
    std::ostringstream glReportHeader;

     glReportHeader << "\n"
        << indent << indent << indent << "           ________________________       \n"
        << indent << indent << indent << "          /                        \\      \n"
        << indent << indent << indent << "     --- ( GL Implementation Report ) --- \n"
        << indent << indent << indent << "          \\________________________/      \n";


    //General Information
    const GLubyte *name     =  glGetString(GL_VENDOR);
    const GLubyte *renderer =  glGetString(GL_RENDERER);
    const GLubyte *version  =  glGetString(GL_VERSION);

    glReportHeader << ""
        << indent << indent << indent << indent << "Vendor:    " << indent << "   " << name << "\n"
        << indent << indent << indent << indent << "Renderer:  " << indent << "   " << renderer << "\n"
        << indent << indent << indent << indent << "Version:   " << indent << "   " << version << "\n";

    return glReportHeader.str();
}

std::string getGLImplementationReport(std::vector<GLenum> )


#endif //REPORT_DETAILS_ON_GL_IMPLEMENTATION_H_


