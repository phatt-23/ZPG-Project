#include "LightingRenderPass.h"
#include "Debug/Asserter.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Renderer/RenderCommand.h"
#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Shader/ShaderProgram.h"
#include "Texture/TextureCubeMap.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/Shader.h"
#include "Buffer/BufferLayout.h"
#include "Profiling/Instrumentor.h"
#include "Texture/Texture2DArray.h"
#include "Texture/TextureCubeMapArray.h"

namespace ZPG
{
    LightingRenderPass::LightingRenderPass()
    {
        ZPG_PROFILE_FUNCTION();
        m_ShaderProgram = ShaderProgram::Create("LightingPass",
        {
            Shader::Create("assets/shaders/multipass/LightingPass.vert"),
            Shader::Create("assets/shaders/multipass/LightingPass.frag"),
        });


        m_NullSkyboxCubeMap = TextureCubeMap::Create("NullSkyboxCubeMap", 1, TextureDataFormat::RGBA8);
        for (int i = 0; i < 6; i++)
        {
            u8 nullCubemapData[] = { 0, 0, 0, 0 };
            m_NullSkyboxCubeMap->SetFaceData((CubemapFace)i, nullCubemapData, sizeof(nullCubemapData));
        }


        m_NullSkydomeMap = Texture2D::Create("NullSkydomeMap", 1, 1, TextureDataFormat::RGBA8);


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

        m_ShaderProgram->SetInt(CommonShaderUniforms::DIRECTIONAL_LIGHT_SHADOW_MAP, RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::SPOTLIGHT_SHADOW_MAP_ARRAY, RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY);
        m_ShaderProgram->SetInt(CommonShaderUniforms::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY, RenderBindingPoints::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY);
        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYBOX_CUBEMAP, RenderBindingPoints::SKYBOX_TEXTURE_SLOT);
        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYDOME_MAP, RenderBindingPoints::SKYDOME_TEXTURE_SLOT);

        m_ShaderProgram->Unbind();
    }

    void LightingRenderPass::Execute(RenderContext &renderContext)
    {
        ZPG_PROFILE_FUNCTION();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        renderContext.MainFramebuffer->Bind();
        {
            RenderCommand::Clear();

            m_ShaderProgram->Bind();

            // SKY
            m_NullSkyboxCubeMap->BindToSlot(RenderBindingPoints::SKYBOX_TEXTURE_SLOT); // bind dummy skybox map texture
            m_NullSkydomeMap->BindToSlot(RenderBindingPoints::SKYDOME_TEXTURE_SLOT); // bind dummy skydome map texture
            if (renderContext.ActiveSky != nullptr)
            {
                switch (renderContext.ActiveSky->GetSkyType()) 
                {
                    case SkyType::Skybox:
                        renderContext.ActiveSky->BindTextureToSlot(RenderBindingPoints::SKYBOX_TEXTURE_SLOT); // this must come first
                        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYTYPE, (i32)SkyType::Skybox);
                        break;

                    case SkyType::Skydome: 
                        renderContext.ActiveSky->BindTextureToSlot(RenderBindingPoints::SKYDOME_TEXTURE_SLOT); // this must come first
                        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYTYPE, (i32)SkyType::Skydome);
                        break;

                    case SkyType::None:
                        ZPG_UNREACHABLE("SkyType::None not supported");
                }
            }

            // G-BUFFER
            // bind the g-buffer's color texture attachments with the collected geometry information
            renderContext.GeometryPassFramebuffer->BindColorTextureAttachments();

            // DIRECTIONAL LIGHT SHADOW
            renderContext.DirectionalLightShadowMap->BindToSlot(RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP);

            // SPOTLIGHT SHADOW MAP ARRAY
            renderContext.SpotLightShadowMapArray->BindToSlot(RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY);

            // POINT LIGHT SHADOW MAP ARRAY
            renderContext.PointLightShadowCubeMapArray->BindToSlot(RenderBindingPoints::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY);

            // bind the uv quad that fills up the NDC
            m_QuadVAO->Bind(); 
            RenderCommand::DrawArrays(*m_QuadVAO);
            m_QuadVAO->Unbind();

            // bind the lighting pass shader program
            m_ShaderProgram->Unbind();

            // Copy the G-Buffer's depth buffer into the main FBO's depth buffer
            renderContext.MainFramebuffer->CopyAttachment(renderContext.GeometryPassFramebuffer, FrameBufferAttachmentType::Depth);

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

