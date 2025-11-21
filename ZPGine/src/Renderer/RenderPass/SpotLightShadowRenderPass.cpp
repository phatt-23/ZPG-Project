#include "SpotLightShadowRenderPass.h"

#include "Material/Material.h"
#include "Model/Mesh.h"
#include "Profiling/Instrumentor.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderContext.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"
#include "Texture/Texture2DArray.h"
#include "Buffer/FrameBuffer.h"

namespace ZPG
{
    void SpotLightShadowRenderPass::Init(RenderContext& context)
    {
        FrameBufferSpecification frameBufferSpec;
        frameBufferSpec.Width = 1024;
        frameBufferSpec.Height = 1024;
        frameBufferSpec.Resizable = false;
        frameBufferSpec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::Depth,
                .TexType = TextureType::Texture2DArray,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0,
                .TextureAttachment = context.Targets.SpotLightShadowMapArray,
            },
        };

        m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);

        m_ShaderProgram = ShaderProgram::Create("SpotLightShadowMap",
        {
            Shader::Create("assets/shaders/shadow-mapping/spotlight/SpotLightShadow.vert"),
            // Shader::Create("assets/shaders/shadow-mapping/spotlight/SpotLightShadow.geom"),
            Shader::Create("assets/shaders/shadow-mapping/spotlight/SpotLightShadow.frag"),
        });
    }


    void SpotLightShadowRenderPass::Execute(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        if (context.Lights.SpotLights.empty()) {
            return;
        }

        m_FrameBuffer->Bind();
        RenderCommand::Clear();

        m_ShaderProgram->Bind();

        int index = 0;
        for (auto& spotLight : context.Lights.SpotLights)
        {
            if (index >= context.Targets.SpotLightShadowMapArray->GetArraySize())
                break;

            m_ShaderProgram->SetInt("u_Index", index);
            index++;
            for (const auto& batch : std::views::concat(
                context.Batches.Shadow | std::views::values,
                context.StaticBatches.Shadow | std::views::values)
            )
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
