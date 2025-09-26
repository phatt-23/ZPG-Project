#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace ZPG {

Ref<VertexArray> VertexArray::Create() {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: return CreateRef<OpenGLVertexArray>();
    case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}

}
