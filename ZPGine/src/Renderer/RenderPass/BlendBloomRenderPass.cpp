#include "BlendBloomRenderPass.h"
#include "Buffer/VertexArray.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"
#include "Renderer/RenderCommand.h"
#include "Shader/CommonShaderUniforms.h"

static const char* blendVertexShader = R"(
    #version 460 core

    layout(location = 0) in vec2 a_Pos;
    layout(location = 1) in vec2 a_TexCoord;

    out vec2 v_TexCoord;

    void main(void)
    {
        v_TexCoord = a_TexCoord;
        gl_Position = vec4(a_Pos, 0.0, 1.0);
    }
)";

static const char* blendFragShader = R"(
    #version 460 core

    layout(location = 0) out vec4 f_Color0;

    in vec2 v_TexCoord;

    uniform sampler2D u_SceneMap;
    uniform sampler2D u_BloomMap;

    void main(void)
    {
        vec3 sceneTex = texture(u_SceneMap, v_TexCoord).rgb;
        vec3 bloomTex = texture(u_BloomMap, v_TexCoord).rgb;
        vec3 color = sceneTex + bloomTex;

        f_Color0 = vec4(color, 1.0);
    }
)";


namespace ZPG
{
    void BlendBloomRenderPass::Init(RenderContext &context)
    {
        m_ShaderProgram = ShaderProgram::Create("blend.program", 
        {
            Shader::CreateFromCode("blend.vert", Shader::Vertex, blendVertexShader),
            Shader::CreateFromCode("blend.frag", Shader::Fragment, blendFragShader),
        });
        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt(CommonShaderUniforms::SCENE_MAP, RenderBindingPoints::SCENE_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::BLOOM_MAP, RenderBindingPoints::BLOOM_MAP);
        m_ShaderProgram->Unbind();

        m_FrameBuffer = context.Targets.MainFrameBuffer;
    }

    void BlendBloomRenderPass::Execute(RenderContext &context)
    {
        context.Targets.MainColorMap->BindToSlot(RenderBindingPoints::SCENE_MAP);
        context.Targets.BloomMap->BindToSlot(RenderBindingPoints::BLOOM_MAP);

        m_FrameBuffer->Bind();
        m_ShaderProgram->Bind();

        // additive blending
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);       
        glBlendEquation(GL_FUNC_ADD);

        m_ScreenQuadVAO->Bind();
        RenderCommand::Draw(*m_ScreenQuadVAO);
        m_ScreenQuadVAO->Unbind();

        // restore
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind();
    }
}

