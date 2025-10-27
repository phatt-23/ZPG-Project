#include "Material.h"

#include <Shader/CommonShaderUniforms.h>

#include "Debug/Asserter.h"
#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Texture/Texture.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {

Material::Material(const std::string& name) 
: m_Name(name) {
    ResourceManager& res = ResourceManager::GetGlobal();

    m_ShaderProgram = res.GetShaderProgram(CommonResources::SHADER_PROGRAM_DEFAULT);

    m_Albedo = v4(1.0);
    m_Roughness = 1.0;
    m_Metallic = 0.0;
    m_Emissive = v4(0.0);

    m_AlbedoMap = res.GetTexture(CommonResources::NULL_ALBEDO_MAP);
    m_NormalMap = res.GetTexture(CommonResources::NULL_NORMAL_MAP);
    m_MetalnessMap = res.GetTexture(CommonResources::NULL_METALNESS_MAP);
    m_RoughnessMap = res.GetTexture(CommonResources::NULL_ROUGHNESS_MAP);
    m_EmissiveMap = res.GetTexture(CommonResources::NULL_EMISSIVE_MAP);
}

Material::~Material() {

}

void Material::SetShaderProgram(const ref<ShaderProgram>& shaderProgram) {
    m_ShaderProgram = shaderProgram;
}
ref<ShaderProgram>& Material::GetShaderProgram() { 
    return m_ShaderProgram; 
}

void Material::SetAlbedoMap(const ref<Texture>& albedoMap) {
    m_AlbedoMap = albedoMap;
}
void Material::SetNormalMap(const ref<Texture>& normalMap) {
    m_NormalMap = normalMap;
}
void Material::SetMetalnessMap(const ref<Texture>& metalnessMap) {
    m_MetalnessMap = metalnessMap;
}
void Material::SetRoughnessMap(const ref<Texture>& roughnessMap) {
    m_RoughnessMap = roughnessMap;
}
void Material::SetEmissiveMap(const ref<Texture>& emissiveMap) {
    m_EmissiveMap = emissiveMap;
}

const ref<Texture>& Material::GetAlbedoMap() const {
    return m_AlbedoMap;
}
const ref<Texture>& Material::GetNormalMap() const {
    return m_NormalMap;
}
const ref<Texture>& Material::GetMetalnessMap() const {
    return m_MetalnessMap;
}
const ref<Texture>& Material::GetRoughnessMap() const {
    return m_RoughnessMap;
}
const ref<Texture>& Material::GetEmissiveMap() const {
    return m_EmissiveMap;
}

void Material::SetName(const std::string& name) { 
    m_Name = name; 
}
std::string const& Material::GetName() const { 
    return m_Name; 
}

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

const v4& Material::GetAlbedo() const {
    return m_Albedo;
}
f32 Material::GetRoughness() const {
    return m_Roughness;
}
f32 Material::GetMetallic() const {
    return m_Metallic;
}
const v4& Material::GetEmissive() const {
    return m_Emissive;
}

}
