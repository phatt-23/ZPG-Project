#include "OpenGLVertexBuffer.h"

namespace ZPG {

OpenGLVertexBuffer::OpenGLVertexBuffer(u32 size) : m_Layout(), m_Size(size) {
    glCreateBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    Unbind();
}
OpenGLVertexBuffer::OpenGLVertexBuffer(const f32* vertices, u32 count) : m_Layout(), m_Size(count * sizeof(f32)) {
    glCreateBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, m_Size, vertices, GL_STATIC_DRAW);
    Unbind();
}
OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}
void OpenGLVertexBuffer::SetData(const void* data, const u32 size) {
    ZPG_CORE_ASSERT(size < m_Size, "Cannot assign size {} to VBO of size {}", size, m_Size);
    Bind();
    // GLenum target, GLintptr offset, GLsizeiptr size, const void *data
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    Unbind();
}
void OpenGLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}
void OpenGLVertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
const BufferLayout& OpenGLVertexBuffer::GetLayout() const {
    return m_Layout;
}
void OpenGLVertexBuffer::SetLayout(BufferLayout&& layout) {
    m_Layout = layout;
}

}
