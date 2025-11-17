#pragma once

namespace ZPG
{

    class ShaderProgram;
    class Texture2D;

    class Material
    {
    public:
        Material(const std::string& name = "");
        ~Material();

        void BindMaps();

        void SetShaderProgram(const ref<ShaderProgram>& shaderProgram);
        ref<ShaderProgram>& GetShaderProgram();

        void SetAlbedo(const v4& albedo);
        void SetRoughness(f32 roughness);
        void SetMetallic(f32 metallic);
        void SetEmissive(const v4& emissive);

        const v4& GetAlbedo() const;
        f32 GetRoughness() const;
        f32 GetMetallic() const;
        const v4& GetEmissive() const;

        void SetAlbedoMap(const ref<Texture2D>& albedoMap);
        void SetNormalMap(const ref<Texture2D>& normalMap);
        void SetMetalnessMap(const ref<Texture2D>& metalnessMap);
        void SetRoughnessMap(const ref<Texture2D>& roughnessMap);
        void SetEmissiveMap(const ref<Texture2D>& emissiveMap);

        const ref<Texture2D>& GetAlbedoMap() const;
        const ref<Texture2D>& GetNormalMap() const;
        const ref<Texture2D>& GetMetalnessMap() const;
        const ref<Texture2D>& GetRoughnessMap() const;
        const ref<Texture2D>& GetEmissiveMap() const;

        void SetName(const std::string& name);
        std::string const& GetName() const;

    public:
        std::string m_Name;
        ref<ShaderProgram> m_ShaderProgram;  // by which this material is drawn

        v4 m_Albedo;
        v4 m_Emissive;
        f32 m_Roughness;
        f32 m_Metallic;
        ref<Texture2D> m_AlbedoMap;
        ref<Texture2D> m_MetalnessMap;
        ref<Texture2D> m_RoughnessMap;
        ref<Texture2D> m_NormalMap;
        ref<Texture2D> m_EmissiveMap;
    };

}
