#include "OpenGLVertexArray.h"
#include "OpenGLCore.h"
#include <glad/glad.h>

#include "Buffer/BufferLayout.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

OpenGLVertexArray::OpenGLVertexArray() {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glGenVertexArrays(1, &m_RendererID));
}
OpenGLVertexArray::~OpenGLVertexArray() {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glDeleteVertexArrays(1, &m_RendererID));
}
void OpenGLVertexArray::Bind() const {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glBindVertexArray(m_RendererID));
}

void OpenGLVertexArray::Unbind() const {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glBindVertexArray(0));
}

void OpenGLVertexArray::AddVertexBuffer(const ref<VertexBuffer>& vertexBuffer) {
    ZPG_PROFILE_FUNCTION();

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

    int idx = 0;

    for (auto const& el : layout) {
        glEnableVertexAttribArray(idx);
        // GLuint index, GLint size, GLenum type, 
        // GLboolean normalized, GLsizei stride, const void *pointer
        glVertexAttribPointer(idx,
            el.GetElementCount(), 
            ShaderDataType::ToOpenGLType(el.Type), 
            el.Normalized,
            layout.GetStride(), 
            reinterpret_cast<void*>(el.Offset));
        idx++;
    }

    this->Unbind();
    vertexBuffer->Unbind();
}

void OpenGLVertexArray::SetIndexBuffer(const ref<IndexBuffer>& indexBuffer) {
    ZPG_PROFILE_FUNCTION();
    m_IndexBuffer = indexBuffer;

    Bind();
    m_IndexBuffer->Bind();
    this->Unbind();
    m_IndexBuffer->Unbind();
}

bool OpenGLVertexArray::HasIndexBuffer() const {
    ZPG_PROFILE_FUNCTION();
    return m_IndexBuffer != nullptr;
}

u32 OpenGLVertexArray::GetVertexCount() const {
    ZPG_PROFILE_FUNCTION();
    return m_VertexCount;
}

const std::vector<ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const {
    ZPG_PROFILE_FUNCTION();
    return m_VertexBuffers;
}

const ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const {
    ZPG_PROFILE_FUNCTION();
    return m_IndexBuffer;
}

}
