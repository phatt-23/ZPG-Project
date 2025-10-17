#include "IndexBuffer.h"

#include "Debug/Asserter.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace ZPG {

ref<IndexBuffer> IndexBuffer::Create(const u32* indices, u32 count) {
    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: 
        return MakeRef<OpenGLIndexBuffer>(indices, count);
    case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}

}
