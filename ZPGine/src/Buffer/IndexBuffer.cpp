#include "IndexBuffer.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Renderer/RendererAPI.h"

namespace ZPG {

ref<IndexBuffer> IndexBuffer::Create(const u32* indices, u32 count) {
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::OpenGL: return MakeRef<OpenGLIndexBuffer>(indices, count);
        case RendererAPI::None: break;
    }

    ZPG_UNREACHABLE();
    return nullptr;
}

}
