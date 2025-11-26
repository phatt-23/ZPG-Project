#pragma once

#include "Buffer/ShaderStorageBuffer.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Renderer/LightStruct/LightStructs.h"

namespace ZPG
{

    class EnvironmentLightShaderStorageBuffer : public ShaderStorageBuffer
    {
    public:
        struct InternalLayout 
        {
            v4 AmbientColor;
            DirectionalLightStruct DirectionalLight;
        };

        EnvironmentLightShaderStorageBuffer(u32 bindingPoint);

        void SetAmbient(const AmbientLight &ambientLight);
        void SetDirectionalLight(const DirectionalLight &directionalLight, const Camera& camera);

        void Null() override;
    };
}
