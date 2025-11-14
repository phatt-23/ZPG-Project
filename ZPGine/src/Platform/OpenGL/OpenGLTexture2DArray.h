//
// Created by phatt on 11/14/25.
//

#ifndef WORKSPACE_OPENGLTEXTURE2DARRAY_H
#define WORKSPACE_OPENGLTEXTURE2DARRAY_H
#include "Texture/Texture2DArray.h"

namespace ZPG
{

    class OpenGLTexture2DArray : public Texture2DArray
    {
    public:
        OpenGLTexture2DArray(const std::string& name, int width, int height, int arraySize, TextureDataFormat dataFormat);
        ~OpenGLTexture2DArray() override;

        void BindToSlot(u32 slotIndex) override;
        void Resize(int width, int height, int slices) override;
        void Invalidate() override;

        void AttachToFrameBuffer(u32 frameBufferRendererID, const FrameBufferAttachment& frameBufferAttachment) override;

        u32 GetRendererID() const override;
        u32 GetWidth() const override;
        u32 GetHeight() const override;
        u32 GetArraySize() const override;
        TextureDataFormat GetDataFormat() const override;
        const std::string& GetName() const override { return m_Name; }

    private:
        std::string m_Name;
        u32 m_RendererID = 0;
        u32 m_Width;
        u32 m_Height;
        u32 m_ArraySize;
        TextureDataFormat m_DataFormat;
    };

} // ZPG

#endif //WORKSPACE_OPENGLTEXTURE2DARRAY_H