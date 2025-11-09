#pragma once

#include "Buffer/ShaderStorageBuffer.h"
#include "Renderer/LightStruct/LightStructs.h"

namespace ZPG
{
    class SpotLightShaderStorageBuffer : public ShaderStorageBuffer
    {
    public:
        struct InternalLayout
        {
            i32 Count;
            i32 ShadowLayer;  // only for debug
            f32 _pad[2];
            SpotLightStruct* LightArray;
        };

        SpotLightShaderStorageBuffer(u32 bindingPoint, u32 capacity);

        void SetCount(i32 count);
        void SetLightArray(const std::vector<SpotLightStruct>& lights);
        void SetShadowLayer(int i);
    private:
        u32 m_Capacity;
    };
}
