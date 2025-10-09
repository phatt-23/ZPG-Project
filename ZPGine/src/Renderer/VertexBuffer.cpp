#include "VertexBuffer.h"
#include "Debug/Asserter.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace ZPG {

Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, u32 size) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: 
        return std::move(CreateRef<OpenGLVertexBuffer>(vertices, size));
    case RendererAPI::None: 
        break;
    }
    ZPG_UNREACHABLE();
}
Ref<VertexBuffer> VertexBuffer::Create(u32 size) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: 
        return std::move(CreateRef<OpenGLVertexBuffer>(size));
    case RendererAPI::None: 
        break;
    }
    ZPG_UNREACHABLE();
}
Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, u32 size, const BufferLayout& layout) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: {
        auto VBO = CreateRef<OpenGLVertexBuffer>(vertices, size);
        VBO->SetLayout(layout);
        return std::move(VBO);
    }
    case RendererAPI::None: 
        break;
    }
    ZPG_UNREACHABLE();
}

}
