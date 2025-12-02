#include "ToneMapRenderPass.h"
#include "Buffer/VertexArray.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"
#include "Renderer/RenderCommand.h"

static const char* vertexShader = R"(
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

static const char* fragmentShader = R"(
    #version 460 core
    #include "ext/ssbo/ProcessingSSBO.glsl"

    layout(location = 0) out vec4 f_Color0;

    in vec2 v_TexCoord;

    uniform sampler2D u_SceneMap;

    void main(void)
    {
        vec3 color = texture(u_SceneMap, v_TexCoord).rgb;

        // exposure tone mapping
        color = vec3(1.0) - exp(-color * ssbo_Processing.Exposure);

        // gamma correct
        color = pow(color, vec3(1.0 / ssbo_Processing.Gamma));

        f_Color0 = vec4(color, 1.0);
    }
)";


namespace ZPG
{
    void ToneMapRenderPass::Init(RenderContext& context)
    {
        m_FrameBuffer = context.Targets.MainFrameBuffer;
        m_ShaderProgram = ShaderProgram::Create("tone_map.program", 
        {
            Shader::CreateFromCode("tone_map.vert", Shader::Vertex, vertexShader),
            Shader::CreateFromCode("tone_map.frag", Shader::Fragment, fragmentShader),
        });
        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt(CommonShaderUniforms::SCENE_MAP, RenderBindingPoints::SCENE_MAP);
        m_ShaderProgram->Unbind();
    }

    void ToneMapRenderPass::Execute(RenderContext& context)
    {
        context.Targets.MainColorMap->BindToSlot(RenderBindingPoints::SCENE_MAP);

        m_FrameBuffer->Bind();
        m_ShaderProgram->Bind();

        glDisable(GL_DEPTH_TEST);
        
        m_ScreenQuadVAO->Bind();
        RenderCommand::Draw(*m_ScreenQuadVAO);
        m_ScreenQuadVAO->Unbind();

        glEnable(GL_DEPTH_TEST);

        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind();
    }
}
