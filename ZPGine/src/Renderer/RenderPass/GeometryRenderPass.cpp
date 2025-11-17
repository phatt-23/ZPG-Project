#include "GeometryRenderPass.h"

#include "Renderer/DrawBatch.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Buffer/FrameBuffer.h"
#include "Renderer/RenderGroups.h"
#include "Material/Material.h"
#include "Renderer/RenderCommand.h"
#include "Shader/ShaderProgram.h"
#include "Shader/CommonShaderUniforms.h"
#include "Texture/Texture2D.h"
#include "Model/Model.h"
#include "Model/Mesh.h"
#include "Profiling/Instrumentor.h"
#include "Shader/Shader.h"

namespace ZPG
{
    void GeometryRenderPass::Init(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        m_ShaderProgram = ShaderProgram::Create("GeometryPass.program",
        {
            Shader::Create("./assets/shaders/multipass/GeometryPass.vert"),
            Shader::Create("./assets/shaders/multipass/GeometryPass.frag"),
        });

        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt(CommonShaderUniforms::ALBEDO_MAP,       RenderBindingPoints::ALBEDO_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::METALNESS_MAP,    RenderBindingPoints::METALNESS_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::ROUGHNESS_MAP,    RenderBindingPoints::ROUGHNESS_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::NORMAL_MAP,       RenderBindingPoints::NORMAL_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::EMISSIVE_MAP,     RenderBindingPoints::EMISSIVE_MAP);
        m_ShaderProgram->Unbind();

        m_FrameBuffer = context.Targets.GeometryFrameBuffer;
    }

    void GeometryRenderPass::Execute(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        m_FrameBuffer->Bind();
        RenderCommand::Clear();

        m_ShaderProgram->Bind(); 

        for (const auto& batch : context.Batches.GeometryBuffer | std::views::values)
        {
            context.SSBO.EntitySSBO.SetEntityIDs(batch.entityIDs);
            context.SSBO.ModelSSBO.SetModels(batch.transforms);
            context.SSBO.MaterialSSBO.SetMaterial(*batch.mesh->GetMaterial());
            batch.mesh->GetMaterial()->BindMaps();
            RenderCommand::DrawInstanced(*batch.mesh->GetVertexArray(), batch.GetSize());
        }

        for (const auto& batch : context.StaticBatches.GeometryBuffer | std::views::values)
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
