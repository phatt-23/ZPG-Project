#include "DirectionalLightShadowRenderPass.h"
#include "Buffer/VertexArray.h"
#include "Material/Material.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Renderer/RenderCommand.h"
#include "Shader/ShaderProgram.h"
#include "Shader/Shader.h"
#include "Renderer/RenderContext.h"
#include "Renderer/RenderGroups.h"

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

    layout(location = 0) out vec4 f_Color0;

    uniform vec4 u_Color;

    void main()
    {
        gl_FragDepth = gl_FragCoord.z;
        // f_Color0 = vec4(vec3(u_Color), 1);
        // f_Color0 = vec4(vec3(gl_FragCoord.z), 1.0);
        f_Color0 = v_WorldPos;
        // f_Color0 = vec4(1.0, 0, 0, 1);
    }
)";

namespace ZPG
{
    DirectionalLightShadowRenderPass::DirectionalLightShadowRenderPass()
    {
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
    }

    void DirectionalLightShadowRenderPass::Execute(RenderContext &renderContext)
    {
        if (renderContext.DirectionalLight == nullptr) {
            return;
        }

        renderContext.DirectionalLightShadowFramebuffer->Bind();
        RenderCommand::Clear();

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
                m_ShaderProgram->SetFloat4("u_Color", mesh->GetMaterial()->GetAlbedo());

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
