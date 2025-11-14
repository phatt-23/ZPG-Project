//
// Created by phatt on 10/18/25.
//

#include "FrameBuffer.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/RendererAPI.h"

namespace ZPG {

    ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification &spec)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::None: ZPG_UNREACHABLE("RendererAPI::None is not supported"); return nullptr;
            case RendererAPI::OpenGL: return MakeRef(new OpenGLFrameBuffer(spec));
        }

        ZPG_UNREACHABLE();
        return nullptr;
    }

}
