#include "LightRenderPass.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Texture/Texture2D.h"
#include "Texture/Texture2DArray.h"
#include "Texture/TextureCubeMapArray.h"
#include "Resource/ResourceManager.h"
#include "Resource/CommonResources.h"

namespace ZPG
{
    LightRenderPass::LightRenderPass()
    {
        m_SphereVAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_SPHERE);
        m_QuadVAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_QUAD);
        m_ConeVAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_CONE);
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
        context.Targets.DirectionalLightShadowMap->BindToSlot(DIRECTIONAL_LIGHT_SHADOW_MAP);
        context.Targets.SpotLightShadowMapArray->BindToSlot(SPOTLIGHT_SHADOW_MAP_ARRAY);
        context.Targets.PointLightShadowCubeMapArray->BindToSlot(POINTLIGHT_SHADOW_CUBE_MAP_ARRAY);
    }
} // namespace ZPG
