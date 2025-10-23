//
// Created by phatt on 10/18/25.
//

#include "UniformBuffer.h"

#include "Debug/Asserter.h"

namespace ZPG {

UniformBuffer::UniformBuffer(u32 size, u32 bindingPoint)
    : m_BindingPoint(bindingPoint)
    , m_Size(size)

{
    glGenBuffers(1, &m_RendererId);
    Bind();
    {
        glBufferData(GL_UNIFORM_BUFFER, m_Size, NULL, GL_DYNAMIC_DRAW);

        glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingPoint, m_RendererId);
        // glBindBufferRange(GL_UNIFORM_BUFFER, m_BindingPoint, m_RendererId, 0, m_Size);
    }
    Unbind();
}

UniformBuffer::~UniformBuffer() {
    Unbind();
    glDeleteBuffers(1, &m_RendererId);
}

void UniformBuffer::Bind() {
    glBindBuffer(GL_UNIFORM_BUFFER, m_RendererId);
}

void UniformBuffer::Unbind() {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::SetData(void* data, u32 size, u32 offset) {
    // void* ptr = glMapBufferRange(GL_UNIFORM_BUFFER, offset, size, GL_MAP_WRITE_BIT);
    // memcpy(ptr, data, size);
    // glUnmapBuffer(GL_UNIFORM_BUFFER);

    ZPG_CORE_ASSERT(offset + size <= m_Size,
        "Cannot write {} bytes at an offset of {} into a UBO with size of {} bytes",
        size, offset, m_Size);

    Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

}
