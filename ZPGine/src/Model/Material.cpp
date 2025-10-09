#include "Material.h"
#include "Core/ResourceManager.h"

namespace ZPG {
Material::Material() {
    ResourceManager& res = ResourceManager::GetGlobal();

    m_ShaderProgram = res.GetShaderProgram("DefaultLit");  
    m_AlbedoMap = res.GetTexture("AlbedoMap:Null");
    m_NormalMap = res.GetTexture("NormalMap:Null");
    m_AlbedoColor = glm::vec3(1.0, 0.0, 0.0);  // default color (red)
    m_Roughness = 0.0;
    m_Metallic = 0.0;
}
Material::~Material() {

}
void Material::Bind() {
    m_ShaderProgram->Bind();

    m_AlbedoMap->BindToSlot(0);
    m_ShaderProgram->SetInt("u_AlbedoMap", 0);

    m_NormalMap->BindToSlot(1);
    m_ShaderProgram->SetInt("u_NormalMap", 1);

    m_ShaderProgram->SetFloat("u_Roughness", m_Roughness);
    m_ShaderProgram->SetFloat("u_Metallic", m_Metallic);
    m_ShaderProgram->SetFloat3("u_AlbedoColor", m_AlbedoColor);
}
void Material::Unbind() {
    m_ShaderProgram->Unbind();
}

void Material::SetShaderProgram(const Ref<ShaderProgram>& shaderProgram) {
    m_ShaderProgram = shaderProgram;
}
void Material::SetAlbedoMap(const Ref<Texture>& albedoMap) {
    m_AlbedoMap = albedoMap;
}
void Material::SetNormalMap(const Ref<Texture>& normalMap) {
    m_NormalMap = normalMap;
}
void Material::SetAlbedoColor(const glm::vec3& albedoColor) {
    m_AlbedoColor = albedoColor;
}
void Material::SetRoughness(f32 roughness) {
    m_Roughness = roughness;
}
void Material::SetMetallic(f32 metallic) {
    m_Metallic = metallic;
}

}
