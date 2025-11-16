#include "Material.h"

#include <Shader/CommonShaderUniforms.h>

#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"
#include "Renderer/RenderBindingPoints.h"
#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Texture/Texture2D.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {

    Material::Material(const std::string& name)
        : m_Name(name)
    {
        ZPG_PROFILE_FUNCTION();
        ResourceManager& res = ResourceManager::GetGlobal();

        m_ShaderProgram = res.GetShaderProgram(CommonResources::SHADER_PROGRAM_DEFAULT);

        m_Albedo = v4(1.0);
        m_Roughness = 1.0;
        m_Metallic = 0.0;
        m_Emissive = v4(0.0);

        m_AlbedoMap = res.GetTexture<Texture2D>(CommonResources::NULL_ALBEDO_MAP);
        m_NormalMap = res.GetTexture<Texture2D>(CommonResources::NULL_NORMAL_MAP);
        m_MetalnessMap = res.GetTexture<Texture2D>(CommonResources::NULL_METALNESS_MAP);
        m_RoughnessMap = res.GetTexture<Texture2D>(CommonResources::NULL_ROUGHNESS_MAP);
        m_EmissiveMap = res.GetTexture<Texture2D>(CommonResources::NULL_EMISSIVE_MAP);
    }

    Material::~Material()
    {
    }

    void Material::BindMaps()
    {
        m_AlbedoMap->BindToSlot(RenderBindingPoints::ALBEDO_MAP);
        m_NormalMap->BindToSlot(RenderBindingPoints::NORMAL_MAP);
        m_EmissiveMap->BindToSlot(RenderBindingPoints::EMISSIVE_MAP);
        m_MetalnessMap->BindToSlot(RenderBindingPoints::METALNESS_MAP);
        m_RoughnessMap->BindToSlot(RenderBindingPoints::ROUGHNESS_MAP);
    }

    void Material::SetShaderProgram(const ref<ShaderProgram>& shaderProgram) {
        ZPG_PROFILE_FUNCTION();
        m_ShaderProgram = shaderProgram;
    }
    ref<ShaderProgram>& Material::GetShaderProgram() {
        ZPG_PROFILE_FUNCTION();
        return m_ShaderProgram;
    }

    void Material::SetAlbedoMap(const ref<Texture2D>& albedoMap) {
        ZPG_PROFILE_FUNCTION();
        m_AlbedoMap = albedoMap;
    }
    void Material::SetNormalMap(const ref<Texture2D>& normalMap) {
        ZPG_PROFILE_FUNCTION();
        m_NormalMap = normalMap;
    }
    void Material::SetMetalnessMap(const ref<Texture2D>& metalnessMap) {
        ZPG_PROFILE_FUNCTION();
        m_MetalnessMap = metalnessMap;
    }
    void Material::SetRoughnessMap(const ref<Texture2D>& roughnessMap) {
        ZPG_PROFILE_FUNCTION();
        m_RoughnessMap = roughnessMap;
    }
    void Material::SetEmissiveMap(const ref<Texture2D>& emissiveMap) {
        ZPG_PROFILE_FUNCTION();
        m_EmissiveMap = emissiveMap;
    }

    const ref<Texture2D>& Material::GetAlbedoMap() const {
        ZPG_PROFILE_FUNCTION();
        return m_AlbedoMap;
    }
    const ref<Texture2D>& Material::GetNormalMap() const {
        ZPG_PROFILE_FUNCTION();
        return m_NormalMap;
    }
    const ref<Texture2D>& Material::GetMetalnessMap() const {
        ZPG_PROFILE_FUNCTION();
        return m_MetalnessMap;
    }
    const ref<Texture2D>& Material::GetRoughnessMap() const {
        ZPG_PROFILE_FUNCTION();
        return m_RoughnessMap;
    }
    const ref<Texture2D>& Material::GetEmissiveMap() const {
        ZPG_PROFILE_FUNCTION();
        return m_EmissiveMap;
    }

    void Material::SetName(const std::string& name) {
        ZPG_PROFILE_FUNCTION();
        m_Name = name;
    }
    std::string const& Material::GetName() const {
        ZPG_PROFILE_FUNCTION();
        return m_Name;
    }

    void Material::SetAlbedo(const v4& albedo) {
        ZPG_PROFILE_FUNCTION();
        m_Albedo = albedo;
    }
    void Material::SetRoughness(f32 roughness) {
        ZPG_PROFILE_FUNCTION();
        m_Roughness = roughness;
    }
    void Material::SetMetallic(f32 metallic) {
        ZPG_PROFILE_FUNCTION();
        m_Metallic = metallic;
    }
    void Material::SetEmissive(const v4& emissive) {
        ZPG_PROFILE_FUNCTION();
        m_Emissive = emissive;
    }

    const v4& Material::GetAlbedo() const {
        ZPG_PROFILE_FUNCTION();
        return m_Albedo;
    }
    f32 Material::GetRoughness() const {
        ZPG_PROFILE_FUNCTION();
        return m_Roughness;
    }
    f32 Material::GetMetallic() const {
        ZPG_PROFILE_FUNCTION();
        return m_Metallic;
    }
    const v4& Material::GetEmissive() const {
        ZPG_PROFILE_FUNCTION();
        return m_Emissive;
    }

}
