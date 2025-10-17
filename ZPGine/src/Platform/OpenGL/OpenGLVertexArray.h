#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

namespace ZPG {

class OpenGLVertexArray : public VertexArray {
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const ref<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(const ref<IndexBuffer>& indexBuffer) override;
    virtual bool HasIndexBuffer() const override;

    virtual const std::vector<ref<VertexBuffer>>& GetVertexBuffers() const override;
    virtual const ref<IndexBuffer>& GetIndexBuffer() const override;

private:
    u32 m_RendererID;
    std::vector<ref<VertexBuffer>> m_VertexBuffers;
    ref<IndexBuffer> m_IndexBuffer = nullptr;
};

}
