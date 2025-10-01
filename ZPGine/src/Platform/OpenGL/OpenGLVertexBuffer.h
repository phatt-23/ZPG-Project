#include "Renderer/VertexBuffer.h"
#include "Renderer/BufferLayout.h"

namespace ZPG {

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(u32 size);
    OpenGLVertexBuffer(const f32* vertices, u32 count);
    ~OpenGLVertexBuffer() override;

    virtual void SetData(const void* data, const u32 size) override;
    virtual u32 GetSize() const override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual const BufferLayout& GetLayout() const override;
    virtual void SetLayout(BufferLayout&& layout) override;
private:
    u32 m_RendererID;
    BufferLayout m_Layout;
    u32 m_Size;
};

}
