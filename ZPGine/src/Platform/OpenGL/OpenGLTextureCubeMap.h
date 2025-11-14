//
// Created by phatt on 11/3/25.
//

#ifndef WORKSPACE_OPENGL_CUBEMAP_TEXTURE_H
#define WORKSPACE_OPENGL_CUBEMAP_TEXTURE_H
#include "Texture/TextureCubeMap.h"

namespace ZPG
{
    struct FrameBufferAttachment;

    class OpenGLTextureCubeMap : public TextureCubeMap {
    public:
        OpenGLTextureCubeMap(
            const std::string& name,
            const std::array<std::string, 6>& faceFilepaths,
            TextureDataFormat dataFormat = TextureDataFormat::RGBA8);

        OpenGLTextureCubeMap(
            const std::string& name,
            u32 length,
            TextureDataFormat dataFormat = TextureDataFormat::RGBA8);

        ~OpenGLTextureCubeMap() override;

        void BindToSlot(u32 slotIndex) override;
        void AttachToFrameBuffer(u32 frameBufferRendererID, const FrameBufferAttachment& frameBufferAttachment) override;
        void Resize(u32 length) override;
        void Invalidate() override;

        void SetFaceData(CubemapFace face, const void* data, u32 size) override;

        const std::string& GetName() const override;
        u32 GetLength() const override;
        u32 GetRendererID() const override;

        u32 GetWidth() const override { return m_Length; }
        u32 GetHeight() const override { return m_Length; }
        TextureDataFormat GetDataFormat() const override { return m_DataFormat; }

    private:
        std::string m_Name;
        u32 m_RendererID = 0;
        u32 m_Length = 0;
        u32 m_SampleSize = 0;
        TextureDataFormat m_DataFormat = TextureDataFormat::None;
    };

}

#endif