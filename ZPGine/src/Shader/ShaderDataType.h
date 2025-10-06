#pragma once
#include "Debug/Asserter.h"
#include <glad/glad.h>

namespace ZPG {

struct ShaderDataType {
    enum Type {
        None = 0,
        Bool,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        Mat3, Mat4,
    };

    static unsigned int Sizeof(Type type) {
        switch (type) {
        case ShaderDataType::Bool: return 1;
        case ShaderDataType::Float: return 4;
        case ShaderDataType::Float2: return 4 * 2;
        case ShaderDataType::Float3: return 4 * 3;
        case ShaderDataType::Float4: return 4 * 4;
        case ShaderDataType::Int: return 4;
        case ShaderDataType::Int2: return 4 * 2;
        case ShaderDataType::Int3: return 4 * 3;
        case ShaderDataType::Int4: return 4 * 4;
        case ShaderDataType::Mat3: return 3 * 3 * 4;
        case ShaderDataType::Mat4: return 4 * 4 * 4;
        case ShaderDataType::None:
        default:
            break;
        }

        ZPG_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    static GLenum ToOpenGLType(Type type) {
        switch (type) {
        case Bool: return GL_BOOL;
        case Float:
        case Float2:
        case Float3:
        case Float4: return GL_FLOAT;
        case Int:
        case Int2:
        case Int3:
        case Int4: return GL_INT;
        case Mat3:
        case Mat4: return GL_FLOAT;
        case None:
          break;
        }
        ZPG_UNREACHABLE();
    }
};

}
