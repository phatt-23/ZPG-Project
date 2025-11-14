#include "PointLightShadowRenderPass.h"
#include "Buffer/VertexArray.h"
#include "Platform/OpenGL/OpenGLCore.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderContext.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Model/Model.h"
#include "Model/Mesh.h"
#include "Texture/TextureCubeMap.h"
#include "Texture/TextureCubeMapArray.h"

namespace ZPG
{

    PointLightShadowRenderPass::PointLightShadowRenderPass()
    {
    }

    PointLightShadowRenderPass::~PointLightShadowRenderPass()
    {
    }

    void PointLightShadowRenderPass::Init(RenderContext& renderContext)
    {
        int length = 1024;
        int numCubeMaps = 10;
        int depth = 6 * numCubeMaps;

        renderContext.PointLightShadowCubeMapArray = TextureCubeMapArray::Create(
            length,
            depth,
            TextureDataFormat::Depth32F
        );

        renderContext.PointLightShadowFramebuffer->AttachTexture(
            renderContext.PointLightShadowCubeMapArray,
            FrameBufferAttachment{
                .AttachType = FrameBufferAttachmentType::Depth,
                .TexType = TextureType::TextureCubeMapArray,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0,
            }
        );

        m_ShaderProgram = ShaderProgram::Create("PointLightShadowMap",
        {
            Shader::Create("assets/shaders/shadow-mapping/pointlight/PointLightShadow.vert"),
            Shader::Create("assets/shaders/shadow-mapping/pointlight/PointLightShadow.geom"),
            Shader::Create("assets/shaders/shadow-mapping/pointlight/PointLightShadow.frag"),
        });
    }

    void PointLightShadowRenderPass::Execute(RenderContext& renderContext)
    {
        if (renderContext.PointLights.empty()) {
            return;
        }

        renderContext.PointLightShadowFramebuffer->Bind();
        RenderCommand::Clear();

        m_ShaderProgram->Bind();

        int index = 0;
        for (auto& pointLight : renderContext.PointLights)
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

        renderContext.PointLightShadowFramebuffer->Unbind();
    }

}
