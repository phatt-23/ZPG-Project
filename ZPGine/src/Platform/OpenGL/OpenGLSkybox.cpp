//
// Created by phatt on 11/1/25.
//

#include "OpenGLSkybox.h"

#include "OpenGLCore.h"
#include "Buffer/VertexArray.h"
#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"
#include "Resource/ResourceManager.h"
#include "Shader/ShaderProgram.h"
#include "stb_image/stb_image.h"
#include "Resource/CommonResources.h"

namespace ZPG {

OpenGLSkybox::OpenGLSkybox(const SkyboxSpecification& spec)
    : m_Specification(spec)
{
    ZPG_PROFILE_FUNCTION();

    // Cube Map texture.
    ZPG_OPENGL_CALL(glGenTextures(1, &m_TextureRendererID));
    ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureRendererID));

    stbi_set_flip_vertically_on_load(false);

    std::filesystem::path directory(m_Specification.Directory + (m_Specification.Directory.ends_with('/') ? "" : "/"));

    for (u32 i = 0; i < m_Specification.Filenames.size(); i++) {
        ZPG_CORE_INFO("Loading cube-map textures - directory: {}, file: {}", directory.c_str(), m_Specification.Filenames[i].c_str());

        int width, height, nrChannels;
        unsigned char* data = stbi_load((directory / m_Specification.Filenames[i]).c_str(), &width, &height, &nrChannels, 0);

        ZPG_CORE_ASSERT(data, "Cube map texture failed to load: {}", directory.c_str());

        ZPG_OPENGL_CALL(glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS));
        ZPG_OPENGL_CALL(glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height,
            0, GL_RGB, GL_UNSIGNED_BYTE, data));

        stbi_image_free(data);
    }

    ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    // Shader program
    {
        // m_ShaderProgram = ShaderProgram::Create("SkyboxSP", {
        //     Shader::CreateFromCode("SkyboxSP.vert", Shader::Vertex, skyboxVertexSource),
        //     Shader::CreateFromCode("SkyboxSP.frag", Shader::Fragment, skyboxFragmentSource),
        // });
        m_ShaderProgram = ResourceManager::GetGlobal().GetShaderProgram(CommonResources::SHADER_PROGRAM_SKYBOX);

        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt("u_SkyboxMap", 0);
        m_ShaderProgram->Unbind();
    }

    // Skybox VAO
    {
        m_VAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_SKYBOX);
    }

}

OpenGLSkybox::~OpenGLSkybox() {
    ZPG_PROFILE_FUNCTION();
    glDeleteTextures(1, &m_TextureRendererID);
}

void OpenGLSkybox::Bind() const {
    ZPG_PROFILE_FUNCTION();
    m_ShaderProgram->Bind();
    m_ShaderProgram->SetInt("u_SkyboxMap", 0);
    BindTextureToSlot(0);
    m_VAO->Bind();
}

void OpenGLSkybox::Unbind() const {
    ZPG_PROFILE_FUNCTION();
    m_VAO->Unbind();
    m_ShaderProgram->Unbind();
}

const ref<VertexArray>& OpenGLSkybox::GetVertexArray() const {
    ZPG_PROFILE_FUNCTION();
    return m_VAO;
}

void OpenGLSkybox::BindTextureToSlot(int slot) const {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureRendererID));
    ZPG_OPENGL_CALL(glActiveTexture(GL_TEXTURE0));
}


} // ZPG
