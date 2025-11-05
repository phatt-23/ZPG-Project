//
// Created by phatt on 11/3/25.
//

#ifndef WORKSPACE_OPENGL_CUBEMAP_TEXTURE_H
#define WORKSPACE_OPENGL_CUBEMAP_TEXTURE_H
#include "Texture/CubemapTexture.h"

namespace ZPG
{

    class OpenGLCubemapTexture : public CubemapTexture
    {
    public:
        OpenGLCubemapTexture(
            const std::string& name,
            const std::array<std::string, 6>& faceFilepaths,
            TextureDataFormat dataFormat = TextureDataFormat::RGBA8);

        OpenGLCubemapTexture(
            const std::string& name,
            u32 width,
            u32 height,
            TextureDataFormat dataFormat = TextureDataFormat::RGBA8);

        ~OpenGLCubemapTexture() override;

        virtual const std::string& GetName() const override;
        virtual void BindToSlot(u32 slotIndex) override;
        virtual void Bind() override;
        virtual void Unbind() override;

        virtual u32 GetWidth() const override;
        virtual u32 GetHeight() const override;

        virtual void SetData(const void *data, u32 size) override; /* Unused for cubemaps */

        virtual void AttachToFrameBuffer(u32 frameBufferID, FrameBufferAttachment frameBufferAttachment) override;

        virtual void Resize(u32 width, u32 height) override;

        virtual u32 GetRendererID() const override;

        virtual void GenerateMipmaps() override;

        virtual void SetFaceData(CubemapFace face, const void* data, u32 size) override;

        virtual void Invalidate() override;

    private:
        std::string m_Name;
        u32 m_RendererID = 0;
        /**
         * These don't make sense much when the cubemap is loaded by specifying 6 filepaths.
         * Because each texture can be of different size.
         * For now, they are the maximal values of from textures.
         */
        u32 m_Width = 0;
        u32 m_Height = 0;
        u32 m_SampleSize = 0;
        TextureDataFormat m_DataFormat = TextureDataFormat::None;
    };

}

#endif