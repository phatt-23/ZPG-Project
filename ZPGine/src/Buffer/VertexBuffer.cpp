#include "VertexBuffer.h"
#include "Debug/Asserter.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace ZPG {

ref<VertexBuffer> VertexBuffer::Create(const void* vertices, u32 size)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::OpenGL:
            return std::move(MakeRef<OpenGLVertexBuffer>(vertices, size));
        case RendererAPI::None:
            break;
    }
    ZPG_UNREACHABLE();
}

ref<VertexBuffer> VertexBuffer::Create(u32 size)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::OpenGL:
            return std::move(MakeRef<OpenGLVertexBuffer>(size));
        case RendererAPI::None:
            break;
    }
    ZPG_UNREACHABLE();
}

ref<VertexBuffer> VertexBuffer::Create(const void* vertices, u32 size, const BufferLayout& layout)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::OpenGL: {
            auto VBO = MakeRef<OpenGLVertexBuffer>(vertices, size);
            VBO->SetLayout(layout);
            return std::move(VBO);
        }
        case RendererAPI::None:
            break;
    }
    ZPG_UNREACHABLE();
}

}
