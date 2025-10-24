#include "Material.h"

#include <Shader/CommonShaderUniforms.h>

#include "Debug/Asserter.h"
#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Texture/Texture.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {
Material::Material() {
    ResourceManager& res = ResourceManager::GetGlobal();

    m_ShaderProgram = res.GetShaderProgram(CommonResources::SHADER_PROGRAM_DEFAULT_LIT);

    m_Albedo = v4(1.0);
    m_Roughness = 0.5;
    m_Metallic = 0.5;
    m_Emissive = v4(0.0);

    m_AlbedoMap = res.GetTexture(CommonResources::NULL_ALBEDO_MAP);
    m_NormalMap = res.GetTexture(CommonResources::NULL_NORMAL_MAP);
    m_MetalnessMap = res.GetTexture(CommonResources::NULL_METALNESS_MAP);
    m_RoughnessMap = res.GetTexture(CommonResources::NULL_ROUGHNESS_MAP);
}
Material::~Material() {

}
void Material::Bind() {
    ZPG_NOT_IMPL();

    m_ShaderProgram->Bind();

    // bind factors
    m_ShaderProgram->SetFloat4(CommonShaderUniforms::ALBEDO, m_Albedo);
    m_ShaderProgram->SetFloat(CommonShaderUniforms::METALLIC, m_Metallic);
    m_ShaderProgram->SetFloat(CommonShaderUniforms::ROUGHNESS, m_Roughness);
    m_ShaderProgram->SetFloat4(CommonShaderUniforms::EMISSIVE, m_Emissive);

    // bind texture maps
    m_AlbedoMap->BindToSlot(0);
    m_ShaderProgram->SetInt(CommonShaderUniforms::ALBEDO_MAP, 0);

    m_NormalMap->BindToSlot(1);
    m_ShaderProgram->SetInt(CommonShaderUniforms::NORMAL_MAP, 1);

    m_NormalMap->BindToSlot(2);
    m_ShaderProgram->SetInt(CommonShaderUniforms::METALNESS_MAP, 2);

    m_NormalMap->BindToSlot(3);
    m_ShaderProgram->SetInt(CommonShaderUniforms::ROUGHNESS_MAP, 3);
}
void Material::Unbind() {
    m_ShaderProgram->Unbind();
}

void Material::SetShaderProgram(const ref<ShaderProgram>& shaderProgram) {
    m_ShaderProgram = shaderProgram;
}


void Material::SetAlbedoMap(const ref<Texture>& albedoMap) {
    m_AlbedoMap = albedoMap;
}
void Material::SetNormalMap(const ref<Texture>& normalMap) {
    m_NormalMap = normalMap;
}

void Material::SetMetalnessMap(const ref<Texture>& metalnessMap)
{
    m_MetalnessMap = metalnessMap;
}
void Material::SetRoughnessMap(const ref<Texture>& roughnessMap) {
    m_RoughnessMap = roughnessMap;
}

void Material::SetName(const std::string& name) { m_Name = name; }

std::string const& Material::GetName() const { return m_Name; }


void Material::SetAlbedo(const v4& albedo) {
    m_Albedo = albedo;
}
void Material::SetRoughness(f32 roughness) {
    m_Roughness = roughness;
}
void Material::SetMetallic(f32 metallic) {
    m_Metallic = metallic;
}
void Material::SetEmissive(const v4& emissive) {
    m_Emissive = emissive;
}

}
