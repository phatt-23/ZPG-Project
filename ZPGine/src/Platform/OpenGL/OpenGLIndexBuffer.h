#pragma once

#include "Buffer/IndexBuffer.h"

namespace ZPG {

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(const u32* indices, u32 count);
    virtual ~OpenGLIndexBuffer() override;
    virtual void Bind() const override;
    virtual void Unbind() const override; 
    virtual u32 GetCount() const override; 
private:
    u32 m_RendererID;
    u32 m_Count;
};

}
