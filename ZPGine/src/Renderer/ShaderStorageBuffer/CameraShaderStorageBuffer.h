#pragma once

#include "Buffer/ShaderStorageBuffer.h"

namespace ZPG
{
    class Camera;

    class CameraShaderStorageBuffer : public ShaderStorageBuffer
    {
    public:
        struct InternalLayout
        {
            m4 View;
            m4 Proj;
            m4 ViewProj;
            v3 CameraPosition;
            float FarPlane;
        };
        
        CameraShaderStorageBuffer(u32 bindingPoint);

        void SetCamera(const Camera& camera);
    };
 }
