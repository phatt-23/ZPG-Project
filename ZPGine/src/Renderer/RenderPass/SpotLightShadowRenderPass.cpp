#include "SpotLightShadowRenderPass.h"

#include "Buffer/VertexArray.h"
#include "Material/Material.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Platform/OpenGL/OpenGLCore.h"
#include "Profiling/Instrumentor.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderContext.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2DArray.h"

namespace ZPG
{
    SpotLightShadowRenderPass::SpotLightShadowRenderPass()
    {
    }

    SpotLightShadowRenderPass::~SpotLightShadowRenderPass()
    {
    }

    void SpotLightShadowRenderPass::Init(RenderContext &renderContext)
    {
        u32 width = renderContext.SpotLightShadowFramebuffer->GetSpecification().Width;
        u32 height = renderContext.SpotLightShadowFramebuffer->GetSpecification().Height;

        const int maxShadowCastingLights = 64;

        renderContext.SpotLightShadowMapArray = Texture2DArray::Create( width,
            height,
            maxShadowCastingLights,
            TextureDataFormat::Depth32F
        );

        renderContext.SpotLightShadowFramebuffer->AttachTexture(
            renderContext.SpotLightShadowMapArray,
            FrameBufferAttachment{
                .AttachType = FrameBufferAttachmentType::Depth,
                .TexType = TextureType::Texture2DArray,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0,
            }
        );

        m_ShaderProgram = ShaderProgram::Create("SpotLightShadowMap",
        {
            Shader::Create("assets/shaders/shadow-mapping/spotlight/SpotLightShadow.vert"),
            Shader::Create("assets/shaders/shadow-mapping/spotlight/SpotLightShadow.geom"),
            Shader::Create("assets/shaders/shadow-mapping/spotlight/SpotLightShadow.frag"),
        });
    }

    void SpotLightShadowRenderPass::Execute(RenderContext &renderContext)
    {
        ZPG_PROFILE_FUNCTION();

        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        // glCullFace(GL_FRONT);

        if (renderContext.SpotLights.empty()) {
            return;
        }

        renderContext.SpotLightShadowFramebuffer->Bind();
        RenderCommand::Clear();

        m_ShaderProgram->Bind();

        int index = 0;
        for (auto& spotLight : renderContext.SpotLights)
        {
            m_ShaderProgram->SetInt("u_Index", index);
            index++;

            for (auto& entity : renderContext.VisibleEntities)
            {
                m4 transform = entity->GetTransformMatrix();
                auto& meshes = entity->GetModel()->GetMeshes();

                for (auto& mesh : meshes)
                {
                    m4 local = mesh->GetLocalTransform();
                    auto& vao = mesh->GetVertexArray();

                    m_ShaderProgram->Bind();
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
        }

        m_ShaderProgram->Unbind();

        // glCullFace(GL_BACK);
        // glDisable(GL_CULL_FACE);

        renderContext.SpotLightShadowFramebuffer->Unbind();
    }

}
