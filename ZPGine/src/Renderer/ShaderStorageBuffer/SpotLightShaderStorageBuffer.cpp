#include "SpotLightShaderStorageBuffer.h"

#include "Profiling/Instrumentor.h"

namespace ZPG
{
    SpotLightShaderStorageBuffer::SpotLightShaderStorageBuffer(u32 bindingPoint, u32 capacity)
        : ShaderStorageBuffer(bindingPoint, 
            sizeof(InternalLayout::Count) + 
            sizeof(InternalLayout::_pad) + 
            sizeof(*InternalLayout::LightArray) * capacity) 
        , m_Capacity(capacity)
    {

    }

    void SpotLightShaderStorageBuffer::SetCount(i32 count)
    {
        ZPG_PROFILE_FUNCTION();
        Bind();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&count, InternalLayout, Count);
        Unbind();
    }

    void SpotLightShaderStorageBuffer::SetLightArray(const std::vector<SpotLightStruct>& lights)
    {
        ZPG_PROFILE_FUNCTION();
        Bind();
        u32 count = lights.size();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&count, InternalLayout, Count);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA_ARRAY((void*)lights.data(), count, InternalLayout, LightArray);
        Unbind();
    }

    void SpotLightShaderStorageBuffer::SetShadowLayer(int i) {
        Bind();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&i, InternalLayout, ShadowLayer);
        Unbind();
    }
}
