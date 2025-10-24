//
// Created by phatt on 10/23/25.
//

#include "ShaderStorageBuffer.h"

#include "Debug/Asserter.h"

namespace ZPG {

ShaderStorageBuffer::ShaderStorageBuffer(u32 bindingPoint, u32 size)
    : m_BindingPoint(bindingPoint)
    , m_Size(size)
{
    glGenBuffers(1, &m_RendererId);
    Bind();
    {
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_Size, NULL, GL_DYNAMIC_DRAW);

        // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_BindingPoint, m_RendererId);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, m_BindingPoint, m_RendererId, 0, m_Size);
    }
    Unbind();
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
    Unbind();
    glDeleteBuffers(1, &m_RendererId);
}

void ShaderStorageBuffer::Bind() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererId);
}

void ShaderStorageBuffer::Unbind() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::SetData(void* data, u32 size, u32 offset) {
    ZPG_CORE_ASSERT(offset + size <= m_Size,
        "Cannot write {} bytes at an offset of {} into a UBO with size of {} bytes",
        size, offset, m_Size);

    Bind();
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
}

} // ZPG