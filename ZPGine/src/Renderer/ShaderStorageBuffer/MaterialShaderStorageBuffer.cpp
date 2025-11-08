#include "MaterialShaderStorageBuffer.h"
#include "Material/Material.h"

namespace ZPG
{
    MaterialShaderStorageBuffer::MaterialShaderStorageBuffer(u32 bindingPoint)
        : ShaderStorageBuffer(bindingPoint, sizeof(InternalLayout))
    {
        
    }

    void MaterialShaderStorageBuffer::SetMaterial(const Material& material)
    {
        Bind();

        InternalLayout internalLayout;
        internalLayout.Albedo = material.GetAlbedo();
        internalLayout.Emissive = material.GetEmissive();
        internalLayout.Roughness = material.GetRoughness();
        internalLayout.Metallic = material.GetMetallic();

        SetData(&internalLayout, sizeof(internalLayout));

        Unbind();
    }

    void MaterialShaderStorageBuffer::SetAlbedo(const v4& albedo)
    {
        Bind();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(glm::value_ptr(*(v4*)&albedo), InternalLayout, Albedo);
        Unbind();
    }

    void MaterialShaderStorageBuffer::SetEmissive(const v4& emissive) 
    {
        Bind();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(glm::value_ptr(*(v4*)&emissive), InternalLayout, Emissive);
        Unbind();
    }

    void MaterialShaderStorageBuffer::SetRoughness(f32 roughness) 
    {
        Bind();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&roughness, InternalLayout, Roughness);
        Unbind();
    }

    void MaterialShaderStorageBuffer::SetMetallic(f32 metallic) 
    {
        Bind();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&metallic, InternalLayout, Metallic);
        Unbind();
    }
}
