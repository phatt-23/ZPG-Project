//
// Created by phatt on 11/14/25.
//

#include "OpenGLTextureCubeMapArray.h"

#include "OpenGLCore.h"
#include "OpenGLMapper.h"

namespace ZPG
{

    OpenGLTextureCubeMapArray::OpenGLTextureCubeMapArray(const std::string& name, int length, int arraySize, TextureDataFormat dataFormat)
        : m_Name(name)
        , m_Length(length)
        , m_ArraySize(arraySize)
        , m_DataFormat(dataFormat)
    {
        Invalidate();
    }

    OpenGLTextureCubeMapArray::~OpenGLTextureCubeMapArray()
    {
        ZPG_OPENGL_CALL(glDeleteTextures(1, &m_RendererID));
    }

    void OpenGLTextureCubeMapArray::BindToSlot(u32 slotIndex)
    {
        ZPG_OPENGL_CALL(glBindTextureUnit(slotIndex, m_RendererID));
    }

    void OpenGLTextureCubeMapArray::AttachToFrameBuffer(u32 frameBufferRendererID, const FrameBufferAttachment& frameBufferAttachment)
    {
        auto gl = OpenGLMapper::ToGL(frameBufferAttachment.AttachType);

        ZPG_OPENGL_CALL(glNamedFramebufferTexture(frameBufferRendererID, gl.Attachment + frameBufferAttachment.Index, m_RendererID, 0));

        int status = glCheckNamedFramebufferStatus(frameBufferRendererID, GL_FRAMEBUFFER);
        ZPG_CORE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete! Status code: {}", status);
    }

    u32 OpenGLTextureCubeMapArray::GetRendererID() const {
        return m_RendererID;
    }

    u32 OpenGLTextureCubeMapArray::GetLength() const {
        return m_Length;
    }

    u32 OpenGLTextureCubeMapArray::GetArraySize() const {
        return m_ArraySize;
    }

    TextureDataFormat OpenGLTextureCubeMapArray::GetDataFormat() const {
        return m_DataFormat;
    }

    void OpenGLTextureCubeMapArray::Resize(int length, int slices)
    {
        m_Length = length;
        m_ArraySize = slices;
        Invalidate();
    }

    void OpenGLTextureCubeMapArray::Invalidate()
    {
        if (m_RendererID != 0) {
            glDeleteTextures(1, &m_RendererID);
        }

        ZPG_OPENGL_CALL(glCreateTextures(GL_TEXTURE_CUBE_MAP_ARRAY, 1, &m_RendererID));

        // GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth
        ZPG_OPENGL_CALL(glTextureStorage3D(
            m_RendererID,
            1,
            GL_DEPTH_COMPONENT32F,
            m_Length,
            m_Length,
            6 * m_ArraySize));

        // Configure sampling
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

        float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        ZPG_OPENGL_CALL(glTextureParameterfv(m_RendererID, GL_TEXTURE_BORDER_COLOR, borderColor));
    }
}
