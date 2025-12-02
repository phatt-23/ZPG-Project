#include "BloomRenderPass.h"
#include "Buffer/FrameBuffer.h"
#include "Buffer/VertexArray.h"
#include "Renderer/DrawCommand.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderContext.h"
#include "Shader/CommonShaderUniforms.h"
#include "Texture/Texture2D.h"
#include "Buffer/FrameBufferAttachment.h"
#include "Shader/ShaderProgram.h"
#include "Shader/Shader.h"

static const char* bloomVertexShader = R"(

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

static const char* bloomFragShader = R"(

    #version 460 core

    layout(location = 0) out vec4 f_Color0;  // target

    in vec2 v_TexCoord;

    uniform sampler2D u_Map;  // source
    uniform int u_Iteration;

    float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216); // gaussian weight distribution

    void main(void)
    {
        vec3 color = vec3(0.0);
        vec2 offset = 1.0 / textureSize(u_Map, 0);

        color = weight[0] * texture(u_Map, v_TexCoord).rgb;

        if ((u_Iteration % 2) == 0)
        {
            for (int i = 1; i < weight.length(); i++)
            {
                color += weight[i] * texture(u_Map, v_TexCoord + vec2(i * offset.x, 0)).rgb;
                color += weight[i] * texture(u_Map, v_TexCoord - vec2(i * offset.x, 0)).rgb;
            }

            f_Color0 = vec4(color, 1.0);
        }
        else
        {
            for (int i = 1; i < weight.length(); i++)
            {
                color += weight[i] * texture(u_Map, v_TexCoord + vec2(0, i * offset.y)).rgb;
                color += weight[i] * texture(u_Map, v_TexCoord - vec2(0, i * offset.y)).rgb;
            }
        }

        f_Color0 = vec4(color, 1.0);
    }
)";


namespace ZPG
{
    static const int TARGET_MAP_BINDING_POINT = 0;

    void BloomRenderPass::Init(RenderContext& context) 
    {
        // settins up ping pong framebuffers

        FrameBufferSpecification frameBufferSpec;
        frameBufferSpec.Width = context.Targets.BloomMap->GetWidth();
        frameBufferSpec.Height = context.Targets.BloomMap->GetHeight();
        frameBufferSpec.Resizable = true;
        frameBufferSpec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::Color,
                .TexType = TextureType::Texture2D,
                .DataFormat = TextureDataFormat::RGBA16F,
                .Index = 0,
                .TextureAttachment = context.Targets.BloomMap,
            },
        };

        m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);


        m_PingPongTexture = Texture2D::Create("intermediate_map.texture2d", 
                                              context.Targets.BloomMap->GetWidth(), 
                                              context.Targets.BloomMap->GetHeight(), 
                                              TextureDataFormat::RGBA16F);

        FrameBufferSpecification pingPongFrameBufferSpec;
        pingPongFrameBufferSpec.Width = m_PingPongTexture->GetWidth();
        pingPongFrameBufferSpec.Height = m_PingPongTexture->GetHeight();
        pingPongFrameBufferSpec.Resizable = true;
        pingPongFrameBufferSpec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::Color,
                .TexType = TextureType::Texture2D,
                .DataFormat = TextureDataFormat::RGBA16F,
                .Index = 0,
                .TextureAttachment = m_PingPongTexture,
            },  // ping pong
        };

        m_PingPongFrameBuffer = FrameBuffer::Create(pingPongFrameBufferSpec);


        m_ShaderProgram = ShaderProgram::Create("bloom.program", 
        {
            Shader::CreateFromCode("bloom.vert", Shader::Vertex, bloomVertexShader),
            Shader::CreateFromCode("bloom.frag", Shader::Fragment, bloomFragShader),
        });
        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt("u_Map", TARGET_MAP_BINDING_POINT);
        m_ShaderProgram->Unbind();
    }
    
    void BloomRenderPass::Execute(RenderContext& context)
    {
        m_ShaderProgram->Bind();
        m_ScreenQuadVAO->Bind();

        int bloomAmount = 2 * context.SSBO.ProcessingSSBO.GetBloomAmount();

        for (int i = 0; i < bloomAmount; i++)
        {
            if ((i % 2) == 0) 
            {
                m_PingPongFrameBuffer->Bind();

                if (i == 0) {
                    // first iteration
                    context.Targets.BrightnessMap->BindToSlot(TARGET_MAP_BINDING_POINT);
                } 
                else {
                    context.Targets.BloomMap->BindToSlot(TARGET_MAP_BINDING_POINT);
                }
            }
            else 
            {
                m_FrameBuffer->Bind();
                m_PingPongTexture->BindToSlot(TARGET_MAP_BINDING_POINT);
            }

            m_ShaderProgram->SetInt("u_Iteration", i);
            RenderCommand::Draw(*m_ScreenQuadVAO);
        }

        m_ScreenQuadVAO->Unbind();
        m_ShaderProgram->Unbind();

        m_FrameBuffer->Unbind();
        m_PingPongFrameBuffer->Unbind();
    }

    void BloomRenderPass::OnResize(u32 width, u32 height) 
    {
        RenderPass::OnResize(width, height);
        m_PingPongFrameBuffer->Resize(width, height);
    }
}
