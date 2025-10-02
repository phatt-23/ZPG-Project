#pragma once

#include "Renderer/Texture.h"

namespace ZPG {

class OpenGLTexture : public Texture {
public:
    OpenGLTexture(const std::string& filepath);
    OpenGLTexture(const std::string& name, const std::string& filepath);
    ~OpenGLTexture() override;

    virtual const std::string& GetName() const override;
    virtual void Bind() override;
    virtual void Unbind() override;

    virtual u32 GetWidth() const override;
    virtual u32 GetHeight() const override;

    virtual void SetData(const void *data, u32 size) override;
private:
    u32 m_RendererID; 
};

}
