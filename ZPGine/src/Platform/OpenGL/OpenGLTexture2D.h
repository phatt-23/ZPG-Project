#pragma once

#include "Texture/Texture2D.h"

namespace ZPG {

    class FrameBufferAttachment;

    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const std::string& filepath, TextureDataFormat dataFormat);
        OpenGLTexture2D(const std::string& name, const std::string& filepath, TextureDataFormat dataFormat);
        OpenGLTexture2D(const std::string& name, u32 width, u32 height, TextureDataFormat dataFormat);
        ~OpenGLTexture2D() override;

        void BindToSlot(u32 slotIndex) override;
        void SetData(const void *data, u32 size) override;

        void AttachToFrameBuffer(u32 frameBufferRendererID, const FrameBufferAttachment& frameBufferAttachment) override;

        const std::string& GetName() const override;
        u32 GetWidth() const override;
        u32 GetHeight() const override;

        void Resize(u32 width, u32 height) override;

        u32 GetRendererID() const override;

        void Invalidate() override;
        TextureDataFormat GetDataFormat() const override { return m_DataFormat; }

    private:
        void LoadTexture(const std::string& path);

    private:

        u32 m_RendererID = 0;
        std::string m_Name;
        u32 m_Width;
        u32 m_Height;
        u32 m_SampleSize; // number of bytes per pixel in the texture
        TextureDataFormat m_DataFormat;
    };

}
