//
// Created by phatt on 11/3/25.
//

#include "OpenGLCubemapTexture.h"

#include "OpenGLCore.h"
#include "OpenGLMapper.h"
#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"
#include "stb_image/stb_image.h"

namespace ZPG {

    namespace Utils
    {
    }

    OpenGLCubemapTexture::OpenGLCubemapTexture(
        const std::string& name,
        const std::array<std::string, 6>& faceFilepaths,
        TextureDataFormat dataFormat
    )
        : m_Name(name)
        , m_DataFormat(dataFormat)
    {
        ZPG_PROFILE_FUNCTION();

        auto gl = OpenGLMapper::ToGL(m_DataFormat);

        // Cube Map texture.
        ZPG_OPENGL_CALL(glGenTextures(1, &m_RendererID));
        ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

        stbi_set_flip_vertically_on_load(false);

        for (u32 i = 0; i < faceFilepaths.size(); i++) {
            ZPG_CORE_INFO("Loading cube-map textures - file: {}", faceFilepaths[i].c_str());

            int width, height, nrChannels;
            unsigned char* data = stbi_load(faceFilepaths[i].c_str(), &width, &height, &nrChannels, 0);
            ZPG_CORE_ASSERT(data, "Cube map texture failed to load: {}", faceFilepaths[i].c_str());

            m_Width = std::max(m_Width, (u32)width);
            m_Height = std::max(m_Height, (u32)height);

            ZPG_CORE_ASSERT(m_SampleSize == 0 || m_SampleSize == nrChannels, "All cube map textures must have the same number of chanells!");
            m_SampleSize = std::max(m_SampleSize, (u32)nrChannels);

            ZPG_OPENGL_CALL(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
            ZPG_OPENGL_CALL(glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, gl.InternalFormat, width, height,
                0, gl.Format, gl.SampleType, data));

            stbi_image_free(data);
        }

        ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    }

    OpenGLCubemapTexture::OpenGLCubemapTexture(const std::string& name, u32 width, u32 height, TextureDataFormat dataFormat)
        : m_Name(name)
        , m_Width(width)
        , m_Height(height)
        , m_DataFormat(dataFormat)
    {
        ZPG_PROFILE_FUNCTION();

        Invalidate();
    }

    void OpenGLCubemapTexture::Invalidate()
    {
        ZPG_PROFILE_FUNCTION();

        if (m_RendererID == 0)
        {
            glDeleteTextures(1, &m_RendererID);
        }

        auto gl = OpenGLMapper::ToGL(m_DataFormat);

        ZPG_OPENGL_CALL(glGenTextures(1, &m_RendererID));
        ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

        for (int i = 0; i < 6; ++i) {
            // GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels
            ZPG_OPENGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, gl.InternalFormat, m_Width, m_Height, 0, gl.Format, gl.SampleType, nullptr));
        }

        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    }

    OpenGLCubemapTexture::~OpenGLCubemapTexture()
    {
        ZPG_PROFILE_FUNCTION();

        glDeleteTextures(1, &m_RendererID);
    }

    const std::string& OpenGLCubemapTexture::GetName() const
    {
        return m_Name;
    }

    void OpenGLCubemapTexture::BindToSlot(u32 slotIndex)
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_OPENGL_CALL(glActiveTexture(GL_TEXTURE0 + slotIndex));
        ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
        ZPG_OPENGL_CALL(glActiveTexture(GL_TEXTURE0));
    }

    void OpenGLCubemapTexture::Bind()
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
    }

    void OpenGLCubemapTexture::Unbind()
    {
        ZPG_PROFILE_FUNCTION();

        ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }

    u32 OpenGLCubemapTexture::GetWidth() const
    {
        ZPG_PROFILE_FUNCTION();

        return m_Width;
    }

    u32 OpenGLCubemapTexture::GetHeight() const
    {
        ZPG_PROFILE_FUNCTION();
        return m_Height;
    }

    void OpenGLCubemapTexture::SetData(const void* data, u32 size)
    {
        ZPG_NOT_IMPL("SetData isn't used with cubemaps, use Set");
    }

    void OpenGLCubemapTexture::AttachToFrameBuffer(u32 frameBufferID, FrameBufferAttachment frameBufferAttachment)
    {
        ZPG_NOT_IMPL();
    }

    void OpenGLCubemapTexture::Resize(u32 width, u32 height)
    {
        ZPG_PROFILE_FUNCTION();
        m_Width = width;
        m_Height = height;
        Invalidate();
    }

    u32 OpenGLCubemapTexture::GetRendererID() const
    {
        ZPG_PROFILE_FUNCTION();
        return m_RendererID;
    }

    void OpenGLCubemapTexture::GenerateMipmaps()
    {
        ZPG_NOT_IMPL();
    }



    void OpenGLCubemapTexture::SetFaceData(CubemapFace face, const void* data, u32 size)
    {
        ZPG_PROFILE_FUNCTION();
        ZPG_OPENGL_CALL(glActiveTexture(GL_TEXTURE0));
        ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
        ZPG_OPENGL_CALL(glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + (u32)face,
            0,
            OpenGLMapper::ToGL(m_DataFormat).InternalFormat,
            m_Width,
            m_Height,
            0,
            OpenGLMapper::ToGL(m_DataFormat).Format,
            OpenGLMapper::ToGL(m_DataFormat).SampleType,
            data));
    }
} // ZPG