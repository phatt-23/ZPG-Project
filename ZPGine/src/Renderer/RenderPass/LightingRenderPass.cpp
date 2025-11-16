#include "LightingRenderPass.h"
#include "Debug/Asserter.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Renderer/RenderCommand.h"
#include "Resource/ResourceManager.h"
#include "Shader/ShaderProgram.h"
#include "Texture/TextureCubeMap.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/Shader.h"
#include "Buffer/BufferLayout.h"
#include "Buffer/VertexArray.h"
#include "Buffer/VertexBuffer.h"
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

        constexpr static float quadVertices[] =
        {
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

    void LightingRenderPass::Init(RenderContext &context)
    {
        ZPG_PROFILE_FUNCTION();

        m_ShaderProgram->Bind();

        m_ShaderProgram->SetInt("g_Color0", RenderBindingPoints::GEOMETRY_POSITION_MAP);
        m_ShaderProgram->SetInt("g_Color1", RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        m_ShaderProgram->SetInt("g_Color2", RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        m_ShaderProgram->SetInt("g_Color3", RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        m_ShaderProgram->SetInt("g_Color4", RenderBindingPoints::GEOMETRY_ENTITY_ID_MAP);

        m_ShaderProgram->SetInt(CommonShaderUniforms::DIRECTIONAL_LIGHT_SHADOW_MAP, RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::SPOTLIGHT_SHADOW_MAP_ARRAY, RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY);
        m_ShaderProgram->SetInt(CommonShaderUniforms::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY, RenderBindingPoints::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY);
        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYBOX_CUBEMAP, RenderBindingPoints::SKYBOX_TEXTURE_SLOT);
        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYDOME_MAP, RenderBindingPoints::SKYDOME_TEXTURE_SLOT);

        m_ShaderProgram->Unbind();

        m_FrameBuffer = context.Targets.MainFrameBuffer;
    }

    void LightingRenderPass::BindGeometryMaps(RenderContext& context) 
    {
        context.Targets.GeometryPositionMap->BindToSlot(RenderBindingPoints::GEOMETRY_POSITION_MAP);
        context.Targets.GeometryNormalMap->BindToSlot(RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        context.Targets.GeometryAlbedoMetallicMap->BindToSlot(RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        context.Targets.GeometryEmissiveRoughnessMap->BindToSlot(RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        context.Targets.GeometryEntityIDMap->BindToSlot(RenderBindingPoints::GEOMETRY_ENTITY_ID_MAP);
    }

    void LightingRenderPass::BindLightMaps(RenderContext& context) 
    {
        context.Targets.DirectionalLightShadowMap->BindToSlot(RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP);
        context.Targets.SpotLightShadowMapArray->BindToSlot(RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY);
        context.Targets.PointLightShadowCubeMapArray->BindToSlot(RenderBindingPoints::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY);
    }

    void LightingRenderPass::BindSky(RenderContext& context)
    {
        // bind dummy skybox map texture
        m_NullSkyboxCubeMap->BindToSlot(RenderBindingPoints::SKYBOX_TEXTURE_SLOT); 

        // bind dummy skydome map texture
        m_NullSkydomeMap->BindToSlot(RenderBindingPoints::SKYDOME_TEXTURE_SLOT); 

        if (context.ActiveSky != nullptr)
        {
            switch (context.ActiveSky->GetSkyType()) 
            {
                case SkyType::Skybox:
                    context.ActiveSky->BindTextureToSlot(RenderBindingPoints::SKYBOX_TEXTURE_SLOT); // this must come first
                    m_ShaderProgram->SetInt(CommonShaderUniforms::SKYTYPE, (i32)SkyType::Skybox);
                    break;

                case SkyType::Skydome: 
                    context.ActiveSky->BindTextureToSlot(RenderBindingPoints::SKYDOME_TEXTURE_SLOT); // this must come first
                    m_ShaderProgram->SetInt(CommonShaderUniforms::SKYTYPE, (i32)SkyType::Skydome);
                    break;

                case SkyType::None:
                    ZPG_UNREACHABLE("SkyType::None not supported");
            }
        }
    }

    void LightingRenderPass::Execute(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        m_FrameBuffer->Bind();

        RenderCommand::Clear();
        m_ShaderProgram->Bind(); 

        BindSky(context);
        BindGeometryMaps(context); 
        BindLightMaps(context);

        m_QuadVAO->Bind(); // bind the uv quad that fills up the NDC
        RenderCommand::Draw(*m_QuadVAO);
        m_QuadVAO->Unbind();

        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind();
    }
}

