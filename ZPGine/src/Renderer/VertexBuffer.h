#pragma once

#include "BufferLayout.h"
#include "Core/Core.h"

namespace ZPG {

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void SetData(const void* data, const u32 size) = 0;
    virtual u32 GetSize() const = 0;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual const BufferLayout& GetLayout() const = 0;
    virtual void SetLayout(BufferLayout&& layout) = 0;

    static Ref<VertexBuffer> Create(const f32* vertices, const u32 count);
    static Ref<VertexBuffer> Create(const u32 size);
};

}
