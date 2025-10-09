#pragma once

#include "BufferLayout.h"
#include "Core/Core.h"

namespace ZPG {

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void SetData(const void* data, u32 size) = 0;
    virtual u32 GetSize() const = 0;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual const BufferLayout& GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout& layout) = 0;

    // 'size' specified in bytes
    static Ref<VertexBuffer> Create(const void* vertices, u32 size);
    static Ref<VertexBuffer> Create(const void* vertices, u32 size, const BufferLayout& layout);
    static Ref<VertexBuffer> Create(const u32 size);
};

}
