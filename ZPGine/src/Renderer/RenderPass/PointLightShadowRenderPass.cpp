#include "PointLightShadowRenderPass.h"

#include "Material/Material.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderContext.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Model/Model.h"
#include "Model/Mesh.h"
#include "Texture/TextureCubeMapArray.h"

namespace ZPG
{
    void PointLightShadowRenderPass::Init(RenderContext& context)
    {
        FrameBufferSpecification framebufferSpec;
        framebufferSpec.Width = 1024;
        framebufferSpec.Height = 1024;
        framebufferSpec.Resizable = false;
        framebufferSpec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::Depth,
                .TexType = TextureType::TextureCubeMapArray,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0,
                .TextureAttachment = context.Targets.PointLightShadowCubeMapArray,
            },
        };

        m_FrameBuffer = FrameBuffer::Create(framebufferSpec);

        m_ShaderProgram = ShaderProgram::Create("PointLightShadowMap",
        {
            Shader::Create("assets/shaders/shadow-mapping/pointlight/PointLightShadow.vert"),
            Shader::Create("assets/shaders/shadow-mapping/pointlight/PointLightShadow.geom"),
            Shader::Create("assets/shaders/shadow-mapping/pointlight/PointLightShadow.frag"),
        });
    }

    void PointLightShadowRenderPass::Execute(RenderContext& context)
    {
        if (context.Lights.PointLights.empty()) {
            return;
        }

        m_FrameBuffer->Bind();
        RenderCommand::Clear();

        m_ShaderProgram->Bind();

        int index = 0;
        for (const auto& pointLight : context.Lights.PointLights)
        {
            m_ShaderProgram->SetInt("u_Index", index);
            index++;

            for (const auto& [_, batch] : context.Batches.Shadow)
            {
                context.SSBO.EntitySSBO.SetEntityIDs(batch.entityIDs);
                context.SSBO.ModelSSBO.SetModels(batch.transforms);
                context.SSBO.MaterialSSBO.SetMaterial(*batch.mesh->GetMaterial());
                batch.mesh->GetMaterial()->BindMaps();
                RenderCommand::DrawInstanced(*batch.mesh->GetVertexArray(), batch.GetSize());
            }
        }

        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind();
    }

}
