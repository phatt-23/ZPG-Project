//
// Created by phatt on 11/14/25.
//

#ifndef WORKSPACE_OPENGL_TEXTURE_CUBE_MAP_ARRAY_H
#define WORKSPACE_OPENGL_TEXTURE_CUBE_MAP_ARRAY_H

#include "Texture/TextureCubeMapArray.h"

namespace ZPG
{
    class OpenGLTextureCubeMapArray : public TextureCubeMapArray
    {
    public:
        OpenGLTextureCubeMapArray(const std::string& name, int length, int arraySize, TextureDataFormat dataFormat);
        ~OpenGLTextureCubeMapArray() override;

        void BindToSlot(u32 slotIndex) override;
        void AttachToFrameBuffer(u32 frameBufferRendererID, const FrameBufferAttachment& frameBufferAttachment) override;

        u32 GetRendererID() const override;
        u32 GetLength() const override;
        u32 GetArraySize() const override;
        TextureDataFormat GetDataFormat() const override;

        u32 GetWidth() const override { return m_Length; }
        u32 GetHeight() const override { return m_Length; }
        const std::string& GetName() const override { return m_Name; }

        void Resize(int length, int slices) override;
        void Invalidate() override;

    private:
        std::string m_Name;
        u32 m_RendererID = 0;
        u32 m_Length;
        u32 m_ArraySize;
        TextureDataFormat m_DataFormat;
    };
}


#endif