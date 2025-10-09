#pragma once

#include "Shader/ShaderProgram.h"
#include "Texture/Texture.h"
#include "Shader/ShaderDataType.h"
#include <variant>

namespace ZPG {

// This will be a standard material that works with only 
// some things for now, not everything that aiMaterial provides.
class Material {
public:
    using UniformName = std::string;
    template<typename T, typename S> using Map = std::unordered_map<T, S>;

    Material();
    ~Material();

    // bind all its uniforms (populates vecs and mats and simple data types and binds textures to slots)
    void Bind();
    void Unbind();

    void SetShaderProgram(const Ref<ShaderProgram>& shaderProgram);
    Ref<ShaderProgram>& GetShaderProgram() { return m_ShaderProgram; }

    void SetAlbedoMap(const Ref<Texture>& albedoMap);
    void SetNormalMap(const Ref<Texture>& normalMap);
    void SetAlbedoColor(const glm::vec3& albedoColor);
    void SetRoughness(f32 roughness);
    void SetMetallic(f32 metallic);
private:
    Ref<ShaderProgram> m_ShaderProgram;  // by which this material is drawn

    Ref<Texture> m_AlbedoMap;
    Ref<Texture> m_NormalMap;
    glm::vec3 m_AlbedoColor;
    f32 m_Roughness;
    f32 m_Metallic;
};

}
