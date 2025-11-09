#include "LightingRenderPass.h"
#include "Debug/Asserter.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Renderer/RenderCommand.h"
#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Shader/ShaderProgram.h"
#include "Texture/CubemapTexture.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/Shader.h"
#include "Buffer/BufferLayout.h"
#include "Profiling/Instrumentor.h"

namespace ZPG
{
    LightingRenderPass::LightingRenderPass()
    {
        ZPG_PROFILE_FUNCTION();
        m_ShaderProgram = ShaderProgram::Create("LightingPassSP", {
            Shader::Create("VertexLightingPassSP", "assets/shaders/multipass/vert/LightingPass.vert"),
            Shader::Create("VertexLightingPassSP", "assets/shaders/multipass/frag/LightingPass.frag"),
        });


        m_NullSkyboxCubeMap = CubemapTexture::Create("NullSkyboxCubeMap", 1, 1, TextureDataFormat::RGBA8);
        for (int i = 0; i < 6; i++)
        {
            u8 nullCubemapData[] = { 0, 0, 0, 0 };
            m_NullSkyboxCubeMap->SetFaceData((CubemapFace)i, nullCubemapData, sizeof(nullCubemapData));
        }


        m_NullSkydomeMap = Texture::Create("NullSkydomeMap", 1, 1, TextureDataFormat::RGBA8);


        constexpr static float quadVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        m_QuadVAO = VertexArray::Create({
            VertexBuffer::Create(quadVertices, sizeof(quadVertices), {
                {ShaderDataType::Float2, "a_Pos"}, {ShaderDataType::Float2, "a_TexCoords"}
            })
        });
    }

    LightingRenderPass::~LightingRenderPass()
    {

    }

    void LightingRenderPass::Init(RenderContext &renderContext)
    {
        ZPG_PROFILE_FUNCTION();
        m_ShaderProgram->Bind();
        for (auto& colorAttachment : renderContext.GeometryPassFramebuffer->GetColorTextureAttachments() | std::views::keys)
        {
            std::string uniformName = "g_Color" + std::to_string(colorAttachment.Index);
            m_ShaderProgram->SetInt(uniformName, colorAttachment.Index);
        }
        m_ShaderProgram->Unbind();
    }

    void LightingRenderPass::Execute(RenderContext &renderContext)
    {
        ZPG_PROFILE_FUNCTION();
        renderContext.MainFramebuffer->Bind();
        {
            RenderCommand::Clear();

            m_ShaderProgram->Bind();

            // SKY

            // bind dummy skybox map texture
            m_NullSkyboxCubeMap->BindToSlot(RenderBindingPoints::SKYBOX_TEXTURE_SLOT);
            m_ShaderProgram->SetInt(CommonShaderUniforms::SKYBOX_CUBEMAP, RenderBindingPoints::SKYBOX_TEXTURE_SLOT);

            // bind dummy skydome map texture
            m_NullSkydomeMap->BindToSlot(RenderBindingPoints::SKYDOME_TEXTURE_SLOT);
            m_ShaderProgram->SetInt(CommonShaderUniforms::SKYDOME_MAP, RenderBindingPoints::SKYDOME_TEXTURE_SLOT);

            if (renderContext.ActiveSky != nullptr)
            {
                switch (renderContext.ActiveSky->GetSkyType()) 
                {
                    case SkyType::Skybox:
                        renderContext.ActiveSky->BindTextureToSlot(RenderBindingPoints::SKYBOX_TEXTURE_SLOT); // this must come first
                        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYTYPE, (i32)SkyType::Skybox);
                        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYBOX_CUBEMAP, RenderBindingPoints::SKYBOX_TEXTURE_SLOT);
                        break;

                    case SkyType::Skydome: 
                        renderContext.ActiveSky->BindTextureToSlot(RenderBindingPoints::SKYDOME_TEXTURE_SLOT); // this must come first
                        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYTYPE, (i32)SkyType::Skydome);
                        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYDOME_MAP, RenderBindingPoints::SKYDOME_TEXTURE_SLOT);
                        break;

                    case SkyType::None:
                        ZPG_UNREACHABLE("SkyType::None not supported");
                }
            }

            // G-BUFFER

            // bind the g-buffer's color texture attachments with the collected geometry information
            renderContext.GeometryPassFramebuffer->BindColorTextureAttachments();

            // DIRECTIONAL LIGHT SHADOW

            auto depthAttachmentIter = std::ranges::find_if(renderContext.DirectionalLightShadowFramebuffer->GetTextureAttachments(),
                [](const std::pair<FrameBufferAttachment, ref<Texture>>& pair)
                {
                    return pair.first.AttachmentType == FrameBufferAttachmentType::Depth;
                });

            ZPG_CORE_ASSERT(depthAttachmentIter != renderContext.DirectionalLightShadowFramebuffer->GetTextureAttachments().end(), "Directional Light Shadow Framebuffer must have a depth attachment");

            m_ShaderProgram->SetInt(CommonShaderUniforms::DIRECTIONAL_LIGHT_SHADOW_MAP, RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP);
            depthAttachmentIter->second->BindToSlot(RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP);

            // SPOTLIGHT SHADOW MAP ARRAY

            m_ShaderProgram->SetInt(CommonShaderUniforms::SPOTLIGHT_SHADOW_MAP_ARRAY, RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY);
            glActiveTexture(GL_TEXTURE0 + RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY);
            glBindTexture(GL_TEXTURE_2D_ARRAY, renderContext.SpotLightShadowMapArrayRendererID);

            // bind the uv quad that fills up the NDC
            m_QuadVAO->Bind(); 
            RenderCommand::DrawArrays(*m_QuadVAO);
            m_QuadVAO->Unbind();

            // bind the lighting pass shader program
            m_ShaderProgram->Unbind();

            // Copy the G-Buffer's depth buffer into the main FBO's depth buffer
            renderContext.MainFramebuffer->CopyAttachment(renderContext.GeometryPassFramebuffer, FrameBufferAttachmentType::Depth);
        }
        renderContext.MainFramebuffer->Unbind();

        renderContext.MainFramebuffer->Bind();
        {
            if (renderContext.ActiveSky) 
            {
                const ref<VertexArray>& skyVertexArray = renderContext.ActiveSky->GetVertexArray();

                glDepthFunc(GL_LEQUAL);
                glDepthMask(GL_FALSE);

                renderContext.ActiveSky->Bind();
                RenderCommand::DrawIndexed(*skyVertexArray, skyVertexArray->GetIndexBuffer()->GetCount());
                renderContext.ActiveSky->Unbind();
                
                glDepthMask(GL_TRUE); // restore
                glDepthFunc(GL_LESS);
            }
        }
        renderContext.MainFramebuffer->Unbind();
    }
}

