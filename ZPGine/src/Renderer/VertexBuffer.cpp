#include "VertexBuffer.h"
#include "Debug/Asserter.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace ZPG {

Ref<VertexBuffer> VertexBuffer::Create(const f32* vertices, const u32 count) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, count);
    case RendererAPI::None: 
        break;
    }
    ZPG_UNREACHABLE();
}
Ref<VertexBuffer> VertexBuffer::Create(const u32 size) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
    case RendererAPI::None: 
        break;
    }
    ZPG_UNREACHABLE();
}

}
