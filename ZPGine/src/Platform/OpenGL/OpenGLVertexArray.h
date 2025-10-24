#pragma once

#include "Buffer/VertexArray.h"

namespace ZPG {

class VertexBuffer;
class IndexBuffer;

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void Bind() const override;
    void Unbind() const override;

    void AddVertexBuffer(const ref<VertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const ref<IndexBuffer>& indexBuffer) override;
    bool HasIndexBuffer() const override;

    u32 GetVertexCount() const override;

    const std::vector<ref<VertexBuffer>>& GetVertexBuffers() const override;
    const ref<IndexBuffer>& GetIndexBuffer() const override;

private:
    u32 m_RendererID;
    std::vector<ref<VertexBuffer>> m_VertexBuffers;
    ref<IndexBuffer> m_IndexBuffer = nullptr;

    u32 m_LayoutLocationIndex = 0;
    i32 m_VertexCount = -1;
};

}
