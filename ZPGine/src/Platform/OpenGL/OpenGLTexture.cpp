#include "OpenGLTexture.h"
#include "Debug/Asserter.h"
#include "Core/Core.h"
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <stb_image/stb_image_write.h>
#include "OpenGLCore.h"

namespace ZPG {

OpenGLTexture::OpenGLTexture(const std::string& filepath) {
    m_Name = filepath.substr(filepath.find_last_of("/\\") + 1, filepath.find_last_of('.') - filepath.find_last_of("/\\") - 1);
    LoadTexture(filepath);
}
OpenGLTexture::OpenGLTexture(const std::string& name, const std::string& filepath)
: m_Name(name) {
    LoadTexture(filepath);
}
OpenGLTexture::~OpenGLTexture() {
    ZPG_OPENGL_CALL(glDeleteTextures(1, &m_RendererID));
}
void OpenGLTexture::LoadTexture(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);
    i32 width;
    i32 height;
    i32 channels;
    byte* imageData = stbi_load(path.c_str(), &width, &height, &channels, 0);
    stbi_set_flip_vertically_on_load(false);

    ZPG_CORE_ASSERT(imageData != nullptr, "Failed to load image ({})!", path.c_str());

    m_Width = width;
    m_Height = height; 
    m_Channels = channels;

    auto[m_DataFormat, m_InternalFormat] = [&] -> std::pair<unsigned int, unsigned int> {
        switch (m_Channels)
        {
            case 1: return {GL_RED, GL_R8};
            case 2: return {GL_RG, GL_RG8};
            case 3: return {GL_RGB, GL_RGB8};
            case 4: return {GL_RGBA, GL_RGBA8};
            default: 
                ZPG_CORE_ASSERT(false, "Unknown texture format (number of channels) for image '{}'!", path.c_str()); 
                return {0, 0};
        }
    }();

    ZPG_OPENGL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID));
    ZPG_OPENGL_CALL(glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height));

    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT));
    ZPG_OPENGL_CALL(glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT));

    ZPG_CORE_INFO("Image ({}) size in bits: {} ({}x{}x{}) and bytes: {}", 
                    m_Name, 
                    m_Width*m_Height*m_Channels, 
                    m_Width, m_Height, m_Channels, 
                    (m_Width*m_Height*m_Channels)/8);

    // WARN: OpenGL (on Linux) leaks memory here (using libgallium).
    ZPG_OPENGL_CALL(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, imageData));
    stbi_image_free(imageData);
}
void OpenGLTexture::BindToSlot(u32 slotIndex) {
    ZPG_OPENGL_CALL(glBindTextureUnit(slotIndex, m_RendererID));
}
void OpenGLTexture::Unbind() {
    ZPG_OPENGL_CALL(glBindTextureUnit(0, m_RendererID));
}
const std::string& OpenGLTexture::GetName() const {
    return m_Name;
}
u32 OpenGLTexture::GetWidth() const {
    return m_Width;
}
u32 OpenGLTexture::GetHeight() const {
    return m_Height;
}
void OpenGLTexture::SetData(const void *data, u32 size) {
    ZPG_CORE_ASSERT(size == m_Width * m_Height * m_Channels, "Size ({}) doesn't match the size of the texture ({})!", size, m_Width * m_Height * m_Channels);
    ZPG_OPENGL_CALL(glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data));
}

}