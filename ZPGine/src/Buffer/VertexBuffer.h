#pragma once

namespace ZPG 
{
    class BufferLayout;

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
        static ref<VertexBuffer> Create(const void* vertices, u32 size);
        static ref<VertexBuffer> Create(const void* vertices, u32 size, const BufferLayout& layout);
        static ref<VertexBuffer> Create(const u32 size);
    };
}
