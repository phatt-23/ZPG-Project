#include "EnvironmentLightShaderStorageBuffer.h"

namespace ZPG
{
    EnvironmentLightShaderStorageBuffer::EnvironmentLightShaderStorageBuffer(u32 bindingPoint)
        : ShaderStorageBuffer(bindingPoint, sizeof(InternalLayout)) 
    {
    }

    void EnvironmentLightShaderStorageBuffer::SetAmbient(const AmbientLight &ambientLight) 
    { 
        v4 color = ambientLight.Color.Get();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(glm::value_ptr(color), InternalLayout, AmbientColor);
    }

    void EnvironmentLightShaderStorageBuffer::SetDirectionalLight(const DirectionalLight &directionalLight, v3 cameraPosition, v3 cameraFront)
    {
        DirectionalLightStruct directionalLightStruct(directionalLight, cameraPosition, cameraFront);

        SetData(glm::value_ptr(directionalLightStruct.Color),
            sizeof(DirectionalLightStruct::Color),
            offsetof(InternalLayout, DirectionalLight) + offsetof(DirectionalLightStruct, Color));

        SetData(glm::value_ptr(directionalLightStruct.Direction),
            sizeof(DirectionalLightStruct::Direction),
            offsetof(InternalLayout, DirectionalLight) + offsetof(DirectionalLightStruct, Direction));

        SetData(glm::value_ptr(directionalLightStruct.ViewProj),
            sizeof(DirectionalLightStruct::ViewProj),
            offsetof(InternalLayout, DirectionalLight) + offsetof(DirectionalLightStruct, ViewProj));
    }

} // namespace ZPG
