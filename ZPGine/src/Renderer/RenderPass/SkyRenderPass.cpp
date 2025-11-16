#include "SkyRenderPass.h"

#include "Renderer/RenderContext.h"
#include "Renderer/RenderCommand.h"
#include "Texture/Texture2D.h"
#include "Texture/TextureDataFormat.h"

namespace ZPG
{

    void SkyRenderPass::Init(RenderContext& context)
    {
        FrameBufferSpecification spec;
        spec.Width = 1024;
        spec.Height = 1024;
        spec.Resizable = false;
        spec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::Depth,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0,
                .TextureAttachment = context.Targets.MainDepthMap,
            },
            {
                .AttachType = FrameBufferAttachmentType::Color,
                .DataFormat = TextureDataFormat::RGBA8,
                .Index = 0,
                .TextureAttachment = context.Targets.MainColorMap,
            },
        };

        m_FrameBuffer = FrameBuffer::Create(spec);
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
