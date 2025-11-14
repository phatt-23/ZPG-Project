//
// Created by phatt on 11/14/25.
//

#include "OpenGLTexture2DArray.h"

#include "OpenGLCore.h"
#include "OpenGLMapper.h"
#include "glad/glad.h"

namespace ZPG
{
    OpenGLTexture2DArray::OpenGLTexture2DArray(const std::string& name, int width, int height, int arraySize, TextureDataFormat dataFormat)
        : m_Name(name)
        , m_Width(width)
        , m_Height(height)
        , m_ArraySize(arraySize)
        , m_DataFormat(dataFormat)
    {
        Invalidate();
    }

    OpenGLTexture2DArray::~OpenGLTexture2DArray()
    {
        ZPG_OPENGL_CALL(glDeleteTextures(1, &m_RendererID));
    }

    void OpenGLTexture2DArray::BindToSlot(u32 slotIndex)
    {
        ZPG_OPENGL_CALL(glBindTextureUnit(slotIndex, m_RendererID));
    }

    void OpenGLTexture2DArray::Resize(int width, int height, int slices)
    {
        m_Width = width;
        m_Height = height;
        m_ArraySize = slices;

        Invalidate();
    }

    void OpenGLTexture2DArray::Invalidate()
    {
        if (m_RendererID != 0) {
            ZPG_OPENGL_CALL(glDeleteTextures(1, &m_RendererID));
        }

        auto gl = OpenGLMapper::ToGL(m_DataFormat);

        ZPG_OPENGL_CALL(glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_RendererID));

        ZPG_OPENGL_CALL(glTextureStorage3D(
            m_RendererID,
            1,
            gl.InternalFormat,
            m_Width,
            m_Height,
            m_ArraySize));

        // Configure sampling
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

        float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        ZPG_OPENGL_CALL(glTextureParameterfv(m_RendererID, GL_TEXTURE_BORDER_COLOR, borderColor));
    }

    void OpenGLTexture2DArray::AttachToFrameBuffer(u32 frameBufferRendererID, const FrameBufferAttachment& frameBufferAttachment)
    {
        auto gl = OpenGLMapper::ToGL(frameBufferAttachment.AttachType);
        ZPG_OPENGL_CALL(glNamedFramebufferTexture(frameBufferRendererID, gl.Attachment + frameBufferAttachment.Index, m_RendererID, 0));
    }

    u32 OpenGLTexture2DArray::GetRendererID() const {
        return m_RendererID;
    }

    u32 OpenGLTexture2DArray::GetWidth() const {
        return m_Width;
    }

    u32 OpenGLTexture2DArray::GetHeight() const {
        return m_Height;
    }

    u32 OpenGLTexture2DArray::GetArraySize() const {
        return m_ArraySize;
    }

    TextureDataFormat OpenGLTexture2DArray::GetDataFormat() const {
        return m_DataFormat;
    }
} // ZPG