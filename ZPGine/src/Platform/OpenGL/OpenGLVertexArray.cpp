#include "OpenGLVertexArray.h"
#include "OpenGLCore.h"
#include <glad/glad.h>

namespace ZPG {

OpenGLVertexArray::OpenGLVertexArray() {
    ZPG_OPENGL_CALL(glGenVertexArrays(1, &m_RendererID));
}
OpenGLVertexArray::~OpenGLVertexArray() {
    ZPG_OPENGL_CALL(glDeleteVertexArrays(1, &m_RendererID));
}
void OpenGLVertexArray::Bind() const {
    ZPG_OPENGL_CALL(glBindVertexArray(m_RendererID));
}
void OpenGLVertexArray::Unbind() const {
    ZPG_OPENGL_CALL(glBindVertexArray(0));
}
void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
    const BufferLayout& layout = vertexBuffer->GetLayout();
    ZPG_CORE_ASSERT( layout.GetElements().size() != 0, 
                    "Layout musn't be empty. Please provide the layout to the VBO before adding the VBO." );

    this->Bind();
    vertexBuffer->Bind();
    m_VertexBuffers.push_back(vertexBuffer);
    
    int index = 0;
    for (auto const& el : layout) {
        glEnableVertexAttribArray(index);
        // GLuint index, GLint size, GLenum type, 
        // GLboolean normalized, GLsizei stride, const void *pointer
        glVertexAttribPointer(index, 
            el.GetElementCount(), 
            ShaderDataType::ToOpenGLType(el.Type), 
            el.Normalized,
            layout.GetStride(), 
            reinterpret_cast<void*>(el.Offset));
        index++;
    }

    this->Unbind();
    vertexBuffer->Unbind();
}
void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
    m_IndexBuffer = indexBuffer;

    Bind();
    m_IndexBuffer->Bind();
    this->Unbind();
    m_IndexBuffer->Unbind();
}
bool OpenGLVertexArray::HasIndexBuffer() const {
    return m_IndexBuffer != nullptr;
}
const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const {
    return m_VertexBuffers;
}
const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const {
    return m_IndexBuffer;
}

}
