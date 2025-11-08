#include "ModelShaderStorageBuffer.h"

namespace ZPG
{
    ModelShaderStorageBuffer::ModelShaderStorageBuffer(u32 bindingPoint, u32 capacity)
        : ShaderStorageBuffer(bindingPoint, 
                              sizeof(InternalLayout::Count) + 
                              sizeof(InternalLayout::_pad) + 
                              sizeof(*InternalLayout::Models) * capacity)
        , m_Capacity(capacity)
    {
        
    }

    void ModelShaderStorageBuffer::SetCount(u32 count) 
    {
        Bind();
        count = std::max(count, m_Capacity);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&count, InternalLayout, Count);
        Unbind();
    }
    
    void ModelShaderStorageBuffer::SetModels(const std::vector<m4>& models) 
    {
        Bind();
        u32 count = models.size();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&count, InternalLayout, Count);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA_ARRAY((void*)models.data(), count, InternalLayout, Models);
        Unbind();
    }

    void ModelShaderStorageBuffer::SetModels(const m4* modelsData, u32 count)
    {
        Bind();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&count, InternalLayout, Count);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA_ARRAY((void*)modelsData, count, InternalLayout, Models);
        Unbind();
    }
}
