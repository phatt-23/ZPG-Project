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
            f32 _pad[1];
        };
        
        CameraShaderStorageBuffer(u32 bindingPoint);

        void SetLayout(const Camera& camera);
    };
 }
