#include "OpenGLCore.h"

const char* OpenGLErrorTypeToString(GLenum glError) 
{
    switch (glError) 
    {
        case GL_INVALID_ENUM:                  return "GL_Invalid_Enum";
        case GL_INVALID_VALUE:                 return "GL_Invalid_Value";
        case GL_INVALID_OPERATION:             return "GL_Invalid_Operation";
        case GL_STACK_OVERFLOW:                return "GL_Stack_Overflow";
        case GL_STACK_UNDERFLOW:               return "GL_Stack_Underflow";
        case GL_OUT_OF_MEMORY:                 return "GL_Out_of_Memory";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_Invalid_Framebuffer_Operation";
        case GL_CONTEXT_LOST:                  return "GL_Context_Lost";
    }

    return "GL_No_Error";
}


