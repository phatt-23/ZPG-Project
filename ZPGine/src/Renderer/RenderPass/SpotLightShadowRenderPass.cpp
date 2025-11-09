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


namespace ZPG
{


static const char* vertexShader = R"(
#version 440 core

layout(location = 0) in vec3 a_Pos; // only need the objects position

struct SpotLight
{
    vec4 Color;
    vec3 Position;
    int ShadowLayer;
    vec3 Direction;
    float BeamSize;
    vec3 Attenuation;
    float BeamBlend;
    mat4 ViewProj;
};

layout (std430, binding = 3) buffer SpotLightShaderStorageBuffer
{
    int Count;
    int ShadowLayer; // just for debug
    SpotLight LightArray[];
} ssbo_SpotLight;

uniform mat4 u_Model;
uniform int u_Index;

void main()
{
    SpotLight light = ssbo_SpotLight.LightArray[u_Index];

    mat4 lightViewProj = light.ViewProj;
    vec4 WorldPos = u_Model * vec4(a_Pos, 1.0);

    gl_Position = lightViewProj * WorldPos;
}
)";

static const char* fragmentShader = R"(

#version 440 core
void main()
{
}

)";

    SpotLightShadowRenderPass::SpotLightShadowRenderPass()
    {
    }

    SpotLightShadowRenderPass::~SpotLightShadowRenderPass()
    {

    }

    const int maxShadowCastingLights = 64;

    void SpotLightShadowRenderPass::Init(RenderContext &renderContext)
    {
        ZPG_OPENGL_CALL(glGenTextures(1, &renderContext.SpotLightShadowMapArrayRendererID));
        ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, renderContext.SpotLightShadowMapArrayRendererID));

        // GLenum target, GLint level, GLint internalformat,
        // GLsizei width, GLsizei height, GLsizei depth,
        // GLint border,
        // GLenum format,
        // GLenum type,
        // const void *pixels
        ZPG_OPENGL_CALL(glTexImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            GL_DEPTH_COMPONENT32F,
            renderContext.SpotLightShadowFramebuffer->GetSpecification().Width,
            renderContext.SpotLightShadowFramebuffer->GetSpecification().Height,
            maxShadowCastingLights,
            0,
            GL_DEPTH_COMPONENT,
            GL_FLOAT,
            nullptr
        ));

        // Configure sampling
        ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

        float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        ZPG_OPENGL_CALL(glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor));
        ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));

        m_ShaderProgram = ShaderProgram::Create("SpotLightShadowMap", {
            Shader::CreateFromCode("SpotLightShadowMap.vert", Shader::Vertex, vertexShader),
            Shader::CreateFromCode("SpotLightShadowMap.frag", Shader::Fragment, fragmentShader),
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

        // ZPG_OPENGL_CALL(glActiveTexture(GL_TEXTURE0 + RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY));
        // ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_2D_ARRAY, renderContext.SpotLightShadowMapArrayRendererID));
        // ZPG_OPENGL_CALL(glBindTextureUnit(RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY, renderContext.SpotLightShadowMapArrayRendererID));


        m_ShaderProgram->Bind();

        int index = 0;
        for (auto& spotLight : renderContext.SpotLights)
        {
            m_ShaderProgram->Bind();
            m_ShaderProgram->SetInt("u_Index", index);
            index++;

            renderContext.SpotLightShadowFramebuffer->Bind();
            // GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer
            ZPG_OPENGL_CALL(glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, renderContext.SpotLightShadowMapArrayRendererID, 0, spotLight.ShadowLayer));
            ZPG_OPENGL_CALL(glClear(GL_DEPTH_BUFFER_BIT));

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
