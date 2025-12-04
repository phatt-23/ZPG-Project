#include "Buffer/ShaderStorageBuffer.h"



namespace ZPG
{
    class Material;

    class MaterialShaderStorageBuffer : public ShaderStorageBuffer 
    {
    public:
        struct InternalLayout 
        {
            v4 Albedo;
            v4 Emissive;
            f32 Roughness;
            f32 Metallic;
            f32 _pad[2];
            v2 TilingFactor = v2(1.0f, 1.0f);
        };

        MaterialShaderStorageBuffer(u32 bindingPoint);

        void SetMaterial(const Material& material);
        void SetAlbedo(const v4& albedo);
        void SetEmissive(const v4& emissive);
        void SetRoughness(f32 roughness);
        void SetMetallic(f32 metallic);
        void SetTilingFactor(const v2& tilingFactor);

        void Null() override;
    };
}   
