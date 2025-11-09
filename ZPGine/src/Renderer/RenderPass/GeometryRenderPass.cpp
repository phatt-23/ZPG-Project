#include "GeometryRenderPass.h"
#include "Buffer/VertexArray.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Buffer/FrameBuffer.h"
#include "Renderer/RenderGroups.h"
#include "Material/Material.h"
#include "Renderer/RenderCommand.h"
#include "Shader/ShaderProgram.h"
#include "Shader/CommonShaderUniforms.h"
#include "Texture/Texture.h"
#include "Model/Model.h"
#include "Model/Mesh.h"
#include "Profiling/Instrumentor.h"
#include "Shader/Shader.h"

namespace ZPG
{
    GeometryRenderPass::GeometryRenderPass()
    {
    }

    GeometryRenderPass::~GeometryRenderPass()
    {
    }

    void GeometryRenderPass::Init(RenderContext& renderContext)
    {
        ZPG_PROFILE_FUNCTION();
        m_ShaderProgram = ShaderProgram::Create("GeometryPassSP", {
            Shader::Create("VertexGeometryPassSP", "./assets/shaders/multipass/vert/GeometryPass.vert"),
            Shader::Create("FragmentGeometryPassSP", "./assets/shaders/multipass/frag/GeometryPass.frag"),
        });

        m_ShaderProgram->Bind();

        m_ShaderProgram->SetInt(CommonShaderUniforms::ALBEDO_MAP,       RenderBindingPoints::ALBEDO_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::METALNESS_MAP,    RenderBindingPoints::METALNESS_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::ROUGHNESS_MAP,    RenderBindingPoints::ROUGHNESS_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::NORMAL_MAP,       RenderBindingPoints::NORMAL_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::EMISSIVE_MAP,     RenderBindingPoints::EMISSIVE_MAP);

        m_ShaderProgram->Unbind();
    }

    void GeometryRenderPass::Execute(RenderContext& renderContext)
    {
        ZPG_PROFILE_FUNCTION();
        renderContext.GeometryPassFramebuffer->Bind(); // bind the g-buffer so the consequent draw calls will draw into it
        RenderCommand::Clear();

        m_ShaderProgram->Bind(); // bind the gpass shader program and set the sampler slots

        for (const auto& entity : renderContext.VisibleEntities)
        {
            const auto& model = entity->GetModel();
            const auto& entTransform = entity->GetTransformMatrix();
            const auto& meshes = model->GetMeshes();

            for (auto& mesh : meshes)
            {
                if (renderContext.Batch.IsFull())
                {
                    Flush(renderContext);
                }

                renderContext.Batch.SubmitMesh(*mesh, entTransform, entity->GetEntityID());
            }
        }

        Flush(renderContext);

        m_ShaderProgram->Unbind();

        renderContext.GeometryPassFramebuffer->Unbind();
    }

    void GeometryRenderPass::Flush(RenderContext& renderContext)
    {
        ZPG_PROFILE_FUNCTION();
        RenderPass::Flush(renderContext);
        renderContext.Statistics.FlushCountPerFrame++;
    }
}
