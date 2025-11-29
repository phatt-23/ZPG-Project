//
// Created by phatt on 11/8/25.
//

#include "RenderPass.h"

#include "Buffer/FrameBuffer.h"
#include "Resource/ResourceManager.h"
#include "Resource/CommonResources.h"

namespace ZPG
{
    RenderPass::RenderPass()
    {
        m_ScreenQuadVAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_QUAD);
    }

    void RenderPass::OnResize(u32 width, u32 height)
    {
        if (m_FrameBuffer != nullptr)
        {
            m_FrameBuffer->Resize(width, height);
        }
    }

}
