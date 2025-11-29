#include "PointLightShaderStorageBuffer.h"

#include "Profiling/Instrumentor.h"

namespace ZPG
{
    PointLightShaderStorageBuffer::PointLightShaderStorageBuffer(u32 bindingPoint, 
                                                                 u32 capacity)
        : ShaderStorageBuffer(bindingPoint, 
            sizeof(InternalLayout::Count) + 
            sizeof(InternalLayout::_pad) + 
            sizeof(*InternalLayout::LightArray) * capacity) 
        , m_Capacity(capacity)
    {

    }

    void PointLightShaderStorageBuffer::SetCount(i32 count)
    {
        ZPG_PROFILE_FUNCTION();
        Bind();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&count, InternalLayout, Count);
        Unbind();
    }

    void PointLightShaderStorageBuffer::SetLightArray(const std::vector<PointLightStruct>& lights)
    {
        ZPG_PROFILE_FUNCTION();
        Bind();
        u32 count = lights.size();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&count, InternalLayout, Count);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA_ARRAY((void*)lights.data(), count, InternalLayout, LightArray);
        Unbind();
    }

    void PointLightShaderStorageBuffer::Null() 
    {
        byte data[sizeof(InternalLayout)] = {0};
        SetData(data, sizeof(InternalLayout));
    }
} // namespace ZPG
