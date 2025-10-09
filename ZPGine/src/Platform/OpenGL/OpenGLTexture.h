#pragma once

#include "Texture/Texture.h"

namespace ZPG {

class OpenGLTexture : public Texture {
public:
    OpenGLTexture(const std::string& filepath);
    OpenGLTexture(const std::string& name, const std::string& filepath);
    OpenGLTexture(const std::string& name, u32 width, u32 height);
    ~OpenGLTexture() override;

    virtual void BindToSlot(u32 slotIndex) override;
    virtual void Unbind() override;
    virtual void SetData(const void *data, u32 size) override;

    virtual const std::string& GetName() const override;
    virtual u32 GetWidth() const override;
    virtual u32 GetHeight() const override;
private:
    void LoadTexture(const std::string& path);
    void CreateEmptyTexture();

private:
    u32 m_RendererID; 
    std::string m_Name;
    u32 m_Width, m_Height, m_Channels;
    u32 m_DataFormat, m_InternalDataFormat;
};

}
