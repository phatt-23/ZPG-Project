#include "SkyRenderPass.h"

#include "Renderer/RenderContext.h"
#include "Renderer/RenderCommand.h"

namespace ZPG
{

    void SkyRenderPass::Init(RenderContext& context)
    {
        m_FrameBuffer = context.Targets.MainFrameBuffer;
    }

    void SkyRenderPass::Execute(RenderContext& context)
    {
        if (context.ActiveSky == nullptr) {
            return;
        }

        m_FrameBuffer->Bind();

        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        context.ActiveSky->Bind();
        RenderCommand::Draw(*context.ActiveSky->GetVertexArray());
        context.ActiveSky->Unbind();

        glDepthMask(GL_TRUE); // restore
        glDepthFunc(GL_LESS);

        m_FrameBuffer->Unbind();
    }

}
