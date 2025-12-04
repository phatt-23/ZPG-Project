#pragma once

#include "Buffer/ShaderStorageBuffer.h"

namespace ZPG
{
    enum DebugFlag : u32
    {
        ShowDirectionalShadowCascades = ZPG_BIT(0),
        RenderDirectionalCascadeShadowMap = ZPG_BIT(1),
    };

    class DebugShaderStorageBuffer : public ShaderStorageBuffer
    {
    public:
        struct InternalLayout 
        {
            u32 Flags = 0;
            u32 DirectionalCascadeIndex = 0;

        };

        DebugShaderStorageBuffer(int bindingPoint)
            : ShaderStorageBuffer(bindingPoint, sizeof(InternalLayout))
        {
        }

        void SetFlag(DebugFlag flag, bool value)
        {
            if (value) {
                m_Data.Flags |= flag;
            }
            else {
                m_Data.Flags &= ~flag;
            }

            ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&m_Data.Flags, InternalLayout, Flags);
        }

        void SetDirectionalCascadeIndex(u32 index) 
        {
            m_Data.DirectionalCascadeIndex = index;
            
            ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&m_Data.DirectionalCascadeIndex, InternalLayout, DirectionalCascadeIndex);
        }

        const InternalLayout& GetData() const { return m_Data; }

        void Null() override
        {
            m_Data.Flags = 0; 
            ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&m_Data.Flags, InternalLayout, Flags);
        }
    private:
        InternalLayout m_Data;
    };
}
