#pragma once

#include "Debug/Asserter.h"
#include "Shader/ShaderDataType.h"

namespace ZPG {

struct BufferElement {
    std::string Name;
    ShaderDataType::Type Type;
    unsigned int Size;
    unsigned int Offset;
    bool Normalized;

    BufferElement(ShaderDataType::Type type, const std::string& name, bool normalized = false)
        : Name(name), Type(type), Size(ShaderDataType::Sizeof(type)), Offset(0), Normalized(normalized) 
    {}

    int GetElementCount() const {
        switch (Type) {
            case ShaderDataType::Texture2D:
            case ShaderDataType::Int:
            case ShaderDataType::Bool:
            case ShaderDataType::Float: return 1;
            case ShaderDataType::Int2:
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Int3:
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Int4:
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3: return 3 * 3;
            case ShaderDataType::Mat4: return 4 * 4;
            case ShaderDataType::None: break;
        }
        ZPG_CORE_ASSERT(false, "Unknown type!");
        return 0;
    }
};

}
