#pragma once

#include <glad/glad.h>
#include "Debug/Asserter.h"

#ifdef ZPG_OPENGL_CALL_ERROR_CHECK
#include <source_location>
#include "Core/Core.h"
const char* OpenGLErrorTypeToString(GLenum glError);
#endif

#ifdef ZPG_OPENGL_CALL_ERROR_CHECK
    #ifdef ZPG_OPENGL_CALL_CRASH_ON_ERROR
        #define ZPG_OPENGL_CALL(_CALL) \
            while (glGetError() != GL_NO_ERROR); \
            _CALL; \
            if (GLenum glErr = glGetError(); glErr != 0) { \
                auto loc = std::source_location::current(); \
                ZPG_CORE_ERROR("OpenGL error '{}' (0x{:x}) returned from call: {}! Occured at: {}:{}:{}: {}",   \
                    OpenGLErrorTypeToString(glErr), \
                    glErr, \
                    #_CALL, \
                    loc.file_name(), \
                    loc.line(), \
                    loc.column(), \
                    loc.function_name()); \
                ZPG_UNREACHABLE(); \
            }
    #else
        #define ZPG_OPENGL_CALL(_CALL) \
            while (glGetError() != GL_NO_ERROR); \
            _CALL; \
            if (GLenum glErr = glGetError(); glErr != 0) { \
                auto loc = std::source_location::current(); \
                ZPG_CORE_ERROR("OpenGL error '{}' (0x{:x}) returned from call: {}! Occured at: {}:{}:{}: {}",   \
                    OpenGLErrorTypeToString(glErr), \
                    glErr, \
                    #_CALL, \
                    loc.file_name(), \
                    loc.line(), \
                    loc.column(), \
                    loc.function_name()); \
            }
    #endif
#else
    #define ZPG_OPENGL_CALL(_CALL) _CALL
#endif
    

