//
// Created by phatt on 10/23/25.
//

#include "ShaderStorageBuffer.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLCore.h"
#include "Profiling/Instrumentor.h"

namespace ZPG 
{

    ShaderStorageBuffer::ShaderStorageBuffer(u32 bindingPoint, u32 size)
        : m_BindingPoint(bindingPoint)
        , m_Size(size)
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_OPENGL_CALL(glCreateBuffers(1, &m_RendererID));
        ZPG_OPENGL_CALL(glNamedBufferStorage(m_RendererID, 
                                             m_Size, 
                                             nullptr, 
                                             GL_DYNAMIC_STORAGE_BIT));
        ZPG_OPENGL_CALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
        ZPG_OPENGL_CALL(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindingPoint, m_RendererID));
        ZPG_OPENGL_CALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
        // ZPG_OPENGL_CALL(glBindBufferRange(
        //     GL_SHADER_STORAGE_BUFFER,
        //     m_BindingPoint,
        //     m_RendererID,
        //     0,
        //     m_Size
        // ));
    }

    ShaderStorageBuffer::~ShaderStorageBuffer()
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_OPENGL_CALL(glDeleteBuffers(1, &m_RendererID));
    }

    void ShaderStorageBuffer::Bind() const
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_OPENGL_CALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID));
    }

    void ShaderStorageBuffer::Unbind() const
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_OPENGL_CALL(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
    }

#if 1
    void* ShaderStorageBuffer::Map(u32 offset, u32 length) const 
    {
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

    void ShaderStorageBuffer::Unmap() const 
    {
        ZPG_OPENGL_CALL(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
    }
#endif

    void ShaderStorageBuffer::SetData(void* data, u32 size, u32 offset) const
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_CORE_ASSERT(offset + size <= m_Size,
            "Cannot write {} bytes at an offset of {} into a UBO with size of {} bytes",
            size, offset, m_Size);

        ZPG_OPENGL_CALL(glNamedBufferSubData(m_RendererID, offset, size, data));
    }

} // ZPG
