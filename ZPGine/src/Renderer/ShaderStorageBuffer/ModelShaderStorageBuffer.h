#pragma once

#include "Buffer/ShaderStorageBuffer.h"

namespace ZPG
{
    struct ModelShaderStorageBuffer : public ShaderStorageBuffer
    {
    public:
        struct InternalLayout
        {
            i32 Count;
            f32 _pad[3];  // 16
            m4* Models; // packed array of 16 bytes offset
        };

        ModelShaderStorageBuffer(u32 bindingPoint, u32 capacity);

        void SetCount(u32 count);
        void SetModels(const std::vector<m4>& models);
        void SetModels(const m4* modelsData, u32 count);

    private:
        u32 m_Capacity;
    };
}
