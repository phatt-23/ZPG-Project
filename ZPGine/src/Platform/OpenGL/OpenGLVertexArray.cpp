#include "OpenGLVertexArray.h"
#include "OpenGLCore.h"
#include <glad/glad.h>

#include "Buffer/BufferLayout.h"

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

void OpenGLVertexArray::AddVertexBuffer(const ref<VertexBuffer>& vertexBuffer) {

    const BufferLayout& layout = vertexBuffer->GetLayout();

    ZPG_CORE_ASSERT( layout.GetElements().size() != 0, 
        "Layout mustn't be empty. Please provide the layout to the VBO before adding the VBO." );

    if (m_VertexCount == -1) {
       m_VertexCount = vertexBuffer->GetSize() / layout.GetStride();
    } else {
        u32 vertexCount = vertexBuffer->GetSize() / vertexBuffer->GetLayout().GetStride();

        ZPG_CORE_ASSERT(m_VertexCount == vertexCount,
            "The new VBO added to a VAO, must have the same vertex count ({}) as previously added VBO.", m_VertexCount);
    }

    m_VertexBuffers.push_back(vertexBuffer);

    this->Bind();
    vertexBuffer->Bind();

    for (auto const& el : layout) {
        glEnableVertexAttribArray(m_LayoutLocationIndex);
        // GLuint index, GLint size, GLenum type, 
        // GLboolean normalized, GLsizei stride, const void *pointer
        glVertexAttribPointer(m_LayoutLocationIndex,
            el.GetElementCount(), 
            ShaderDataType::ToOpenGLType(el.Type), 
            el.Normalized,
            layout.GetStride(), 
            reinterpret_cast<void*>(el.Offset));
        m_LayoutLocationIndex++;
    }

    this->Unbind();
    vertexBuffer->Unbind();
}

void OpenGLVertexArray::SetIndexBuffer(const ref<IndexBuffer>& indexBuffer) {
    m_IndexBuffer = indexBuffer;

    Bind();
    m_IndexBuffer->Bind();
    this->Unbind();
    m_IndexBuffer->Unbind();
}

bool OpenGLVertexArray::HasIndexBuffer() const {
    return m_IndexBuffer != nullptr;
}

u32 OpenGLVertexArray::GetVertexCount() const {
    return m_VertexCount;
}

const std::vector<ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const {
    return m_VertexBuffers;
}

const ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const {
    return m_IndexBuffer;
}

}
