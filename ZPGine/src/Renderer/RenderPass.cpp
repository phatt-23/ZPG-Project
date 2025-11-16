//
// Created by phatt on 11/8/25.
//

#include "RenderPass.h"

#include "Buffer/FrameBuffer.h"

namespace ZPG
{

    void RenderPass::OnResize(u32 width, u32 height)
    {
        m_FrameBuffer->Resize(width, height);
    }

}
