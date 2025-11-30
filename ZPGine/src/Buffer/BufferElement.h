#pragma once

#include "Shader/ShaderDataType.h"

namespace ZPG
{

    struct BufferElement
    {
        std::string Name;
        ShaderDataType::Type Type;
        unsigned int Size;
        unsigned int Offset;
        bool Normalized;

        BufferElement(ShaderDataType::Type type, const std::string& name = "not_specified", bool normalized = false);
        int GetElementCount() const;
    };

}
