#include "CameraShaderStorageBuffer.h"
#include "Camera/Camera.h"
#include "Profiling/Instrumentor.h"

namespace ZPG
{
    CameraShaderStorageBuffer::CameraShaderStorageBuffer(u32 bindingPoint) 
        : ShaderStorageBuffer(bindingPoint, sizeof(InternalLayout))
    {

    }

    void CameraShaderStorageBuffer::SetCamera(const Camera& camera)
    {
        ZPG_PROFILE_FUNCTION();

        m4 view = camera.GetViewMatrix();
        m4 proj = camera.GetProjMatrix();
        m4 viewProj = camera.GetViewProjMatrix();
        v3 cameraPosition = camera.GetPosition();
        f32 farPlane = camera.GetZFar();

        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(glm::value_ptr(view), InternalLayout, View);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(glm::value_ptr(proj), InternalLayout, Proj);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(glm::value_ptr(viewProj), InternalLayout, ViewProj);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(glm::value_ptr(cameraPosition), InternalLayout, CameraPosition);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&farPlane, InternalLayout, FarPlane);
    }
}
