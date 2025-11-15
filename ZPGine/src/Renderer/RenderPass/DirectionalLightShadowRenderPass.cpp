#include "DirectionalLightShadowRenderPass.h"
#include "Buffer/VertexArray.h"
#include "Material/Material.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Profiling/Instrumentor.h"
#include "Renderer/RenderCommand.h"
#include "Shader/ShaderProgram.h"
#include "Shader/Shader.h"
#include "Renderer/RenderContext.h"
#include "Renderer/RenderGroups.h"
#include "Texture/Texture2D.h"

namespace ZPG
{
    DirectionalLightShadowRenderPass::DirectionalLightShadowRenderPass()
    {
    }

    DirectionalLightShadowRenderPass::~DirectionalLightShadowRenderPass()
    {
    }

    void DirectionalLightShadowRenderPass::Init(RenderContext &renderContext)
    {
        ZPG_PROFILE_FUNCTION();

        u32 width = renderContext.DirectionalLightShadowFramebuffer->GetSpecification().Width;
        u32 height = renderContext.DirectionalLightShadowFramebuffer->GetSpecification().Height;

        renderContext.DirectionalLightShadowMap = Texture2D::Create(
            "DirectionalShadow.texture2d",
            width,
            height,
            TextureDataFormat::Depth32F
        );

        renderContext.DirectionalLightShadowFramebuffer->AttachTexture(
            renderContext.DirectionalLightShadowMap,
            FrameBufferAttachment{
                .AttachType = FrameBufferAttachmentType::Depth,
                .TexType = TextureType::Texture2D,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0,
            }
        );

        m_ShaderProgram = ShaderProgram::Create("DirectionalShadow.program",
        {
            Shader::Create("./assets/shaders/shadow-mapping/directional/DirectionalShadow.vert"),
            Shader::Create("./assets/shaders/shadow-mapping/directional/DirectionalShadow.frag"),
        });
    }

    void DirectionalLightShadowRenderPass::Execute(RenderContext &renderContext)
    {
        ZPG_PROFILE_FUNCTION();

        renderContext.DirectionalLightShadowFramebuffer->Bind();
        RenderCommand::Clear();

        if (renderContext.DirectionalLight == nullptr) {
            return;
        }

        m_ShaderProgram->Bind();

        for (auto& entity : renderContext.VisibleEntities)
        {
            m4 transform = entity->GetTransformMatrix();
            auto& meshes = entity->GetModel()->GetMeshes();

            for (auto& mesh : meshes)
            {
                m4 local = mesh->GetLocalTransform();
                auto& vao = mesh->GetVertexArray();

                m_ShaderProgram->SetMat4("u_Model", transform * local);

                vao->Bind();

                if (vao->HasIndexBuffer()) {
                    RenderCommand::DrawIndexed(*vao, vao->GetIndexBuffer()->GetCount());
                }
                else {
                    RenderCommand::DrawArrays(*vao);
                }

                vao->Unbind();
            }
        }

        m_ShaderProgram->Unbind();

        renderContext.DirectionalLightShadowFramebuffer->Unbind();
    }
}
