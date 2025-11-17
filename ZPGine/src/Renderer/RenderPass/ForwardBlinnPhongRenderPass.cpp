//
// Created by phatt on 11/15/25.
//

#include "ForwardBlinnPhongRenderPass.h"

#include "Debug/Asserter.h"
#include "Material/Material.h"
#include "Model/Mesh.h"
#include "Profiling/Instrumentor.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderContext.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"

namespace ZPG
{

    void ForwardBlinnPhongRenderPass::Init(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        m_ShaderProgram = ShaderProgram::Create("forward_blinn_phong.program",
        {
            Shader::Create("./assets/shaders/multipass/Forward-BlinnPhong.vert"),
            Shader::Create("./assets/shaders/multipass/Forward-BlinnPhong.frag"),
        });

        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt(CommonShaderUniforms::ALBEDO_MAP,    RenderBindingPoints::ALBEDO_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::NORMAL_MAP,    RenderBindingPoints::NORMAL_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::EMISSIVE_MAP,  RenderBindingPoints::EMISSIVE_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::METALNESS_MAP, RenderBindingPoints::METALNESS_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::ROUGHNESS_MAP, RenderBindingPoints::ROUGHNESS_MAP);
        m_ShaderProgram->Unbind();

        ZPG_CORE_ASSERT(context.Targets.MainFrameBuffer != nullptr, "Main Frame Buffer is null!");

        m_FrameBuffer = context.Targets.MainFrameBuffer;
    }

    void ForwardBlinnPhongRenderPass::Execute(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        m_FrameBuffer->Bind();
        RenderCommand::Clear();

        m_ShaderProgram->Bind();

        for (const auto& batch : std::views::concat(
            context.Batches.GeometryBuffer | std::views::values,
            context.StaticBatches.GeometryBuffer | std::views::values)
        )
        {
            Material& material = *batch.mesh->GetMaterial();

            context.SSBO.EntitySSBO.SetEntityIDs(batch.entityIDs);
            context.SSBO.ModelSSBO.SetModels(batch.transforms);
            context.SSBO.MaterialSSBO.SetMaterial(material);
            material.BindMaps();

            RenderCommand::DrawInstanced(*batch.mesh->GetVertexArray(), batch.GetSize());
        }

        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind();
    }
}
