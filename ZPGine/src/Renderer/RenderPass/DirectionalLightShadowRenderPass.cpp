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

    static const char* vertexShader = R"(
        #version 440 core

        struct DirectionalLight
        {
            vec4 Color;
            vec3 Direction;
            float pad;
            mat4 ViewProj;
        };

        layout(location = 0) in vec3 a_Pos; // only need the objects position

        layout (std430, binding = 1) buffer EnvironmentLightShaderStorageBuffer
        {
            vec4 AmbientColor;
            DirectionalLight DirLight;
        } ssbo_EnvironmentLight;

        uniform mat4 u_Model;

        out vec4 v_WorldPos;

        void main()
        {
            mat4 lightViewProj = ssbo_EnvironmentLight.DirLight.ViewProj;
            v_WorldPos = u_Model * vec4(a_Pos, 1.0);
            gl_Position = lightViewProj * v_WorldPos;
        }
    )";

    static const char* fragShader = R"(
        #version 440 core

        in vec4 v_WorldPos;

        uniform vec4 u_Color;

        void main()
        {
            gl_FragDepth = gl_FragCoord.z;
        }
    )";

    DirectionalLightShadowRenderPass::DirectionalLightShadowRenderPass()
    {
        ZPG_PROFILE_FUNCTION();
        m_ShaderProgram = ShaderProgram::Create("directional_light_sp", {
            Shader::CreateFromCode("vertex_directional_light_sp", Shader::Vertex, vertexShader),
            Shader::CreateFromCode("frag_directional_light_sp", Shader::Fragment, fragShader),
        });
    }

    DirectionalLightShadowRenderPass::~DirectionalLightShadowRenderPass()
    {

    }

    void DirectionalLightShadowRenderPass::Init(RenderContext &renderContext)
    {
        u32 width = renderContext.DirectionalLightShadowFramebuffer->GetSpecification().Width;
        u32 height = renderContext.DirectionalLightShadowFramebuffer->GetSpecification().Height;

        renderContext.DirectionalLightShadowMap = Texture2D::Create(
            "directional_light_shadow_map",
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
    }

    void DirectionalLightShadowRenderPass::Execute(RenderContext &renderContext)
    {
        ZPG_PROFILE_FUNCTION();

        renderContext.DirectionalLightShadowFramebuffer->Bind();
        RenderCommand::Clear();

        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);

        if (renderContext.DirectionalLight != nullptr) {
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
        }

        // glCullFace(GL_BACK);

        renderContext.DirectionalLightShadowFramebuffer->Unbind();
    }
}
