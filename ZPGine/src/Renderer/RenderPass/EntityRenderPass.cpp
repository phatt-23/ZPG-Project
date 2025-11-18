//
// Created by phatt on 11/18/25.
//

#include "EntityRenderPass.h"

#include "Buffer/VertexArray.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderContext.h"
#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"

namespace ZPG
{

    void EntityRenderPass::Init(RenderContext& context)
    {
        m_FrameBuffer = context.Targets.MainFrameBuffer;

        m_QuadVAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_QUAD);

        m_ShaderProgram = ShaderProgram::Create("entity_render_pass.program",
        {

            Shader::CreateFromCode("entity_render_pass.vert", Shader::Vertex, R"(
                #version 440 core

                layout(location = 0) in vec3 a_Pos;
                layout(location = 1) in vec2 a_TexCoord;

                out vec2 v_TexCoord;

                void main()
                {
                    v_TexCoord = a_TexCoord;
                    gl_Position = vec4(a_Pos, 1.0);
                }
            )"),
            Shader::CreateFromCode("entity_render_pass.frag", Shader::Fragment, R"(
                #version 440 core
                #include "ext/ssbo/EntitySSBO.glsl"

                layout(location = 1) out int f_Color1;

                in vec2 v_TexCoord;
                uniform isampler2D g_EntityIDMap;

                void main()
                {
                    f_Color1 = texture(g_EntityIDMap, v_TexCoord).r;
                }
            )"),
        });
        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_ENTITY_ID_MAP, RenderBindingPoints::GEOMETRY_ENTITY_ID_MAP);
        m_ShaderProgram->Unbind();
    }

    void EntityRenderPass::Execute(RenderContext& context)
    {
        context.Targets.GeometryEntityIDMap->BindToSlot(RenderBindingPoints::GEOMETRY_ENTITY_ID_MAP);

        glDepthMask(GL_FALSE);

        m_FrameBuffer->Bind();
        m_ShaderProgram->Bind();
        m_QuadVAO->Bind();
        RenderCommand::Draw(*m_QuadVAO);
        m_QuadVAO->Unbind();
        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind();

        glDepthMask(GL_TRUE);
    }
} // ZPG