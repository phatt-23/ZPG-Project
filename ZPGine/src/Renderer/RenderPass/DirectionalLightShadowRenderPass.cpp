#include "DirectionalLightShadowRenderPass.h"
#include "Buffer/VertexArray.h"
#include "Material/Material.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Profiling/Instrumentor.h"
#include "Renderer/DrawCommand.h"
#include "Renderer/DrawBatch.h"
#include "Renderer/RenderCommand.h"
#include "Shader/ShaderProgram.h"
#include "Shader/Shader.h"
#include "Renderer/RenderContext.h"
#include "Renderer/RenderGroups.h"
#include "Texture/Texture2D.h"

namespace ZPG
{
    void DirectionalLightShadowRenderPass::Init(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        FrameBufferSpecification directionalLightFramebufferSpec;
        directionalLightFramebufferSpec.Width = context.Targets.DirectionalLightShadowMap->GetWidth();
        directionalLightFramebufferSpec.Height = context.Targets.DirectionalLightShadowMap->GetHeight();
        directionalLightFramebufferSpec.Resizable = false;
        directionalLightFramebufferSpec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::Depth,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0,
                .TextureAttachment = context.Targets.DirectionalLightShadowMap,
            },
        };

        m_FrameBuffer = FrameBuffer::Create(directionalLightFramebufferSpec);

        m_ShaderProgram = ShaderProgram::Create("DirectionalShadow.program",
        {
            Shader::Create("./assets/shaders/shadow-mapping/directional/DirectionalShadow.vert"),
            // Shader::Create("./assets/shaders/shadow-mapping/directional/DirectionalShadow.geom"),
            Shader::Create("./assets/shaders/shadow-mapping/directional/DirectionalShadow.frag"),
        });
    }

    void DirectionalLightShadowRenderPass::Execute(RenderContext &context)
    {
        ZPG_PROFILE_FUNCTION();

        m_FrameBuffer->Bind();
        RenderCommand::Clear();

        if (context.Lights.DirectionalLight == nullptr) {
            m_FrameBuffer->Unbind();
            return;
        }

        m_ShaderProgram->Bind();

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

        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind();
    }
}
