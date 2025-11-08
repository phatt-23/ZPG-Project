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
        };

        MaterialShaderStorageBuffer(u32 bindingPoint);

        void SetMaterial(const Material& material);
        void SetAlbedo(const v4& albedo);
        void SetEmissive(const v4& emissive);
        void SetRoughness(f32 roughness);
        void SetMetallic(f32 metallic);
    };
}   
