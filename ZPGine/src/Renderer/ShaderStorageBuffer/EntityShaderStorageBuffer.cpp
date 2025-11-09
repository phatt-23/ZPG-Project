#include "EntityShaderStorageBuffer.h"

#include "Profiling/Instrumentor.h"

namespace ZPG
{
    EntityShaderStorageBuffer::EntityShaderStorageBuffer(u32 bindingPoint, u32 capacity)
        : ShaderStorageBuffer(bindingPoint, 
                              sizeof(InternalLayout::Count) + 
                              sizeof(InternalLayout::_pad) + 
                              sizeof(*InternalLayout::EntityIDs) * capacity)
        , m_Capacity(capacity)
    {
        
    }

    void EntityShaderStorageBuffer::SetCount(u32 count) 
    {
        ZPG_PROFILE_FUNCTION();
        Bind();
        count = std::max(count, m_Capacity);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&count, InternalLayout, Count);
        Unbind();
    }
    
    void EntityShaderStorageBuffer::SetEntityIDs(const std::vector<i32>& entityIDs) 
    {
        ZPG_PROFILE_FUNCTION();
        Bind();
        u32 count = entityIDs.size();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&count, InternalLayout, Count);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA_ARRAY((void*)entityIDs.data(), count, InternalLayout, EntityIDs);
        Unbind();
    }

    void EntityShaderStorageBuffer::SetEntityIDs(const i32* entityIDsData, u32 count) 
    {
        ZPG_PROFILE_FUNCTION();
        Bind();
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&count, InternalLayout, Count);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA_ARRAY((void*)entityIDsData, count, InternalLayout, EntityIDs);
        Unbind();
    }
}
