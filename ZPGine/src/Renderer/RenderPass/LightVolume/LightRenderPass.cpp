#include "LightRenderPass.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"
#include "Texture/Texture2DArray.h"
#include "Texture/TextureCubeMap.h"
#include "Texture/TextureCubeMapArray.h"
#include "Resource/ResourceManager.h"
#include "Resource/CommonResources.h"
#include "Shader/CommonShaderUniforms.h"

namespace ZPG
{
    LightRenderPass::LightRenderPass()
    {
        m_SphereVAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_SPHERE);
        m_QuadVAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_QUAD);
        m_ConeVAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_CONE);

        m_NullSkyboxCubeMap = TextureCubeMap::Create("NullSkyboxCubeMap", 1, TextureDataFormat::RGBA8);
        for (int i = 0; i < 6; i++)
        {
            u8 nullCubemapData[] = { 0, 0, 0, 0 };
            m_NullSkyboxCubeMap->SetFaceData((CubemapFace)i, nullCubemapData, sizeof(nullCubemapData));
        }

        m_NullSkydomeMap = Texture2D::Create("NullSkydomeMap", 1, 1, TextureDataFormat::RGBA8);
    }

    LightRenderPass::~LightRenderPass()
    {
    }

    void LightRenderPass::BindGeometryMaps(const RenderContext &context) 
    {
        using namespace RenderBindingPoints;
        context.Targets.GeometryPositionMap->BindToSlot(GEOMETRY_POSITION_MAP);
        context.Targets.GeometryNormalMap->BindToSlot(GEOMETRY_NORMAL_MAP);
        context.Targets.GeometryAlbedoMetallicMap->BindToSlot(GEOMETRY_ALBEDO_METALLIC_MAP);
        context.Targets.GeometryEmissiveRoughnessMap->BindToSlot(GEOMETRY_EMISSIVE_ROUGHNESS_MAP); 
        context.Targets.GeometryEntityIDMap->BindToSlot(GEOMETRY_ENTITY_ID_MAP);
    }

    void LightRenderPass::BindLightMaps(const RenderContext &context) 
    {
        using namespace RenderBindingPoints;
        context.Targets.DirectionalLightShadowMapArray->BindToSlot(DIRECTIONAL_LIGHT_SHADOW_MAP_ARRAY);
        context.Targets.SpotLightShadowMapArray->BindToSlot(SPOTLIGHT_SHADOW_MAP_ARRAY);
        context.Targets.PointLightShadowCubeMapArray->BindToSlot(POINTLIGHT_SHADOW_CUBE_MAP_ARRAY);
    }

    void LightRenderPass::BindSky(RenderContext& context)
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

} // namespace ZPG
