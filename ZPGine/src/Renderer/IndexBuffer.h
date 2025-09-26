#pragma once

#include "Core/Core.h"

namespace ZPG {

class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual u32 GetCount() const = 0;
    static Ref<IndexBuffer> Create(const u32* indices, u32 count);
};

}
