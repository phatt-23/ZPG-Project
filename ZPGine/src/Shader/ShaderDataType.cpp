//
// Created by phatt on 10/18/25.
//

#include "ShaderDataType.h"
#include "Debug/Asserter.h"
#include <glad/glad.h>


namespace ZPG {

unsigned int ShaderDataType::Sizeof(Type type) {
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
    case ShaderDataType::Texture2D: return 4;
    case ShaderDataType::None:
    default:
        break;
    }

    ZPG_UNREACHABLE("Unknown ShaderDataType!");
    return 0;
}

GLenum ShaderDataType::ToOpenGLType(Type type) {
    switch (type) {
    case Bool: return GL_BOOL;
    case Float:
    case Float2:
    case Float3:
    case Float4: return GL_FLOAT;
    case Texture2D:
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
    return -1;
}


}
