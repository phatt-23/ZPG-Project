//
// Created by phatt on 10/23/25.
//

#include "ShaderStorageBuffer.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLCore.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

ShaderStorageBuffer::ShaderStorageBuffer(u32 bindingPoint, u32 size)
    : m_BindingPoint(bindingPoint)
    , m_Size(size)
{
    ZPG_PROFILE_FUNCTION();

    ZPG_OPENGL_CALL(glGenBuffers(1, &m_RendererId));
    Bind();
    {
        ZPG_OPENGL_CALL(glBufferData(
            GL_SHADER_STORAGE_BUFFER, 
            m_Size, 
            nullptr, 
            GL_DYNAMIC_DRAW | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT));

        #if 0
        ZPG_OPENGL_CALL(glBufferStorage(
            GL_SHADER_STORAGE_BUFFER,
            m_Size,
            nullptr,
            GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT));
        #endif

        // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindingPoint, m_RendererId);
        ZPG_OPENGL_CALL(glBindBufferRange(GL_SHADER_STORAGE_BUFFER, m_BindingPoint, m_RendererId, 0, m_Size));
    }
    Unbind();
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
    ZPG_PROFILE_FUNCTION();
    Unbind();
    ZPG_OPENGL_CALL(glDeleteBuffers(1, &m_RendererId));
}

void ShaderStorageBuffer::Bind() {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererId));
}

void ShaderStorageBuffer::Unbind() {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

#if 0
void* ShaderStorageBuffer::Map(u32 offset, u32 length) {
    Bind();
    u32 len = length ? length : m_Size - offset;
    
                                    // GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access
    void* mapped;
    ZPG_OPENGL_CALL(mapped = glMapBufferRange(
        GL_SHADER_STORAGE_BUFFER, 
        0, 
        m_Size, 
        GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT));
    return mapped;
}
void ShaderStorageBuffer::Unmap() {
    ZPG_OPENGL_CALL(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
}
#endif

void ShaderStorageBuffer::SetData(void* data, u32 size, u32 offset) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(offset + size <= m_Size,
        "Cannot write {} bytes at an offset of {} into a UBO with size of {} bytes",
        size, offset, m_Size);

    Bind();
    ZPG_OPENGL_CALL(glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data));

    #if 0
    void* handle = Map();
    handle = ((char*)handle + offset);
    memcpy(handle, data, size);
    Unmap();
    #endif
}

} // ZPG
