#include "OpenGLVertexBuffer.h"
#include "OpenGLCore.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

OpenGLVertexBuffer::OpenGLVertexBuffer(u32 size) : m_Layout(), m_Size(size) {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glCreateBuffers(1, &m_RendererID));
    Bind();
    ZPG_OPENGL_CALL(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW));
    Unbind();
}
OpenGLVertexBuffer::OpenGLVertexBuffer(const void* vertices, u32 size) : m_Layout(), m_Size(size) {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glCreateBuffers(1, &m_RendererID));
    Bind();
    ZPG_OPENGL_CALL(glBufferData(GL_ARRAY_BUFFER, m_Size, vertices, GL_STATIC_DRAW));
    Unbind();
}
OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glDeleteBuffers(1, &m_RendererID));
}
void OpenGLVertexBuffer::SetData(const void* data, u32 size) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(size < m_Size, "Cannot assign size {} to VBO of size {}", size, m_Size);
    Bind();
    // GLenum target, GLintptr offset, GLsizeiptr size, const void *data
    ZPG_OPENGL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
    Unbind();
}
u32 OpenGLVertexBuffer::GetSize() const {
    ZPG_PROFILE_FUNCTION();
    return m_Size;
}
void OpenGLVertexBuffer::Bind() const {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
void OpenGLVertexBuffer::Unbind() const {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
const BufferLayout& OpenGLVertexBuffer::GetLayout() const {
    ZPG_PROFILE_FUNCTION();
    return m_Layout;
}
void OpenGLVertexBuffer::SetLayout(const BufferLayout& layout) {
    ZPG_PROFILE_FUNCTION();
    m_Layout = layout;
}

}
