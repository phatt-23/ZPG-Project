#include "EnvironmentLightShaderStorageBuffer.h"

#include "Camera/Camera.h"
#include "Profiling/Instrumentor.h"

namespace ZPG
{
    EnvironmentLightShaderStorageBuffer::EnvironmentLightShaderStorageBuffer(u32 bindingPoint)
        : ShaderStorageBuffer(bindingPoint, sizeof(InternalLayout)) 
    {
    }

    void EnvironmentLightShaderStorageBuffer::SetAmbient(const AmbientLight &ambientLight) 
    { 
        ZPG_PROFILE_FUNCTION();
        v4 color = ambientLight.Color.Get();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(glm::value_ptr(color), InternalLayout, AmbientColor);
    }

    void EnvironmentLightShaderStorageBuffer::SetDirectionalLight(const DirectionalLight &directionalLight, const Camera& camera)
    {
        ZPG_PROFILE_FUNCTION();
        DirectionalLightStruct directionalLightStruct(directionalLight, camera);

        SetData(glm::value_ptr(directionalLightStruct.Color),
            sizeof(DirectionalLightStruct::Color),
            offsetof(InternalLayout, DirectionalLight) + offsetof(DirectionalLightStruct, Color));

        SetData(glm::value_ptr(directionalLightStruct.Direction),
            sizeof(DirectionalLightStruct::Direction),
            offsetof(InternalLayout, DirectionalLight) + offsetof(DirectionalLightStruct, Direction));

        SetData(&directionalLightStruct.CascadeCount,
            sizeof(DirectionalLightStruct::CascadeCount),
            offsetof(InternalLayout, DirectionalLight) + offsetof(DirectionalLightStruct, CascadeCount));

        SetData(&directionalLightStruct.PlaneDistance,
            sizeof(DirectionalLightStruct::PlaneDistance),
            offsetof(InternalLayout, DirectionalLight) + offsetof(DirectionalLightStruct, PlaneDistance));

        SetData(directionalLightStruct.ViewProj,
            sizeof(DirectionalLightStruct::ViewProj),
            offsetof(InternalLayout, DirectionalLight) + offsetof(DirectionalLightStruct, ViewProj));
    }

    void EnvironmentLightShaderStorageBuffer::Null() 
    {
        byte data[sizeof(InternalLayout)] = {0};
        SetData(data, sizeof(InternalLayout));
    }
} // namespace ZPG
