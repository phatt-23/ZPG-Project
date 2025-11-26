#pragma once

#include "Buffer/ShaderStorageBuffer.h"

namespace ZPG
{
    struct EntityShaderStorageBuffer : public ShaderStorageBuffer
    {
    public:
        struct InternalLayout
        {
            i32 Count;
            f32 _pad[3];  // 16
            i32* EntityIDs; // packed array of 4 bytes offset
        };

        EntityShaderStorageBuffer(u32 bindingPoint, u32 capacity);

        void SetCount(u32 count);
        void SetEntityIDs(const std::vector<i32>& entityIDs);
        void SetEntityIDs(const i32* entityIDs, u32 count);

        void Null() override;

      private:
        u32 m_Capacity;
    };
}
