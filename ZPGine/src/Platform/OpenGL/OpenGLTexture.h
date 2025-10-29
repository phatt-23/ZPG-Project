#pragma once

#include "Texture/Texture.h"

namespace ZPG {

class OpenGLTexture : public Texture {
public:
    OpenGLTexture(const std::string& filepath, BufferDataFormat format);
    OpenGLTexture(const std::string& name, const std::string& filepath, BufferDataFormat format);
    OpenGLTexture(const std::string& name, u32 width, u32 height, BufferDataFormat format);
    ~OpenGLTexture() override;

    void Bind() override;
    void BindToSlot(u32 slotIndex) override;
    void Unbind() override;
    void SetData(const void *data, u32 size) override;

    void AttachToFrameBuffer(u32 frameBufferID, Attachment attachment) override;

    const std::string& GetName() const override;
    u32 GetWidth() const override;
    u32 GetHeight() const override;

    void Resize(u32 width, u32 height) override;

private:
    void LoadTexture(const std::string& path);
    void CreateEmptyTexture();

private:
    u32 m_RendererID; 
    std::string m_Name;
    u32 m_Width;
    u32 m_Height;
    u32 m_SampleSize; // number of bytes per pixel in the texture
    BufferDataFormat m_DataFormat;
};

}
